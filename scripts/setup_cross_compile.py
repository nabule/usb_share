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
    
    # 使用 aqt 安装 Qt 库
    cmd = [
        sys.executable, "-m", "aqt", "install-qt",
        QT_HOST, QT_TARGET, QT_VERSION, QT_ARCH,
        "--outputdir", str(INSTALL_DIR)
    ]
    run_command(cmd)

def generate_toolchain():
    print("--- 4. Generating CMake Toolchain File ---")
    
    # 查找 MinGW 编译器路径 (标准 Linux 路径)
    c_compiler = "/usr/bin/x86_64-w64-mingw32-gcc"
    cxx_compiler = "/usr/bin/x86_64-w64-mingw32-g++"
    rc_compiler = "/usr/bin/x86_64-w64-mingw32-windres"
    
    # 确定 Qt 的 cmake 目录
    # aqt 安装路径通常为: INSTALL_DIR/QT_VERSION/mingw_64
    qt_base_path = INSTALL_DIR / QT_VERSION / "mingw_64"
    qt_cmake_dir = qt_base_path / "lib" / "cmake"
    
    toolchain_content = f"""
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# 指定交叉编译器
set(CMAKE_C_COMPILER {c_compiler})
set(CMAKE_CXX_COMPILER {cxx_compiler})
set(CMAKE_RC_COMPILER {rc_compiler})

# 指定 Windows 根文件系统路径 (MinGW + Qt)
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32 {str(qt_base_path).replace(os.sep, '/')})

# 调整查找行为
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Qt 路径配置
set(Qt6_DIR "{str(qt_cmake_dir).replace(os.sep, '/')}")
set(QT_DIR "{str(qt_cmake_dir).replace(os.sep, '/')}")
"""
    
    with open("toolchain-mingw.cmake", "w") as f:
        f.write(toolchain_content)
    
    print(f"Toolchain file generated at: {Path.cwd() / 'toolchain-mingw.cmake'}")

if __name__ == "__main__":
    # 如果作为脚本直接运行，则执行完整流程
    # 注意：在单元测试中我们只调用单个函数
    install_system_deps()
    install_python_deps()
    install_qt_libs()
    generate_toolchain()
    print("\n✅ Environment Setup Script Completed!")
    print("Next step: cmake -DCMAKE_TOOLCHAIN_FILE=toolchain-mingw.cmake -B build_win_cross .")
