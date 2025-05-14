/**
 * @file ElaScrollBarPrivate.h
 * @brief 定义 ElaScrollBarPrivate 类，管理滚动条的私有实现
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELASCROLLBARPRIVATE_H
#define ELASCROLLBARPRIVATE_H

#include <QAbstractScrollArea>
#include <QObject>
#include <QScrollBar>

#include "stdafx.h"

class QTimer;
class QPropertyAnimation;
class ElaScrollBar;

/**
 * @class ElaScrollBarPrivate
 * @brief 滚动条私有实现类，管理动画和滚动逻辑
 */
class ElaScrollBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaScrollBar)
    Q_PROPERTY_CREATE_D(bool,   IsAnimation)
    Q_PROPERTY_CREATE_D(qreal,  SpeedLimit)
    Q_PROPERTY_CREATE(int,      TargetMaximum)

public:
    /**
     * @brief 构造函数，初始化滚动条私有对象
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaScrollBarPrivate(QObject* parent = nullptr);

    /**
     * @brief 析构函数，释放滚动条私有资源
     */
    ~ElaScrollBarPrivate();

    /**
     * @brief 范围变化事件处理
     * @param min 最小值
     * @param max 最大值
     */
    Q_SLOT void onRangeChanged(int min, int max);

private:
    /**
     * @brief 原始滚动条
     */
    QScrollBar*              _originScrollBar{nullptr};

    /**
     * @brief 原始滚动区域
     */
    QAbstractScrollArea*     _originScrollArea{nullptr};

    /**
     * @brief 展开定时器
     */
    QTimer*                  _expandTimer{nullptr};

    /**
     * @brief 是否展开
     */
    bool                     _isExpand{false};

    /**
     * @brief 平滑滑动动画
     */
    QPropertyAnimation*      _slideSmoothAnimation{nullptr};

    /**
     * @brief 当前滚动值
     */
    int                      _scrollValue{-1};

    /**
     * @brief 执行滚动
     * @param modifiers 键盘修饰键
     * @param value 滚动值
     */
    void _scroll(Qt::KeyboardModifiers modifiers, int value);

    /**
     * @brief 将像素位置转换为范围值
     * @param pos 像素位置
     * @return 对应的范围值
     */
    int _pixelPosToRangeValue(int pos) const;

    /**
     * @brief 初始化所有配置
     */
    void _initAllConfig();

    /**
     * @brief 处理滚动条值变化
     * @param scrollBar 目标滚动条
     * @param value 新值
     */
    void _handleScrollBarValueChanged(QScrollBar* scrollBar, int value);

    /**
     * @brief 处理滚动条范围变化
     * @param min 最小值
     * @param max 最大值
     */
    void _handleScrollBarRangeChanged(int min, int max);

    /**
     * @brief 处理滚动条几何形状
     */
    void _handleScrollBarGeometry();
};

#endif // ELASCROLLBARPRIVATE_H