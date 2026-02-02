# Implementation Plan - Linux Cross-Compilation Setup

## Phase 1: Environment Setup Script Development
在此阶段，我们将开发 Python 脚本，用于自动化安装依赖和生成工具链文件。我们将采用测试驱动开发（TDD）模式，先编写测试来验证脚本的逻辑（如依赖检测、命令构建、文件生成），而不是直接运行耗时的安装过程。

- [x] Task: Create test harness for setup script [975a14c]
    - [x] Create `tests/scripts/test_setup_cross_compile.py`
    - [x] Implement mock tests for `install_system_deps` (verify apt/dnf commands are constructed correctly based on OS detection)
    - [x] Implement mock tests for `install_qt_libs` (verify `aqt` command arguments for Qt 6.6.2)
    - [x] Implement mock tests for `generate_toolchain` (verify file content generation)
- [ ] Task: Implement `scripts/setup_cross_compile.py` logic
    - [ ] Implement System Dependency Detection & Installation (apt/dnf)
    - [ ] Implement Python Dependency Installation (`aqtinstall`)
    - [ ] Implement Qt Library Download Logic (Qt 6.6.2, MinGW 64-bit)
    - [ ] Implement `toolchain-mingw.cmake` Generation Logic
- [ ] Task: Verify script execution
    - [ ] Run the script locally to perform the actual installation (Downloads Qt ~1GB+)
    - [ ] Verify `qt_env` directory structure and `toolchain-mingw.cmake` existence
- [ ] Task: Conductor - User Manual Verification 'Environment Setup Script Development' (Protocol in workflow.md)

## Phase 2: Build Integration & Documentation
在此阶段，我们将使用生成的工具链实际编译项目，修复 CMake 兼容性问题，并记录使用文档。

- [ ] Task: Verify CMake Configuration
    - [ ] Run CMake configure step using the new toolchain
    - [ ] Fix any `CMakeLists.txt` issues specific to MinGW environment (vs MSVC)
- [ ] Task: Verify Build Process
    - [ ] Run build command (e.g., `cmake --build`)
    - [ ] Ensure binaries (`usb_share.exe`, etc.) are generated successfully
- [ ] Task: Documentation
    - [ ] Create `docs/linux_cross_compile.md` with usage instructions
    - [ ] Update `README.md` to reference the cross-compilation guide
- [ ] Task: Conductor - User Manual Verification 'Build Integration & Documentation' (Protocol in workflow.md)
