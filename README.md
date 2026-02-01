# USB-Share: 底层原生 USB 远程共享工具

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com/your-repo/usb-share)
[![Platform](https://img.shields.io/badge/platform-Windows-blue.svg)](https://www.microsoft.com/windows)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Qt](https://img.shields.io/badge/Qt-6.x-green.svg)](https://www.qt.io/)

**USB-Share** 是一款极简且高性能的底层原生 USB 设备共享工具。它允许用户通过 TCP/IP 网络，将本地插入的 USB 设备（如加密狗、扫描仪、开发板等）以“原生插拔”的方式共享给远程电脑。

---

## 📖 项目简介

USB-Share 旨在解决远程办公或多机协作场景下物理 USB 设备无法灵活调度的痛点。不同于文件共享或流媒体转发，USB-Share 在 **USB 协议层 (URB)** 进行拦截与重定向，使远程电脑识别到的设备如同直接插入物理接口一样。

### 核心价值
- **原生体验**：远程电脑需安装设备原始驱动，功能与物理连接完全一致。
- **极致稳定**：针对网络波动优化，支持断线自动重连。
- **极简设计**：绿色免安装，单文件运行，不产生系统垃圾。

### 主要特性
- ✅ **底层转发**：基于 USBIP 协议，支持各类 USB 设备（HID、存储、加密狗等）。
- ✅ **局域网发现**：内置 UDP 广播机制，自动搜索网内可用设备。
- ✅ **状态监控**：实时显示设备连接状态（本地、共享中、远程连接中）。
- ✅ **高性能**：禁用 Nagle 算法，确保低延迟数据传输。

---

## 📋 目录
- [安装指南](#-安装指南)
- [快速开始](#-快速开始)
- [详细用法](#-详细用法)
- [技术栈](#-技术栈)
- [开发与构建](#-开发与构建)
- [路线图](#-路线图)
- [许可证](#-许可证)

---

## 🚀 安装指南

### 环境要求
- **操作系统**: Windows 10 / 11 (64-bit)
- **网络**: 两台电脑需处于同一局域网（或通过 VPN 互通）

### 1. 安装核心驱动 (推荐方式)
本项目使用 `usbipd-win` 作为底层驱动引擎，它提供了经过数字签名的稳定驱动。
- 访问 [usbipd-win Releases](https://github.com/dorssel/usbipd-win/releases)。
- 下载最新的 `.msi` 安装包并安装。
- 安装完成后，在管理员权限的 PowerShell 中运行 `usbipd list` 确认环境正常。

### 2. 下载并运行本程序
- 从 [Releases](https://github.com/your-repo/usb-share/releases) 页面下载最新的 `usb_share.zip` 并解压。
- 直接运行 `usb_share.exe`。

> **注意**: 如果您坚持使用手动安装方式，请参考 `scripts/` 目录下的脚本，但需自行解决驱动数字签名问题。

---

## 💡 快速开始

### 场景：将 PC-A 的加密狗分享给 PC-B

1. **在 PC-A (服务端) 操作：**
   - 运行 `usb_share.exe`。
   - 在 **"Local Devices"** 选项卡点击 **"Refresh"**。
   - 找到您的加密狗，点击 **"Share"** (或右键选择)。
   
2. **在 PC-B (客户端) 操作：**
   - 运行 `usb_share.exe`。
   - 切换到 **"Remote Connect"** 选项卡。
   - 在下方的 **"Discovered Devices"** 列表中，双击 PC-A 的条目。
   - 点击 **"Connect"**。
   - 听到 Windows 硬件插入提示音，即表示成功。

---

## 🛠 详细用法

### 设备发现与连接
- **自动发现**：只要服务端运行了 `usb_share.exe`，客户端的发现列表就会实时更新。
- **手动连接**：如果自动发现无效（如跨网段），可以直接在地址栏输入服务端的 IP 地址。

### 驱动引擎说明
本程序是 `usbipd-win` 的图形化封装与增强工具。底层共享逻辑由 `usbipd-win` 服务处理，本程序负责：
- 自动化的广播发现。
- 直观的 GUI 设备选择。
- 跨网络的连接管理。

---

## 🏗 技术栈

- **核心驱动**: [usbipd-win](https://github.com/dorssel/usbipd-win) (推荐) / [usbip-win](https://github.com/cezanne/usbip-win)
- **底层协议**: USBIP Protocol
- **应用逻辑**: C++ 17
- **UI 框架**: Qt 6 (Widgets)
- **网络通信**: 原生 TCP Sockets (禁用 Nagle 算法) + UDP Discovery

---

## 💻 开发与构建

### 准备工作
- 安装 [aqtinstall](https://github.com/miurahr/aqtinstall) 进行 Qt 命令行安装（推荐）。
- 安装 CMake。
- 安装 MinGW 编译器。

### 构建步骤
1. 克隆仓库：
   ```bash
   git clone --recursive https://github.com/your-repo/usb-share.git
   ```
2. 运行 Windows 自动化编译脚本：
   ```powershell
   .\build_windows.ps1
   ```

---

## 🗺 路线图

- [x] MVP 核心转发功能
- [x] 局域网 UDP 自动发现
- [x] Windows 自动化构建脚本
- [ ] 系统托盘最小化运行
- [ ] 传输层数据加密
- [ ] 访问密码与 IP 白名单

---

## 📄 许可证

本项目采用 MIT 许可证 - 详情请参阅 [LICENSE](LICENSE) 文件。

---

## 📞 支持与联系

- **问题反馈**: 请提交 [GitHub Issue](https://github.com/your-repo/usb-share/issues)
- **致谢**: 感谢 [usbipd-win](https://github.com/dorssel/usbipd-win) 和 [usbip-win](https://github.com/cezanne/usbip-win) 项目提供的底层支持。

---
*保持简单，保持原生。*