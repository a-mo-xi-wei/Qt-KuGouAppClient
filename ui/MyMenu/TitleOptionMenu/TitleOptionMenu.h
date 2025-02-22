//
// Created by WeiWang on 25-1-12.
//

#ifndef TITLEOPTIONMENU_H
#define TITLEOPTIONMENU_H

#include "../BaseMenu.h"

class TitleOptionMenu : public BaseMenu {
    Q_OBJECT

public:
    TitleOptionMenu(QWidget* parent = nullptr);

    void initMenu() override;

    const TitleOptionMenu* getMenu() const override;

signals:
    //titleMenu设置相关
    void wallpaper();

    void phonePlay();

    void uploadToDevice();

    void earnCoin();

    void controller();

    void balance();

    void aiHelpYou();

    void soundPlugin();

    void timeSetting();

    void appTool();

    void restoreWindow();

    void checkUpdate();

    void useHelp();

    void feedback();

    void community();

    void updateInfo();

    void about();

    void settings();

    void logOut();

    void exit();


};



#endif //TITLEOPTIONMENU_H
