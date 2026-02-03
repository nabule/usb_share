# Linux 下交叉编译 Windows (Qt6) 指南

本项目支持在 Linux (Ubuntu/Debian/Fedora) 环境下直接编译针对 Windows 10 64位的可执行程序。这主要用于 GUI 和应用逻辑的快速构建与测试。

## 1. 快速搭建环境

我们提供了一个自动化脚本来完成所有依赖安装和配置：

```bash
python3 scripts/setup_cross_compile.py
```

### 该脚本安装的内容清单 (Installed Items):
- **系统软件包** (通过 apt/dnf):
    - `cmake`: 构建工具。
    - `mingw-w64`: 交叉编译器工具链 (gcc, g++, windres)。
    - `ninja-build`: 高效构建系统。
- **Python 依赖**:
    - `aqtinstall`: 用于从 Qt 官方下载预编译库。
- **项目目录产物**:
    - `qt_env/`: 包含针对 Windows 的 Qt 6.6.2 库及针对 Linux 的编译工具。
    - `toolchain-mingw.cmake`: CMake 交叉编译配置文件。
- **特殊处理**:
    - 自动在 `qt_env` 目录下创建软链接，将 Windows 版的 `moc.exe` 等指向 Linux 原生版本，以解决交叉编译兼容性。

## 2. 编译项目
... (略)

## 4. 环境卸载

如果您需要清理交叉编译环境，请运行卸载脚本：

```bash
python3 scripts/uninstall_cross_compile.py
```

该脚本会安全移除项目目录下的库文件和配置文件。系统级依赖请参考脚本输出进行手动卸载。

环境搭建完成后，使用以下命令进行编译：

```bash
# 1. 配置
cmake -DCMAKE_TOOLCHAIN_FILE=toolchain-mingw.cmake -B build_win_cross .

# 2. 编译
cmake --build build_win_cross -j$(nproc)
```

编译产物（`.exe` 文件）将位于 `build_win_cross/` 目录下。

## 3. 注意事项

- **驱动程序**：底层的 USB 驱动（`usbip2_ude.sys` 等）**无法**在 Linux 下交叉编译。请直接从 [Releases](...) 下载预编译的驱动，或在 Windows 下使用 Visual Studio 编译驱动部分。
- **运行**：生成的 `.exe` 文件需要复制到 Windows 环境运行。如果您安装了 Wine，也可以尝试直接在 Linux 下运行：`wine build_win_cross/usb_share.exe`。
- **依赖库**：如果添加了新的第三方库，请确保使用 MinGW 版本的库进行链接。
