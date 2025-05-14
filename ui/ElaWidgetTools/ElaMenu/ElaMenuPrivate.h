/**
* @file ElaMenuPrivate.h
 * @brief 定义 ElaMenuPrivate 类，管理菜单的私有实现
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAMENUPRIVATE_H
#define ELAMENUPRIVATE_H

#include <QObject>
#include <QPixmap>
#include <QPoint>

#include "stdafx.h"

class ElaMenu;
class ElaMenuStyle;

/**
 * @class ElaMenuPrivate
 * @brief 菜单私有实现类，管理动画、样式和交互逻辑
 */
class ElaMenuPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaMenu)
    Q_PROPERTY_CREATE(int, AnimationImagePosY)

public:
    /**
     * @brief 构造函数，初始化菜单私有对象
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaMenuPrivate(QObject* parent = nullptr);

    /**
     * @brief 析构函数，释放菜单私有资源
     */
    ~ElaMenuPrivate() override;

private:
    /**
     * @brief 动画图像
     */
    QPixmap              _animationPix;

    /**
     * @brief 是否处于关闭动画状态
     */
    bool                _isCloseAnimation{false};

    /**
     * @brief 鼠标按下位置
     */
    QPoint              _mousePressPoint;

    /**
     * @brief 菜单样式指针
     */
    ElaMenuStyle*       _menuStyle{nullptr};

    /**
     * @brief 是否阻止菜单隐藏
     */
    bool                m_preventHide{false};
};

#endif // ELAMENUPRIVATE_H