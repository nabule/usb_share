#!/usr/bin/env python3
import os
import shutil
from pathlib import Path

# --- 配置 ---
QT_VERSION = "6.6.2"
BASE_DIR = Path.cwd()
QT_BIN_DIR = BASE_DIR / "qt_env" / QT_VERSION / "mingw_64" / "bin"
QT_PLUGINS_DIR = BASE_DIR / "qt_env" / QT_VERSION / "mingw_64" / "plugins"
BUILD_DIR = BASE_DIR / "build_win_cross"
DIST_DIR = BASE_DIR / "dist_windows"

# 需要复制的核心 DLL 列表
REQUIRED_DLLS = [
    # Qt 核心库
    "Qt6Core.dll",
    "Qt6Gui.dll",
    "Qt6Widgets.dll",
    "Qt6Network.dll",
    # MinGW 运行时库 (非常重要)
    "libgcc_s_seh-1.dll",
    "libstdc++-6.dll",
    "libwinpthread-1.dll",
]

# 需要的核心插件 (否则界面无法显示)
REQUIRED_PLUGINS = {
    "platforms": ["qwindows.dll"],
    "styles": ["qwindowsvistastyle.dll"]
}

def deploy():
    if not DIST_DIR.exists():
        DIST_DIR.mkdir()
    
    print(f"--- 1. Copying Executables from {BUILD_DIR} ---")
    for exe in BUILD_DIR.glob("*.exe"):
        print(f"Copying: {exe.name}")
        shutil.copy2(exe, DIST_DIR)

    print(f"\n--- 2. Copying Qt & MinGW DLLs from {QT_BIN_DIR} ---")
    for dll in REQUIRED_DLLS:
        src = QT_BIN_DIR / dll
        if src.exists():
            print(f"Copying: {dll}")
            shutil.copy2(src, DIST_DIR)
        else:
            print(f"Warning: {dll} not found in {QT_BIN_DIR}")

    print(f"\n--- 3. Copying Plugins from {QT_PLUGINS_DIR} ---")
    for folder, dlls in REQUIRED_PLUGINS.items():
        plugin_dist_dir = DIST_DIR / folder
        plugin_dist_dir.mkdir(exist_ok=True)
        for dll in dlls:
            src = QT_PLUGINS_DIR / folder / dll
            if src.exists():
                print(f"Copying Plugin: {folder}/{dll}")
                shutil.copy2(src, plugin_dist_dir)
            else:
                print(f"Warning: Plugin {folder}/{dll} not found")

    print(f"\n✅ Deployment folder ready: {DIST_DIR}")
    print("Please copy the ENTIRE 'dist_windows' folder to your Windows machine and run 'usb_share.exe'.")

if __name__ == "__main__":
    if not BUILD_DIR.exists():
        print(f"Error: Build directory {BUILD_DIR} not found. Build the project first.")
    else:
        deploy()
