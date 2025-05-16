/**
 * @file qtmaterialfab.cpp
 * @brief 实现 QtMaterialFloatingActionButton 类，提供 Material Design 浮动动作按钮
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#include "qtmaterialfab.h"
#include "qtmaterialfab_p.h"
#include "qtmaterialrippleoverlay.h"

#include <QGraphicsDropShadowEffect>
#include <QState>
#include <QEvent>
#include <QPainter>

/**
 * @brief 构造函数，初始化私有类
 * @param q 指向 QtMaterialFloatingActionButton 的指针
 * @note 内部实现
 */
QtMaterialFloatingActionButtonPrivate::QtMaterialFloatingActionButtonPrivate(QtMaterialFloatingActionButton *q)
    : QtMaterialRaisedButtonPrivate(q)
{
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialFloatingActionButtonPrivate::~QtMaterialFloatingActionButtonPrivate()
{
}

/**
 * @brief 初始化浮动动作按钮
 */
void QtMaterialFloatingActionButtonPrivate::init()
{
    Q_Q(QtMaterialFloatingActionButton);

    corner = Qt::BottomRightCorner; ///< 默认右下角
    mini   = false;                 ///< 默认标准尺寸
    offsX  = 34;                    ///< 默认 X 偏移
    offsY  = 36;                    ///< 默认 Y 偏移

    q->setRole(Material::Primary);  ///< 设置主角色
    q->setFixedSize(DefaultDiameter, DefaultDiameter); ///< 设置默认尺寸
    q->setGeometry(fabGeometry());  ///< 设置几何位置

    setupProperties();              ///< 配置阴影效果

    if (q->parentWidget()) {
        q->parentWidget()->installEventFilter(q); ///< 安装父控件事件过滤器
    }

    q->setFixedRippleRadius(50);    ///< 设置固定水波纹半径
}

/**
 * @brief 计算按钮几何位置
 * @return 按钮的矩形区域
 */
QRect QtMaterialFloatingActionButtonPrivate::fabGeometry() const
{
    Q_Q(const QtMaterialFloatingActionButton);

    QWidget *parent = q->parentWidget();
    if (!parent) {
        return QRect(); ///< 无父控件返回空矩形
    }

    const int s = mini ? MiniDiameter : DefaultDiameter; ///< 尺寸

    switch (corner)
    {
    case Qt::TopLeftCorner:     ///< 左上角
        return QRect(offsX, offsY, s, s);
    case Qt::TopRightCorner:    ///< 右上角
        return QRect(parent->width()-(offsX+s), offsY, s, s);
    case Qt::BottomLeftCorner:  ///< 左下角
        return QRect(offsX, parent->height()-(offsY+s), s, s);
    case Qt::BottomRightCorner: ///< 右下角
    default:
        break;
    }
    return QRect(parent->width()-(offsX+s), parent->height()-(offsY+s), s, s);
}

/**
 * @brief 配置阴影效果属性
 */
void QtMaterialFloatingActionButtonPrivate::setupProperties()
{
    if (mini) {
        effect->setColor(QColor(0, 0, 0, 80));           ///< 迷你模式阴影颜色
        normalState->assignProperty(effect, "offset", QPointF(0, 3)); ///< 正常状态偏移
        normalState->assignProperty(effect, "blurRadius", 13);         ///< 正常状态模糊半径
        pressedState->assignProperty(effect, "offset", QPointF(0, 7)); ///< 按下状态偏移
        pressedState->assignProperty(effect, "blurRadius", 20);        ///< 按下状态模糊半径
    } else {
        effect->setColor(QColor(0, 0, 0, 105));          ///< 标准模式阴影颜色
        normalState->assignProperty(effect, "offset", QPointF(0, 6)); ///< 正常状态偏移
        normalState->assignProperty(effect, "blurRadius", 16);         ///< 正常状态模糊半径
        pressedState->assignProperty(effect, "offset", QPointF(0, 11)); ///< 按下状态偏移
        pressedState->assignProperty(effect, "blurRadius", 28);         ///< 按下状态模糊半径
    }
}

/**
 * @brief 构造函数，初始化浮动动作按钮
 * @param icon 按钮图标
 * @param parent 父控件指针，默认为 nullptr
 */
QtMaterialFloatingActionButton::QtMaterialFloatingActionButton(const QIcon &icon, QWidget *parent)
    : QtMaterialRaisedButton(*new QtMaterialFloatingActionButtonPrivate(this), parent)
{
    d_func()->init(); ///< 初始化私有类

    setIcon(icon);    ///< 设置图标
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialFloatingActionButton::~QtMaterialFloatingActionButton()
{
}

/**
 * @brief 获取推荐尺寸
 * @return 按钮尺寸
 * @note 重写基类方法
 */
QSize QtMaterialFloatingActionButton::sizeHint() const
{
    Q_D(const QtMaterialFloatingActionButton);

    if (d->mini) {
        return QSize(QtMaterialFloatingActionButtonPrivate::MiniDiameter,
                     QtMaterialFloatingActionButtonPrivate::MiniDiameter); ///< 迷你尺寸
    } else {
        return QSize(QtMaterialFloatingActionButtonPrivate::DefaultDiameter,
                     QtMaterialFloatingActionButtonPrivate::DefaultDiameter); ///< 标准尺寸
    }
}

/**
 * @brief 设置迷你模式
 * @param state 是否启用迷你模式
 */
void QtMaterialFloatingActionButton::setMini(bool state)
{
    Q_D(QtMaterialFloatingActionButton);

    if (d->mini == state) {
        return; ///< 状态未变
    }

    d->mini = state;

    setFixedSize(d->diameter(), d->diameter()); ///< 更新尺寸
    setFixedRippleRadius(state ? 30 : 50);      ///< 更新水波纹半径

    d->setupProperties();                       ///< 更新阴影效果
    updateClipPath();                           ///< 更新剪裁路径
    setGeometry(d->fabGeometry());              ///< 更新几何位置
    update();                                   ///< 刷新界面
}

/**
 * @brief 检查是否为迷你模式
 * @return 是否为迷你模式
 */
bool QtMaterialFloatingActionButton::isMini() const
{
    Q_D(const QtMaterialFloatingActionButton);

    return d->mini;
}

/**
 * @brief 设置按钮所在角落
 * @param corner 角落位置
 */
void QtMaterialFloatingActionButton::setCorner(Qt::Corner corner)
{
    Q_D(QtMaterialFloatingActionButton);

    if (d->corner == corner) {
        return; ///< 角落未变
    }

    d->corner = corner;
    setGeometry(d->fabGeometry()); ///< 更新几何位置
    update();                      ///< 刷新界面
}

/**
 * @brief 获取按钮所在角落
 * @return 角落位置
 */
Qt::Corner QtMaterialFloatingActionButton::corner() const
{
    Q_D(const QtMaterialFloatingActionButton);

    return d->corner;
}

/**
 * @brief 设置按钮偏移
 * @param x X 轴偏移
 * @param y Y 轴偏移
 */
void QtMaterialFloatingActionButton::setOffset(int x, int y)
{
    Q_D(QtMaterialFloatingActionButton);

    d->offsX = x;
    d->offsY = y;
    setGeometry(d->fabGeometry()); ///< 更新几何位置
    update();                      ///< 刷新界面
}

/**
 * @brief 获取按钮偏移
 * @return 偏移尺寸
 */
QSize QtMaterialFloatingActionButton::offset() const
{
    Q_D(const QtMaterialFloatingActionButton);

    return QSize(d->offsX, d->offsY);
}

/**
 * @brief 设置 X 轴偏移
 * @param x X 轴偏移
 */
void QtMaterialFloatingActionButton::setXOffset(int x)
{
    Q_D(QtMaterialFloatingActionButton);

    d->offsX = x;
    setGeometry(d->fabGeometry()); ///< 更新几何位置
    update();                      ///< 刷新界面
}

/**
 * @brief 获取 X 轴偏移
 * @return X 轴偏移
 */
int QtMaterialFloatingActionButton::xOffset() const
{
    Q_D(const QtMaterialFloatingActionButton);

    return d->offsX;
}

/**
 * @brief 设置 Y 轴偏移
 * @param y Y 轴偏移
 */
void QtMaterialFloatingActionButton::setYOffset(int y)
{
    Q_D(QtMaterialFloatingActionButton);

    d->offsY = y;
    setGeometry(d->fabGeometry()); ///< 更新几何位置
    update();                      ///< 刷新界面
}

/**
 * @brief 获取 Y 轴偏移
 * @return Y 轴偏移
 */
int QtMaterialFloatingActionButton::yOffset() const
{
    Q_D(const QtMaterialFloatingActionButton);

    return d->offsY;
}

/**
 * @brief 处理事件
 * @param event 事件对象
 * @return 是否处理事件
 * @note 重写基类方法
 */
bool QtMaterialFloatingActionButton::event(QEvent *event)
{
    Q_D(QtMaterialFloatingActionButton);

    if (!parent()) {
        return QtMaterialRaisedButton::event(event); ///< 无父控件调用基类
    }
    switch (event->type())
    {
    case QEvent::ParentChange: ///< 父控件变更
    {
        parent()->installEventFilter(this); ///< 安装事件过滤器
        setGeometry(d->fabGeometry());      ///< 更新几何位置
        break;
    }
    case QEvent::ParentAboutToChange: ///< 父控件即将变更
    {
        parent()->removeEventFilter(this);  ///< 移除事件过滤器
        break;
    }
    default:
        break;
    }
    return QtMaterialRaisedButton::event(event);
}

/**
 * @brief 事件过滤器
 * @param obj 对象
 * @param event 事件对象
 * @return 是否处理事件
 * @note 重写基类方法
 */
bool QtMaterialFloatingActionButton::eventFilter(QObject *obj, QEvent *event)
{
    const QEvent::Type type = event->type();

    if (QEvent::Move == type || QEvent::Resize == type)
    {
        Q_D(QtMaterialFloatingActionButton);
        setGeometry(d->fabGeometry()); ///< 父控件移动或调整大小时更新位置
    }

    return QtMaterialRaisedButton::eventFilter(obj, event);
}

/**
 * @brief 绘制按钮
 * @param event 绘制事件
 * @note 重写基类方法
 */
void QtMaterialFloatingActionButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    Q_D(QtMaterialFloatingActionButton);

    QRect square = QRect(0, 0, d->diameter(), d->diameter());
    square.moveCenter(rect().center()); ///< 居中正方形

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing); ///< 启用抗锯齿

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);

    if (isEnabled()) {
        brush.setColor(backgroundColor()); ///< 启用时的背景色
    } else {
        brush.setColor(disabledBackgroundColor()); ///< 禁用时的背景色
    }

    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(square); ///< 绘制圆形按钮

    QRect iconGeometry(0, 0, d->iconSize(), d->iconSize());
    iconGeometry.moveCenter(square.center()); ///< 居中图标

    QPixmap pixmap = icon().pixmap(QSize(d->iconSize(), d->iconSize()));
    QPainter icon(&pixmap);
    icon.setCompositionMode(QPainter::CompositionMode_SourceIn);
    icon.fillRect(pixmap.rect(), isEnabled() ? foregroundColor()
                                             : disabledForegroundColor()); ///< 设置图标颜色
    painter.drawPixmap(iconGeometry, pixmap); ///< 绘制图标
}

/**
 * @brief 更新剪裁路径
 * @note 重写基类方法
 */
void QtMaterialFloatingActionButton::updateClipPath()
{
    Q_D(QtMaterialFloatingActionButton);

    QPainterPath path;
    path.addEllipse(0, 0, d->diameter(), d->diameter()); ///< 圆形剪裁路径
    d->rippleOverlay->setClipPath(path); ///< 设置水波纹剪裁
}