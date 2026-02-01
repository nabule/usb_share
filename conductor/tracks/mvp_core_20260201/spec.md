# Track MVP Core Specification

## Scope
实现一个基础的 Windows USB 共享程序，允许两台 PC 之间通过 TCP/IP 共享物理 USB 设备。

## Functional Requirements
- **驱动集成**：集成 usbip-win 的虚拟总线和桩驱动。
- **传输逻辑**：实现基于 TCP 的 URB 数据包转发。
- **GUI 交互**：
    - 列出本地可分享的 USB 设备。
    - 输入远程 IP 地址进行连接。
    - 显示当前共享/连接状态。
- **稳定性**：支持网络连接闪断后的自动重连。

## Technical Requirements
- 开发语言：C++
- UI 框架：Qt 6
- 驱动：WDK / usbip-win
- 环境：Windows 10/11
