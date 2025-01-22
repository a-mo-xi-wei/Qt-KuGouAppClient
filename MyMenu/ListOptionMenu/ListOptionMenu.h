//
// Created by WeiWang on 25-1-12.
//

#ifndef LISTOPTIONMENU_H
#define LISTOPTIONMENU_H

#include "../BaseMenu.h"

class ListOptionMenu : public BaseMenu {
    Q_OBJECT

public:
    ListOptionMenu(QWidget *parent = nullptr);

    void initMenu() override;

    const ListOptionMenu* getMenu()const override;

signals:

};



#endif //LISTOPTIONMENU_H
