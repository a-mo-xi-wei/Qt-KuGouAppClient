//
// Created by WeiWang on 25-3-13.
//

#include "MyTrayIcon.h"

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

    addSysTrayMenu();	// 初始化菜单

    this->show();		// 显示托盘

    // 检测鼠标位置定时器
    connect(m_checkTimer, &QTimer::timeout, this, &MyTrayIcon::checkTrayIconHover);
    m_checkTimer->setInterval(500);

    connect(m_flashTimer, &QTimer::timeout, this,&MyTrayIcon::onFlashingTrayIcon);

}

void MyTrayIcon::onIconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {		// 点击触发
        m_pParent->activateWindow();
        m_pParent->showNormal();

    } else if (reason == QSystemTrayIcon::Context) {	// 右键触发
        m_trayMenu->exec(QCursor::pos());
    }
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

void MyTrayIcon::addSysTrayMenu() {
    m_trayMenu = new QMenu(m_pParent);		// 新建菜单，指定父对象
    // 新建菜单子项
    QAction *action1 = new QAction(m_trayMenu);
    QAction *action2 = new QAction(m_trayMenu);

    action1->setIcon(QIcon("show.png"));	// 设置菜单图标
    action1->setText(QString("Show"));		// 设置带单文本
    action2->setIcon(QIcon("close.png"));
    action2->setText(QString("Quit"));


    // 添加菜单子项
    m_trayMenu->addAction(action1);
    m_trayMenu->addAction(action2);

    // 显示窗体
    connect(action1, &QAction::triggered, [this](bool) {
        m_pParent->activateWindow();
        m_pParent->showNormal();
    });

    // 退出
    connect(action2, &QAction::triggered, [this](bool) { this->m_pParent->close(); });
}

void MyTrayIcon::showMessage() {
    QSystemTrayIcon::showMessage("Notice",				            // 消息窗口标题
                              "The form is minimized！",             // 消息内容
                              QSystemTrayIcon::MessageIcon::Information,// 消息窗口图标
                              1000);					            // 消息窗口显示时长(经测试，win10系统下，没有效果)
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
