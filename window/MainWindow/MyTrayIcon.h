//
// Created by WeiWang on 25-3-13.
//

#ifndef MYTRAYICON_H
#define MYTRAYICON_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>

class MyTrayIcon final : public QSystemTrayIcon {
    Q_OBJECT

public:
    explicit MyTrayIcon(QWidget *parent = nullptr);

    private slots:
        void onIconActivated(QSystemTrayIcon::ActivationReason reason); // 处理事件函数

private:
    void initSysTray(); // 初始化托盘

    void addSysTrayMenu(); // 添加菜单

    void showMessage(); // 显示消息框

    void flashingTrayIcon(const int& msec);

    void stopFlashingTrayIcon();

    signals:
        void showTrayMessage(); // 触发显示系统托盘消息框

    private slots:
        void checkTrayIconHover();

    void onFlashingTrayIcon();


private:
    QWidget *m_pParent{}; // 父类，用于显示窗体
    QMenu *m_trayMenu{}; // 菜单
    // 托盘闪烁控制
    bool    m_showIcon;
    QIcon   m_emptyIcon{};
    QIcon   m_trayIcon{};
    // 检测托盘与鼠标位置定时器
    QTimer*    m_checkTimer;
    // 闪烁定时器
    QTimer*    m_flashTimer{};
};


#endif //MYTRAYICON_H