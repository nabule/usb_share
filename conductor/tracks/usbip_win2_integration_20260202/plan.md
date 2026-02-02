# Implementation Plan - Research and Integrate usbip-win2

## Phase 1: Technical Analysis & Feasibility
- [x] Task: usbip-win2 源码架构分析 a352edf
    - [x] 克隆并分析 vadimgrn/usbip-win2 仓库，重点关注 VHCI 驱动部分 (driver/vhci) 和用户态工具 (userspace)。
    - [x] 分析其与标准 Linux usbip 协议的差异（如果有）。
    - [x] 产出《usbip-win2 技术分析报告》(docs/analysis/usbip-win2-analysis.md)。
- [x] Task: 编译与构建脚本开发 1be3a8d
    - [x] 编写 PowerShell 脚本 `scripts/build_usbip_win2.ps1`，用于在 Windows 环境下自动化拉取和构建 usbip-win2（若需要）。
    - [x] 确认驱动签名需求和测试模式安装流程。
- [x] Task: Conductor - User Manual Verification 'Phase 1: Technical Analysis & Feasibility' (Protocol in workflow.md) [checkpoint: de9010e]

## Phase 2: Client Integration (POC)
- [ ] Task: 协议交互模拟测试
    - [ ] 编写 C++ 单元测试，模拟客户端向服务端发送 `usbip-win2` 风格的 attach 指令。
    - [ ] 验证服务端（标准 usbipd）的响应兼容性。
- [ ] Task: Qt 客户端集成逻辑
    - [ ] 创建 `UsbIpWin2Client` 类，封装对新工具/驱动的调用。
    - [ ] 实现核心命令：`usbip attach -r <remote_ip> -b <bus_id>` 的程序化调用。
- [ ] Task: Conductor - User Manual Verification 'Phase 2: Client Integration (POC)' (Protocol in workflow.md)

## Phase 3: Documentation & Verification
- [ ] Task: 编写集成与使用文档
    - [ ] 更新 `README.md`，增加关于 usbip-win2 驱动安装的说明。
    - [ ] 编写《Windows 客户端互联指南》(docs/windows_client_guide.md)。
- [ ] Task: Conductor - User Manual Verification 'Phase 3: Documentation & Verification' (Protocol in workflow.md)
