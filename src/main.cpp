#include <QApplication>
#include <QMessageBox>
#include "gui/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Check for common issues
    if (QCoreApplication::libraryPaths().isEmpty()) {
        // This might happen if plugins are not found
    }

    try {
        MainWindow w;
        w.show();
        return app.exec();
    } catch (const std::exception &e) {
        QMessageBox::critical(nullptr, "Startup Error", e.what());
        return 1;
    } catch (...) {
        QMessageBox::critical(nullptr, "Startup Error", "An unknown error occurred during startup.");
        return 1;
    }
}
