//
// Created by WeiWang on 25-1-12.
//

#ifndef SORTOPTIONMENU_H
#define SORTOPTIONMENU_H

#include "../BaseMenu.h"
#include "../MenuBtn.h"

class SortOptionMenu : public BaseMenu {
    Q_OBJECT

public:
    SortOptionMenu(QWidget *parent = nullptr);

    const SortOptionMenu* getMenu()const override;

    void initMenu() override;

    void btnClickAgain() const;

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

private:
    //记录最后一次是谁选用了哪种排序方式
    MenuBtn* m_sortBtn{};

};



#endif //SORTOPTIONMENU_H
