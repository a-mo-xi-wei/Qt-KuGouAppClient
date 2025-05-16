/**
 * @file qtmaterialcircularprogress.cpp
 * @brief 实现 QtMaterialCircularProgress 类，提供 Material Design 圆形进度条
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#include "qtmaterialcircularprogress.h"
#include "qtmaterialcircularprogress_p.h"
#include "qtmaterialcircularprogress_internal.h"
#include "qtmaterialstyle.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPainter>
#include <QPainterPath>

/**
 * @brief 构造函数，初始化私有类
 * @param q 指向 QtMaterialCircularProgress 的指针
 * @note 内部实现
 */
QtMaterialCircularProgressPrivate::QtMaterialCircularProgressPrivate(QtMaterialCircularProgress *q)
    : q_ptr(q)
{
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialCircularProgressPrivate::~QtMaterialCircularProgressPrivate()
{
}

/**
 * @brief 初始化圆形进度条
 */
void QtMaterialCircularProgressPrivate::init()
{
    Q_Q(QtMaterialCircularProgress);

    delegate       = new QtMaterialCircularProgressDelegate(q); ///< 创建动画代理
    progressType   = Material::IndeterminateProgress;           ///< 默认不确定性进度
    penWidth       = 6.25;                                     ///< 默认线宽
    size           = 64;                                       ///< 默认尺寸
    useThemeColors = true;                                     ///< 默认使用主题颜色

    q->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
                                 QSizePolicy::MinimumExpanding)); ///< 设置尺寸策略

    QParallelAnimationGroup *group = new QParallelAnimationGroup(q);
    group->setLoopCount(-1); ///< 无限循环

    QPropertyAnimation *animation;

    // 虚线长度动画
    animation = new QPropertyAnimation(q);
    animation->setPropertyName("dashLength");
    animation->setTargetObject(delegate);
    animation->setEasingCurve(QEasingCurve::InOutQuad); ///< 缓动曲线
    animation->setStartValue(0.1);                      ///< 起始值
    animation->setKeyValueAt(0.15, 0.2);                ///< 关键帧
    animation->setKeyValueAt(0.6, 20);
    animation->setKeyValueAt(0.7, 20);
    animation->setEndValue(20);                         ///< 终止值
    animation->setDuration(2050);                       ///< 动画时长

    group->addAnimation(animation);

    // 虚线偏移动画
    animation = new QPropertyAnimation(q);
    animation->setPropertyName("dashOffset");
    animation->setTargetObject(delegate);
    animation->setEasingCurve(QEasingCurve::InOutSine); ///< 缓动曲线
    animation->setStartValue(0);                        ///< 起始值
    animation->setKeyValueAt(0.15, 0);                  ///< 关键帧
    animation->setKeyValueAt(0.6, -7);
    animation->setKeyValueAt(0.7, -7);
    animation->setEndValue(-25);                        ///< 终止值
    animation->setDuration(2050);                       ///< 动画时长

    group->addAnimation(animation);

    // 旋转角度动画
    animation = new QPropertyAnimation(q);
    animation->setPropertyName("angle");
    animation->setTargetObject(delegate);
    animation->setStartValue(0);                        ///< 起始值
    animation->setEndValue(719);                        ///< 终止值（两圈）
    animation->setDuration(2050);                       ///< 动画时长

    group->addAnimation(animation);

    group->start(); ///< 启动动画
}

/**
 * @brief 构造函数，初始化圆形进度条
 * @param parent 父控件指针，默认为 nullptr
 */
QtMaterialCircularProgress::QtMaterialCircularProgress(QWidget *parent)
    : QProgressBar(parent),
      d_ptr(new QtMaterialCircularProgressPrivate(this))
{
    d_func()->init(); ///< 初始化私有类
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialCircularProgress::~QtMaterialCircularProgress()
{
}

/**
 * @brief 设置进度条类型
 * @param type 进度条类型（Material::ProgressType）
 */
void QtMaterialCircularProgress::setProgressType(Material::ProgressType type)
{
    Q_D(QtMaterialCircularProgress);

    d->progressType = type;
    update(); ///< 刷新界面
}

/**
 * @brief 获取进度条类型
 * @return 进度条类型
 */
Material::ProgressType QtMaterialCircularProgress::progressType() const
{
    Q_D(const QtMaterialCircularProgress);

    return d->progressType;
}

/**
 * @brief 设置是否使用主题颜色
 * @param value 是否使用主题颜色
 */
void QtMaterialCircularProgress::setUseThemeColors(bool value)
{
    Q_D(QtMaterialCircularProgress);

    if (d->useThemeColors == value) {
        return;
    }

    d->useThemeColors = value;
    update(); ///< 刷新界面
}

/**
 * @brief 获取是否使用主题颜色
 * @return 是否使用主题颜色
 */
bool QtMaterialCircularProgress::useThemeColors() const
{
    Q_D(const QtMaterialCircularProgress);

    return d->useThemeColors;
}

/**
 * @brief 设置进度条线宽
 * @param width 线宽
 */
void QtMaterialCircularProgress::setLineWidth(qreal width)
{
    Q_D(QtMaterialCircularProgress);

    d->penWidth = width;
    update();        ///< 刷新界面
    updateGeometry(); ///< 更新几何尺寸
}

/**
 * @brief 获取进度条线宽
 * @return 线宽
 */
qreal QtMaterialCircularProgress::lineWidth() const
{
    Q_D(const QtMaterialCircularProgress);

    return d->penWidth;
}

/**
 * @brief 设置进度条尺寸
 * @param size 尺寸
 */
void QtMaterialCircularProgress::setSize(int size)
{
    Q_D(QtMaterialCircularProgress);

    d->size = size;
    update();        ///< 刷新界面
    updateGeometry(); ///< 更新几何尺寸
}

/**
 * @brief 获取进度条尺寸
 * @return 尺寸
 */
int QtMaterialCircularProgress::size() const
{
    Q_D(const QtMaterialCircularProgress);

    return d->size;
}

/**
 * @brief 设置进度条颜色
 * @param color 颜色
 */
void QtMaterialCircularProgress::setColor(const QColor &color)
{
    Q_D(QtMaterialCircularProgress);

    d->color = color;

    MATERIAL_DISABLE_THEME_COLORS ///< 禁用主题颜色宏
    update();                    ///< 刷新界面
}

/**
 * @brief 获取进度条颜色
 * @return 颜色
 */
QColor QtMaterialCircularProgress::color() const
{
    Q_D(const QtMaterialCircularProgress);

    if (d->useThemeColors || !d->color.isValid()) {
        return QtMaterialStyle::instance().themeColor("primary1"); ///< 使用主题颜色
    } else {
        return d->color; ///< 使用自定义颜色
    }
}

/**
 * @brief 获取推荐尺寸
 * @return 推荐尺寸
 * @note 重写基类方法
 */
QSize QtMaterialCircularProgress::sizeHint() const
{
    Q_D(const QtMaterialCircularProgress);

    const qreal s = d->size + d->penWidth + 8; ///< 尺寸+线宽+边距
    return QSize(s, s);
}

/**
 * @brief 绘制事件
 * @param event 绘制事件对象
 * @note 重写基类方法
 */
void QtMaterialCircularProgress::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    Q_D(QtMaterialCircularProgress);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); ///< 启用抗锯齿

    if (!isEnabled())
    {
        QPen pen;
        pen.setCapStyle(Qt::RoundCap);
        pen.setWidthF(d->penWidth);
        pen.setColor(QtMaterialStyle::instance().themeColor("border")); ///< 禁用状态颜色
        painter.setPen(pen);
        painter.drawLine(rect().center() - QPointF(20, 20), rect().center() + QPointF(20, 20)); ///< 绘制交叉线
        painter.drawLine(rect().center() + QPointF(20, -20), rect().center() - QPointF(20, -20));
        return;
    }

    if (Material::IndeterminateProgress == d->progressType)
    {
        painter.translate(width() / 2, height() / 2); ///< 移动到中心
        painter.rotate(d->delegate->angle());         ///< 旋转角度
    }

    QPen pen;
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidthF(d->penWidth);
    pen.setColor(color()); ///< 设置颜色

    if (Material::IndeterminateProgress == d->progressType)
    {
        QVector<qreal> pattern;
        pattern << d->delegate->dashLength() * d->size / 50 << 30 * d->size / 50; ///< 虚线模式

        pen.setDashOffset(d->delegate->dashOffset() * d->size / 50); ///< 虚线偏移
        pen.setDashPattern(pattern);

        painter.setPen(pen);

        painter.drawEllipse(QPoint(0, 0), d->size / 2, d->size / 2); ///< 绘制圆环
    }
    else
    {
        painter.setPen(pen);

        const qreal x = (width() - d->size) / 2; ///< 水平偏移
        const qreal y = (height() - d->size) / 2; ///< 垂直偏移

        const qreal a = 360 * (value() - minimum()) / (maximum() - minimum()); ///< 弧度角度

        QPainterPath path;
        path.arcMoveTo(x, y, d->size, d->size, 0); ///< 移动到起始角度
        path.arcTo(x, y, d->size, d->size, 0, a);   ///< 绘制弧形

        painter.drawPath(path); ///< 绘制路径
    }
}