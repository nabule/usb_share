# Track Specification: Research and Integrate usbip-win2 for Windows Client

## 1. Overview
目前项目面临的关键阻塞是 Windows 客户端 (Machine B) 无法正确挂载远程 USB 设备。现有的 `usbipd` 主要作为服务端使用，缺乏 Windows 端的 VHCI (Virtual Host Controller Interface) 驱动支持。
本 Track 旨在引入 [vadimgrn/usbip-win2](https://github.com/vadimgrn/usbip-win2) 项目，利用其 VHCI 驱动实现 Windows 客户端功能，并验证其与服务端 (`usbipd`) 的兼容性。

## 2. Goals
1.  **技术调研**: 深入分析 `usbip-win2` 的架构、编译方式及其 VHCI 驱动的工作原理。
2.  **兼容性分析**: 确认 `usbip-win2` 客户端驱动能否与标准的 `usbipd` (或项目当前使用的服务端) 协同工作。
3.  **方案落地**: 制定将 `usbip-win2` 集成到现有 Qt 项目中的路线图。
4.  **原型验证 (POC)**: 输出详细的操作指南和适配代码，演示如何建立连接和挂载设备。

## 3. Scope
-   **In Scope**:
    -   分析 `usbip-win2` 源码。
    -   编写 Windows 下的构建/安装脚本 (PowerShell/CMake)。
    -   在 Qt 客户端中封装调用 `usbip-win2` 驱动/工具的逻辑（如 `attach` 命令）。
    -   输出《技术分析与集成报告》。
-   **Out of Scope**:
    -   开发 Linux 版本的 UI（专注于 Windows 互传）。
    -   重写 `usbip-win2` 的内核驱动代码（除非有严重 Bug 且必须修复）。

## 4. Requirements
### Functional
-   **客户端驱动**: 必须能够成功编译并安装 `usbip-win2` 的 VHCI 驱动。
-   **协议匹配**: 客户端发出的 `attach` 指令必须能被服务端 `usbipd` 正确识别并响应。
-   **设备挂载**: 最终效果是 B 机设备管理器中出现远程的 USB 设备。

### Non-Functional
-   **文档**: 提供清晰的步骤，指导如何在开发机上部署此方案。

## 5. Acceptance Criteria
-   [ ] 完成《usbip-win2 技术分析报告》，包含架构图、协议流分析及服务端兼容性结论。
-   [ ] 提供 `build_usbip_win2.ps1` 或类似脚本，用于构建客户端所需组件。
-   [ ] 提供 Demo 代码或集成方案，展示如何通过 Qt 调用新引入的工具进行连接。
-   [ ] (可选/手动验证) 在 Windows 实体机上按指南操作，成功挂载远程设备。

## 6. Implementation Notes (Environment Constraints)
由于当前 Agent 运行在 Linux 环境：
-   我们将侧重于源码分析、脚本编写和 C++ 逻辑代码的实现。
-   无法直接执行 `.exe` 或安装 `.sys` 驱动。
-   验证将通过“模拟协议交互”的单元测试或详细的步骤文档来进行。
