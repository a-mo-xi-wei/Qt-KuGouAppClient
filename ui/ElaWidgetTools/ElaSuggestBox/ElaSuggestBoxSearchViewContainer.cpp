/**
* @file ElaSuggestBoxSearchViewContainer.cpp
 * @brief 实现 ElaSuggestBoxSearchViewContainer 类，建议框容器
 * @author [Liniyous]
 * @date 2025-06-25
 * @version 1.0
 */

#include "ElaSuggestBoxSearchViewContainer.h"

#include <QPainter>

#include "ElaTheme.h"

/**
 * @brief 构造函数
 * @param parent 父控件指针，默认为 nullptr
 */
ElaSuggestBoxSearchViewContainer::ElaSuggestBoxSearchViewContainer(QWidget *parent)
    : QWidget{parent}
{
    setContentsMargins(8, 8, 8, 8);                           ///< 设置边距
    setObjectName("ElaSuggestBoxSearchViewBaseWidget");
    setStyleSheet("#ElaSuggestBoxSearchViewBaseWidget{background-color:transparent}"); ///< 设置样式
    _themeMode = eTheme->getThemeMode();                      ///< 获取主题模式
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;                               ///< 更新主题模式
    });
}

/**
 * @brief 析构函数
 */
ElaSuggestBoxSearchViewContainer::~ElaSuggestBoxSearchViewContainer()
= default;

/**
 * @brief 绘制事件
 * @param event 绘制事件
 * @note 绘制阴影和圆角矩形背景
 */
void ElaSuggestBoxSearchViewContainer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing);           ///< 启用抗锯齿
    eTheme->drawEffectShadow(&painter, rect(), 6, 8);         ///< 绘制阴影，宽度 6，偏移 8
    painter.setPen(ElaThemeColor(_themeMode, PopupBorder));   ///< 设置边框颜色
    painter.setBrush(ElaThemeColor(_themeMode, PopupBase));   ///< 设置背景颜色
    QRect foregroundRect(6, 0, rect().width() - 2 * 6, rect().height() - 6); ///< 前景矩形
    painter.drawRoundedRect(foregroundRect, 8, 8);            ///< 绘制圆角矩形，半径 8
    painter.restore();
}