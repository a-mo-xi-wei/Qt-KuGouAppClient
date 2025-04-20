//
// Created by WeiWang on 25-3-13.
//

#ifndef MYTRAYICON_H
#define MYTRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>

#if defined(MYWINDOW_LIBRARY)
#define MYWINDOW_EXPORT Q_DECL_EXPORT
#else
#define MYWINDOW_EXPORT Q_DECL_IMPORT
#endif

class ElaMenu;

class MYWINDOW_EXPORT MyTrayIcon final : public QSystemTrayIcon {
    Q_OBJECT

public:
    explicit MyTrayIcon(QWidget *parent = nullptr);

private:
    void initSysTray(); // 初始化托盘

    void initSysTrayMenu(); // 处理菜单信号

    void showMessage(const QString& title, const QString& content); // 显示消息框

    void flashingTrayIcon(const int& msec);

    void stopFlashingTrayIcon();

signals:
    void showTrayMessage(QString title = "Notice",QString content = "How about today ?"); // 触发显示系统托盘消息框

    void noVolume(const bool &flag);

    void showAboutDialog(const bool &flag);

    void exit();

private slots:
    void onIconActivated(QSystemTrayIcon::ActivationReason reason); // 处理事件函数

    void checkTrayIconHover();

    void onFlashingTrayIcon();

private:
    QWidget *m_pParent{}; // 父类，用于显示窗体
    ElaMenu *m_trayMenu{}; // 菜单
    //菜单位置
    QPoint m_menuPosition;
    // 托盘闪烁控制
    bool    m_showIcon;
    QIcon   m_emptyIcon{};
    QIcon   m_trayIcon{};
    // 检测托盘与鼠标位置定时器
    QTimer* m_checkTimer;
    // 闪烁定时器
    QTimer* m_flashTimer{};
    //关于 对话框 显示与否
    bool    m_flagDialogShow = false;
    bool    m_flagVolume = false;

};


#endif //MYTRAYICON_H