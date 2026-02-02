import unittest
from unittest.mock import patch, MagicMock, mock_open
import sys
import os
from pathlib import Path

# 将根目录添加到路径以便导入 scripts
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../..')))

try:
    from scripts import setup_cross_compile
except ImportError:
    setup_cross_compile = None

class TestSetupCrossCompile(unittest.TestCase):

    def setUp(self):
        if setup_cross_compile is None:
            self.skipTest("setup_cross_compile.py not found or empty")

    @patch('shutil.which')
    @patch('subprocess.check_call')
    def test_install_system_deps_apt(self, mock_run, mock_which):
        # 模拟存在 apt-get
        mock_which.side_effect = lambda x: "/usr/bin/apt-get" if x == "apt-get" else None
        
        setup_cross_compile.install_system_deps()
        
        mock_run.assert_any_call(["sudo", "apt-get", "update"])
        mock_run.assert_any_call(["sudo", "apt-get", "install", "-y", "cmake", "mingw-w64", "ninja-build"])

    @patch('subprocess.check_call')
    def test_install_qt_libs(self, mock_run):
        setup_cross_compile.install_qt_libs()
        
        expected_cmd = [
            sys.executable, "-m", "aqt", "install-qt",
            "windows", "desktop", setup_cross_compile.QT_VERSION, setup_cross_compile.QT_ARCH,
            "--outputdir", str(setup_cross_compile.INSTALL_DIR)
        ]
        mock_run.assert_called_with(expected_cmd)

    @patch('builtins.open', new_callable=mock_open)
    @patch('shutil.which')
    @patch('pathlib.Path.exists')
    def test_generate_toolchain(self, mock_exists, mock_which, mock_file):
        mock_which.return_value = "/usr/bin/x86_64-w64-mingw32-g++"
        mock_exists.return_value = True
        
        setup_cross_compile.generate_toolchain()
        
        mock_file.assert_called_with("toolchain-mingw.cmake", "w")
        # 验证是否写入了关键内容
        handle = mock_file()
        calls = [call.args[0] for call in handle.write.call_args_list]
        full_content = "".join(calls)
        self.assertIn("CMAKE_SYSTEM_NAME Windows", full_content)
        self.assertIn("CMAKE_CXX_COMPILER", full_content)

if __name__ == '__main__':
    unittest.main()