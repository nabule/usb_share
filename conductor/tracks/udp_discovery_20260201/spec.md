# Track UDP Discovery Specification

## Scope
实现基于 UDP 广播的服务发现机制，使用户无需手动输入 IP 即可找到局域网内的共享端。

## Functional Requirements
- **服务广播 (Broadcaster)**: 共享端 (Server) 每隔固定时间（如 3秒）向局域网广播自身信息。
- **服务发现 (Listener)**: 连接端 (Client) 监听广播端口，维护一个“在线设备列表”。
- **UI 更新**: 在 "Remote Connect" 界面增加“搜索/刷新”按钮和结果列表，点击列表项自动填入 IP。

## Technical Requirements
- **Protocol**: UDP
- **Port**: 3241 (独立于数据传输端口 3240)
- **Packet Format**: JSON
    ```json
    {
      "cmd": "DISCOVERY_ANNOUNCE",
      "hostname": "PC-A",
      "ip": "192.168.1.10",
      "port": 3240,
      "version": "1.0"
    }
    ```
- **Framework**: Qt Network (QUdpSocket)
