//
// Created by WeiWang on 24-10-19.
//

//************************** 待优化 ， 使用设计模式 **********************

#ifndef MYWIDGETMENU_H
#define MYWIDGETMENU_H

#include "BaseMenu.h"

#include <QWidget>

class MyMenu : public QWidget {
    Q_OBJECT

public:
    enum MenuKind {
        SongOption,
        SortOption,
        TitleOption,
        ListOption,
        ListenOption,
        GLOption,
        None
    };

    explicit MyMenu(const MenuKind &kind, QWidget *parent = nullptr);

    // 模板方法声明
    template <typename T>
    T* getMenu() const {
        return static_cast<T*>(this->m_menu);
    }


private:
    BaseMenu *m_menu{}; // 策略对象
    MenuKind m_kind;
};



#endif //MYWIDGETMENU_H
