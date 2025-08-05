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

    connect(m_trayIcon, &MyTrayIcon::active, this, [this]
    {
        if (m_isLoginAccepted)
        {
            m_client->activateWindow();
            m_client->showNormal();
        }
        else
        {
            m_login->activateWindow();
        }
    });

    connect(m_trayIcon, &MyTrayIcon::exit, this, [this]
    {
        if (!m_isLoginAccepted)
        {
            m_login->close();
            qApp->quit();
            return;
        }
        m_client->activateWindow();
        m_client->showNormal();
        m_client->onTrayIconExit();
    });

    connect(m_trayIcon, &MyTrayIcon::pinTheWindow, this, [this](bool flag)
    {
        auto applyPin = [this, flag](QWidget * w)
        {
            if (!w)
                return;
            w->setWindowFlag(Qt::WindowStaysOnTopHint, flag);
            w->show(); ///< 更新 flag 后需重新 show
        };

        if (m_isLoginAccepted)
        {
            applyPin(m_client);
        }
        else
        {
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

void AppController::showSystemLoginInfo()
{
    // 异步获取 IP 和位置
    auto* manager = new QNetworkAccessManager(this);
    const QNetworkRequest request(QUrl("http://api.ipify.org?format=json"));
    QNetworkReply* reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [ = ]()
    {
        if (reply->error() == QNetworkReply::NoError)
        {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QString ip        = doc.object().value("ip").toString();

            // 获取地理位置
            const QNetworkRequest geoRequest(QUrl("http://ip-api.com/json/" + ip));
            QNetworkReply* geoReply = manager->get(geoRequest);

            connect(geoReply, &QNetworkReply::finished, this, [ = ]()
            {
                if (geoReply->error() == QNetworkReply::NoError)
                {
                    QJsonDocument geoDoc = QJsonDocument::fromJson(geoReply->readAll());
                    QString location     = geoDoc.object().value("city").toString();
                    QString loginTime    = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                    QString deviceInfo   = QSysInfo::machineHostName();
                    bool isUnusualIp     = true; // 需实现判断逻辑

                    // 构造消息
                    QString message = QString("你的帐号于 %1 在%2IP地址 %3(%4) 登录%5，如非本人操作，建议尽快修改帐户密码。")
                                      .arg(loginTime)
                                      .arg(isUnusualIp ? "不常用的" : "")
                                      .arg(ip)
                                      .arg(location)
                                      .arg(deviceInfo.isEmpty() ? "" : QString("，设备：%1").arg(deviceInfo));

                    emit m_trayIcon->showTrayMessage("登录提示", message);
                }
                geoReply->deleteLater();
            });
        }
        reply->deleteLater();
    });
}

void AppController::start()
{
    m_login->show();

    // 连接登录成功的信号
    connect(m_login, &QDialog::accepted, this, [this]
    {
        showSystemLoginInfo();
        // 继承置顶状态
        if (m_login->windowFlags() & Qt::WindowStaysOnTopHint)
        {
            m_client->setWindowFlag(Qt::WindowStaysOnTopHint, true);
        }
        m_client->show();
        connect(m_trayIcon, &MyTrayIcon::showAboutDialog,
                m_client, &MainWindow::onShowAboutDialog);

        connect(m_trayIcon, &MyTrayIcon::noVolume,
                m_client, &KuGouClient::onTrayIconNoVolume);
        m_isLoginAccepted = true;
    });

    // 连接切换账号信号
    auto handleChangeAccount = [this]
    {
        // 隐藏客户端窗口
        m_client->hide();
        // 重置登录状态
        m_isLoginAccepted = false;
        // 断开与当前客户端相关的信号（避免重复连接）
        disconnect(m_trayIcon, &MyTrayIcon::showAboutDialog,
                   m_client, &MainWindow::onShowAboutDialog);
        disconnect(m_trayIcon, &MyTrayIcon::noVolume,
                   m_client, &KuGouClient::onTrayIconNoVolume);

        // 显示登录窗口
        m_login->show();

        // 显示切换账号提示
        emit m_trayIcon->showTrayMessage("切换账号", "请重新登录。");
    };
    connect(m_trayIcon, &MyTrayIcon::switchAccount, this, handleChangeAccount);
    connect(m_client, &KuGouClient::logOut, this, handleChangeAccount);

}

void AppController::initFontRes()
{
    // 加载 dialog.ttf 字体
    auto fontId = QFontDatabase::addApplicationFont(":/Res/font/TaiwanPearl-SemiBold.ttf"); ///< 加载对话字体
    if (fontId == -1)
    {
        // @note 未使用，保留用于调试
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。"; ///< 记录警告日志
        return;
    }
    // auto families = QFontDatabase::applicationFontFamilies(fontId).value(0);
    // qDebug() << "Loaded font families:" << families; // 输出实际字体名称  //TaiwanPearl

    // 加载 dialog.ttf 字体
    fontId = QFontDatabase::addApplicationFont(":/Res/font/dialog.ttf"); ///< 加载对话字体
    if (fontId == -1)
    {
        // @note 未使用，保留用于调试
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。"; ///< 记录警告日志
        return;
    }
    // @note 未使用，保留用于调试
    // auto families = QFontDatabase::applicationFontFamilies(fontId).value(0);
    // qDebug() << "Loaded font families:" << families; // 输出实际字体名称  //AaSongLiuKaiTi

    // 加载 ElaAwesome.ttf 字体
    fontId = QFontDatabase::addApplicationFont(":/Res/font/ElaAwesome.ttf"); ///< 加载图标字体
    if (fontId == -1)
    {
        // @note 未使用，保留用于调试
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。"; ///< 记录警告日志
        return;
    }
    // @note 未使用，保留用于调试
    // families = QFontDatabase::applicationFontFamilies(fontId).value(0);
    // qDebug() << "Loaded font families:" << families; // 输出实际字体名称  //ElaAwesome

    // 加载 qing-ning-you-yuan.ttf 字体
    fontId = QFontDatabase::addApplicationFont(":/Res/font/qing-ning-you-yuan.ttf"); ///< 加载优圆字体
    if (fontId == -1)
    {
        // @note 未使用，保留用于调试
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。"; ///< 记录警告日志
        return;
    }
    // @note 未使用，保留用于调试
    // families = QFontDatabase::applicationFontFamilies(fontId).value(0);
    // qDebug() << "Loaded font families:" << families; // 输出实际字体名称  //YouYuan

    // 加载 JetBrainsMonoNerdFont-Bold.ttf 字体
    fontId = QFontDatabase::addApplicationFont(":/Res/font/JetBrainsMonoNerdFont-Bold.ttf"); ///< 加载代码字体
    if (fontId == -1)
    {
        // @note 未使用，保留用于调试
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。"; ///< 记录警告日志
        return;
    }
    // @note 未使用，保留用于调试
    // families = QFontDatabase::applicationFontFamilies(fontId).value(0);
    // qDebug() << "Loaded font families:" << families; // 输出实际字体名称  //JetBrainsMono NF

    // 加载 chinese-simplify.ttf 字体
    fontId = QFontDatabase::addApplicationFont(":/Res/font/chinese-simplify.ttf"); ///< 加载简体中文字体
    if (fontId == -1)
    {
        // @note 未使用，保留用于调试
        qWarning() << "字体加载失败。。。";
        STREAM_WARN() << "字体加载失败。。。"; ///< 记录警告日志
        return;
    }
    // @note 未使用，保留用于调试
    // auto families = QFontDatabase::applicationFontFamilies(fontId).value(0);
    // qDebug() << "Loaded font families:" << families; // 输出实际字体名称    //dingliehuobanfont
}

