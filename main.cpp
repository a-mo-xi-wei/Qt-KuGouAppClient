#include "AppController.h"
#include"SApp.h"

int main(int argc, char* argv[])
{
    // enable dpi scale
#if (QT_VERSION > QT_VERSION_CHECK(5, 14, 0))
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    SApp a(argc, argv);
    AppController::instance().start();
    return a.exec();
}
