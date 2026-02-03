# Specification - Linux Cross-Compilation Setup (Qt6 Windows)

## 1. 概述 (Overview)
本任务旨在实现一个 Python 自动化脚本，用于在 Linux 系统上搭建针对 Windows 10 64位平台的 Qt6 交叉编译环境。该方案将跳过底层的驱动编译（驱动部分直接下载预编译版本），仅针对 GUI 和核心业务逻辑（Qt 相关组件）进行交叉编译。

## 2. 功能需求 (Functional Requirements)
- **自动化环境搭建脚本**：创建一个名为 `scripts/setup_cross_compile.py` 的 Python 脚本。
- **系统依赖安装**：脚本应能自动检测系统包管理器（apt 或 dnf），并安装必要的依赖项：`cmake`, `ninja-build`, `mingw-w64`。
- **Qt Windows 库下载**：利用 `aqtinstall` 工具，在本地目录（建议为 `qt_env`）下载针对 Windows 平台预编译的 Qt 6.6.2 (MinGW 64-bit) 库。
- **CMake Toolchain 生成**：自动生成一个 `toolchain-mingw.cmake` 文件，配置交叉编译器路径（x86_64-w64-mingw32-g++）及 Qt 库路径。
- **驱动处理**：明确此脚本不涉及驱动编译，仅需在文档或脚本输出中引导用户从指定位置下载预编译驱动。

## 3. 非功能需求 (Non-Functional Requirements)
- **幂等性**：脚本应支持重复运行而不导致重复安装或冲突。
- **可维护性**：代码应清晰标注各安装阶段，方便未来升级 Qt 版本。

## 4. 验收标准 (Acceptance Criteria)
- [ ] 运行 `python3 scripts/setup_cross_compile.py` 能成功下载 Qt 库并生成 `toolchain-mingw.cmake`。
- [ ] 产生的 toolchain 文件中包含正确的 `CMAKE_SYSTEM_NAME` (Windows) 和编译器路径。
- [ ] 开发者能够通过 `cmake -DCMAKE_TOOLCHAIN_FILE=toolchain-mingw.cmake ...` 命令成功配置项目而不报错。

## 5. 超出范围 (Out of Scope)
- 编译 Windows 驱动程序 (`usbip2_ude.sys` 等)。
- 自动化 CI/CD 流程集成。
- 在 Linux 下运行生成的 .exe 程序（不强制要求包含 Wine 环境配置）。
