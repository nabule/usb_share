# MVP Manual Test Plan

## Prerequisites
- Two Windows PCs (A and B) on the same LAN.
- `usb_share.exe` installed on both.
- USBIP Stub driver installed on both (run `scripts/install_driver.ps1`).
- A physical USB device (e.g., Flash Drive or Mouse) plugged into PC A.

## Test Case 1: Basic Sharing
1. **PC A (Server):**
   - Run `usb_share.exe`.
   - Go to "Local Devices" tab.
   - Click "Refresh". Verify the USB device appears.
   - (Note: Sharing logic is currently stubbed/manual via usbip CLI in this MVP Phase).
   - Manually run `usbipd bind --busid <busid>` (or use UI if implemented).
   - Start the listener (implied by app start).

2. **PC B (Client):**
   - Run `usb_share.exe`.
   - Go to "Remote Connect" tab.
   - Enter IP of PC A.
   - Click "Connect".
   - Verify Status changes to "Connected".
   - Verify new hardware found sound/notification plays.
   - Verify device appears in Device Manager.

## Test Case 2: Auto-Reconnection
1. **Setup:** Establish connection as in Test Case 1.
2. **Action:** Disconnect network cable on PC B for 5 seconds, then reconnect.
3. **Expectation:**
   - App should detect socket error.
   - App should retry connecting.
   - Connection should restore automatically without user intervention.
