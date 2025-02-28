#include "KuGouApp.h"
#include "logger.hpp"

#include <QApplication>
//void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
//    QByteArray localMsg = msg.toLocal8Bit();
//    fprintf(stderr, "[Qt Message] %s (File: %s, Line: %d, Function: %s)\n",
//            localMsg.constData(), context.file, context.line, context.function);
//}
int main(int argc, char *argv[])
{
    using namespace mylog;
    if (!logger::get().init("logs/test.log")) {
        return 1;
    }
    logger::get().set_level(spdlog::level::trace);
    //三种方式
    STREAM_DEBUG() << "STM_DEBUG : 程序开始（trace）" << 1;
    PRINT_WARN("PRINT_WARN : 程序开始, %d", 1);
    LOG_INFO("LOG_INFO：程序开始 {}", 1);

    logger::get().set_level(spdlog::level::info);
    STREAM_DEBUG() << "STM_DEBUG : 程序开始（info）" << 2;
    PRINT_WARN("PRINT_WARN, %d", 2);
    LOG_INFO("LOG_INFO {}", 2);

    //qInstallMessageHandler(myMessageHandler);
    QApplication a(argc, argv);
    KuGouApp w;
    w.show();
    // call before spdlog static variables destroy
    logger::get().shutdown();
    return a.exec();
}
