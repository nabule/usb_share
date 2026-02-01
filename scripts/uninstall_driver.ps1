# Uninstall Driver Script

$ErrorActionPreference = "Stop"

Write-Host "Uninstalling USBIP Stub Driver..."

# USBIP Stub typical hardware ID, verify with actual INF
$hardwareId = "USB\VID_80EE&PID_CAFE" 

if (Get-Command "devcon.exe" -ErrorAction SilentlyContinue) {
    devcon.exe remove $hardwareId
} else {
    Write-Warning "devcon.exe not found. Trying pnputil..."
    # pnputil removal is trickier as it needs the published name (oemXX.inf)
    # This is a placeholder for better logic
    Write-Host "Please use Device Manager to uninstall 'USBIP Stub' device."
}

Write-Host "Uninstallation attempt complete."
