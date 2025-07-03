#include "AppController.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppController::instance().start();
    return a.exec();
}