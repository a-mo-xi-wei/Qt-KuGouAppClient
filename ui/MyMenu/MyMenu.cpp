//
// Created by WeiWang on 24-10-19.
//

#include "MyMenu.h"
#include "SongOptionMenu/SongOptionMenu.h"
#include "SortOptionMenu/SortOptionMenu.h"
#include "TitleOptionMenu/TitleOptionMenu.h"
#include "ListOptionMenu/ListOptionMenu.h"
#include "ListenOptionMenu/ListenOptionMenu.h"
#include "GLOptionMenu/GLOptionMenu.h"
#include "TrayIconMenu/TrayIconMenu.h"

MyMenu::MyMenu(const MenuKind &kind, QWidget *parent)
    : QWidget(parent)
    , m_kind(kind)
{
    switch (kind) {
        case SongOption: {
            this->m_menu = new SongOptionMenu(this);
            //if (this->m_menu)qDebug()<<"成功分配内存";
            break;
        }
        case SortOption: {
            this->m_menu = new SortOptionMenu(this);
            break;
        }
        case TitleOption: {
            this->m_menu = new TitleOptionMenu(this);
            break;
        }
        case ListOption: {
            this->m_menu = new ListOptionMenu(this);
            break;
        }
        case ListenOption: {
            this->m_menu = new ListenOptionMenu(this);
            break;
        }
        case GLOption: {
            this->m_menu = new GLOptionMenu(this);
            break;
        }
        case TrayIcon: {
            this->m_menu = new TrayIconMenu(this);
            break;
        }
        case None: break;
        default: break;
    }
    // 使用策略初始化菜单
    if (m_menu) {
        m_menu->initMenu();
    }
}
