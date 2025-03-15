//
// Created by WeiWang on 25-3-15.
//

#ifndef TRAYICONMENU_H
#define TRAYICONMENU_H

#include "../BaseMenu.h"

#if defined(TRAYICONMENU_LIBRARY)
#define TRAYICONMENU_EXPORT Q_DECL_EXPORT
#else
#define TRAYICONMENU_EXPORT Q_DECL_IMPORT
#endif

class TRAYICONMENU_EXPORT TrayIconMenu : public BaseMenu {
    Q_OBJECT

public:
    explicit TrayIconMenu(QWidget* parent = nullptr);

    const TrayIconMenu* getMenu()const override;

private:
    void initMenu() override;
signals:
    void openWindow();

    void exit();

};



#endif //TRAYICONMENU_H
