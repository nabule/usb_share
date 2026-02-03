#!/usr/bin/env python3
import os
import subprocess
import sys
import shutil
from pathlib import Path

# --- 配置部分 ---
QT_VERSION = "6.6.2"       # 想要安装的 Qt 版本
QT_HOST = "windows"        # 目标平台
QT_TARGET = "desktop"
QT_ARCH = "win64_mingw"    # 对应 MinGW 64位
INSTALL_DIR = Path.cwd() / "qt_env"  # 安装目录

def run_command(cmd, shell=False):
    print(f"Executing: {' '.join(cmd) if isinstance(cmd, list) else cmd}")
    try:
        subprocess.check_call(cmd, shell=shell)
    except subprocess.CalledProcessError as e:
        print(f"Error executing command: {e}")
        sys.exit(1)

def install_system_deps():
    print("--- 1. Installing System Dependencies ---")
    # 检测包管理器
    if shutil.which("apt-get"):
        pkgs = ["cmake", "mingw-w64", "ninja-build"]
        run_command(["sudo", "apt-get", "update"])
        run_command(["sudo", "apt-get", "install", "-y"] + pkgs)
    elif shutil.which("dnf"):
        pkgs = ["cmake", "mingw64-gcc-c++", "ninja-build"]
        run_command(["sudo", "dnf", "install", "-y"] + pkgs)
    else:
        print("Warning: Could not detect apt or dnf. Please ensure 'mingw-w64', 'cmake', and 'ninja-build' are installed manually.")

def install_python_deps():
    print("--- 2. Installing Python Dependencies (aqtinstall) ---")
    run_command([sys.executable, "-m", "pip", "install", "aqtinstall"])

def install_qt_libs():
    print(f"--- 3. Downloading Qt {QT_VERSION} for Windows (MinGW) ---")
    if not INSTALL_DIR.exists():
        INSTALL_DIR.mkdir(parents=True)
    
    # 使用 aqt 安装 Qt 库 (Windows Target)
    cmd = [
        sys.executable, "-m", "aqt", "install-qt",
        QT_HOST, QT_TARGET, QT_VERSION, QT_ARCH,
        "--outputdir", str(INSTALL_DIR)
    ]
    run_command(cmd)

def install_linux_host_tools():
    print(f"--- 3b. Downloading Qt {QT_VERSION} for Linux (Host Tools) ---")
    # 为了解决交叉编译时的 AUTOMOC 问题，需要 Linux 本地的 Qt 工具 (moc, rcc 等)
    # 我们安装最小化的 Linux Qt 到 qt_env/linux
    
    # 检查是否已安装 (简单检查目录)
    linux_qt_dir = INSTALL_DIR / QT_VERSION / "gcc_64"
    if linux_qt_dir.exists():
        print("Linux Qt tools already appear to be installed.")
        return

    cmd = [
        sys.executable, "-m", "aqt", "install-qt",
        "linux", "desktop", QT_VERSION, "gcc_64",
        "--outputdir", str(INSTALL_DIR),
        # "-m", "qtbase" # qtbase is usually included in the base installation
    ]
    run_command(cmd)

def generate_toolchain():
    print("--- 4. Generating CMake Toolchain File ---")
    
    # 查找 MinGW 编译器路径 (标准 Linux 路径)
    c_compiler = "/usr/bin/x86_64-w64-mingw32-gcc"
    cxx_compiler = "/usr/bin/x86_64-w64-mingw32-g++"
    rc_compiler = "/usr/bin/x86_64-w64-mingw32-windres"
    
    # 确定 Qt 的 cmake 目录
    # Windows Target Path
    qt_target_path = INSTALL_DIR / QT_VERSION / "mingw_64"
    qt_target_cmake_dir = qt_target_path / "lib" / "cmake"
    
    # Linux Host Path
    qt_host_path = INSTALL_DIR / QT_VERSION / "gcc_64"
    
    # 在 Qt6 中，工具可能在 bin 或 libexec 目录下
    possible_host_tool_dirs = [qt_host_path / "bin", qt_host_path / "libexec"]

    # 创建软链接欺骗 CMake
    target_bin = qt_target_path / "bin"
    if not target_bin.exists():
        target_bin.mkdir(parents=True)

    for tool in ["moc", "rcc", "uic", "qmake"]:
        target_tool = target_bin / f"{tool}.exe"
        
        # 寻找 Host 工具
        host_tool = None
        for d in possible_host_tool_dirs:
            p = d / tool
            if p.exists():
                host_tool = p
                break
        
        if host_tool:
            if target_tool.exists() or target_tool.is_symlink():
                try:
                    os.remove(target_tool)
                except:
                    pass
            try:
                os.symlink(host_tool, target_tool)
                print(f"Created symlink: {target_tool} -> {host_tool}")
            except Exception as e:
                print(f"Failed to create symlink for {tool}: {e}")
        else:
            print(f"Warning: Host tool {tool} not found in {possible_host_tool_dirs}")


    host_tools = {}
    for tool in ["moc", "rcc", "uic", "qmake"]:
        for d in possible_host_tool_dirs:
            p = d / tool
            if p.exists():
                host_tools[tool] = p
                break
    
    toolchain_content = f"""
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# 指定交叉编译器
set(CMAKE_C_COMPILER {c_compiler})
set(CMAKE_CXX_COMPILER {cxx_compiler})
set(CMAKE_RC_COMPILER {rc_compiler})

# 指定 Windows 根文件系统路径 (MinGW + Qt Target)
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32 {str(qt_target_path).replace(os.sep, '/')})

# 关键：指定 Host 工具路径，解决 AUTOMOC 权限问题
set(QT_HOST_PATH "{str(qt_host_path).replace(os.sep, '/')}")

# 显式指定 Qt 工具路径 (Linux 原生版本) 并强制覆盖 CMake 变量
set(QT_MOC_EXECUTABLE "{str(host_tools.get('moc', '')).replace(os.sep, '/')}" CACHE FILEPATH "" FORCE)
set(QT_RCC_EXECUTABLE "{str(host_tools.get('rcc', '')).replace(os.sep, '/')}" CACHE FILEPATH "" FORCE)
set(QT_UIC_EXECUTABLE "{str(host_tools.get('uic', '')).replace(os.sep, '/')}" CACHE FILEPATH "" FORCE)
set(QT_QMAKE_EXECUTABLE "{str(host_tools.get('qmake', '')).replace(os.sep, '/')}" CACHE FILEPATH "" FORCE)

# 针对 AUTOMOC 的核心变量
set(CMAKE_MOC_EXECUTABLE "${{QT_MOC_EXECUTABLE}}" CACHE FILEPATH "" FORCE)
set(CMAKE_RCC_EXECUTABLE "${{QT_RCC_EXECUTABLE}}" CACHE FILEPATH "" FORCE)
set(CMAKE_UIC_EXECUTABLE "${{QT_UIC_EXECUTABLE}}" CACHE FILEPATH "" FORCE)

# 针对 Qt6 的特殊配置
set(Qt6_MOC_EXECUTABLE "${{QT_MOC_EXECUTABLE}}" CACHE FILEPATH "" FORCE)
set(Qt6_RCC_EXECUTABLE "${{QT_RCC_EXECUTABLE}}" CACHE FILEPATH "" FORCE)
set(Qt6_UIC_EXECUTABLE "${{QT_UIC_EXECUTABLE}}" CACHE FILEPATH "" FORCE)
"""
    
    toolchain_content += f"""
# 调整查找行为
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Qt 路径配置 (指向 Target)
set(Qt6_DIR "{str(qt_target_cmake_dir).replace(os.sep, '/')}")
set(QT_DIR "{str(qt_target_cmake_dir).replace(os.sep, '/')}")
"""
    
    with open("toolchain-mingw.cmake", "w") as f:
        f.write(toolchain_content)
    
    print(f"Toolchain file generated at: {Path.cwd() / 'toolchain-mingw.cmake'}")

if __name__ == "__main__":
    # 如果作为脚本直接运行，则执行完整流程
    install_system_deps()
    install_python_deps()
    install_qt_libs()
    install_linux_host_tools() # 新增步骤
    generate_toolchain()

    print("\n✅ Environment Setup Script Completed!")
    print("Next step: cmake -DCMAKE_TOOLCHAIN_FILE=toolchain-mingw.cmake -B build_win_cross .")
