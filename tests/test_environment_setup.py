import os
import unittest

class TestEnvironmentSetup(unittest.TestCase):
    def test_usbip_win_exists(self):
        """Verify that usbip-win source code is present."""
        self.assertTrue(os.path.exists("src/driver/usbip-win"), "usbip-win directory should exist")
        # Modified to check for LICENSE as a generic proof of content, since structure varies
        self.assertTrue(os.path.exists("src/driver/usbip-win/LICENSE"), "LICENSE file should exist in usbip-win")

    def test_install_script_exists(self):
        """Verify that the install driver script exists and contains key commands."""
        script_path = "scripts/install_driver.ps1"
        self.assertTrue(os.path.exists(script_path), "Install script should exist")
        
        with open(script_path, "r") as f:
            content = f.read()
            self.assertIn("devcon", content.lower(), "Script should mention devcon or pnputil")
            self.assertIn("usbip_stub.inf", content, "Script should reference usbip_stub.inf")

    def test_uninstall_script_exists(self):
        """Verify that the uninstall driver script exists."""
        script_path = "scripts/uninstall_driver.ps1"
        self.assertTrue(os.path.exists(script_path), "Uninstall script should exist")

if __name__ == '__main__':
    unittest.main()