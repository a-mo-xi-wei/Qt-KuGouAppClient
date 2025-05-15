/**
* @file MyMenu.cpp
 * @brief 实现 MyMenu 类，使用策略模式管理不同类型的菜单
 * @author WeiWang
 * @date 2024-10-19
 * @version 1.0
 */

#include "MyMenu.h"
#include "SongOptionMenu/SongOptionMenu.h"
#include "SortOptionMenu/SortOptionMenu.h"
#include "TitleOptionMenu/TitleOptionMenu.h"
#include "ListOptionMenu/ListOptionMenu.h"
#include "ListenOptionMenu/ListenOptionMenu.h"

/**
 * @brief 构造函数，根据菜单类型初始化菜单
 * @param kind 菜单类型
 * @param parent 父控件指针，默认为 nullptr
 */
MyMenu::MyMenu(const MenuKind &kind, QWidget *parent)
    : QWidget(parent)
    , m_kind(kind)
{
    // 根据菜单类型创建具体菜单对象
    switch (kind) {
        case SongOption:
            this->m_menu = new SongOptionMenu(this);
        // if (this->m_menu) qDebug() << "成功分配内存"; ///< 调试用，记录内存分配
        break;
        case SortOption:
            this->m_menu = new SortOptionMenu(this);
        break;
        case TitleOption:
            this->m_menu = new TitleOptionMenu(this);
        break;
        case ListOption:
            this->m_menu = new ListOptionMenu(this);
        break;
        case ListenOption:
            this->m_menu = new ListenOptionMenu(this);
        break;
        case None:
            default:
                break;
    }

    // 初始化菜单
    if (m_menu) {
        m_menu->initMenu();
    }
}