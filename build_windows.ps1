# All-in-one Windows Build Script (Improved Path Injection)
$SearchRoot = "C:\Qt"

Write-Host "Scanning Qt and Toolchains..."

$CMakeBin = Get-ChildItem -Path $SearchRoot -Filter "cmake.exe" -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1 -ExpandProperty DirectoryName
$MinGWBin = Get-ChildItem -Path $SearchRoot -Filter "g++.exe" -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1 -ExpandProperty DirectoryName
$QtLibBin = Get-ChildItem -Path $SearchRoot -Filter "Qt6Core.dll" -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1 -ExpandProperty DirectoryName

if (-not ($CMakeBin -and $MinGWBin -and $QtLibBin)) {
    Write-Host "Error: Could not find complete environment in $SearchRoot"
    exit 1
}

# The actual Qt root (e.g., C:\Qt\6.6.2\mingw_64)
$QtRoot = Split-Path -Parent $QtLibBin

# Inject environment variables for the current session
$env:Path = "$CMakeBin;$MinGWBin;$QtLibBin;" + $env:Path

Write-Host "Found CMake: $CMakeBin"
Write-Host "Found MinGW: $MinGWBin"
Write-Host "Found Qt Root: $QtRoot"

# Start Build
$BuildDir = "build_windows"
if (Test-Path $BuildDir) { 
    Write-Host "Cleaning old build directory..."
    Remove-Item -Recurse -Force $BuildDir 
}
New-Item -ItemType Directory -Path $BuildDir
cd $BuildDir

Write-Host "Configuring Project..."
# Pass BOTH CMAKE_PREFIX_PATH and the specific Qt6_DIR to be absolutely sure
cmake -G "MinGW Makefiles" `
      -DCMAKE_PREFIX_PATH="$QtRoot" `
      -DQt6_DIR="$QtRoot/lib/cmake/Qt6" `
      ..

Write-Host "Building..."
cmake --build . --parallel

if ($LASTEXITCODE -eq 0) {
    Write-Host "`nRunning windeployqt to package dependencies..." -ForegroundColor Cyan
    $WinDeployQt = Get-ChildItem -Path $SearchRoot -Filter "windeployqt.exe" -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1 -ExpandProperty FullName
    if ($WinDeployQt) {
        & $WinDeployQt --no-compiler-runtime usb_share.exe
        
        Write-Host "Copying MinGW runtime DLLs..." -ForegroundColor Cyan
        $MinGwDlls = @("libgcc_s_seh-1.dll", "libstdc++-6.dll", "libwinpthread-1.dll")
        foreach ($dll in $MinGwDlls) {
            $dllSource = Get-ChildItem -Path $MinGWBin -Filter $dll -ErrorAction SilentlyContinue | Select-Object -First 1 -ExpandProperty FullName
            if ($dllSource) {
                Copy-Item $dllSource . -Force
                Write-Host "Copied $dll"
            }
        }
    } else {
        Write-Host "Warning: windeployqt.exe not found. You might need to manually copy Qt DLLs." -ForegroundColor Yellow
    }

    Write-Host "`nBuild Successful! Executable is in: $BuildDir" -ForegroundColor Green
} else {
    Write-Host "Build Failed."
}