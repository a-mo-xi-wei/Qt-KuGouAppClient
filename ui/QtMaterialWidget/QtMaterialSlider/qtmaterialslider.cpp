/**
 * @file qtmaterialslider.cpp
 * @brief 实现 QtMaterialSlider 类，Material 风格滑块控件
 * @author laserpants
 * @date 2024-12-15
 * @version 1.0
 */

#include "qtmaterialslider.h"
#include "qtmaterialslider_p.h"
#include "qtmaterialslider_internal.h"
#include "qtmaterialstyle.h"
#include "qtmaterialstatetransitionevent.h"

#include <QApplication>
#include <QMouseEvent>

/**
 * @class QtMaterialSliderPrivate
 * @brief 滑块控件的私有实现
 * @note 内部使用
 */

/**
 * @brief 构造函数
 * @param q 滑块控件指针
 */
QtMaterialSliderPrivate::QtMaterialSliderPrivate(QtMaterialSlider *q)
    : q_ptr(q)
{
}

/**
 * @brief 析构函数
 */
QtMaterialSliderPrivate::~QtMaterialSliderPrivate()
{
}

/**
 * @brief 初始化滑块控件
 * @note 设置滑块、轨道、状态机及交互属性
 */
void QtMaterialSliderPrivate::init()
{
    Q_Q(QtMaterialSlider);

    thumb = new QtMaterialSliderThumb(q);                      ///< 创建滑块
    track = new QtMaterialSliderTrack(thumb, q);               ///< 创建轨道
    stateMachine = new QtMaterialSliderStateMachine(q, thumb, track); ///< 创建状态机
    stepTo = 0;                                               ///< 初始化步进目标
    oldValue = q->value();                                    ///< 记录初始值
    trackWidth = 2;                                           ///< 设置轨道宽度
    hoverTrack = false;                                       ///< 初始化轨道悬停状态
    hoverThumb = false;                                       ///< 初始化滑块悬停状态
    hover = false;                                            ///< 初始化整体悬停状态
    step = false;                                             ///< 初始化步进状态
    pageStepMode = true;                                      ///< 启用页面步进模式
    useThemeColors = true;                                    ///< 启用主题颜色

    q->setMouseTracking(true);                                ///< 启用鼠标追踪
    q->setFocusPolicy(Qt::StrongFocus);                       ///< 设置焦点策略
    q->setPageStep(1);                                        ///< 设置页面步进

    QSizePolicy sp(QSizePolicy::Expanding, QSizePolicy::Fixed); ///< 设置尺寸策略
    if (q->orientation() == Qt::Vertical)
    {
        sp.transpose();                                       ///< 垂直方向调整策略
    }

    q->setSizePolicy(sp);                                     ///< 应用尺寸策略
    q->setAttribute(Qt::WA_WState_OwnSizePolicy, false);      ///< 禁用私有尺寸策略

    stateMachine->start();                                    ///< 启动状态机
    QCoreApplication::processEvents();                        ///< 处理事件队列
}

/**
 * @brief 获取轨道边界矩形
 * @return 轨道边界矩形
 */
QRectF QtMaterialSliderPrivate::trackBoundingRect() const
{
    Q_Q(const QtMaterialSlider);

    qreal hw = static_cast<qreal>(trackWidth) / 2;            ///< 轨道半宽

    return Qt::Horizontal == q->orientation()
               ? QRectF(QT_MATERIAL_SLIDER_MARGIN, q->height() / 2 - hw,
                        q->width() - QT_MATERIAL_SLIDER_MARGIN * 2, hw * 2) ///< 水平轨道
               : QRectF(q->width() / 2 - hw, QT_MATERIAL_SLIDER_MARGIN, hw * 2,
                        q->height() - QT_MATERIAL_SLIDER_MARGIN * 2); ///< 垂直轨道
}

/**
 * @brief 获取滑块边界矩形
 * @return 滑块边界矩形
 */
QRectF QtMaterialSliderPrivate::thumbBoundingRect() const
{
    Q_Q(const QtMaterialSlider);

    return Qt::Horizontal == q->orientation()
               ? QRectF(thumb->offset(), q->height() / 2 - QT_MATERIAL_SLIDER_MARGIN,
                        QT_MATERIAL_SLIDER_MARGIN * 2, QT_MATERIAL_SLIDER_MARGIN * 2) ///< 水平滑块
               : QRectF(q->width() / 2 - QT_MATERIAL_SLIDER_MARGIN, thumb->offset(),
                        QT_MATERIAL_SLIDER_MARGIN * 2, QT_MATERIAL_SLIDER_MARGIN * 2); ///< 垂直滑块
}

/**
 * @brief 根据位置计算值
 * @param pos 鼠标位置
 * @return 对应的滑块值
 */
int QtMaterialSliderPrivate::valueFromPosition(const QPoint &pos) const
{
    Q_Q(const QtMaterialSlider);

    const int position = Qt::Horizontal == q->orientation() ? pos.x() : pos.y(); ///< 获取坐标

    const int span = Qt::Horizontal == q->orientation()
                         ? q->width() - QT_MATERIAL_SLIDER_MARGIN * 2
                         : q->height() - QT_MATERIAL_SLIDER_MARGIN * 2; ///< 计算范围

    return QtMaterialStyle::sliderValueFromPosition(
        q->minimum(),
        q->maximum(),
        position - QT_MATERIAL_SLIDER_MARGIN,
        span,
        q->invertedAppearance());                         ///< 计算值
}

/**
 * @brief 设置悬停状态
 * @param status 是否悬停
 */
void QtMaterialSliderPrivate::setHovered(bool status)
{
    Q_Q(QtMaterialSlider);

    if (hover == status)
    {
        return;                                               ///< 状态未变化，直接返回
    }

    hover = status;                                           ///< 更新悬停状态

    if (!q->hasFocus())
    {
        if (status)
        {
            stateMachine->postEvent(new QtMaterialStateTransitionEvent(SliderNoFocusMouseEnter)); ///< 鼠标进入
        }
        else
        {
            stateMachine->postEvent(new QtMaterialStateTransitionEvent(SliderNoFocusMouseLeave)); ///< 鼠标离开
        }
    }

    q->update();                                              ///< 触发重绘
}

/**
 * @class QtMaterialSlider
 * @brief Material 风格滑块控件
 */

/**
 * @brief 构造函数
 * @param parent 父控件指针，默认为 nullptr
 */
QtMaterialSlider::QtMaterialSlider(QWidget *parent)
    : QAbstractSlider(parent),
      d_ptr(new QtMaterialSliderPrivate(this))
{
    d_func()->init();                                         ///< 初始化
}

/**
 * @brief 析构函数
 */
QtMaterialSlider::~QtMaterialSlider()
{
}

/**
 * @brief 设置是否使用主题颜色
 * @param value 是否使用主题颜色
 */
void QtMaterialSlider::setUseThemeColors(bool value)
{
    Q_D(QtMaterialSlider);

    if (d->useThemeColors == value)
    {
        return;                                               ///< 状态未变化，直接返回
    }

    d->useThemeColors = value;                                ///< 更新主题颜色设置
    d->stateMachine->setupProperties();                       ///< 更新状态机属性
}

/**
 * @brief 获取是否使用主题颜色
 * @return 是否使用主题颜色
 */
bool QtMaterialSlider::useThemeColors() const
{
    Q_D(const QtMaterialSlider);

    return d->useThemeColors;                                 ///< 返回主题颜色设置
}

/**
 * @brief 设置滑块颜色
 * @param color 滑块颜色
 */
void QtMaterialSlider::setThumbColor(const QColor &color)
{
    Q_D(QtMaterialSlider);

    d->thumbColor = color;                                    ///< 设置滑块颜色

    MATERIAL_DISABLE_THEME_COLORS                             ///< 禁用主题颜色
    d->stateMachine->setupProperties();                       ///< 更新状态机属性
    update();                                                 ///< 触发重绘
}

/**
 * @brief 获取滑块颜色
 * @return 滑块颜色
 */
QColor QtMaterialSlider::thumbColor() const
{
    Q_D(const QtMaterialSlider);

    if (d->useThemeColors || !d->thumbColor.isValid())
    {
        return QtMaterialStyle::instance().themeColor("primary1"); ///< 返回主题颜色
    }
    else
    {
        return d->thumbColor;                                 ///< 返回自定义颜色
    }
}

/**
 * @brief 设置轨道颜色
 * @param color 轨道颜色
 */
void QtMaterialSlider::setTrackColor(const QColor &color)
{
    Q_D(QtMaterialSlider);

    d->trackColor = color;                                    ///< 设置轨道颜色

    MATERIAL_DISABLE_THEME_COLORS                             ///< 禁用主题颜色
    d->stateMachine->setupProperties();                       ///< 更新状态机属性
    update();                                                 ///< 触发重绘
}

/**
 * @brief 获取轨道颜色
 * @return 轨道颜色
 */
QColor QtMaterialSlider::trackColor() const
{
    Q_D(const QtMaterialSlider);

    if (d->useThemeColors || !d->trackColor.isValid())
    {
        return QtMaterialStyle::instance().themeColor("accent3"); ///< 返回主题颜色
    }
    else
    {
        return d->trackColor;                                 ///< 返回自定义颜色
    }
}

/**
 * @brief 设置禁用状态颜色
 * @param color 禁用状态颜色
 */
void QtMaterialSlider::setDisabledColor(const QColor &color)
{
    Q_D(QtMaterialSlider);

    d->disabledColor = color;                                 ///< 设置禁用颜色

    MATERIAL_DISABLE_THEME_COLORS                             ///< 禁用主题颜色
    d->stateMachine->setupProperties();                       ///< 更新状态机属性
    update();                                                 ///< 触发重绘
}

/**
 * @brief 获取禁用状态颜色
 * @return 禁用状态颜色
 */
QColor QtMaterialSlider::disabledColor() const
{
    Q_D(const QtMaterialSlider);

    if (d->useThemeColors || !d->disabledColor.isValid())
    {
        return QtMaterialStyle::instance().themeColor("disabled"); ///< 返回主题颜色
    }
    else
    {
        return d->disabledColor;                              ///< 返回自定义颜色
    }
}

/**
 * @brief 设置页面步进模式
 * @param pageStep 是否启用页面步进
 */
void QtMaterialSlider::setPageStepMode(bool pageStep)
{
    Q_D(QtMaterialSlider);

    d->pageStepMode = pageStep;                               ///< 设置页面步进模式
}

/**
 * @brief 获取页面步进模式
 * @return 是否启用页面步进
 */
bool QtMaterialSlider::pageStepMode() const
{
    Q_D(const QtMaterialSlider);

    return d->pageStepMode;                                   ///< 返回页面步进模式
}

/**
 * @brief 获取最小尺寸提示
 * @return 最小尺寸
 */
QSize QtMaterialSlider::minimumSizeHint() const
{
    return Qt::Horizontal == orientation()
               ? QSize(130, 34)                               ///< 水平方向尺寸
               : QSize(34, 130);                              ///< 垂直方向尺寸
}

/**
 * @brief 设置反向外观
 * @param value 是否反向
 */
void QtMaterialSlider::setInvertedAppearance(bool value)
{
    QAbstractSlider::setInvertedAppearance(value);            ///< 设置反向外观

    updateThumbOffset();                                      ///< 更新滑块偏移
}

/**
 * @brief 滑块变化处理
 * @param change 变化类型
 */
void QtMaterialSlider::sliderChange(SliderChange change)
{
    Q_D(QtMaterialSlider);

    if (SliderOrientationChange == change)
    {
        QSizePolicy sp(QSizePolicy::Expanding, QSizePolicy::Fixed);
        if (orientation() == Qt::Vertical)
        {
            sp.transpose();                                   ///< 垂直方向调整策略
        }
        setSizePolicy(sp);                                    ///< 更新尺寸策略
    }
    else if (SliderValueChange == change)
    {
        if (minimum() == value())
        {
            triggerAction(SliderToMinimum);                   ///< 触发最小值动作
            d->stateMachine->postEvent(new QtMaterialStateTransitionEvent(SliderChangedToMinimum)); ///< 最小值状态
        }
        else if (maximum() == value())
        {
            triggerAction(SliderToMaximum);                   ///< 触发最大值动作
        }
        if (minimum() == d->oldValue)
        {
            d->stateMachine->postEvent(new QtMaterialStateTransitionEvent(SliderChangedFromMinimum)); ///< 从最小值变化
        }
        d->oldValue = value();                                ///< 更新旧值
    }

    updateThumbOffset();                                      ///< 更新滑块偏移

    QAbstractSlider::sliderChange(change);                    ///< 调用父类处理
}

/**
 * @brief 鼠标移动事件
 * @param event 鼠标事件
 */
void QtMaterialSlider::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(QtMaterialSlider);

    if (isSliderDown())
    {
        setSliderPosition(d->valueFromPosition(event->pos())); ///< 更新滑块位置
    }
    else
    {
        QRectF track(d->trackBoundingRect().adjusted(-2, -2, 2, 2)); ///< 轨道区域

        if (track.contains(event->pos()) != d->hoverTrack)
        {
            d->hoverTrack = !d->hoverTrack;                   ///< 更新轨道悬停状态
            update();                                         ///< 触发重绘
        }

        QRectF thumb(0, 0, 16, 16);
        thumb.moveCenter(d->thumbBoundingRect().center());    ///< 滑块区域

        if (thumb.contains(event->pos()) != d->hoverThumb)
        {
            d->hoverThumb = !d->hoverThumb;                   ///< 更新滑块悬停状态
            update();                                         ///< 触发重绘
        }

        d->setHovered(d->hoverTrack || d->hoverThumb);        ///< 更新整体悬停状态
    }

    QAbstractSlider::mouseMoveEvent(event);                   ///< 调用父类处理
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 */
void QtMaterialSlider::mousePressEvent(QMouseEvent *event)
{
    Q_D(QtMaterialSlider);

    const QPoint pos = event->pos();                          ///< 获取鼠标位置

    QRectF thumb(0, 0, 16, 16);
    thumb.moveCenter(d->thumbBoundingRect().center());        ///< 滑块区域

    if (thumb.contains(pos))
    {
        setSliderDown(true);                                  ///< 启动滑块拖动
        return;
    }

    if (!d->pageStepMode)
    {
        setSliderPosition(d->valueFromPosition(event->pos())); ///< 直接设置位置
        d->thumb->setHaloSize(0);                             ///< 隐藏光晕
        setSliderDown(true);                                  ///< 启动拖动
        return;
    }

    d->step = true;                                           ///< 启用步进
    d->stepTo = d->valueFromPosition(pos);                    ///< 设置步进目标

    SliderAction action = d->stepTo > sliderPosition()
                              ? SliderPageStepAdd
                              : SliderPageStepSub;            ///< 确定步进方向

    triggerAction(action);                                    ///< 触发步进动作
    setRepeatAction(action, 400, 8);                          ///< 设置重复动作
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件
 */
void QtMaterialSlider::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(QtMaterialSlider);

    if (isSliderDown())
    {
        setSliderDown(false);                                 ///< 结束滑块拖动
    }
    else if (d->step)
    {
        d->step = false;                                      ///< 结束步进
        setRepeatAction(SliderNoAction, 0);                   ///< 清除重复动作
    }

    QAbstractSlider::mouseReleaseEvent(event);                ///< 调用父类处理
}

/**
 * @brief 鼠标离开事件
 * @param event 事件
 */
void QtMaterialSlider::leaveEvent(QEvent *event)
{
    Q_D(QtMaterialSlider);

    if (d->hoverTrack)
    {
        d->hoverTrack = false;                                ///< 清除轨道悬停
        update();                                             ///< 触发重绘
    }
    if (d->hoverThumb)
    {
        d->hoverThumb = false;                                ///< 清除滑块悬停
        update();                                             ///< 触发重绘
    }

    d->setHovered(false);                                     ///< 清除整体悬停

    QAbstractSlider::leaveEvent(event);                       ///< 调用父类处理
}

/**
 * @brief 更新滑块偏移
 */
void QtMaterialSlider::updateThumbOffset()
{
    Q_D(QtMaterialSlider);

    const int offset = QtMaterialStyle::sliderPositionFromValue(
        minimum(),
        maximum(),
        sliderPosition(),
        Qt::Horizontal == orientation()
            ? width() - QT_MATERIAL_SLIDER_MARGIN * 2
            : height() - QT_MATERIAL_SLIDER_MARGIN * 2,
        invertedAppearance());                            ///< 计算偏移

    d->thumb->setOffset(offset);                              ///< 设置滑块偏移
}