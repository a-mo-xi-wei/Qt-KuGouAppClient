/**
* @file MyMenu.cpp
 * @brief 实现 MyMenu 类，使用注册型工厂模式管理不同类型的菜单
 * @author WeiWang
 * @date 2024-10-19
 * @version 1.0
 */

#include "MyMenu.h"

QMap<MyMenu::MenuKind, MyMenu::MenuCreator>& MyMenu::registry()
{
    static QMap<MenuKind, MenuCreator> instance;
    return instance;
}

void MyMenu::registerMenu(MenuKind kind, const MenuCreator& creator)
{
    registry()[kind] = creator;
}

/**
 * @brief 构造函数，根据菜单类型初始化菜单
 * @param kind 菜单类型
 * @param parent 父控件指针，默认为 nullptr
 */
MyMenu::MyMenu(const MenuKind &kind, QWidget *parent)
    : m_kind(kind)
{
    /*
    // 根据菜单类型创建具体菜单对象
    switch (kind) {
        case MenuKind::SongOption:
            this->m_menu = new SongOptionMenu(parent);
        // if (this->m_menu) qDebug() << "成功分配内存"; ///< 调试用，记录内存分配
        break;
        case MenuKind::SortOption:
            this->m_menu = new SortOptionMenu(parent);
        break;
        case MenuKind::TitleOption:
            this->m_menu = new TitleOptionMenu(parent);
        break;
        case MenuKind::ListOption:
            this->m_menu = new ListOptionMenu(parent);
        break;
        case MenuKind::ListenOption:
            this->m_menu = new ListenOptionMenu(parent);
        break;
        case MenuKind::None:
            default:
                break;
    }

    // 初始化菜单
    if (m_menu) {
        m_menu->initMenu();
    }
    */

    ///< 升级版  ---*>  注册式菜单工厂
    if (registry().contains(kind))          ///< 如果菜单类型在注册表中,则菜单指针指向被创建的对应的菜单
    {
        m_menu = registry()[kind](parent);
        if (m_menu)
            m_menu->initMenu();
    }
}

MyMenu::~MyMenu()
{
    m_menu = nullptr;
}
