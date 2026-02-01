# Enhanced Driver Installation Script
$DriverBinPath = Join-Path $PSScriptRoot "..\src\driver\bin\stub"
$InfPath = Join-Path $DriverBinPath "usbip_stub.inf"

if (-not (Test-Path $InfPath)) {
    Write-Host "Error: Driver binaries not found at $DriverBinPath" -ForegroundColor Red
    Write-Host "Please download pre-compiled drivers and place them in src/driver/bin/stub/" -ForegroundColor Yellow
    exit 1
}

Write-Host "Installing USBIP Stub Driver from $InfPath..." -ForegroundColor Cyan

# Try pnputil (modern Windows)
pnputil /add-driver $InfPath /install

if ($LASTEXITCODE -ne 0) {
    Write-Host "Installation failed. Please ensure you are running as Administrator and drivers are signed." -ForegroundColor Red
} else {
    Write-Host "Driver installed successfully." -ForegroundColor Green
}