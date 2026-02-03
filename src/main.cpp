#include <QApplication>
#include <QMessageBox>
#include "gui/MainWindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include "gui/MainWindow.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    static QFile outFile("usb_share_debug.txt");
    if (!outFile.isOpen()) {
        outFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    }
    QTextStream ts(&outFile);
    QString typeStr;
    switch (type) {
        case QtDebugMsg: typeStr = "Debug"; break;
        case QtInfoMsg: typeStr = "Info"; break;
        case QtWarningMsg: typeStr = "Warning"; break;
        case QtCriticalMsg: typeStr = "Critical"; break;
        case QtFatalMsg: typeStr = "Fatal"; break;
    }
    ts << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ") << "[" << typeStr << "] " << msg << Qt::endl;
    ts.flush();
}

int main(int argc, char *argv[]) {
    // 每次启动时清空旧的日志文件
    {
        QFile logFile("usb_share_debug.txt");
        logFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        logFile.close();
    }

    qInstallMessageHandler(myMessageOutput);
    qDebug() << "Application starting...";
    qDebug() << "Working directory:" << QDir::currentPath();
    qDebug() << "Library paths:" << QCoreApplication::libraryPaths();

    QApplication app(argc, argv);
    
    // Check for common issues
    if (QCoreApplication::libraryPaths().isEmpty()) {
        qCritical() << "No library paths found!";
    }

    try {
        MainWindow w;
        w.show();
        qDebug() << "MainWindow shown, entering event loop.";
        return app.exec();
    } catch (const std::exception &e) {
        qCritical() << "Exception caught:" << e.what();
        QMessageBox::critical(nullptr, "Startup Error", e.what());
        return 1;
    } catch (...) {
        qCritical() << "Unknown exception caught during startup.";
        QMessageBox::critical(nullptr, "Startup Error", "An unknown error occurred during startup.");
        return 1;
    }
}
