#include "AppController.h"
#include"SApp.h"

// 自定义消息处理程序
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        //要打印的信息，文件名字，文件行号，所在函数
        fprintf(stderr,
                "Debug: %s (%s:%u, %s)\n",
                localMsg.constData(),
                context.file,
                context.line,
                context.function);
        break;
    case QtInfoMsg: fprintf(stderr,
                            "Info: %s (%s:%u, %s)\n",
                            localMsg.constData(),
                            context.file,
                            context.line,
                            context.function);
        break;
    case QtWarningMsg: fprintf(stderr,
                               "Warning: %s (%s:%u, %s)\n",
                               localMsg.constData(),
                               context.file,
                               context.line,
                               context.function);
        break;
    case QtCriticalMsg: fprintf(stderr,
                                "Critical: %s (%s:%u, %s)\n",
                                localMsg.constData(),
                                context.file,
                                context.line,
                                context.function);
        break;
    case QtFatalMsg: fprintf(stderr,
                             "Fatal: %s (%s:%u, %s)\n",
                             localMsg.constData(),
                             context.file,
                             context.line,
                             context.function);
        abort();
    }
    fflush(stderr);
}

int main(int argc, char *argv[])
{
    // enable dpi scale
#if (QT_VERSION > QT_VERSION_CHECK(5, 14, 0))
    QApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    SApp a(argc, argv);
#if QT_DEBUG
    qInstallMessageHandler(myMessageOutput);
    // 打印信息
    qDebug("This is a debug message.");
    qWarning("This is a warning message.");
    qCritical("This is a critical message.");
#endif
    AppController::instance().start();
    return a.exec();
}