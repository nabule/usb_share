#!/usr/bin/env python3
import os
import shutil
from pathlib import Path

# --- 配置部分 (需与安装脚本一致) ---
INSTALL_DIR = Path.cwd() / "qt_env"
TOOLCHAIN_FILE = Path.cwd() / "toolchain-mingw.cmake"
BUILD_DIR = Path.cwd() / "build_win_cross"

def remove_path(path):
    if path.exists():
        print(f"Removing: {path}")
        if path.is_dir():
            shutil.rmtree(path)
        else:
            os.remove(path)
    else:
        print(f"Not found (skipping): {path}")

def uninstall():
    print("--- 1. Cleaning Project Files ---")
    remove_path(TOOLCHAIN_FILE)
    remove_path(BUILD_DIR)
    remove_path(INSTALL_DIR)

    print("\n--- 2. Python Dependencies ---")
    print("To uninstall aqtinstall, run: pip uninstall aqtinstall -y")

    print("\n--- 3. System Dependencies (Manual Cleanup Recommended) ---")
    print("If you want to remove system packages, use your package manager:")
    print("Ubuntu/Debian: sudo apt remove cmake mingw-w64 ninja-build")
    print("Fedora: sudo dnf remove cmake mingw64-gcc-c++ ninja-build")

    print("\n✅ Cleanup Complete!")

if __name__ == "__main__":
    confirm = input("This will delete all cross-compilation environments (qt_env, toolchain, build_win_cross). Continue? (y/N): ")
    if confirm.lower() == 'y':
        uninstall()
    else:
        print("Aborted.")
