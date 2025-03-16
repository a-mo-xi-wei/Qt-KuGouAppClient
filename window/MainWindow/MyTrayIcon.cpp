//
// Created by WeiWang on 25-3-13.
//

#include "MyTrayIcon.h"

#include <QGuiApplication>
#include <QScreen>
#include <QTimer>

#pragma execution_character_set("utf-8")    // qt支持显示中文

MyTrayIcon::MyTrayIcon(QWidget *parent)
    : QSystemTrayIcon(parent)
    , m_pParent(parent)
    , m_showIcon(false)
    , m_checkTimer(new QTimer(this))
    , m_flashTimer(new QTimer(this))
{
    initSysTray();		// 初始化托盘

    handleSysTrayMenu();	// 处理菜单

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

void MyTrayIcon::handleSysTrayMenu() {
    auto menu = new MyMenu(MyMenu::MenuKind::TrayIcon,this->m_pParent);

    this->m_trayMenu = menu->getMenu<TrayIconMenu>();

    connect(m_trayMenu, &TrayIconMenu::openWindow, this, [this]{
        m_pParent->activateWindow();
        m_pParent->showNormal();
    });

    connect(m_trayMenu, &TrayIconMenu::noVolume,this,[this](const bool& flag) {
        //qDebug()<<"MyTrayIcon 托盘图标点击: "<<(flag?"静音":"开启声音");
        emit noVolume(flag);
    });

    connect(m_trayMenu, &TrayIconMenu::exit, this, [this] { this->m_pParent->close(); });
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

void MyTrayIcon::getMenuPosition(const QPoint &pos) {
    this->m_menuPosition = pos;
    // 获取屏幕的尺寸
    const QScreen *screen = QGuiApplication::primaryScreen();
    const QRect screenGeometry = screen->geometry();

    // 计算菜单右侧的全局位置
    //int menuLeftPos = pos.x() - m_menu->width();
    const int menuRightPos = pos.x() + m_trayMenu->width();
    const int menuBottomPos = pos.y() + m_trayMenu->height();
    //int menuTopPos = pos.y() - m_menu->height();
    // 若菜单左侧超出屏幕左侧 (不存在)
    //if(menuLeftPos < 0) {
    //    // 动态调整菜单位置，使其在屏幕内显示
    //    m_menuPosition.setX(10);
    //}
    // 如果菜单右侧超出屏幕右侧
    if (menuRightPos > screenGeometry.right()) {
        // 动态调整菜单位置，使其在屏幕内显示
        const int offset = menuRightPos - screenGeometry.right() + 5;
        m_menuPosition.setX(pos.x() - offset);
    }
    // 如果菜单下侧超出屏幕下侧
    if (menuBottomPos > screenGeometry.bottom()) {
        // 动态调整菜单位置，使其在屏幕内显示
        const int offset = menuBottomPos - screenGeometry.bottom() + 5;
        m_menuPosition.setY(pos.y() - offset);
    }
}

void MyTrayIcon::onIconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {		// 点击触发
        m_pParent->activateWindow();
        m_pParent->showNormal();
        //emit showTrayMessage(); //测试成功
        //flashingTrayIcon(400); //测试成功
    }
    else if (reason == QSystemTrayIcon::Context) {	// 右键触发
        //m_trayMenu->exec(QCursor::pos());
        getMenuPosition(QCursor::pos());
        this->m_trayMenu->move(this->m_menuPosition);
        this->m_trayMenu->show();
    }
}

void MyTrayIcon::checkTrayIconHover() {
    QRect trayGeometry = this->geometry(); // 需要平台特定方法获取实际区域
    QPoint mousePos = QCursor::pos();
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
