#include "SystemTrayIcon.h"
#include <QTimer>
#include <QSystemTrayIcon>
#include <QSignalMapper>
#include <QMessageBox>
#include <QApplication>
#include <QAction>
#include <QMenu>
#include <QPainter>

#if defined (WIN32)
#include <Windows.h>
#pragma comment(lib, "User32.lib")
#endif

#define IDD_EMPTYICON_WIDTH 48
#define IDD_EMPTYICON_HEIGHT 48

QWidget* getMainWindow()
{
    foreach (QWidget *w, qApp->topLevelWidgets())
    {
        if (QWidget* mainWin = qobject_cast<QWidget*>(w))
            return mainWin;
    }

    return nullptr;
}

SystemTrayIcon::SystemTrayIcon(QObject *parent) :
    QObject(parent),
    _showIcon(false),
    MainWindow(NULL),
    _emptyIcon(QPixmap(IDD_EMPTYICON_WIDTH,IDD_EMPTYICON_HEIGHT))
{
    // 检测鼠标位置定时器
    _checkTimer = new QTimer(this);
    connect(_checkTimer, SIGNAL(timeout()), this, SLOT(slot_checkTrayIconHover()));
    _checkTimer->setInterval(200);

    // 托盘图标闪烁定时器
    _flashTimer = new QTimer(this);
    connect(_flashTimer, SIGNAL(timeout()), this, SLOT(slot_flashingTrayIcon()));

    // 建立一个空的图标
    createEmptyIcon();
}

void SystemTrayIcon::slot_checkTrayIconHover()
{

}

/**
 * @brief SystemTrayIcon::createEmptyIcon 建立一个空的图标
 */
void SystemTrayIcon::createEmptyIcon()
{
    QPainter painter(&_emptyIcon);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.drawRect(QRect(0,0,IDD_EMPTYICON_WIDTH,IDD_EMPTYICON_HEIGHT));
}

/**
 * @brief SystemTrayIcon::showMessage 显示提示信息
 * @param title
 * @param message
 * @param icon
 * @param millisecondsTimeoutHint
 */
void SystemTrayIcon::showMessage(const QString &title,
                 const QString &message,
                 QSystemTrayIcon::MessageIcon icon,
                 int millisecondsTimeoutHint)
{
    if(_trayIcon == NULL) return;

    _trayIcon->showMessage(title,message,icon,millisecondsTimeoutHint);
}

void SystemTrayIcon::flashingTrayIcon(const QPixmap &pixmap, int msec)
{
    // 开始闪烁
    _pixmap = pixmap;
    if (_flashTimer->isActive())
        _flashTimer->stop();
    _flashTimer->setInterval(msec);
    _flashTimer->start();

    // 开始检测鼠标与托盘区域
    _checkTimer->start();
}

void SystemTrayIcon::stopFlashingTrayIcon()
{
    _trayIcon->setIcon(QIcon(_trayIconPath));
    if (_flashTimer->isActive())
        _flashTimer->stop();

    if (_checkTimer->isActive())
        _checkTimer->stop();
}

/**
 * @brief SystemTrayIcon::flashingMessage 闪烁图标并提示信息
 * @param title
 * @param message
 * @param icon
 * @param millisecondsTimeoutHint
 */
void SystemTrayIcon::flashingMessage(const QString &title,
                     const QString &message,
                     QSystemTrayIcon::MessageIcon icon,
                     int millisecondsTimeoutHint)
{
    if(!title.isEmpty() && !message.isEmpty())
        showMessage(title,message,icon,millisecondsTimeoutHint);

    flashingTrayIcon(_trayIconPath);
}

void SystemTrayIcon::slot_modifyState(const QString &state)
{
    emit signal_menuitemselected(state);
}

void SystemTrayIcon::slot_quitApp()
{
    qApp->exit(0);
}

void SystemTrayIcon::createTrayIcon(QVector<tagTrayIconMenu> items)
{
    // 创建托盘对象
    _trayIcon = new QSystemTrayIcon(this);
    _trayIcon->setIcon(QIcon(_trayIconPath));
    _trayIcon->setToolTip(MainWindow->windowTitle());

    connect(_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
        this, SLOT(slot_trayIconActivated(QSystemTrayIcon::ActivationReason)));

    // 建立信号槽
    QSignalMapper* trayMap = new QSignalMapper(this);
    connect(trayMap, SIGNAL(mapped(const QString&)), this, SLOT(slot_modifyState(const QString&)));

    if(!items.isEmpty())
    {
        trayIconMenu = new QMenu();

        for(int i=0;i<items.size();i++)
        {
            QAction* talkAction = new QAction(items[i].icon,items[i].name, this);
            connect(talkAction, SIGNAL(triggered()), trayMap, SLOT(map()));
            trayMap->setMapping(talkAction, items[i].name);

            trayIconMenu->addAction(talkAction);
        }

        _trayIcon->setContextMenu(trayIconMenu);
    }

    _trayIcon->show();
}

void SystemTrayIcon::slot_trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(QSystemTrayIcon::Trigger == reason)
    {
        //QMessageBox::warning(NULL, "提示", "激活托盘，此处应该弹出程序");
        if(MainWindow)
        {
            MainWindow->showNormal();

#if defined (WIN32)
            //设置窗口置顶
            ::SetWindowPos(HWND(MainWindow->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
            ::SetWindowPos(HWND(MainWindow->winId()), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
#endif
        }
    }

    emit signal_trayIconeventprocessed(reason);
}

void SystemTrayIcon::slot_flashingTrayIcon()
{
    if (_showIcon)
    {
        _trayIcon->setIcon(QIcon(_emptyIcon));
        _showIcon = false;
    }
    else
    {
        _trayIcon->setIcon(QIcon(_pixmap));
        _showIcon = true;
    }
}

void SystemTrayIcon::slot_NoMessage()
{
    stopFlashingTrayIcon();
}
