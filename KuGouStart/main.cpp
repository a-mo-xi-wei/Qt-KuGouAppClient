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
    if (!logger::get().init("logs/main.log")) {
        return 1;
    }
    logger::get().set_level(spdlog::level::info);
    //三种方式
    STREAM_INFO() << "STREAM_INFO : 程序开始（info）" << 1;
    PRINT_INFO("PRINT_INFO : 程序开始（info）%d",1);
    LOG_INFO("LOG_INFO : 程序开始（info）{}" , 1);

    //qInstallMessageHandler(myMessageHandler);
    QApplication a(argc, argv);
    KuGouApp w;
    w.show();
    return a.exec();
}
