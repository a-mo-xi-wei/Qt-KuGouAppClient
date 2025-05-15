/**
 * @file MyTrayIcon.h
 * @brief 定义 MyTrayIcon 类，提供系统托盘图标功能
 * @author WeiWang
 * @date 2025-03-13
 * @version 1.0
 */

#ifndef MYTRAYICON_H
#define MYTRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYWINDOW_LIBRARY)
#define MYWINDOW_EXPORT Q_DECL_EXPORT
#else
#define MYWINDOW_EXPORT Q_DECL_IMPORT
#endif

// 前向声明
class ElaMenu;
class QTimer;

/**
 * @class MyTrayIcon
 * @brief 自定义系统托盘图标类，继承自 QSystemTrayIcon，支持闪烁、菜单和消息显示
 */
class MYWINDOW_EXPORT MyTrayIcon final : public QSystemTrayIcon {
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化系统托盘图标
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit MyTrayIcon(QWidget *parent = nullptr);

private:
    /**
     * @brief 初始化系统托盘图标
     */
    void initSysTray();

    /**
     * @brief 初始化系统托盘菜单
     */
    void initSysTrayMenu();

    /**
     * @brief 显示系统托盘消息框
     * @param title 消息标题
     * @param content 消息内容
     */
    void showMessage(const QString &title, const QString &content);

    /**
     * @brief 启动托盘图标闪烁
     * @param msec 闪烁间隔（毫秒）
     */
    void flashingTrayIcon(const int &msec);

    /**
     * @brief 停止托盘图标闪烁
     */
    void stopFlashingTrayIcon();

signals:
    /**
     * @brief 显示托盘消息信号
     * @param title 消息标题，默认为 "Notice"
     * @param content 消息内容，默认为 "How about today ?"
     */
    void showTrayMessage(QString title = "Notice", QString content = "How about today ?");

    /**
     * @brief 音量开关信号
     * @param flag 是否静音
     */
    void noVolume(const bool &flag);

    /**
     * @brief 显示关于对话框信号
     * @param flag 是否显示
     */
    void showAboutDialog(const bool &flag);

    /**
     * @brief 退出信号，通知程序退出
     */
    void exit();

private slots:
    /**
     * @brief 处理托盘图标激活事件
     * @param reason 激活原因
     */
    void onIconActivated(QSystemTrayIcon::ActivationReason reason);

    /**
     * @brief 检查鼠标是否悬停在托盘图标上
     */
    void checkTrayIconHover();

    /**
     * @brief 处理托盘图标闪烁逻辑
     */
    void onFlashingTrayIcon();

private:
    QWidget *m_pParent{};          ///< 父控件，用于显示主窗口
    ElaMenu *m_trayMenu{};         ///< 托盘菜单
    QPoint m_menuPosition;         ///< 菜单显示位置
    bool m_showIcon;               ///< 闪烁状态标志
    QIcon m_emptyIcon{};           ///< 空图标，用于闪烁
    QIcon m_trayIcon{};            ///< 托盘图标
    QTimer *m_checkTimer;          ///< 鼠标位置检测定时器
    QTimer *m_flashTimer{};        ///< 闪烁定时器
    bool m_flagDialogShow = false; ///< 关于对话框显示标志
    bool m_flagVolume = false;     ///< 音量开关标志
};

#endif // MYTRAYICON_H