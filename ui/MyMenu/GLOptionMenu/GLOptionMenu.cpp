//
// Created by WeiWang on 25-2-24.
//

#include "GLOptionMenu.h"

GLOptionMenu::GLOptionMenu(QWidget *parent)
    : BaseMenu(parent)
{

}

const GLOptionMenu * GLOptionMenu::getMenu() const {
    return this;
}

void GLOptionMenu::initMenu() {
    this->setFixedSize(60,80);



    this->hide();
}
