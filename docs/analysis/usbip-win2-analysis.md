# usbip-win2 Technical Analysis Report

## 1. Executive Summary
`usbip-win2` is a complete rewrite of the USB/IP client for Windows, featuring a WHLK-certified kernel-mode driver (`usbip2_ude.sys`) that implements the USB Device Emulation (UDE) interface. Unlike the Linux implementation where userspace handles the TCP connection, `usbip-win2`'s driver handles the TCP/IP connection directly in kernel mode using Winsock Kernel (WSK).

This architecture provides better performance and reliability but changes how our client application must interact with the system. Instead of implementing a TCP client, our application must interface with the driver.

## 2. Architecture Overview

### 2.1 Components
-   **VHCI Driver (`usbip2_ude.sys`)**:
    -   Implements a Virtual Host Controller Interface.
    -   Handles TCP connections to remote USB/IP servers using WSK.
    -   Implements the USB/IP protocol (Client side).
    -   Exposes a device interface (`GUID_DEVINTERFACE_USB_HOST_CONTROLLER`) for userspace control.
-   **Userspace Library (`libusbip`)**:
    -   Provides C++ wrappers for communicating with the driver via `DeviceIoControl`.
    -   Implements `OP_REQ_DEVLIST` (Discovery) in userspace (via standard Winsock).
-   **CLI Tool (`usbip.exe`)**:
    -   A command-line tool built on top of `libusbip` for listing, attaching, and detaching devices.

### 2.2 Protocol Compliance
-   **Version**: The driver checks for `USBIP_VERSION` (0x0111) in the handshake.
-   **Handshake**:
    -   **Discovery**: Handled in userspace (`list -r`). Sends `OP_REQ_DEVLIST` (0x8005).
    -   **Attachment**: Handled in kernel mode. Driver sends `OP_REQ_IMPORT` (0x8003) upon receiving `IOCTL_PLUGIN_HARDWARE`.
-   **Data Transfer**: Uses `CMD_SUBMIT` and `CMD_UNLINK` standard USB/IP commands.

### 2.3 Driver Interaction (IOCTL)
The primary interface for "Attaching" a device is the `IOCTL_PLUGIN_HARDWARE` control code.
Input Structure: `usbip::vhci::ioctl::plugin_hardware`
```cpp
struct plugin_hardware {
    ULONG size;
    int port; // OUT
    char busid[32];
    char service[32]; // Port (e.g. "3240")
    char host[1025];  // Hostname/IP
};
```
When this IOCTL is sent, the driver:
1.  Resolves the hostname.
2.  Connects to the server.
3.  Sends `OP_REQ_IMPORT` for the specified `busid`.
4.  Creates a child PDO representing the imported USB device.

## 3. Integration Strategy for usb_share

### 3.1 Client Architecture (Machine B)
We should **not** rewrite the kernel driver logic in our userspace app. Instead, we should leverage the robust `usbip-win2` driver.

**Proposed Workflow:**
1.  **Driver Installation**: Deploy `usbip2_ude.sys` and install it (requires Test Signing or valid cert).
2.  **Discovery (Qt App)**:
    -   Use existing `NetworkTransport` (or new logic) to connect to Server A (TCP 3240).
    -   Send `OP_REQ_DEVLIST` to get the list of available BusIDs.
    -   Parse response and display to user.
3.  **Attachment (Qt App)**:
    -   User selects a device.
    -   Qt App calls `usbip.exe attach -r <IP> -b <BusID>` (Simplest Integration).
    -   *Alternative (Advanced)*: Qt App opens `GUID_DEVINTERFACE_USB_HOST_CONTROLLER` handle and calls `DeviceIoControl` directly, bypassing `usbip.exe`.

### 3.2 Server Architecture (Machine A)
-   **Option A**: Use standard Linux `usbipd` (if A is Linux).
-   **Option B**: Use `usbipd-win` (older project) or similar server implementation on Windows. `usbip-win2` repository **removed the server (stub driver)** component.
    -   *Critical Risk*: `usbip-win2` is Client-only. We need a Windows Server solution.
    -   We must ensure our "MVP Core" server (if based on `usbip-win` old stub) is compatible.
    -   The analysis confirms `usbip-win2` speaks standard protocol, so it *should* work with any standard server.

## 4. Build Requirements
-   Visual Studio 2022 with WDK.
-   Git for Windows.
-   Test Certificate for driver signing.

## 5. Conclusion
`usbip-win2` is a viable and high-quality solution for the Windows Client (VHCI).
**Recommendation**: Adopt `usbip-win2` for the client side. Ensure we have a working Windows Server (from another source, e.g., `dorssel/usbipd-win` or `cezanne/usbip-win`).
