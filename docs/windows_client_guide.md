# Windows 客户端互联指南 (usbip-win2)

## 简介
为了让 Windows 电脑能够作为**客户端**（连接并使用远程 USB 设备），必须安装虚拟总线驱动（VHCI Driver）。本项目集成了 `usbip-win2` 作为客户端驱动方案。

## ⚠️ 重要提示
`usbip-win2` 驱动尚未获得微软官方签名（WHLK 认证需要企业证书）。因此，在安装和使用前，必须启用 Windows 的**测试签名模式**。

**请勿在生产环境或对安全性要求极高的系统上操作。**

## 1. 启用测试模式
在管理员权限的 CMD 或 PowerShell 中执行：
```powershell
bcdedit /set testsigning on
```
执行后**必须重启电脑**才能生效。重启后，桌面右下角会出现“测试模式”的水印。

## 2. 构建驱动与工具
我们提供了一个脚本来自动下载并构建 `usbip-win2`。

### 环境要求
- Visual Studio 2022
- Windows Driver Kit (WDK)
- Git

### 构建步骤
1. 打开 `x64 Native Tools Command Prompt for VS 2022`。
2. 进入项目目录的 `scripts` 文件夹。
3. 运行构建脚本：
   ```powershell
   powershell -ExecutionPolicy Bypass -File .\build_usbip_win2.ps1
   ```
4. 构建成功后，产物位于 `driver_temp\usbip-win2\x64\Release`。

## 3. 安装驱动

### A. 安装测试证书
构建生成的驱动使用自签名证书 `usbip.pfx`。
1. 进入 `driver_temp\usbip-win2\drivers\package` 目录。
2. 双击 `usbip.pfx`（或右键安装）。
3. 密码输入：`usbip`。
4. **关键步骤**：将证书存储在 **"受信任的根证书颁发机构"** 和 **"受信任的发布者"** 中。

### B. 安装驱动文件
1. 找到构建输出目录 `driver_temp\usbip-win2\x64\Release`。
2. 里面应该包含 `usbip2_ude.sys`, `usbip2_ude.inf`, `usbip.exe` 等文件。
3. 按照 `usbip-win2` 的说明，可以使用 `devnode.exe` (如果构建了) 或 `pnputil` 安装。
   或者运行 `usbip.exe install` (如果支持)。
   
   **推荐手动安装方法：**
   使用 `devcon` 或 `pnputil`:
   ```powershell
   pnputil /add-driver usbip2_ude.inf /install
   ```
   或者在设备管理器中添加过时硬件 -> 手动选择驱动。

## 4. 使用方法
驱动安装成功后，你可以使用 `usb_share.exe` 进行连接：
1. 启动 `usb_share.exe`。
2. 切换到 "Remote Connect" 页签。
3. 输入远程服务器 IP。
4. 点击 "Connect"。
   - 程序会自动调用底层的 `usbip.exe` (需放在同级目录或 PATH 中) 来挂载设备。

## 5. 卸载
如果不再使用，请卸载驱动并关闭测试模式：
1. 在设备管理器中卸载 "USB/IP VHCI" 设备。
2. 关闭测试模式：
   ```powershell
   bcdedit /set testsigning off
   ```
3. 重启电脑。
