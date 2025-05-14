/**
* @file ElaPushButtonPrivate.h
 * @brief 定义 ElaPushButtonPrivate 类，管理按钮的私有实现
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAPUSHBUTTONPRIVATE_H
#define ELAPUSHBUTTONPRIVATE_H

#include <QColor>

#include "Def.h"
#include "stdafx.h"

class ElaPushButton;

/**
 * @class ElaPushButtonPrivate
 * @brief 按钮私有实现类，管理按钮状态和主题
 */
class ElaPushButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaPushButton)
    Q_PROPERTY_CREATE_D(int,    BorderRadius)
    Q_PROPERTY_CREATE_D(QColor, LightDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, LightHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightPressColor)
    Q_PROPERTY_CREATE_D(QColor, DarkPressColor)

public:
    /**
     * @brief 构造函数，初始化按钮私有对象
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaPushButtonPrivate(QObject* parent = nullptr);

    /**
     * @brief 析构函数，释放按钮私有资源
     */
    ~ElaPushButtonPrivate();

private:
    /**
     * @brief 亮色主题文本颜色
     */
    QColor                   _lightTextColor;

    /**
     * @brief 暗色主题文本颜色
     */
    QColor                   _darkTextColor;

    /**
     * @brief 按钮是否被按下
     */
    bool                     _isPressed{false};

    /**
     * @brief 阴影边框宽度
     */
    int                      _shadowBorderWidth{3};

    /**
     * @brief 当前主题模式
     */
    ElaThemeType::ThemeMode  _themeMode;
};

#endif // ELAPUSHBUTTONPRIVATE_H