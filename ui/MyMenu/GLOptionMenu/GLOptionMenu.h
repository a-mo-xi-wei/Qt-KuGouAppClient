//
// Created by WeiWang on 25-2-24.
//

#ifndef GLOPTIONMENU_H
#define GLOPTIONMENU_H

#include "../BaseMenu.h"

#if defined(MYMENU_LIBRARY)
#define MYMENU_EXPORT Q_DECL_EXPORT
#else
#define MYMENU_EXPORT Q_DECL_IMPORT
#endif

class MYMENU_EXPORT GLOptionMenu : public BaseMenu {
Q_OBJECT

public:
    explicit GLOptionMenu(QWidget *parent = nullptr);

    const GLOptionMenu* getMenu()const override;

private:
    void initMenu() override;

signals:
    void getModel(const QString& model);
};


#endif //GLOPTIONMENU_H
