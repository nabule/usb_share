# Implementation Plan - UDP Discovery

## Phase 1: Core Network Logic
- [x] Task: 实现 UdpBroadcaster 类 b40f47d
    - [ ] 编写测试：验证广播数据包格式 (JSON) 正确性
    - [ ] 实现定时广播逻辑 (QUdpSocket)
- [x] Task: 实现 UdpListener 类 8633ae5
    - [ ] 编写测试：验证接收和解析广播包的逻辑
    - [ ] 实现设备列表维护 (去重、超时移除)
- [ ] Task: Conductor - User Manual Verification 'Phase 1: Core Network Logic' (Protocol in workflow.md)

## Phase 2: GUI Integration
- [ ] Task: 更新 MainWindow UI
    - [ ] 添加设备发现列表控件 (Remote Tab)
    - [ ] 集成 UdpListener 到 UI，实现点击连接功能
- [ ] Task: Conductor - User Manual Verification 'Phase 2: GUI Integration' (Protocol in workflow.md)
