#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

#include <QObject>
#include <QSystemTrayIcon>
#include <utility>

/**
 * 使用方法：
 *
 *  m_trayIcon = new SystemTrayIcon();
 *  m_trayIcon->settrayIconPath(QPixmap(":/res/favicon.ico"));
 *  m_trayIcon->setMainWindow(this);
 *  m_trayIcon->createTrayIcon();
 *
 */

struct tagTrayIconMenu
{
    tagTrayIconMenu() {}
    tagTrayIconMenu(QIcon picon, QString pname)
        : icon(std::move(picon)),name(std::move(pname)) {}

    QIcon icon;
    QString name;
};

class QTimer;
class SystemTrayIcon : public QObject
{
    Q_OBJECT
public:
    explicit SystemTrayIcon(QObject *parent = nullptr);
    ~SystemTrayIcon() override = default;

    // 设置图标资源路径
    void setTrayIconPath(const QPixmap &iconPath) { _trayIconPath = iconPath; }
    // 设置主窗口
    void setMainWindow(QWidget *pMainWindow) { MainWindow = pMainWindow; }
    // 创建托盘图标
    void createTrayIcon(QVector<tagTrayIconMenu> items=QVector<tagTrayIconMenu>());
    // 停止托盘闪烁
    void stopFlashingTrayIcon();
    // 显示提示信息
    void showMessage(const QString &title,
                     const QString &message,
                     QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information,
                     int millisecondsTimeoutHint = 10000);
    // 闪烁图标并提示信息
    void flashingMessage(const QString &title="",
                         const QString &message="",
                         QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information,
                         int millisecondsTimeoutHint = 10000);

signals:
    // 菜单选择消息
    void signal_menuitemselected(const QString &state);
    // trayIcon事件处理
    void signal_trayIconeventprocessed(QSystemTrayIcon::ActivationReason reason);

private slots:
    // 检测鼠标是否在托盘内部
    void slot_checkTrayIconHover();

    // 修改在线状态
    void slot_modifyState(const QString& state);

    // 退出应用程序
    void slot_quitApp();

    // 托盘图标被激活
    void slot_trayIconActivated(QSystemTrayIcon::ActivationReason reason);

    // 无消息
    void slot_NoMessage();

    void slot_flashingTrayIcon();

private:
    // 托盘闪烁
    void flashingTrayIcon(const QPixmap& pixmap, int msec=500);
    // 建立一个空的图标
    void createEmptyIcon();

private:
    // 窗口消息提示窗口
    //MessageTipForm*    _messageTipForm;
    QWidget *MainWindow;

    // 检测托盘与鼠标位置定时器
    QTimer*    _checkTimer;

    // 托盘闪烁控制
    bool    _showIcon;
    // 当前托盘图标
    QPixmap    _pixmap;
    // 闪烁定时器
    QTimer*    _flashTimer;
    // 空图标
    QPixmap _emptyIcon;

    // 托盘图标对象
    QSystemTrayIcon*    _trayIcon;
    QPixmap _trayIconPath;

    QMenu*    trayIconMenu;

};

#endif // SYSTEMTRAYICON_H
