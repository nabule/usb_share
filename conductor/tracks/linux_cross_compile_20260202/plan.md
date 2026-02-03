# Implementation Plan - Linux Cross-Compilation Setup

## Phase 1: Environment Setup Script Development
在此阶段，我们将开发 Python 脚本，用于自动化安装依赖和生成工具链文件。我们将采用测试驱动开发（TDD）模式，先编写测试来验证脚本的逻辑（如依赖检测、命令构建、文件生成），而不是直接运行耗时的安装过程。

- [x] Task: Create test harness for setup script [975a14c]
    - [x] Create `tests/scripts/test_setup_cross_compile.py`
    - [x] Implement mock tests for `install_system_deps` (verify apt/dnf commands are constructed correctly based on OS detection)
    - [x] Implement mock tests for `install_qt_libs` (verify `aqt` command arguments for Qt 6.6.2)
    - [x] Implement mock tests for `generate_toolchain` (verify file content generation)
- [x] Task: Implement `scripts/setup_cross_compile.py` logic [67f011c]
    - [x] Implement System Dependency Detection & Installation (apt/dnf)
    - [x] Implement Python Dependency Installation (`aqtinstall`)
    - [x] Implement Qt Library Download Logic (Qt 6.6.2, MinGW 64-bit)
    - [x] Implement `toolchain-mingw.cmake` Generation Logic
- [x] Task: Verify script execution [e8e69b0]
    - [x] Run the script locally to perform the actual installation (Downloads Qt ~1GB+)
    - [x] Verify `qt_env` directory structure and `toolchain-mingw.cmake` existence
- [x] Task: Conductor - User Manual Verification 'Environment Setup Script Development' (Protocol in workflow.md) [checkpoint: 81d1f1a]

## Phase 2: Build Integration & Documentation
在此阶段，我们将使用生成的工具链实际编译项目，修复 CMake 兼容性问题，并记录使用文档。

- [x] Task: Verify CMake Configuration [609927]
    - [x] Run CMake configure step using the new toolchain
    - [x] Fix any `CMakeLists.txt` issues specific to MinGW environment (vs MSVC)
    - [x] **Fix**: Install Linux Host Qt Tools (moc/rcc/uic) to resolve AUTOMOC execution failure.
- [ ] Task: Verify Build Process
    - [ ] Run build command (e.g., `cmake --build`)
    - [ ] Ensure binaries (`usb_share.exe`, etc.) are generated successfully
- [ ] Task: Documentation
    - [ ] Create `docs/linux_cross_compile.md` with usage instructions
    - [ ] Update `README.md` to reference the cross-compilation guide
- [ ] Task: Conductor - User Manual Verification 'Build Integration & Documentation' (Protocol in workflow.md)
