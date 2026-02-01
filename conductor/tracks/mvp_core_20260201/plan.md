# Implementation Plan - MVP Core

## Phase 1: Environment & Driver Scaffolding
- [x] Task: 环境搭建与驱动集成 8760b1d
    - [ ] 配置 Visual Studio WDK 开发环境
    - [ ] 导入 usbip-win 源代码并建立本地编译链
    - [ ] 编写简单的驱动安装/卸载脚本以供开发测试
- [ ] Task: Conductor - User Manual Verification 'Phase 1: Environment & Driver Scaffolding' (Protocol in workflow.md)

## Phase 2: Core Logic Implementation
- [ ] Task: 实现 USB 导出器 (Exporter/Stub) 逻辑
    - [ ] 编写测试以验证 USB 设备列表获取功能
    - [ ] 实现 stub 驱动的绑定与解绑控制逻辑
- [ ] Task: 实现网络传输协议层
    - [ ] 实现基于 TCP 的数据流封装与解析逻辑
    - [ ] 编写网络连接管理的单元测试
- [ ] Task: Conductor - User Manual Verification 'Phase 2: Core Logic Implementation' (Protocol in workflow.md)

## Phase 3: GUI & Integration
- [ ] Task: 构建 Qt 用户界面
    - [ ] 实现设备列表显示的 UI 模块
    - [ ] 实现连接配置对话框
- [ ] Task: 端到端集成测试
    - [ ] 验证 A 机分享、B 机识别的完整链路
    - [ ] 验证自动重连逻辑
- [ ] Task: Conductor - User Manual Verification 'Phase 3: GUI & Integration' (Protocol in workflow.md)
