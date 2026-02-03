# USB-Share: 底层原生 USB 远程共享工具

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com/your-repo/usb-share)
[![Platform](https://img.shields.io/badge/platform-Windows-blue.svg)](https://www.microsoft.com/windows)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Qt](https://img.shields.io/badge/Qt-6.x-green.svg)](https://www.qt.io/)

**USB-Share** 是一款极简且高性能的底层原生 USB 设备共享工具。它基于标准的 USB/IP 协议，允许用户通过 TCP/IP 网络，将本地插入的 USB 设备（如加密狗、扫描仪、开发板等）以“原生插拔”的方式共享给远程电脑。远程电脑识别到的设备如同直接插入物理接口，确保所有原始驱动和功能完整可用。

---

## 📖 项目简介

USB-Share 解决了远程办公或多机协作场景下，物理 USB 设备（尤其是加密狗或特定硬件）无法灵活调度的痛点。

### 核心价值
- **原生体验**：在 USB 协议层 (URB) 进行拦截与重定向，远程端无需模拟，直接使用原始驱动。
- **高性能传输**：针对网络波动优化，禁用 Nagle 算法，确保低延迟的数据同步。
- **跨机器连接**：支持局域网自动发现与手动 IP 指定。

### 主要特性
- ✅ **底层原生共享**：支持各种类型的 USB 设备（HID、Mass Storage、Vendor Specific）。
- ✅ **双端角色支持**：同一程序既可作为服务端（分享者）也可作为客户端（使用者）。
- ✅ **自动发现机制**：内置 UDP 广播，一键搜索局域网内的分享端。
- ✅ **连接保护**：内置心跳检测与超时重连逻辑，解决连接卡死问题。

---

## 📋 目录
- [环境要求](#-环境要求)
- [安装指南 (重要)](#-安装指南-重要)
- [快速开始](#-快速开始)
- [开发与构建](#-开发与构建)
- [故障排除](#-故障排除)
- [路线图](#-路线图)
- [致谢与许可证](#-致谢与许可证)

---

## 💻 环境要求
- **操作系统**: Windows 10 Version 1903 (Build 18362) 或更高版本 / Windows 11。
- **网络**: 两台电脑需处于同一局域网，或通过 VPN 建立 TCP/IP 通道。
- **权限**: 驱动安装与程序运行需要**管理员权限**。

---

## 🚀 安装指南 (重要)

本工具依赖特定的内核驱动来实现 USB 转发。根据您的角色（分享者或使用者），需要安装不同的驱动组件。

### 1. 作为服务端 (分享设备的一方)
服务端需要安装“桩驱动” (Stub Driver) 来接管物理 USB 接口。
- **推荐安装**: [usbipd-win (dorssel)](https://github.com/dorssel/usbipd-win/releases)。
- **步骤**: 
    1. 下载并运行 `.msi` 安装包。
    2. 重启电脑。
    3. 在管理员权限下运行 `usbipd list` 确认安装成功。

### 2. 作为客户端 (使用远程设备的一方)
客户端需要安装“虚拟总线驱动” (VHCI Driver) 来创建虚拟 USB 端口。
- **核心组件**: [usbip-win2 (vadimgrn)](https://github.com/vadimgrn/usbip-win2)。
- **关键步骤**:
    1. **开启测试模式**: 管理员权限执行 `bcdedit /set testsigning on` 并**重启电脑**（必须，否则驱动无法加载）。
    2. **安装证书**: 在 `driver_temp/usbip-win2/drivers/package` 中找到 `usbip.pfx`，双击安装。将其导入到“受信任的根证书颁发机构”和“受信任的发布者”。
    3. **安装驱动**: 在管理员终端进入驱动目录，执行：
       ```powershell
       pnputil /add-driver usbip2_ude.inf /install
       ```

---

## 💡 快速开始

### 场景：将 PC-A 的加密狗共享给 PC-B

1. **在 PC-A (服务端) 操作：**
   - 运行 `usb_share.exe`。
   - 在 **"Local Devices"** 选项卡中点击 **"Refresh"**。
   - 选中您的 USB 设备，点击 **"Share"**。
   
2. **在 PC-B (客户端) 操作：**
   - 运行 `usb_share.exe`。
   - 切换到 **"Remote Connect"** 选项卡。
   - 在 **"Discovered Devices"** 列表中应该能看到 PC-A，双击它或手动输入 IP。
   - 点击 **"Connect"**。
   - 成功后，Windows 会发出发现新硬件的声音，设备将出现在您的设备管理器中。

---

## 🛠 开发与构建

### 1. Windows 平台编译
- **IDE**: Visual Studio 2022
- **编译器**: MSVC (C++ 17+)
- **框架**: Qt 6.x (Widgets)
- **驱动开发**: Windows Driver Kit (WDK)

### 2. Linux 平台交叉编译 (推荐)
如果您在 Linux 下开发，可以使用我们的自动化脚本快速编译 Windows 版本的 GUI 程序。
- **详细指南**: [Linux 下交叉编译指南](./docs/linux_cross_compile.md)
- **快捷命令**:
  ```bash
  python3 scripts/setup_cross_compile.py
  cmake -DCMAKE_TOOLCHAIN_FILE=toolchain-mingw.cmake -B build_win_cross .
  cmake --build build_win_cross
  ```

### 构建步骤
1. **安装依赖**: 确保已安装 Qt 6 和 CMake。
2. **生成项目**:
   ```bash
   mkdir build && cd build
   cmake ..
   ```
3. **编译**: 使用 VS 2022 打开 `.sln` 或直接运行 `msbuild`。
4. **自动化脚本**: 我们提供了 `build_windows.ps1` 用于一键打包。

---

## ❓ 故障排除

| 问题 | 原因 | 解决方法 |
| :--- | :--- | :--- |
| **连接一直处于 Connecting** | 协议握手失败或防火墙拦截 | 确保服务端 `usbipd` 已启动，且防火墙允许 TCP 3240 端口。 |
| **客户端无法安装驱动** | 未开启测试签名模式 | 执行 `bcdedit /set testsigning on` 并重启。 |
| **找不到设备** | UDP 广播被拦截 | 检查防火墙是否允许 UDP 3241 端口，或手动输入 IP。 |
| **连接超时** | 网络延迟过高或服务端无响应 | 本程序已设置 5s 超时自动断开，请检查网络质量。 |

---

## 🗺 路线图
- [x] MVP 核心转发功能
- [x] 局域网 UDP 自动发现
- [x] 基于 `usbip-win2` 的客户端 VHCI 方案集成
- [x] 连接握手优化与超时处理
- [ ] 系统托盘最小化运行
- [ ] 传输层 AES 数据加密
- [ ] 自动挂载规则（Persistent Devices）

---

## 📄 致谢与许可证

- 特别感谢 [usbipd-win](https://github.com/dorssel/usbipd-win) 和 [usbip-win2](https://github.com/vadimgrn/usbip-win2) 提供的底层驱动支持。
- 本项目采用 **MIT 许可证** 授权。

---
*保持简单，保持原生。*