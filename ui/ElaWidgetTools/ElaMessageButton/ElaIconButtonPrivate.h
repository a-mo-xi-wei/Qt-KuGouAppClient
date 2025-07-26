/**
* @file ElaIconButtonPrivate.h
 * @brief 定义 ElaIconButtonPrivate 类，管理图标按钮的私有实现
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAICONBUTTONPRIVATE_H
#define ELAICONBUTTONPRIVATE_H

#include <QPixmap>

#include "Def.h"

class ElaIconButton;

/**
 * @class ElaIconButtonPrivate
 * @brief 图标按钮私有实现类，管理图标、动画和主题
 */
class ElaIconButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaIconButton)
    Q_PROPERTY_CREATE_D(int,                     BorderRadius)
    Q_PROPERTY_CREATE_D(qreal,                   Opacity)
    Q_PROPERTY_CREATE_D(ElaIconType::IconName,  Awesome)
    Q_PROPERTY_CREATE_D(QColor,                 LightHoverColor)
    Q_PROPERTY_CREATE_D(QColor,                 DarkHoverColor)
    Q_PROPERTY_CREATE_D(QColor,                 LightIconColor)
    Q_PROPERTY_CREATE_D(QColor,                 DarkIconColor)
    Q_PROPERTY_CREATE_D(QColor,                 LightHoverIconColor)
    Q_PROPERTY_CREATE_D(QColor,                 DarkHoverIconColor)
    Q_PROPERTY_CREATE_D(bool,                   IsSelected)
    Q_PROPERTY_CREATE(int,                      HoverAlpha)

public:
    /**
     * @brief 构造函数，初始化图标按钮私有对象
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaIconButtonPrivate(QObject* parent = nullptr);

    /**
     * @brief 析构函数，释放图标按钮私有资源
     */
    ~ElaIconButtonPrivate() override;

private:
    /**
     * @brief 图标图像
     */
    QPixmap                   _iconPix;

    /**
     * @brief 透明度动画是否完成
     */
    bool                     _isAlphaAnimationFinished{true};

    /**
     * @brief 当前主题模式
     */
    ElaThemeType::ThemeMode  _themeMode;
};

#endif // ELAICONBUTTONPRIVATE_H