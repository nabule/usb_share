# Track Specification: Fix Connection Hang Issue (Client B to Server A)

## 1. Overview
当前在局域网环境下，Client B (客户端) 可以通过 UDP 广播发现 Server A (服务端)，但在点击“连接”后，客户端界面一直卡在“连接中”状态，而服务端没有任何反应。此任务旨在排查并修复该 TCP 连接建立失败的问题。

## 2. Problem Description
- **现象**:
    - Client B 成功发现 Server A。
    - Client B 点击连接，状态栏显示“Connecting...”（或类似等待状态），无超时报错，也无成功反馈。
    - Server A 端无任何日志输出或界面变化，似乎未接收到 TCP `SYN` 包或在握手阶段被静默丢弃。
- **环境**:
    - Windows 系统。
    - 同一局域网 (LAN)。
    - 预期防火墙已关闭或允许通行 (用户反馈环境为 A 选项)。

## 3. Root Cause Analysis (Hypothesis)
需要验证以下潜在原因：
1. **端口监听错误**: Server A 虽然广播了 IP，但实际 TCP 监听端口并未正确绑定或处于 LISTEN 状态。
2. **防火墙静默拦截**: 尽管认为是局域网直连，Windows 防火墙可能默认拦截了入站 TCP 请求（仅允许了 UDP 发现）。
3. **IP 地址绑定错误**: Server A 可能监听在 `127.0.0.1` 而非 `0.0.0.0` 或局域网 IP，导致外部无法连接。

## 4. Requirements
### Functional
- **修复连接逻辑**: Client B 点击连接后，应能成功与 Server A 建立 TCP 会话。
- **超时处理**: 如果连接确实无法建立，Client B 应在 5-10 秒内超时并报错，而不是无限期卡在“连接中”。
- **服务端日志**: Server A 应增加网络层日志，明确显示“收到连接请求”、“握手成功”或“握手失败”。

### Non-Functional
- **稳定性**: 修复不应影响现有的 UDP 发现功能。

## 5. Acceptance Criteria
- [ ] 启动 Server A 和 Client B。
- [ ] Client B 列表显示 Server A。
- [ ] Client B 点击连接，状态迅速变为“Connected”。
- [ ] Server A 端显示“Client B connected”。
- [ ] 若强制断开 Server A 网络，Client B 点击连接后应在 10 秒内提示“Connection Failed”。

## 6. Out of Scope
- USB 设备本身的挂载与数据传输（仅关注 TCP 链路建立）。
- 跨网段/NAT 穿透功能。