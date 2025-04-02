//
// Created by WeiWang on 25-3-15.
//

#ifndef TRAYICONMENU_H
#define TRAYICONMENU_H

#include "../BaseMenu.h"

#if defined(MYMENU_LIBRARY)
#define MYMENU_EXPORT Q_DECL_EXPORT
#else
#define MYMENU_EXPORT Q_DECL_IMPORT
#endif

class MYMENU_EXPORT TrayIconMenu : public BaseMenu {
    Q_OBJECT

public:
    explicit TrayIconMenu(QWidget* parent = nullptr);

    const TrayIconMenu* getMenu()const override;

    void setShadow(const int &width, const int &radius,const QColor& color) override;

private:
    void initMenu() override;

signals:
    void openWindow();

    void exit();

    void noVolume(const bool& flag);

    void showAboutDialog(const bool& flag);

private:
    //是否静音
    bool m_volumeFlag = false;
    //是否显示关于窗口
    bool m_aboutFlag = false;
};



#endif //TRAYICONMENU_H
