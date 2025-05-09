//
// Created by WeiWang on 25-3-13.
//

#include "MyTrayIcon.h"
#include "ElaMenu.h"

#include <QDesktopServices>
#include <QGuiApplication>
#include <QScreen>
#include <QTimer>
#include <QUrl>

MyTrayIcon::MyTrayIcon(QWidget *parent)
    : QSystemTrayIcon(parent)
    , m_pParent(parent)
    , m_showIcon(false)
    , m_checkTimer(new QTimer(this))
    , m_flashTimer(new QTimer(this))
{
    initSysTray();		// 初始化托盘

    initSysTrayMenu();	// 处理菜单

    this->show();		// 显示托盘

    // 检测鼠标位置定时器
    connect(m_checkTimer, &QTimer::timeout, this, &MyTrayIcon::checkTrayIconHover);
    m_checkTimer->setInterval(500);

    connect(m_flashTimer, &QTimer::timeout, this,&MyTrayIcon::onFlashingTrayIcon);

}

void MyTrayIcon::initSysTray() {
    this->setToolTip(tr("我的酷狗"));		// 设置工具提示
    this->m_trayIcon = QIcon(QStringLiteral(":/Res/window/windowIcon.ico"));
    this->setIcon(this->m_trayIcon);	// 设置托盘图标

    // 点击托盘所作的操作
    connect(this, &MyTrayIcon::activated, this, &MyTrayIcon::onIconActivated);

    // 点击消息框后显示主页面
    connect(this, &MyTrayIcon::messageClicked, [this] {
        m_pParent->activateWindow();
        m_pParent->showNormal();
    });

    // 显示系统托盘消息框
    connect(this, &MyTrayIcon::showTrayMessage, this, &MyTrayIcon::showMessage);
}

void MyTrayIcon::initSysTrayMenu() {
    //auto menu = new MyMenu(MyMenu::MenuKind::TrayIcon,this->m_pParent);

    //this->m_trayMenu = menu->getMenu<TrayIconMenu>();

    this->m_trayMenu = new ElaMenu;
    this->m_trayMenu->setOpacity(0.85);
    this->m_trayMenu->setMenuItemHeight(27);
    //this->m_trayMenu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;

    action = this->m_trayMenu->addElaIconAction(ElaIconType::IconName::ArrowsMaximize,tr("打开我的酷狗"));
    connect(action, &QAction::triggered, this, [this]{
        m_pParent->activateWindow();
        m_pParent->showNormal();
    });

    action = this->m_trayMenu->addElaIconAction(ElaIconType::IconName::Volume,tr("打开/关闭声音"));
    connect(action, &QAction::triggered, this, [this]{
        this->m_trayMenu->setPreventHide(true); // 阻止菜单关闭
        //qDebug()<<"MyTrayIcon 托盘图标点击: "<<(flag?"静音":"开启声音");
        this->m_flagVolume = !this->m_flagVolume;
        emit noVolume(this->m_flagVolume);
    });

    action = this->m_trayMenu->addElaIconAction(ElaIconType::IconName::CircleInfo,tr("关于我的酷狗"));
    connect(action,&QAction::triggered,this,[this] {
        this->m_trayMenu->setPreventHide(true); // 阻止菜单关闭
        //qDebug()<<"MyTrayIcon 托盘图标点击: "<<(flag?"关于我的酷狗":"关闭关于我的酷狗");
        m_flagDialogShow = !m_flagDialogShow;
        emit showAboutDialog(m_flagDialogShow);
    });
    action = this->m_trayMenu->addElaIconAction(ElaIconType::IconName::LocationArrow,tr("前往我的酷狗"));
    connect(action, &QAction::triggered, this, [this] {
        QDesktopServices::openUrl(QUrl("https://gitee.com/a-mo-xi-wei/KuGouApp"));
    });
    action = this->m_trayMenu->addElaIconAction(ElaIconType::IconName::PowerOff,tr("退出我的酷狗"));
    connect(action, &QAction::triggered, this, [this] { emit exit(); });
    this->setContextMenu(m_trayMenu); // 可以选择设为托盘默认右键菜单
    //this->m_trayMenu->popup(QCursor::pos());
}

void MyTrayIcon::showMessage(const QString& title, const QString& content) {// 消息窗口标题 消息内容
    QSystemTrayIcon::showMessage(title,	content,
                                QSystemTrayIcon::MessageIcon::Information,1000);
    // 消息窗口图标
    // 消息窗口显示时长(经测试，win10系统下，没有效果)
}

void MyTrayIcon::flashingTrayIcon(const int& msec) {
    if (m_flashTimer->isActive())
        m_flashTimer->stop();
    m_flashTimer->setInterval(msec);
    m_flashTimer->start();

    // 开始检测鼠标与托盘区域
    m_checkTimer->start();
}

void MyTrayIcon::stopFlashingTrayIcon() {
    this->setIcon(m_trayIcon);
    if (m_flashTimer->isActive())
        m_flashTimer->stop();

    if (m_checkTimer->isActive())
        m_checkTimer->stop();
}

void MyTrayIcon::onIconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {		// 点击触发
        m_pParent->activateWindow();
        m_pParent->showNormal();
        //emit showTrayMessage(); //测试成功
        //flashingTrayIcon(400); //测试成功
    }
    // else if (reason == QSystemTrayIcon::Context) {	// 右键触发
    //     //m_trayMenu->exec(QCursor::pos());
    //     activeSysTrayMenu();
    // }
}

void MyTrayIcon::checkTrayIconHover() {
    const QRect trayGeometry = this->geometry(); // 需要平台特定方法获取实际区域
    const QPoint mousePos = QCursor::pos();
    if (trayGeometry.contains(mousePos)) {
        // 鼠标在托盘图标上，停止闪烁
        stopFlashingTrayIcon();
    }
}

void MyTrayIcon::onFlashingTrayIcon() {
    if (m_showIcon)
    {
        this->setIcon(QIcon(m_emptyIcon));
        m_showIcon = false;
    }
    else
    {
        this->setIcon(QIcon(m_trayIcon));
        m_showIcon = true;
    }
}
