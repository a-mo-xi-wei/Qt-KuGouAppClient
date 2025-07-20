#include "AppController.h"

#include <QFontDatabase>

AppController& AppController::instance()
{
    static AppController instance;
    return instance;
}

AppController::AppController()
    : login(new LoginRegisterForm)
{
    // @note 初始化字体资源
    initFontRes();
    // @note 生成客户端
    client = new KuGouClient;
    // @note 生成服务器
    server = new KuGouServer;
    login->hide();
    client->hide();
}

AppController::~AppController()
{
    delete login;
    delete client;
    delete server;
}

void AppController::start()
{
    login->show();
    connect(login,&QDialog::accepted,this,[this] {
        //qDebug()<<"登录界面隐藏,显示客户端";
        client->show();  // 再启动客户端
    });

}

void AppController::initFontRes()  {
    // 加载 dialog.ttf 字体
    auto fontId = QFontDatabase::addApplicationFont(":/Res/font/dialog.ttf"); ///< 加载对话字体
    if (fontId == -1) {
        // @note 未使用，保留用于调试
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。";                    ///< 记录警告日志
        return;
    }
    // @note 未使用，保留用于调试
    // auto families = QFontDatabase::applicationFontFamilies(fontId);
    // qDebug() << "Loaded font families:" << families; // 输出实际字体名称  //AaSongLiuKaiTi

    // 加载 ElaAwesome.ttf 字体
    fontId = QFontDatabase::addApplicationFont(":/Res/font/ElaAwesome.ttf"); ///< 加载图标字体
    if (fontId == -1) {
        // @note 未使用，保留用于调试
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。";                    ///< 记录警告日志
        return;
    }
    // @note 未使用，保留用于调试
    // families = QFontDatabase::applicationFontFamilies(fontId);
    // qDebug() << "Loaded font families:" << families; // 输出实际字体名称  //ElaAwesome

    // 加载 qing-ning-you-yuan.ttf 字体
    fontId = QFontDatabase::addApplicationFont(":/Res/font/qing-ning-you-yuan.ttf"); ///< 加载优圆字体
    if (fontId == -1) {
        // @note 未使用，保留用于调试
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。";                    ///< 记录警告日志
        return;
    }
    // @note 未使用，保留用于调试
    // families = QFontDatabase::applicationFontFamilies(fontId);
    // qDebug() << "Loaded font families:" << families; // 输出实际字体名称  //YouYuan

    // 加载 JetBrainsMonoNerdFont-Bold.ttf 字体
    fontId = QFontDatabase::addApplicationFont(":/Res/font/JetBrainsMonoNerdFont-Bold.ttf"); ///< 加载代码字体
    if (fontId == -1) {
        // @note 未使用，保留用于调试
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。";                    ///< 记录警告日志
        return;
    }
    // @note 未使用，保留用于调试
    // families = QFontDatabase::applicationFontFamilies(fontId);
    // qDebug() << "Loaded font families:" << families; // 输出实际字体名称  //JetBrainsMono NF

    // 加载 chinese-simplify.ttf 字体
    fontId = QFontDatabase::addApplicationFont(":/Res/font/chinese-simplify.ttf"); ///< 加载简体中文字体
    if (fontId == -1) {
        // @note 未使用，保留用于调试
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。";                    ///< 记录警告日志
        return;
    }
    // @note 未使用，保留用于调试
    // auto families = QFontDatabase::applicationFontFamilies(fontId);
    // qDebug() << "Loaded font families:" << families; // 输出实际字体名称    //dingliehuobanfont
}