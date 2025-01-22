//
// Created by WeiWang on 25-1-12.
//

#ifndef SORTOPTIONMENU_H
#define SORTOPTIONMENU_H

#include "../BaseMenu.h"

class SortOptionMenu : public BaseMenu {
    Q_OBJECT

public:
    SortOptionMenu(QWidget *parent = nullptr);

    void initMenu() override;

    const SortOptionMenu* getMenu()const override;

signals:
    //音乐选中相关
    void selected();

    void deselected();
    //排序相关
    void defaultSort(const bool &down = true);

    void addTimeSort(const bool &down = true);

    void songNameSort(const bool &down = true);

    void singerSort(const bool &down = true);

    void durationSort(const bool &down = true);

    void playCountSort(const bool &down = true);

    void randomSort();

};



#endif //SORTOPTIONMENU_H
