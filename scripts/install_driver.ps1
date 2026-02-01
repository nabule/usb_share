# Install Driver Script
# Requires Administrator privileges

$ErrorActionPreference = "Stop"
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = (Resolve-Path "$scriptDir\..").Path
$stubInf = "$repoRoot\src\driver\usbip-win\driver\stub\usbip_stub.inf"

Write-Host "Installing USBIP Stub Driver..."
Write-Host "Path: $stubInf"

# Check for devcon (assuming it's in PATH or locally available)
if (Get-Command "devcon.exe" -ErrorAction SilentlyContinue) {
    # Replace 'USB\VID_80EE&PID_CAFE' with actual Hardware ID if different
    devcon.exe install $stubInf "USB\VID_80EE&PID_CAFE"
} else {
    Write-Warning "devcon.exe not found. Trying pnputil..."
    pnputil.exe /add-driver $stubInf /install
}

Write-Host "Installation attempt complete."
