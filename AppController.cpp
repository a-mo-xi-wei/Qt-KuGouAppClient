#include "AppController.h"
#include "MyTrayIcon.h"

#include <QFontDatabase>

AppController& AppController::instance()
{
    static AppController instance;
    return instance;
}

AppController::AppController()
    : m_trayIcon(new MyTrayIcon)
    , m_login(new LoginRegisterForm)
{
    initFontRes();
    // @note 生成客户端
    m_client = new KuGouClient;
    // @note 生成服务器
    m_server = new KuGouServer;
    m_login->hide();
    m_client->hide();

    connect(m_trayIcon, &MyTrayIcon::active, this, [this] {
        if (m_isLoginAccepted) {
            m_client->activateWindow();
            m_client->showNormal();
        } else {
            m_login->activateWindow();
        }
    });

    connect(m_trayIcon, &MyTrayIcon::exit, this, [this] {
        if (!m_isLoginAccepted) {
            m_login->close();
            qApp->quit();
            return;
        }
        m_client->activateWindow();
        m_client->showNormal();
        m_client->onTrayIconExit();
    });

    connect(m_trayIcon, &MyTrayIcon::pinTheWindow, this, [this](bool flag) {
        auto applyPin = [this,flag](QWidget* w) {
            if (!w) return;
            w->setWindowFlag(Qt::WindowStaysOnTopHint, flag);
            w->show(); ///< 更新 flag 后需重新 show
        };

        if (m_isLoginAccepted) {
            applyPin(m_client);
        }
        else {
            applyPin(m_login);
        }
    });
}

AppController::~AppController()
{
    // 可省略析构函数，QScopedPointer 自动释放资源
    // 若需要调试输出，可保留以下代码：
    delete m_trayIcon;
    delete m_login;
    delete m_client;
    delete m_server;
    qDebug() << "AppController destroyed.";
}

void AppController::start()
{
    m_login->show();

    connect(m_login, &QDialog::accepted, this, [this] {
        // ✅ 关键：继承置顶状态
        if (m_login->windowFlags() & Qt::WindowStaysOnTopHint) {
            m_client->setWindowFlag(Qt::WindowStaysOnTopHint, true);
        }
        m_client->show();
        connect(m_trayIcon, &MyTrayIcon::showAboutDialog,
                m_client, &MainWindow::onShowAboutDialog);

        connect(m_trayIcon, &MyTrayIcon::noVolume,
                m_client, &KuGouClient::onTrayIconNoVolume);
        m_isLoginAccepted = true;
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

