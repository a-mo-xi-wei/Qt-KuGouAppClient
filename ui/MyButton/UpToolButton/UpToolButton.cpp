/**
* @file UpToolButton.cpp
 * @brief 实现 UpToolButton 类，提供上移按钮功能
 * @author WeiWang
 * @date 2024-10-16
 * @version 1.0
 */

#include "UpToolButton.h"
#include <QFile>

/**
 * @brief 构造函数，初始化上移按钮
 * @param parent 父控件指针，默认为 nullptr
 */
UpToolButton::UpToolButton(QWidget *parent)
    : QToolButton(parent)
{
    initUi();
}

/**
 * @brief 初始化按钮界面
 */
void UpToolButton::initUi() {
    // 设置固定尺寸
    this->setFixedSize(34, 34);
    // 设置样式表
    this->setStyleSheet(QStringLiteral("border-radius:5px;background-color:rgba(177,177,177,200);"));
    // 设置鼠标手型光标
    this->setCursor(Qt::PointingHandCursor);
    // 设置默认图标
    this->setIcon(QIcon(QStringLiteral(":/Res/window/up-white.svg")));
    // 初始隐藏
    this->hide();
}

/**
 * @brief 鼠标进入事件，切换悬浮图标
 * @param event 进入事件
 */
void UpToolButton::enterEvent(QEnterEvent *event) {
    QToolButton::enterEvent(event);
    this->setIcon(QIcon(QStringLiteral(":/Res/window/up-hover.svg")));
}

/**
 * @brief 鼠标离开事件，恢复默认图标
 * @param event 事件
 */
void UpToolButton::leaveEvent(QEvent *event) {
    QToolButton::leaveEvent(event);
    this->setIcon(QIcon(QStringLiteral(":/Res/window/up-white.svg")));
}