/**
 * @file qtmaterialslider_internal.h
 * @brief 定义滑块控件的内部实现类
 * @author laserpants
 * @date 2024-12-15
 * @version 1.0
 */

#ifndef QTMATERIALSLIDER_INTERNAL_H
#define QTMATERIALSLIDER_INTERNAL_H

#include <QStateMachine>
#include "qtmaterialoverlaywidget.h"

class QtMaterialSlider;
class QtMaterialSliderThumb;
class QtMaterialSliderTrack;

/**
 * @class QtMaterialSliderStateMachine
 * @brief 滑块控件的状态机，管理交互状态
 * @note 内部使用
 */
class QtMaterialSliderStateMachine : public QStateMachine
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param slider 滑块控件
     * @param thumb 滑块对象
     * @param track 轨道对象
     */
    QtMaterialSliderStateMachine(QtMaterialSlider *slider,
                                 QtMaterialSliderThumb *thumb,
                                 QtMaterialSliderTrack *track);

    /**
     * @brief 析构函数
     */
    ~QtMaterialSliderStateMachine();

    /**
     * @brief 设置状态机属性
     */
    void setupProperties();

private:
    Q_DISABLE_COPY(QtMaterialSliderStateMachine)              ///< 禁用拷贝

    QtMaterialSlider *const m_slider;                         ///< 滑块控件
    QtMaterialSliderThumb *const m_thumb;                     ///< 滑块对象
    QtMaterialSliderTrack *const m_track;                     ///< 轨道对象
    QState *const m_topState;                                 ///< 顶层状态
    QState *const m_fstState;                                 ///< 第一状态组
    QState *const m_sndState;                                 ///< 第二状态组
    QState *const m_inactiveState;                            ///< 非激活状态
    QState *const m_focusState;                               ///< 焦点状态
    QState *const m_slidingState;                             ///< 滑动状态
    QState *const m_pulseOutState;                            ///< 光晕放大状态
    QState *const m_pulseInState;                             ///< 光晕缩小状态
    QState *const m_minState;                                 ///< 最小值状态
    QState *const m_normalState;                              ///< 正常状态
};

/**
 * @class QtMaterialSliderThumb
 * @brief 滑块控件的手柄，绘制滑块和光晕
 * @note 内部使用
 */
class QtMaterialSliderThumb : public QtMaterialOverlayWidget
{
    Q_OBJECT

    Q_PROPERTY(qreal diameter WRITE setDiameter READ diameter) ///< 滑块直径属性
    Q_PROPERTY(qreal borderWidth WRITE setBorderWidth READ borderWidth) ///< 边框宽度属性
    Q_PROPERTY(QColor borderColor WRITE setBorderColor READ borderColor) ///< 边框颜色属性
    Q_PROPERTY(QColor fillColor WRITE setFillColor READ fillColor) ///< 填充颜色属性
    Q_PROPERTY(qreal haloSize WRITE setHaloSize READ haloSize) ///< 光晕尺寸属性
    Q_PROPERTY(QColor haloColor WRITE setHaloColor READ haloColor) ///< 光晕颜色属性

public:
    /**
     * @brief 构造函数
     * @param slider 滑块控件
     */
    explicit QtMaterialSliderThumb(QtMaterialSlider *slider);

    /**
     * @brief 析构函数
     */
    ~QtMaterialSliderThumb();

    /**
     * @brief 设置滑块直径
     * @param diameter 直径
     */
    inline void setDiameter(qreal diameter);

    /**
     * @brief 获取滑块直径
     * @return 直径
     */
    inline qreal diameter() const;

    /**
     * @brief 设置边框宽度
     * @param width 宽度
     */
    inline void setBorderWidth(qreal width);

    /**
     * @brief 获取边框宽度
     * @return 宽度
     */
    inline qreal borderWidth() const;

    /**
     * @brief 设置边框颜色
     * @param color 颜色
     */
    inline void setBorderColor(const QColor &color);

    /**
     * @brief 获取边框颜色
     * @return 颜色
     */
    inline QColor borderColor() const;

    /**
     * @brief 设置填充颜色
     * @param color 颜色
     */
    inline void setFillColor(const QColor &color);

    /**
     * @brief 获取填充颜色
     * @return 颜色
     */
    inline QColor fillColor() const;

    /**
     * @brief 设置光晕尺寸
     * @param size 尺寸
     */
    inline void setHaloSize(qreal size);

    /**
     * @brief 获取光晕尺寸
     * @return 尺寸
     */
    inline qreal haloSize() const;

    /**
     * @brief 设置光晕颜色
     * @param color 颜色
     */
    inline void setHaloColor(const QColor &color);

    /**
     * @brief 获取光晕颜色
     * @return 颜色
     */
    inline QColor haloColor() const;

    /**
     * @brief 设置滑块偏移
     * @param offset 偏移量
     */
    inline void setOffset(int offset);

    /**
     * @brief 获取滑块偏移
     * @return 偏移量
     */
    inline int offset() const;

protected:
    /**
     * @brief 事件过滤
     * @param obj 对象
     * @param event 事件
     * @return 是否处理事件
     */
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief 绘制事件
     * @param event 绘制事件
     * @note 绘制滑块和光晕
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(QtMaterialSliderThumb)                     ///< 禁用拷贝

    const QtMaterialSlider *const m_slider;                   ///< 滑块控件
    QColor m_borderColor;                                     ///< 边框颜色
    QColor m_fillColor;                                       ///< 填充颜色
    QColor m_haloColor;                                       ///< 光晕颜色
    qreal m_diameter;                                         ///< 滑块直径
    qreal m_borderWidth;                                      ///< 边框宽度
    qreal m_haloSize;                                         ///< 光晕尺寸
    int m_offset;                                             ///< 滑块偏移
};

/**
 * @brief 设置滑块直径
 * @param diameter 直径
 */
inline void QtMaterialSliderThumb::setDiameter(qreal diameter)
{
    m_diameter = diameter;
    update();                                                 ///< 触发重绘
}

/**
 * @brief 获取滑块直径
 * @return 直径
 */
inline qreal QtMaterialSliderThumb::diameter() const
{
    return m_diameter;
}

/**
 * @brief 设置边框宽度
 * @param width 宽度
 */
inline void QtMaterialSliderThumb::setBorderWidth(qreal width)
{
    m_borderWidth = width;
    update();                                                 ///< 触发重绘
}

/**
 * @brief 获取边框宽度
 * @return 宽度
 */
inline qreal QtMaterialSliderThumb::borderWidth() const
{
    return m_borderWidth;
}

/**
 * @brief 设置边框颜色
 * @param color 颜色
 */
inline void QtMaterialSliderThumb::setBorderColor(const QColor &color)
{
    m_borderColor = color;
    update();                                                 ///< 触发重绘
}

/**
 * @brief 获取边框颜色
 * @return 颜色
 */
inline QColor QtMaterialSliderThumb::borderColor() const
{
    return m_borderColor;
}

/**
 * @brief 设置填充颜色
 * @param color 颜色
 */
inline void QtMaterialSliderThumb::setFillColor(const QColor &color)
{
    m_fillColor = color;
    update();                                                 ///< 触发重绘
}

/**
 * @brief 获取填充颜色
 * @return 颜色
 */
inline QColor QtMaterialSliderThumb::fillColor() const
{
    return m_fillColor;
}

/**
 * @brief 设置光晕尺寸
 * @param size 尺寸
 */
inline void QtMaterialSliderThumb::setHaloSize(qreal size)
{
    m_haloSize = size;
    update();                                                 ///< 触发重绘
}

/**
 * @brief 获取光晕尺寸
 * @return 尺寸
 */
inline qreal QtMaterialSliderThumb::haloSize() const
{
    return m_haloSize;
}

/**
 * @brief 设置光晕颜色
 * @param color 颜色
 */
inline void QtMaterialSliderThumb::setHaloColor(const QColor &color)
{
    m_haloColor = color;
    update();                                                 ///< 触发重绘
}

/**
 * @brief 获取光晕颜色
 * @return 颜色
 */
inline QColor QtMaterialSliderThumb::haloColor() const
{
    return m_haloColor;
}

/**
 * @brief 设置滑块偏移
 * @param offset 偏移量
 */
inline void QtMaterialSliderThumb::setOffset(int offset)
{
    m_offset = offset;
    update();                                                 ///< 触发重绘
}

/**
 * @brief 获取滑块偏移
 * @return 偏移量
 */
inline int QtMaterialSliderThumb::offset() const
{
    return m_offset;
}

/**
 * @class QtMaterialSliderTrack
 * @brief 滑块控件的轨道，绘制轨道区域
 * @note 内部使用
 */
class QtMaterialSliderTrack : public QtMaterialOverlayWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor fillColor WRITE setFillColor READ fillColor) ///< 填充颜色属性

public:
    /**
     * @brief 构造函数
     * @param thumb 滑块对象
     * @param slider 滑块控件
     */
    explicit QtMaterialSliderTrack(QtMaterialSliderThumb *thumb, QtMaterialSlider *slider);

    /**
     * @brief 析构函数
     */
    ~QtMaterialSliderTrack();

    /**
     * @brief 设置填充颜色
     * @param color 颜色
     */
    inline void setFillColor(const QColor &color);

    /**
     * @brief 获取填充颜色
     * @return 颜色
     */
    inline QColor fillColor() const;

    /**
     * @brief 设置轨道宽度
     * @param width 宽度
     */
    inline void setTrackWidth(int width);

    /**
     * @brief 获取轨道宽度
     * @return 宽度
     */
    inline int trackWidth() const;

protected:
    /**
     * @brief 事件过滤
     * @param obj 对象
     * @param event 事件
     * @return 是否处理事件
     */
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief 绘制事件
     * @param event 绘制事件
     * @note 绘制轨道区域
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(QtMaterialSliderTrack)                     ///< 禁用拷贝

    const QtMaterialSlider *const m_slider;                   ///< 滑块控件
    QtMaterialSliderThumb *const m_thumb;                     ///< 滑块对象
    QColor m_fillColor;                                       ///< 填充颜色
    int m_trackWidth;                                         ///< 轨道宽度
};

/**
 * @brief 设置填充颜色
 * @param color 颜色
 */
inline void QtMaterialSliderTrack::setFillColor(const QColor &color)
{
    m_fillColor = color;
    update();                                                 ///< 触发重绘
}

/**
 * @brief 获取填充颜色
 * @return 颜色
 */
inline QColor QtMaterialSliderTrack::fillColor() const
{
    return m_fillColor;
}

/**
 * @brief 设置轨道宽度
 * @param width 宽度
 */
inline void QtMaterialSliderTrack::setTrackWidth(int width)
{
    m_trackWidth = width;
    update();                                                 ///< 触发重绘
}

/**
 * @brief 获取轨道宽度
 * @return 宽度
 */
inline int QtMaterialSliderTrack::trackWidth() const
{
    return m_trackWidth;
}

#endif // QTMATERIALSLIDER_INTERNAL_H