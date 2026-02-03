# Linux 下交叉编译 Windows (Qt6) 指南

本项目支持在 Linux (Ubuntu/Debian/Fedora) 环境下直接编译针对 Windows 10 64位的可执行程序。这主要用于 GUI 和应用逻辑的快速构建与测试。

## 1. 快速搭建环境

我们提供了一个自动化脚本来完成所有依赖安装和配置：

```bash
python3 scripts/setup_cross_compile.py
```

该脚本会自动执行：
- 安装系统依赖 (`cmake`, `mingw-w64`, `ninja`)。
- 下载 Windows 版 Qt 6.6.2 库到 `qt_env`。
- 下载 Linux 版 Qt 6.6.2 工具（用于编译过程中的 `moc`/`rcc`）。
- 生成 `toolchain-mingw.cmake` 交叉编译配置文件。
- 自动处理 Qt 工具的软链接以解决 CMake 兼容性问题。

## 2. 编译项目

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
