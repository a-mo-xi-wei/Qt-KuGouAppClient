#include "KuGouApp.h"

#include <QApplication>
//void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
//    QByteArray localMsg = msg.toLocal8Bit();
//    fprintf(stderr, "[Qt Message] %s (File: %s, Line: %d, Function: %s)\n",
//            localMsg.constData(), context.file, context.line, context.function);
//}
int main(int argc, char *argv[])
{
    //qInstallMessageHandler(myMessageHandler);
    QApplication a(argc, argv);
    KuGouApp w;
    w.show();
    return a.exec();
}
