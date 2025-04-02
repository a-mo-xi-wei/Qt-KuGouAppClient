//
// Created by WeiWang on 25-1-12.
//

#ifndef LISTOPTIONMENU_H
#define LISTOPTIONMENU_H

#include "../BaseMenu.h"

#if defined(MYMENU_LIBRARY)
#define MYMENU_EXPORT Q_DECL_EXPORT
#else
#define MYMENU_EXPORT Q_DECL_IMPORT
#endif

class MYMENU_EXPORT ListOptionMenu : public BaseMenu {
    Q_OBJECT

public:
    explicit ListOptionMenu(QWidget *parent = nullptr);

private:
    void initMenu() override;

    //初始化各个窗口
    void initSceneWidget(QWidget* widget);

    void initThemeWidget(QWidget* widget);

    void initLanguageWidget(QWidget* widget);

    void initStyleWidget(QWidget* widget);

    void initMoodWidget(QWidget* widget);

    void initPeriodWidget(QWidget* widget);

    //重写获取菜单
    const ListOptionMenu* getMenu()const override;

};



#endif //LISTOPTIONMENU_H
