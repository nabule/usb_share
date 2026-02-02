<#
.SYNOPSIS
    Automates the build of usbip-win2.
.DESCRIPTION
    Clones vadimgrn/usbip-win2 and builds it using MSBuild.
    Requires Visual Studio 2022 with WDK.
#>

$ErrorActionPreference = "Stop"

$RepoUrl = "https://github.com/vadimgrn/usbip-win2.git"
# Path relative to script location
$ScriptDir = Split-Path $MyInvocation.MyCommand.Path
$ProjectRoot = Resolve-Path "$ScriptDir\.."
$SourceDir = "$ProjectRoot\driver_temp\usbip-win2"

if (-not (Test-Path $SourceDir)) {
    Write-Host "Cloning usbip-win2 to $SourceDir..."
    git clone $RepoUrl $SourceDir
} else {
    Write-Host "Repo already exists at $SourceDir"
}

Push-Location $SourceDir

# Run bootstrap to restore Nuget packages
Write-Host "Running bootstrap.bat..."
cmd /c bootstrap.bat

# Build Solution
Write-Host "Building usbip_win2.sln (Release|x64)..."

# Note: The pfx is in drivers\package\usbip.pfx. 
# We rely on the project file picking it up or we might need to set it.
# The README says we need to set it in VS. 
# We can try to build and see if it fails, or pass properties.

$MSBuildArgs = @(
    "usbip_win2.sln",
    "/p:Configuration=Release",
    "/p:Platform=x64",
    "/t:Rebuild"
)

# Attempt to locate MSBuild (simple heuristic, better to run from Dev Cmd Prompt)
if (Get-Command "msbuild" -ErrorAction SilentlyContinue) {
    & msbuild $MSBuildArgs
} else {
    Write-Error "MSBuild not found in PATH. Please run this script from 'x64 Native Tools Command Prompt for VS 2022'."
}

if ($LASTEXITCODE -eq 0) {
    Write-Host "Build Successful."
    Write-Host "Binaries should be in $SourceDir\x64\Release"
} else {
    Write-Error "Build Failed."
}

Pop-Location
