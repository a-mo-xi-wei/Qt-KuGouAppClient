/**
 * @file qtmaterialslider_internal.cpp
 * @brief 实现滑块控件的内部实现类
 * @author laserpants
 * @date 2024-12-15
 * @version 1.0
 */

#include "qtmaterialslider_internal.h"
#include "qtmaterialslider.h"
#include "qtmaterialstyle.h"
#include "qtmaterialstatetransition.h"

#include <QState>
#include <QSignalTransition>
#include <QEventTransition>
#include <QPropertyAnimation>
#include <QPainter>

/**
 * @class QtMaterialSliderStateMachine
 * @brief 滑块控件的状态机，管理交互状态
 * @note 内部使用
 */

/**
 * @brief 构造函数
 * @param slider 滑块控件
 * @param thumb 滑块对象
 * @param track 轨道对象
 * @note 初始化并行状态机，包含焦点、滑动和最小值状态
 */
QtMaterialSliderStateMachine::QtMaterialSliderStateMachine(
    QtMaterialSlider *slider,
    QtMaterialSliderThumb *thumb,
    QtMaterialSliderTrack *track)
    : QStateMachine(slider),
      m_slider(slider),
      m_thumb(thumb),
      m_track(track),
      m_topState(new QState(QState::ParallelStates)),
      m_fstState(new QState(m_topState)),
      m_sndState(new QState(m_topState)),
      m_inactiveState(new QState(m_fstState)),
      m_focusState(new QState(m_fstState)),
      m_slidingState(new QState(m_fstState)),
      m_pulseOutState(new QState(m_focusState)),
      m_pulseInState(new QState(m_focusState)),
      m_minState(new QState(m_sndState)),
      m_normalState(new QState(m_sndState))
{
    addState(m_topState);                                     ///< 添加顶层状态
    setInitialState(m_topState);                              ///< 设置初始状态

    m_fstState->setInitialState(m_inactiveState);             ///< 设置第一状态组初始状态
    m_focusState->setInitialState(m_pulseOutState);           ///< 设置焦点状态初始状态

    m_inactiveState->assignProperty(thumb, "haloSize", 0);    ///< 非激活状态无光晕
    m_slidingState->assignProperty(thumb, "haloSize", 0);     ///< 滑动状态无光晕

    m_pulseOutState->assignProperty(thumb, "haloSize", 35);   ///< 光晕放大状态
    m_pulseInState->assignProperty(thumb, "haloSize", 28);    ///< 光晕缩小状态

    m_inactiveState->assignProperty(thumb, "diameter", 11);   ///< 非激活状态滑块直径
    m_focusState->assignProperty(thumb, "diameter", 11);      ///< 焦点状态滑块直径
    m_slidingState->assignProperty(thumb, "diameter", 17);    ///< 滑动状态滑块直径

    QAbstractTransition *transition;
    QtMaterialStateTransition *customTransition;
    QPropertyAnimation *animation;

    // @note 鼠标进入显示光晕
    customTransition = new QtMaterialStateTransition(SliderNoFocusMouseEnter);
    customTransition->setTargetState(m_focusState);

    animation = new QPropertyAnimation(thumb, "haloSize", this);
    animation->setEasingCurve(QEasingCurve::InOutSine);
    customTransition->addAnimation(animation);
    customTransition->addAnimation(new QPropertyAnimation(track, "fillColor", this));
    m_inactiveState->addTransition(customTransition);

    // @note 获得焦点显示光晕
    transition = new QEventTransition(slider, QEvent::FocusIn);
    transition->setTargetState(m_focusState);

    animation = new QPropertyAnimation(thumb, "haloSize", this);
    animation->setEasingCurve(QEasingCurve::InOutSine);
    transition->addAnimation(animation);
    transition->addAnimation(new QPropertyAnimation(track, "fillColor", this));
    m_inactiveState->addTransition(transition);

    // @note 失去焦点隐藏光晕
    transition = new QEventTransition(slider, QEvent::FocusOut);
    transition->setTargetState(m_inactiveState);

    animation = new QPropertyAnimation(thumb, "haloSize", this);
    animation->setEasingCurve(QEasingCurve::InOutSine);
    transition->addAnimation(animation);
    transition->addAnimation(new QPropertyAnimation(track, "fillColor", this));
    m_focusState->addTransition(transition);

    // @note 鼠标离开隐藏光晕，除非有焦点
    customTransition = new QtMaterialStateTransition(SliderNoFocusMouseLeave);
    customTransition->setTargetState(m_inactiveState);

    animation = new QPropertyAnimation(thumb, "haloSize", this);
    animation->setEasingCurve(QEasingCurve::InOutSine);
    customTransition->addAnimation(animation);
    customTransition->addAnimation(new QPropertyAnimation(track, "fillColor", this));
    m_focusState->addTransition(customTransition);

    // @note 光晕缩小
    transition = new QSignalTransition(m_pulseOutState, SIGNAL(propertiesAssigned()));
    transition->setTargetState(m_pulseInState);

    animation = new QPropertyAnimation(thumb, "haloSize", this);
    animation->setEasingCurve(QEasingCurve::InOutSine);
    animation->setDuration(1000);
    transition->addAnimation(animation);
    m_pulseOutState->addTransition(transition);

    // @note 光晕放大
    transition = new QSignalTransition(m_pulseInState, SIGNAL(propertiesAssigned()));
    transition->setTargetState(m_pulseOutState);

    animation = new QPropertyAnimation(thumb, "haloSize", this);
    animation->setEasingCurve(QEasingCurve::InOutSine);
    animation->setDuration(1000);
    transition->addAnimation(animation);
    m_pulseInState->addTransition(transition);

    // @note 滑块按下
    transition = new QSignalTransition(slider, SIGNAL(sliderPressed()));
    transition->setTargetState(m_slidingState);
    animation = new QPropertyAnimation(thumb, "diameter", this);
    animation->setDuration(70);
    transition->addAnimation(animation);

    animation = new QPropertyAnimation(thumb, "haloSize", this);
    animation->setEasingCurve(QEasingCurve::InOutSine);
    transition->addAnimation(animation);
    m_focusState->addTransition(transition);

    // @note 滑块释放
    transition = new QSignalTransition(slider, SIGNAL(sliderReleased()));
    transition->setTargetState(m_focusState);
    animation = new QPropertyAnimation(thumb, "diameter", this);
    animation->setDuration(70);
    transition->addAnimation(animation);

    animation = new QPropertyAnimation(thumb, "haloSize", this);
    animation->setEasingCurve(QEasingCurve::InOutSine);
    transition->addAnimation(animation);
    m_slidingState->addTransition(transition);

    // @note 最小值状态
    m_minState->assignProperty(thumb, "borderWidth", 2);      ///< 最小值边框宽度
    m_normalState->assignProperty(thumb, "borderWidth", 0);   ///< 正常状态无边框

    m_sndState->setInitialState(m_minState);                  ///< 设置第二状态组初始状态

    customTransition = new QtMaterialStateTransition(SliderChangedFromMinimum);
    customTransition->setTargetState(m_normalState);

    animation = new QPropertyAnimation(thumb, "fillColor", this);
    animation->setDuration(200);
    customTransition->addAnimation(animation);

    animation = new QPropertyAnimation(thumb, "haloColor", this);
    animation->setDuration(300);
    customTransition->addAnimation(animation);

    animation = new QPropertyAnimation(thumb, "borderColor", this);
    animation->setDuration(200);
    customTransition->addAnimation(animation);

    animation = new QPropertyAnimation(thumb, "borderWidth", this);
    animation->setDuration(200);
    customTransition->addAnimation(animation);

    m_minState->addTransition(customTransition);

    customTransition = new QtMaterialStateTransition(SliderChangedToMinimum);
    customTransition->setTargetState(m_minState);

    animation = new QPropertyAnimation(thumb, "fillColor", this);
    animation->setDuration(200);
    customTransition->addAnimation(animation);

    animation = new QPropertyAnimation(thumb, "haloColor", this);
    animation->setDuration(300);
    customTransition->addAnimation(animation);

    animation = new QPropertyAnimation(thumb, "borderColor", this);
    animation->setDuration(200);
    customTransition->addAnimation(animation);

    animation = new QPropertyAnimation(thumb, "borderWidth", this);
    animation->setDuration(200);
    customTransition->addAnimation(animation);

    m_normalState->addTransition(customTransition);

    setupProperties();                                        ///< 设置状态机属性
}

/**
 * @brief 析构函数
 */
QtMaterialSliderStateMachine::~QtMaterialSliderStateMachine()
{
}

/**
 * @brief 设置状态机属性
 * @note 配置轨道和滑块的颜色属性
 */
void QtMaterialSliderStateMachine::setupProperties()
{
    QColor trackColor = m_slider->trackColor();                ///< 获取轨道颜色
    QColor thumbColor = m_slider->thumbColor();                ///< 获取滑块颜色

    m_inactiveState->assignProperty(m_track, "fillColor", trackColor.lighter(130)); ///< 非激活状态轨道颜色
    m_slidingState->assignProperty(m_track, "fillColor", trackColor); ///< 滑动状态轨道颜色
    m_focusState->assignProperty(m_track, "fillColor", trackColor); ///< 焦点状态轨道颜色

    QColor holeColor = m_slider->palette().color(QPalette::Base); ///< 获取背景颜色

    if (m_slider->parentWidget())
    {
        holeColor = m_slider->parentWidget()->palette().color(m_slider->backgroundRole()); ///< 使用父控件背景色
    }

    m_minState->assignProperty(m_thumb, "fillColor", holeColor); ///< 最小值状态填充颜色

    m_minState->assignProperty(m_thumb, "haloColor", trackColor); ///< 最小值状态光晕颜色
    m_minState->assignProperty(m_thumb, "borderColor", trackColor); ///< 最小值状态边框颜色

    m_normalState->assignProperty(m_thumb, "fillColor", thumbColor); ///< 正常状态填充颜色
    m_normalState->assignProperty(m_thumb, "haloColor", thumbColor); ///< 正常状态光晕颜色
    m_normalState->assignProperty(m_thumb, "borderColor", thumbColor); ///< 正常状态边框颜色

    m_slider->update();                                       ///< 触发重绘
}

/**
 * @class QtMaterialSliderThumb
 * @brief 滑块控件的手柄，绘制滑块和光晕
 * @note 内部使用
 */

/**
 * @brief 构造函数
 * @param slider 滑块控件
 */
QtMaterialSliderThumb::QtMaterialSliderThumb(QtMaterialSlider *slider)
    : QtMaterialOverlayWidget(slider->parentWidget()),
      m_slider(slider),
      m_diameter(11),
      m_borderWidth(2),
      m_haloSize(0),
      m_offset(0)
{
    slider->installEventFilter(this);                         ///< 安装事件过滤器

    setAttribute(Qt::WA_TransparentForMouseEvents, true);     ///< 设置鼠标事件透明
}

/**
 * @brief 析构函数
 */
QtMaterialSliderThumb::~QtMaterialSliderThumb()
{
}

/**
 * @brief 事件过滤
 * @param obj 对象
 * @param event 事件
 * @return 是否处理事件
 */
bool QtMaterialSliderThumb::eventFilter(QObject *obj, QEvent *event)
{
    if (QEvent::ParentChange == event->type())
    {
        setParent(m_slider->parentWidget());                  ///< 更新父控件
    }

    return QtMaterialOverlayWidget::eventFilter(obj, event);  ///< 调用父类处理
}

/**
 * @brief 绘制事件
 * @param event 绘制事件
 * @note 绘制光晕和滑块
 */
void QtMaterialSliderThumb::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);            ///< 启用抗锯齿

    // @note 绘制光晕
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(m_haloColor);                              ///< 设置光晕颜色
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);                                ///< 无边框

    QPointF disp = Qt::Horizontal == m_slider->orientation()
                       ? QPointF(QT_MATERIAL_SLIDER_MARGIN + m_offset, m_slider->height() / 2)
                       : QPointF(m_slider->width() / 2, QT_MATERIAL_SLIDER_MARGIN + m_offset); ///< 计算偏移

    QRectF halo((m_slider->pos() - QPointF(m_haloSize, m_haloSize) / 2) + disp,
                QSizeF(m_haloSize, m_haloSize));              ///< 光晕区域

    painter.setOpacity(0.15);                                 ///< 设置光晕透明度
    painter.drawEllipse(halo);                                ///< 绘制光晕

    // @note 绘制滑块
    const bool isMin = m_slider->value() == m_slider->minimum(); ///< 是否最小值

    brush.setColor(m_slider->isEnabled()
                       ? m_fillColor
                       : m_slider->disabledColor());          ///< 设置填充颜色
    painter.setBrush(!m_slider->isEnabled() && isMin
                         ? Qt::NoBrush
                         : brush);                                ///< 禁用且最小值时无填充

    if (m_slider->isEnabled() || isMin)
    {
        QPen pen;
        pen.setColor(m_borderColor);                          ///< 设置边框颜色
        pen.setWidthF((isMin && !m_slider->isEnabled()) ? 1.7 : m_borderWidth); ///< 设置边框宽度
        painter.setPen(pen);
    }
    else
    {
        painter.setPen(Qt::NoPen);                            ///< 无边框
    }

    QRectF geometry = Qt::Horizontal == m_slider->orientation()
                          ? QRectF(m_offset, m_slider->height() / 2 - QT_MATERIAL_SLIDER_MARGIN,
                                   QT_MATERIAL_SLIDER_MARGIN * 2, QT_MATERIAL_SLIDER_MARGIN * 2).translated(
                              m_slider->pos())
                          : QRectF(m_slider->width() / 2 - QT_MATERIAL_SLIDER_MARGIN, m_offset,
                                   QT_MATERIAL_SLIDER_MARGIN * 2, QT_MATERIAL_SLIDER_MARGIN * 2).translated(
                              m_slider->pos());               ///< 滑块区域

    qreal s = m_slider->isEnabled() ? m_diameter : 7;         ///< 滑块尺寸

    QRectF thumb(0, 0, s, s);

    thumb.moveCenter(geometry.center());                      ///< 居中滑块

    painter.setOpacity(1);                                    ///< 恢复不透明
    painter.drawEllipse(thumb);                               ///< 绘制滑块
}

/**
 * @class QtMaterialSliderTrack
 * @brief 滑块控件的轨道，绘制轨道区域
 * @note 内部使用
 */

/**
 * @brief 构造函数
 * @param thumb 滑块对象
 * @param slider 滑块控件
 */
QtMaterialSliderTrack::QtMaterialSliderTrack(QtMaterialSliderThumb *thumb, QtMaterialSlider *slider)
    : QtMaterialOverlayWidget(slider->parentWidget()),
      m_slider(slider),
      m_thumb(thumb),
      m_trackWidth(2)
{
    slider->installEventFilter(this);                         ///< 安装事件过滤器

    setAttribute(Qt::WA_TransparentForMouseEvents, true);     ///< 设置鼠标事件透明

    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(update())); ///< 连接滑块移动信号
}

/**
 * @brief 析构函数
 */
QtMaterialSliderTrack::~QtMaterialSliderTrack()
{
}

/**
 * @brief 事件过滤
 * @param obj 对象
 * @param event 事件
 * @return 是否处理事件
 */
bool QtMaterialSliderTrack::eventFilter(QObject *obj, QEvent *event)
{
    if (QEvent::ParentChange == event->type())
    {
        setParent(m_slider->parentWidget());                  ///< 更新父控件
    }

    return QtMaterialOverlayWidget::eventFilter(obj, event);  ///< 调用父类处理
}

/**
 * @brief 绘制事件
 * @param event 绘制事件
 * @note 绘制轨道区域
 */
void QtMaterialSliderTrack::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);            ///< 启用抗锯齿

    QBrush fg;
    fg.setStyle(Qt::SolidPattern);
    fg.setColor(m_slider->isEnabled()
                    ? m_slider->thumbColor()
                    : m_slider->disabledColor());             ///< 前景颜色
    QBrush bg;
    bg.setStyle(Qt::SolidPattern);
    bg.setColor(m_slider->isEnabled()
                    ? m_fillColor
                    : m_slider->disabledColor());             ///< 背景颜色

    qreal offset = m_thumb->offset();                         ///< 获取滑块偏移

    if (Qt::Horizontal == m_slider->orientation())
    {
        painter.translate(m_slider->x() + QT_MATERIAL_SLIDER_MARGIN,
                          m_slider->y() + m_slider->height() / 2
                          - static_cast<qreal>(m_trackWidth) / 2); ///< 水平方向平移
    }
    else
    {
        painter.translate(m_slider->x() + m_slider->width() / 2
                          - static_cast<qreal>(m_trackWidth) / 2,
                          m_slider->y() + QT_MATERIAL_SLIDER_MARGIN); ///< 垂直方向平移
    }

    QRectF geometry = Qt::Horizontal == m_slider->orientation()
                          ? QRectF(0, 0, m_slider->width() - QT_MATERIAL_SLIDER_MARGIN * 2, m_trackWidth)
                          : QRectF(0, 0, m_trackWidth, m_slider->height() - QT_MATERIAL_SLIDER_MARGIN * 2); ///< 轨道区域

    QRectF bgRect;
    QRectF fgRect;

    if (Qt::Horizontal == m_slider->orientation())
    {
        fgRect = QRectF(0, 0, offset, m_trackWidth);          ///< 水平前景区域
        bgRect = QRectF(offset, 0, m_slider->width(), m_trackWidth).intersected(geometry); ///< 水平背景区域
    }
    else
    {
        fgRect = QRectF(0, 0, m_trackWidth, offset);          ///< 垂直前景区域
        bgRect = QRectF(0, offset, m_trackWidth, m_slider->height()).intersected(geometry); ///< 垂直背景区域
    }

    if (!m_slider->isEnabled())
    {
        fgRect = fgRect.width() < 9 ? QRectF() : fgRect.adjusted(0, 0, -6, 0); ///< 禁用状态调整前景
        bgRect = bgRect.width() < 9 ? QRectF() : bgRect.adjusted(6, 0, 0, 0); ///< 禁用状态调整背景
    }

    if (m_slider->invertedAppearance())
    {
        qSwap(bgRect, fgRect);                                ///< 反向外观交换区域
    }

    painter.fillRect(bgRect, bg);                             ///< 绘制背景
    painter.fillRect(fgRect, fg);                             ///< 绘制前景
}