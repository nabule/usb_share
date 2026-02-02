# 实施计划 - 修复连接卡死问题

## 阶段 1：诊断与复现
- [x] 任务：创建复现测试用例 5321bd2
    - [x] 编写脚本/测试模拟 Client B 连接 Server A。
    - [x] 尝试通过代码复现“卡死”状态，或记录清晰的手动复现步骤。
- [x] 任务：增强服务端日志 558535e
    - [x] 在 `UdpListener` 和 `NetworkTransport`（或相关类）中增加详细日志。
    - [x] 记录 TCP 接收连接 (accept)、握手开始及完成的关键点。
    - [x] 立即记录任何套接字错误或超时。
- [x] 任务：Conductor - 用户手册验证 '阶段 1：诊断与复现' (协议见 workflow.md) [checkpoint: e9bb70e]

## 阶段 2：修复与稳定
- [x] 任务：实现 USBIP 协议握手 7dec153
    - [x] 修改 `NetworkTransport` 以在连接建立后发送 USBIP 协议头 (OP_REQ_IMPORT 或类似)。
    - [x] 确保与 `usbipd` 的握手兼容。
- [x] 任务：实现客户端超时机制与 UI 反馈 7dec153
    - [x] 修改 `MainWindow` 以监听 `connected` 信号并更新 UI。
    - [x] 修改客户端连接逻辑，设置严格的 Socket 超时时间（如 5-10秒）。
    - [x] 确保 UI 能优雅处理超时（从“连接中”变更为“失败”，并显示提示）。
- [x] 任务：防火墙/安全策略检查（代码层） a5614bb
    - [x] (可选) 在服务端启动时增加检查，如果端口看似被封锁或进程缺乏网络权限，发出警告（如果用户态可行）。
- [x] 任务：Conductor - 用户手册验证 '阶段 2：修复与稳定' (协议见 workflow.md) [checkpoint: 8c70779]

## 阶段 3：最终验证
- [x] 任务：端到端连接测试
    - [x] 验证 B 成功连接 A。
    - [x] 验证 A 记录连接日志。
    - [x] 验证断开连接的处理逻辑。
- [x] 任务：Conductor - 用户手册验证 '阶段 3：最终验证' (协议见 workflow.md) [checkpoint: 071ceea]