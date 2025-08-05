/**
 * @file MyTrayIcon.cpp
 * @brief 实现 MyTrayIcon 类，提供系统托盘图标功能
 * @author WeiWang
 * @date 2025-03-13
 * @version 1.0
 */

#include "MyTrayIcon.h"
#include "ElaMenu.h"

#include <QDesktopServices>
#include <QPainter>
#include <QTimer>
#include <QUrl>

/**
 * @brief 构造函数，初始化系统托盘图标
 * @param parent 父控件指针，默认为 nullptr
 */
MyTrayIcon::MyTrayIcon(QWidget *parent)
    : QSystemTrayIcon(parent)
    , m_showIcon(false)
    , m_checkTimer(new QTimer(this))
    , m_flashTimer(new QTimer(this))
{
    initSysTray();      ///< 初始化托盘
    initSysTrayMenu();  ///< 初始化托盘菜单
    show();             ///< 显示托盘图标

    // 连接鼠标位置检测定时器
    connect(m_checkTimer, &QTimer::timeout, this, &MyTrayIcon::checkTrayIconHover);
    m_checkTimer->setInterval(500);

    // 连接闪烁定时器
    connect(m_flashTimer, &QTimer::timeout, this, &MyTrayIcon::onFlashingTrayIcon);
}

/**
 * @brief 初始化系统托盘图标
 */
void MyTrayIcon::initSysTray()
{
    this->setToolTip(tr("我的酷狗")); ///< 设置工具提示
    this->m_trayIcon = QIcon(QStringLiteral(":/Res/window/windowIcon.ico"));
    this->setIcon(this->m_trayIcon); ///< 设置托盘图标

    // 连接托盘激活信号
    connect(this, &MyTrayIcon::activated, this, &MyTrayIcon::onIconActivated);

    // 连接消息框点击信号，显示主窗口
    connect(this, &MyTrayIcon::messageClicked, [this]
    {
        emit active();
    });

    // 连接显示消息信号
    connect(this, &MyTrayIcon::showTrayMessage, this, &MyTrayIcon::showMessage);
}

/**
 * @brief 初始化系统托盘菜单
 */
void MyTrayIcon::initSysTrayMenu()
{
    // 以下为调试用 MyMenu 代码，当前未启用
    // auto menu = new MyMenu(MyMenu::MenuKind::TrayIcon, this->m_pParent);
    // this->m_trayMenu = menu->getMenu<TrayIconMenu>();

    this->m_trayMenu = new ElaMenu; ///< 创建托盘菜单
    this->m_trayMenu->setOpacity(1); ///< 设置菜单透明度
    this->m_trayMenu->setMenuItemHeight(30); ///< 设置菜单项高度
    this->m_trayMenu->setMenuItemHoveredBackgroundColor(QColor(0x0066FF)); ///< 设置菜单项悬停时的颜色
    this->m_trayMenu->setMenuItemHoveredFontColor(Qt::white); ///< 设置菜单项悬停时的颜色

    QAction *action = nullptr;

    // 添加“打开我的酷狗”菜单项
    action = this->m_trayMenu->addElaIconAction(ElaIconType::IconName::House, tr("打开我的酷狗"));
    connect(action, &QAction::triggered, this, [this]
    {
        emit active();
    });

    // 添加“打开/关闭声音”菜单项
    action = this->m_trayMenu->addElaIconAction(ElaIconType::IconName::Volume, tr("打开/关闭声音"));
    connect(action, &QAction::triggered, this, [this]
    {
        this->m_trayMenu->setPreventHide(true); ///< 阻止菜单关闭
        // qDebug() << "MyTrayIcon 托盘图标点击: " << (flag ? "静音" : "开启声音"); ///< 调试用
        this->m_flagVolume = !this->m_flagVolume;
        emit noVolume(this->m_flagVolume);
    });

    this->m_trayMenu->addSeparator();

    // 添加“关于我的酷狗”菜单项
    action = this->m_trayMenu->addElaIconAction(ElaIconType::IconName::CircleInfo, tr("关于我的酷狗"));
    connect(action, &QAction::triggered, this, [this]
    {
        this->m_trayMenu->setPreventHide(true); ///< 阻止菜单关闭
        // qDebug() << "MyTrayIcon 托盘图标点击: " << (flag ? "关于我的酷狗" : "关闭关于我的酷狗"); ///< 调试用
        m_flagDialogShow = !m_flagDialogShow;
        emit showAboutDialog(m_flagDialogShow);
    });

    // 添加“前往我的酷狗”菜单项
    action = this->m_trayMenu->addElaIconAction(ElaIconType::IconName::LocationArrow, tr("前往我的酷狗"));
    connect(action, &QAction::triggered, this, [this]
    {
        QDesktopServices::openUrl(QUrl("https://gitee.com/a-mo-xi-wei/KuGouApp"));
    });

    this->m_trayMenu->addSeparator();

    // 添加“帮助”菜单项
    action = this->m_trayMenu->addElaIconAction(ElaIconType::IconName::CircleQuestion, tr("帮助"));
    connect(action, &QAction::triggered, this, [this]
    {
        this->m_trayMenu->setPreventHide(true); ///< 阻止菜单关闭
    });

    // 添加“意见反馈”菜单选项
    action = this->m_trayMenu->addElaIconAction(ElaIconType::IconName::FileSignature, tr("意见反馈"));
    connect(action, &QAction::triggered, this, [this]
    {
        this->m_trayMenu->setPreventHide(true); ///< 阻止菜单关闭
    });
    // 添加“检查更新”菜单项
    action = this->m_trayMenu->addElaIconAction(ElaIconType::IconName::Rotate, QString("检查更新"));
    action->setProperty("showRedDot", true);  // 标记显示红点
    connect(action, &QAction::triggered, this, [this]
    {
        this->m_trayMenu->setPreventHide(true); ///< 阻止菜单关闭
    });

    this->m_trayMenu->addSeparator();

    // 添加“锁定酷狗”菜单项
    action = this->m_trayMenu->addElaIconAction(ElaIconType::IconName::Thumbtack, tr("锁定酷狗"));
    connect(action, &QAction::triggered, this, [this, action]
    {
        this->m_trayMenu->setPreventHide(true); ///< 阻止菜单关闭
        m_flagPin = !m_flagPin;
        if (m_flagPin) action->setText("解锁酷狗");
        else action->setText("锁定酷狗");
        emit pinTheWindow(m_flagPin);
    });

    // 添加“切换账号”菜单项
    action = this->m_trayMenu->addElaIconAction(ElaIconType::IconName::UserGear, tr("切换账号"));
    connect(action, &QAction::triggered, this, [this]
    {
        this->m_trayMenu->setPreventHide(true); ///< 阻止菜单关闭
    });

    this->m_trayMenu->addSeparator();

    // 添加“退出我的酷狗”菜单项
    action = this->m_trayMenu->addElaIconAction(ElaIconType::IconName::PowerOff, tr("退出我的酷狗"));
    connect(action, &QAction::triggered, this, [this] { emit exit(); });

    this->setContextMenu(m_trayMenu); ///< 设置托盘右键菜单
}

/**
 * @brief 显示系统托盘消息框
 * @param title 消息标题
 * @param content 消息内容
 */
void MyTrayIcon::showMessage(const QString &title, const QString &content)
{
    QSystemTrayIcon::showMessage(title, content, QSystemTrayIcon::MessageIcon::Information, 1000); ///< 显示消息
    // 注意：Windows 10 下消息显示时长可能无效
}

/**
 * @brief 启动托盘图标闪烁
 * @param msec 闪烁间隔（毫秒）
 */
void MyTrayIcon::flashingTrayIcon(const int& msec)
{
    if (m_flashTimer->isActive())
        m_flashTimer->stop();
    m_flashTimer->setInterval(msec);
    m_flashTimer->start();
    m_checkTimer->start(); ///< 启动鼠标位置检测
}

/**
 * @brief 停止托盘图标闪烁
 */
void MyTrayIcon::stopFlashingTrayIcon()
{
    this->setIcon(m_trayIcon); ///< 恢复默认图标
    if (m_flashTimer->isActive())
        m_flashTimer->stop();
    if (m_checkTimer->isActive())
        m_checkTimer->stop();
}

/**
 * @brief 处理托盘图标激活事件
 * @param reason 激活原因
 */
void MyTrayIcon::onIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
    {
        emit active(); ///< 点击托盘显示主窗口
        // emit showTrayMessage(); ///< 调试用，测试消息显示
        // flashingTrayIcon(400); ///< 调试用，测试闪烁效果
    }
    // 以下为调试用右键菜单代码，当前未启用
    // else if (reason == QSystemTrayIcon::Context) {
    //     m_trayMenu->exec(QCursor::pos());
    //     activeSysTrayMenu();
    // }
}

/**
 * @brief 检查鼠标是否悬停在托盘图标上
 */
void MyTrayIcon::checkTrayIconHover()
{
    const QRect trayGeometry = this->geometry(); ///< 获取托盘区域
    const QPoint mousePos = QCursor::pos();
    if (trayGeometry.contains(mousePos))
    {
        stopFlashingTrayIcon(); ///< 鼠标悬停时停止闪烁
    }
}

/**
 * @brief 处理托盘图标闪烁逻辑
 */
void MyTrayIcon::onFlashingTrayIcon()
{
    if (m_showIcon)
    {
        this->setIcon(QIcon(m_emptyIcon)); ///< 显示空图标
        m_showIcon = false;
    }
    else
    {
        this->setIcon(QIcon(m_trayIcon)); ///< 显示托盘图标
        m_showIcon = true;
    }
}