/**
 * @file ElaMenuBar.cpp
 * @brief 实现 ElaMenuBar 类，提供自定义菜单栏功能
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaMenuBar.h"

#include <QEvent>
#include <QPainter>
#include <QToolButton>

#include "ElaIcon.h"
#include "ElaMenu.h"
#include "ElaMenuBarStyle.h"

/**
 * @brief 构造函数，初始化菜单栏
 * @param parent 父控件指针，默认为 nullptr
 */
ElaMenuBar::ElaMenuBar(QWidget* parent)
    : QMenuBar(parent)
{
    setMouseTracking(true);
    setObjectName("ElaMenuBar");
    setStyle(new ElaMenuBarStyle(style()));
    QToolButton* tool = this->findChild<QToolButton*>();
    if (tool->objectName() == "qt_menubar_ext_button")
    {
        QMenu* oldMenu = tool->menu();
        ElaMenu* menu = new ElaMenu(this);
        menu->setObjectName("ElaExtendMenu");
        menu->setMenuItemHeight(27);
        if (oldMenu)
        {
            oldMenu->deleteLater();
        }
        tool->setMenu(menu);
    }
}

/**
 * @brief 析构函数，释放菜单栏资源
 */
ElaMenuBar::~ElaMenuBar()
{
}

/**
 * @brief 添加现有菜单
 * @param menu 菜单指针
 * @return 菜单对应的动作
 */
QAction* ElaMenuBar::addMenu(QMenu* menu)
{
    ElaMenu* elaMenu = dynamic_cast<ElaMenu*>(menu);
    if (elaMenu)
    {
        elaMenu->setMenuItemHeight(27);
    }
    return QMenuBar::addMenu(menu);
}

/**
 * @brief 添加新菜单（仅标题）
 * @param title 菜单标题
 * @return 新创建的菜单指针
 */
ElaMenu* ElaMenuBar::addMenu(const QString& title)
{
    ElaMenu* menu = new ElaMenu(title, this);
    menu->setMenuItemHeight(27);
    QMenuBar::addAction(menu->menuAction());
    return menu;
}

/**
 * @brief 添加新菜单（带图标）
 * @param icon 菜单图标
 * @param title 菜单标题
 * @return 新创建的菜单指针
 */
ElaMenu* ElaMenuBar::addMenu(const QIcon& icon, const QString& title)
{
    ElaMenu* menu = new ElaMenu(title, this);
    menu->setMenuItemHeight(27);
    menu->setIcon(icon);
    QMenuBar::addAction(menu->menuAction());
    return menu;
}

/**
 * @brief 添加新菜单（带 Ela 图标）
 * @param icon Ela 图标类型
 * @param title 菜单标题
 * @return 新创建的菜单指针
 */
ElaMenu* ElaMenuBar::addMenu(ElaIconType::IconName icon, const QString& title)
{
    ElaMenu* menu = new ElaMenu(title, this);
    menu->setMenuItemHeight(27);
    menu->menuAction()->setProperty("ElaIconType", QChar((unsigned short)icon));
    menu->menuAction()->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::Broom, 1));
    QMenuBar::addAction(menu->menuAction());
    return menu;
}

/**
 * @brief 添加带 Ela 图标的动作
 * @param icon Ela 图标类型
 * @param text 动作文本
 * @return 新创建的动作指针
 */
QAction* ElaMenuBar::addElaIconAction(ElaIconType::IconName icon, const QString& text)
{
    QAction* action = new QAction(text, this);
    action->setProperty("ElaIconType", QChar((unsigned short)icon));
    action->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::Broom, 1));
    QMenuBar::addAction(action);
    return action;
}