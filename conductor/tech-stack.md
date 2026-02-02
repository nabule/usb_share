# Technology Stack - USB Over Network (USB-Share)

## 1. Core Driver & System Logic
- **Language:** C/C++
- **Framework:** Windows Driver Kit (WDK)
- **Model:** Kernel-Mode Driver Framework (KMDF) - 用于处理底层的 USB 请求块 (URB) 转发。
- **Base Implementation:**
    - **Server (Stub):** **USBIP-WIN** (Open Source) / `usbipd-win` (Standard Stub)
    - **Client (VHCI):** **usbip-win2** (VHCI Driver + Tools)
        - 采用其 WHLK 认证级别的 VHCI 驱动 (`usbip2_ude.sys`) 以支持 Windows 10+ 客户端功能。
    - 采用标准 TCP 传输协议 (USBIP v1.1.1) 以确保互通性。

## 2. User Interface & Control App
- **Language:** C++
- **Framework:** Qt 6 (Widgets Module)
    - 选择 Qt Widgets 而非 QML，以获得更原生的 Windows 控件外观和更低的资源占用。
    - 通过 Qt Network 模块处理部分非实时的控制指令（如设备发现、状态查询）。

## 3. Network Transport
- **Protocol:** Raw TCP/IP Sockets
    - 直接操作套接字以获得最低延迟。
    - 禁用 Nagle 算法 (`TCP_NODELAY`) 以优化小包传输性能（对 USB 鼠标/键盘等设备至关重要）。

## 4. Build & Tooling
- **IDE:** Visual Studio 2022 (Required for WDK integration)
- **Compiler:** MSVC (Microsoft Visual C++)
- **Version Control:** Git
- **Installer:** Inno Setup (用于生成单文件包或简单的驱动安装脚本)
