/**
 * @file qtmaterialflatbutton.cpp
 * @brief 实现 QtMaterialFlatButton 类，提供 Material Design 扁平按钮
 * @author laserpants
 * @date 2025-05CU-16
 * @version 1.0
 */

#include "qtmaterialflatbutton.h"
#include "qtmaterialflatbutton_p.h"
#include "qtmaterialrippleoverlay.h"
#include "qtmaterialripple.h"
#include "qtmaterialstyle.h"
#include "qtmaterialflatbutton_internal.h"

#include <QPainter>
#include <QResizeEvent>
#include <QIcon>

/**
 * @brief 构造函数，初始化私有类
 * @param q 指向 QtMaterialFlatButton 的指针
 * @note 内部实现
 */
QtMaterialFlatButtonPrivate::QtMaterialFlatButtonPrivate(QtMaterialFlatButton *q)
    : q_ptr(q)
{
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialFlatButtonPrivate::~QtMaterialFlatButtonPrivate()
{
}

/**
 * @brief 初始化扁平按钮
 */
void QtMaterialFlatButtonPrivate::init()
{
    Q_Q(QtMaterialFlatButton);

    rippleOverlay        = new QtMaterialRippleOverlay(q); ///< 创建水波纹覆盖层
    stateMachine         = new QtMaterialFlatButtonStateMachine(q); ///< 创建状态机
    role                 = Material::Default;              ///< 默认角色
    rippleStyle          = Material::PositionedRipple;     ///< 默认水波纹样式
    iconPlacement        = Material::LeftIcon;             ///< 默认图标位置
    overlayStyle         = Material::GrayOverlay;          ///< 默认覆盖层样式
    bgMode               = Qt::TransparentMode;            ///< 默认背景模式
    textAlignment        = Qt::AlignHCenter;               ///< 默认文本对齐
    fixedRippleRadius    = 64;                             ///< 默认水波纹半径
    cornerRadius         = 3;                              ///< 默认圆角半径
    baseOpacity          = 0.13;                           ///< 默认基础透明度
    fontSize             = 10;                             ///< 默认字体大小
    useThemeColors       = true;                           ///< 默认使用主题颜色
    useFixedRippleRadius = false;                          ///< 默认非固定水波纹半径
    haloVisible          = true;                           ///< 默认显示光晕

    q->setStyle(&QtMaterialStyle::instance());             ///< 设置 Material 风格
    q->setAttribute(Qt::WA_Hover);                        ///< 启用悬停
    q->setMouseTracking(true);                            ///< 启用鼠标跟踪

    QFont font("Roboto", fontSize, QFont::Medium);        ///< 设置 Roboto 字体
    font.setCapitalization(QFont::AllUppercase);          ///< 全部大写
    q->setFont(font);

    QPainterPath path;
    path.addRoundedRect(q->rect(), cornerRadius, cornerRadius); ///< 设置圆角剪裁
    rippleOverlay->setClipPath(path);
    rippleOverlay->setClipping(true);                     ///< 启用剪裁

    stateMachine->setupProperties();                      ///< 配置状态机属性
    stateMachine->startAnimations();                      ///< 启动动画
}

/**
 * @brief 构造函数，初始化扁平按钮
 * @param parent _every_父控件指针，默认为 nullptr
 * @param preset 按钮预设样式
 */
QtMaterialFlatButton::QtMaterialFlatButton(QWidget *parent, Material::ButtonPreset preset)
    : QPushButton(parent),
      d_ptr(new QtMaterialFlatButtonPrivate(this))
{
    d_func()->init(); ///< 初始化私有类

    applyPreset(preset); ///< 应用预设样式
}

/**
 * @brief 构造函数，初始化扁平按钮
 * @param text 按钮文本
 * @param parent 父控件指针，默认为 nullptr
 * @param preset 按钮预设样式
 */
QtMaterialFlatButton::QtMaterialFlatButton(const QString &text, QWidget *parent, Material::ButtonPreset preset)
    : QPushButton(text, parent),
      d_ptr(new QtMaterialFlatButtonPrivate(this))
{
    d_func()->init(); ///< 初始化私有类

    applyPreset(preset); ///< 应用预设样式
}

/**
 * @brief 构造函数，初始化扁平按钮
 * @param text 按钮文本
 * @param role 按钮角色
 * @param parent 父控件指针，默认为 nullptr
 * @param preset 按钮预设样式
 */
QtMaterialFlatButton::QtMaterialFlatButton(const QString &text, Material::Role role, QWidget *parent, Material::ButtonPreset preset)
    : QPushButton(text, parent),
      d_ptr(new QtMaterialFlatButtonPrivate(this))
{
    d_func()->init(); ///< 初始化私有类

    applyPreset(preset); ///< 应用预设样式
    setRole(role);       ///< 设置角色
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialFlatButton::~QtMaterialFlatButton()
{
}

/**
 * @brief 应用预设样式
 * @param preset 按钮预设样式
 */
void QtMaterialFlatButton::applyPreset(Material::ButtonPreset preset)
{
    switch (preset)
    {
    case Material::FlatPreset: ///< 扁平样式
        setOverlayStyle(Material::NoOverlay);
        break;
    case Material::CheckablePreset: ///< 可选中样式
        setOverlayStyle(Material::NoOverlay);
        setCheckable(true);
        setHaloVisible(false);
        break;
    default:
        break;
    }
}

/**
 * @brief 设置是否使用主题颜色
 * @param value 是否使用主题颜色
 */
void QtMaterialFlatButton::setUseThemeColors(bool value)
{
    Q_D(QtMaterialFlatButton);

    if (d->useThemeColors == value) {
        return; ///< 状态未变
    }

    d->useThemeColors = value;
    d->stateMachine->setupProperties(); ///< 更新状态机属性
}

/**
 * @brief 检查是否使用主题颜色
 * @return 是否使用主题颜色
 */
bool QtMaterialFlatButton::useThemeColors() const
{
    Q_D(const QtMaterialFlatButton);

    return d->useThemeColors;
}

/**
 * @brief 设置按钮角色
 * @param role 按钮角色
 */
void QtMaterialFlatButton::setRole(Material::Role role)
{
    Q_D(QtMaterialFlatButton);

    d->role = role;
    d->stateMachine->setupProperties(); ///< 更新状态机属性
}

/**
 * @brief 获取按钮角色
 * @return 按钮角色
 */
Material::Role QtMaterialFlatButton::role() const
{
    Q_D(const QtMaterialFlatButton);

    return d->role;
}

/**
 * @brief 设置前景色
 * @param color 前景色
 */
void QtMaterialFlatButton::setForegroundColor(const QColor &color)
{
    Q_D(QtMaterialFlatButton);

    d->foregroundColor = color;

    MATERIAL_DISABLE_THEME_COLORS ///< 禁用主题颜色
    update();                     ///< 刷新界面
}

/**
 * @brief 获取前景色
 * @return 前景色
 */
QColor QtMaterialFlatButton::foregroundColor() const
{
    Q_D(const QtMaterialFlatButton);

    if (d->useThemeColors || !d->foregroundColor.isValid())
    {
        if (Qt::OpaqueMode == d->bgMode) {
            return QtMaterialStyle::instance().themeColor("canvas"); ///< 不透明模式使用画布色
        }
        switch (d->role)
        {
        case Material::Primary:   ///< 主角色
            return QtMaterialStyle::instance().themeColor("primary1");
        case Material::Secondary: ///< 次角色
            return QtMaterialStyle::instance().themeColor("accent1");
        case Material::Default:   ///< 默认角色
        default:
            return QtMaterialStyle::instance().themeColor("text");
        }
    }
    return d->foregroundColor;
}

/**
 * @brief 设置背景色
 * @param color 背景色
 */
void QtMaterialFlatButton::setBackgroundColor(const QColor &color)
{
    Q_D(QtMaterialFlatButton);

    d->backgroundColor = color;

    MATERIAL_DISABLE_THEME_COLORS ///< 禁用主题颜色
    update();                     ///< 刷新界面
}

/**
 * @brief 获取背景色
 * @return 背景色
 */
QColor QtMaterialFlatButton::backgroundColor() const
{
    Q_D(const QtMaterialFlatButton);

    if (d->useThemeColors || !d->backgroundColor.isValid())
    {
        switch (d->role)
        {
        case Material::Primary:   ///< 主角色
            return QtMaterialStyle::instance().themeColor("primary1");
        case Material::Secondary: ///< 次角色
            return QtMaterialStyle::instance().themeColor("accent1");
        case Material::Default:   ///< 默认角色
        default:
            return QtMaterialStyle::instance().themeColor("text");
        }
    }
    return d->backgroundColor;
}

/**
 * @brief 设置覆盖层颜色
 * @param color 覆盖层颜色
 */
void QtMaterialFlatButton::setOverlayColor(const QColor &color)
{
    Q_D(QtMaterialFlatButton);

    d->overlayColor = color;

    MATERIAL_DISABLE_THEME_COLORS ///< 禁用主题颜色

    setOverlayStyle(Material::TintedOverlay); ///< 设置着色覆盖层
    update();                                 ///< 刷新界面
}

/**
 * @brief 获取覆盖层颜色
 * @return 覆盖层颜色
 */
QColor QtMaterialFlatButton::overlayColor() const
{
    Q_D(const QtMaterialFlatButton);

    if (d->useThemeColors || !d->overlayColor.isValid()) {
        return foregroundColor(); ///< 默认使用前景色
    }
    return d->overlayColor;
}

/**
 * @brief 设置禁用状态前景色
 * @param color 禁用状态前景色
 */
void QtMaterialFlatButton::setDisabledForegroundColor(const QColor &color)
{
    Q_D(QtMaterialFlatButton);

    d->disabledColor = color;

    MATERIAL_DISABLE_THEME_COLORS ///< 禁用主题颜色
    update();                     ///< 刷新界面
}

/**
 * @brief 获取禁用状态前景色
 * @return 禁用状态前景色
 */
QColor QtMaterialFlatButton::disabledForegroundColor() const
{
    Q_D(const QtMaterialFlatButton);

    if (d->useThemeColors || !d->disabledColor.isValid()) {
        return QtMaterialStyle::instance().themeColor("disabled"); ///< 默认禁用颜色
    } else {
        return d->disabledColor;
    }
}

/**
 * @brief 设置禁用状态背景色
 * @param color 禁用状态背景色
 */
void QtMaterialFlatButton::setDisabledBackgroundColor(const QColor &color)
{
    Q_D(QtMaterialFlatButton);

    d->disabledBackgroundColor = color;

    MATERIAL_DISABLE_THEME_COLORS ///< 禁用主题颜色
    update();                     ///< 刷新界面
}

/**
 * @brief 获取禁用状态背景色
 * @return 禁用状态背景色
 */
QColor QtMaterialFlatButton::disabledBackgroundColor() const
{
    Q_D(const QtMaterialFlatButton);

    if (d->useThemeColors || !d->disabledBackgroundColor.isValid()) {
        return QtMaterialStyle::instance().themeColor("disabled3"); ///< 默认禁用背景色
    } else {
        return d->disabledBackgroundColor;
    }
}

/**
 * @brief 设置字体大小
 * @param size 字体大小
 */
void QtMaterialFlatButton::setFontSize(qreal size)
{
    Q_D(QtMaterialFlatButton);

    d->fontSize = size;

    QFont f(font());
    f.setPointSizeF(size);
    setFont(f); ///< 更新字体

    update();   ///< 刷新界面
}

/**
 * @brief 获取字体大小
 * @return 字体大小
 */
qreal QtMaterialFlatButton::fontSize() const
{
    Q_D(const QtMaterialFlatButton);

    return d->fontSize;
}

/**
 * @brief 设置光晕是否可见
 * @param visible 是否显示光晕
 */
void QtMaterialFlatButton::setHaloVisible(bool visible)
{
    Q_D(QtMaterialFlatButton);

    d->haloVisible = visible;
    update(); ///< 刷新界面
}

/**
 * @brief 检查光晕是否可见
 * @return 是否显示光晕
 */
bool QtMaterialFlatButton::isHaloVisible() const
{
    Q_D(const QtMaterialFlatButton);

    return d->haloVisible;
}

/**
 * @brief 设置覆盖层样式
 * @param style 覆盖层样式
 */
void QtMaterialFlatButton::setOverlayStyle(Material::OverlayStyle style)
{
    Q_D(QtMaterialFlatButton);

    d->overlayStyle = style;
    update(); ///< 刷新界面
}

/**
 * @brief 获取覆盖层样式
 * @return 覆盖层样式
 */
Material::OverlayStyle QtMaterialFlatButton::overlayStyle() const
{
    Q_D(const QtMaterialFlatButton);

    return d->overlayStyle;
}

/**
 * @brief 设置水波纹样式
 * @param style 水波纹样式
 */
void QtMaterialFlatButton::setRippleStyle(Material::RippleStyle style)
{
    Q_D(QtMaterialFlatButton);

    d->rippleStyle = style;
}

/**
 * @brief 获取水波纹样式
 * @return 水波纹样式
 */
Material::RippleStyle QtMaterialFlatButton::rippleStyle() const
{
    Q_D(const QtMaterialFlatButton);

    return d->rippleStyle;
}

/**
 * @brief 设置图标位置
 * @param placement 图标位置
 */
void QtMaterialFlatButton::setIconPlacement(Material::ButtonIconPlacement placement)
{
    Q_D(QtMaterialFlatButton);

    d->iconPlacement = placement;
    update(); ///< 刷新界面
}

/**
 * @brief 获取图标位置
 * @return 图标位置
 */
Material::ButtonIconPlacement QtMaterialFlatButton::iconPlacement() const
{
    Q_D(const QtMaterialFlatButton);

    return d->iconPlacement;
}

/**
 * @brief 设置圆角半径
 * @param radius 圆角半径
 */
void QtMaterialFlatButton::setCornerRadius(qreal radius)
{
    Q_D(QtMaterialFlatButton);

    d->cornerRadius = radius;
    updateClipPath(); ///< 更新剪裁路径
    update();         ///< 刷新界面
}

/**
 * @brief 获取圆角半径
 * @return 圆角半径
 */
qreal QtMaterialFlatButton::cornerRadius() const
{
    Q_D(const QtMaterialFlatButton);

    return d->cornerRadius;
}

/**
 * @brief 设置背景模式
 * @param mode 背景模式
 */
void QtMaterialFlatButton::setBackgroundMode(Qt::BGMode mode)
{
    Q_D(QtMaterialFlatButton);

    d->bgMode = mode;
    d->stateMachine->setupProperties(); ///< 更新状态机属性
}

/**
 * @brief 获取背景模式
 * @return 背景模式
 */
Qt::BGMode QtMaterialFlatButton::backgroundMode() const
{
    Q_D(const QtMaterialFlatButton);

    return d->bgMode;
}

/**
 * @brief 设置基础透明度
 * @param opacity 基础透明度
 */
void QtMaterialFlatButton::setBaseOpacity(qreal opacity)
{
    Q_D(QtMaterialFlatButton);

    d->baseOpacity = opacity;
    d->stateMachine->setupProperties(); ///< 更新状态机属性
}

/**
 * @brief 获取基础透明度
 * @return 基础透明度
 */
qreal QtMaterialFlatButton::baseOpacity() const
{
    Q_D(const QtMaterialFlatButton);

    return d->baseOpacity;
}

/**
 * @brief 设置是否可选中
 * @param value 是否可选中
 */
void QtMaterialFlatButton::setCheckable(bool value)
{
    Q_D(QtMaterialFlatButton);

    d->stateMachine->updateCheckedStatus(); ///< 更新选中状态

    QPushButton::setCheckable(value);
}

/**
 * @brief 设置是否使用固定水波纹半径
 * @param value 是否固定
 */
void QtMaterialFlatButton::setHasFixedRippleRadius(bool value)
{
    Q_D(QtMaterialFlatButton);

    d->useFixedRippleRadius = value;
}

/**
 * @brief 检查是否使用固定水波纹半径
 * @return 是否固定
 */
bool QtMaterialFlatButton::hasFixedRippleRadius() const
{
    Q_D(const QtMaterialFlatButton);

    return d->useFixedRippleRadius;
}

/**
 * @brief 设置固定水波纹半径
 * @param radius 水波纹半径
 */
void QtMaterialFlatButton::setFixedRippleRadius(qreal radius)
{
    Q_D(QtMaterialFlatButton);

    d->fixedRippleRadius = radius;
    setHasFixedRippleRadius(true); ///< 启用固定半径
}

/**
 * @brief 设置文本对齐方式
 * @param alignment 文本对齐方式
 */
void QtMaterialFlatButton::setTextAlignment(Qt::Alignment alignment)
{
    Q_D(QtMaterialFlatButton);

    d->textAlignment = alignment;
}

/**
 * @brief 获取文本对齐方式
 * @return 文本对齐方式
 */
Qt::Alignment QtMaterialFlatButton::textAlignment() const
{
    Q_D(const QtMaterialFlatButton);

    return d->textAlignment;
}

/**
 * @brief 获取推荐尺寸
 * @return 按钮尺寸
 * @note 重写基类方法
 */
QSize QtMaterialFlatButton::sizeHint() const
{
    ensurePolished();

    QSize label(fontMetrics().size(Qt::TextSingleLine, text())); ///< 文本尺寸

    int w = 20 + label.width(); ///< 宽度
    int h = label.height();     ///< 高度
    if (!icon().isNull()) {
        w += iconSize().width() + QtMaterialFlatButton::IconPadding; ///< 包含图标
        h = qMax(h, iconSize().height());
    }
    return QSize(w, 20 + h);
}

/**
 * @brief 构造函数，初始化扁平按钮（私有）
 * @param d 私有类引用
 * @param parent 父控件指针，默认为 nullptr
 * @param preset 按钮预设样式
 * @note 内部实现
 */
QtMaterialFlatButton::QtMaterialFlatButton(QtMaterialFlatButtonPrivate &d, QWidget *parent, Material::ButtonPreset preset)
    : QPushButton(parent),
      d_ptr(&d)
{
    d_func()->init(); ///< 初始化私有类

    applyPreset(preset); ///< 应用预设样式
}

/**
 * @brief 检查选中状态
 * @note 重写基类方法
 */
void QtMaterialFlatButton::checkStateSet()
{
    Q_D(QtMaterialFlatButton);

    d->stateMachine->updateCheckedStatus(); ///< 更新选中状态

    QPushButton::checkStateSet();
}

/**
 * @brief 处理鼠标按下事件
 * @param event 鼠标事件
 * @note 重写基类方法
 */
void QtMaterialFlatButton::mousePressEvent(QMouseEvent *event)
{
    Q_D(QtMaterialFlatButton);

    if (Material::NoRipple != d->rippleStyle)
    {
        QPoint pos;
        qreal radiusEndValue;

        if (Material::CenteredRipple == d->rippleStyle) {
            pos = rect().center(); ///< 中心水波纹
        } else {
            pos = event->pos();    ///< 鼠标位置水波纹
        }

        if (d->useFixedRippleRadius) {
            radiusEndValue = d->fixedRippleRadius; ///< 固定半径
        } else {
            radiusEndValue = static_cast<qreal>(width())/2; ///< 动态半径
        }

        QtMaterialRipple *ripple = new QtMaterialRipple(pos);

        ripple->setRadiusEndValue(radiusEndValue);      ///< 设置终止半径
        ripple->setOpacityStartValue(0.35);             ///< 设置起始透明度
        ripple->setColor(foregroundColor());            ///< 设置水波纹颜色
        ripple->radiusAnimation()->setDuration(600);    ///< 半径动画时长
        ripple->opacityAnimation()->setDuration(1300);  ///< 透明度动画时长

        d->rippleOverlay->addRipple(ripple);            ///< 添加水波纹
    }

    QPushButton::mousePressEvent(event);
}

/**
 * @brief 处理鼠标释放事件
 * @param event 鼠标事件
 * @note 重写基类方法
 */
void QtMaterialFlatButton::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(QtMaterialFlatButton);

    QPushButton::mouseReleaseEvent(event);

    d->stateMachine->updateCheckedStatus(); ///< 更新选中状态
}

/**
 * @brief 处理调整大小事件
 * @param event 调整大小事件
 * @note 重写基类方法
 */
void QtMaterialFlatButton::resizeEvent(QResizeEvent *event)
{
    QPushButton::resizeEvent(event);

    updateClipPath(); ///< 更新剪裁路径
}

/**
 * @brief 绘制按钮
 * @param event 绘制事件
 * @note 重写基类方法
 */
void QtMaterialFlatButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    Q_D(QtMaterialFlatButton);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); ///< 启用抗锯齿

    const qreal cr = d->cornerRadius;

    if (cr > 0)
    {
        QPainterPath path;
        path.addRoundedRect(rect(), cr, cr); ///< 设置圆角剪裁

        painter.setClipPath(path);
        painter.setClipping(true); ///< 启用剪裁
    }

    paintBackground(&painter); ///< 绘制背景
    paintHalo(&painter);       ///< 绘制光晕

    painter.setOpacity(1);
    painter.setClipping(false);

    paintForeground(&painter); ///< 绘制前景
}

/**
 * @brief 绘制背景
 * @param painter 画笔对象
 * @note 内部实现
 */
void QtMaterialFlatButton::paintBackground(QPainter *painter)
{
    Q_D(QtMaterialFlatButton);

    const qreal overlayOpacity = d->stateMachine->overlayOpacity(); ///< 覆盖层透明度
    const qreal checkedProgress = d->stateMachine->checkedOverlayProgress(); ///< 选中进度

    if (Qt::OpaqueMode == d->bgMode) {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        if (isEnabled()) {
            brush.setColor(backgroundColor()); ///< 启用时的背景色
        } else {
            brush.setColor(disabledBackgroundColor()); ///< 禁用时的背景色
        }
        painter->setOpacity(1);
        painter->setBrush(brush);
        painter->setPen(Qt::NoPen);
        painter->drawRect(rect()); ///< 绘制背景矩形
    }

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    painter->setPen(Qt::NoPen);

    if (!isEnabled()) {
        return; ///< 禁用时不绘制覆盖层
    }

    if ((Material::NoOverlay != d->overlayStyle) && (overlayOpacity > 0)) {
        if (Material::TintedOverlay == d->overlayStyle) {
            brush.setColor(overlayColor()); ///< 着色覆盖层
        } else {
            brush.setColor(Qt::gray);       ///< 灰色覆盖层
        }
        painter->setOpacity(overlayOpacity);
        painter->setBrush(brush);
        painter->drawRect(rect());          ///< 绘制覆盖层
    }

    if (isCheckable() && checkedProgress > 0) {
        const qreal q = Qt::TransparentMode == d->bgMode ? 0.45 : 0.7; ///< 透明度系数
        brush.setColor(foregroundColor());
        painter->setOpacity(q*checkedProgress);
        painter->setBrush(brush);
        QRect r(rect());
        r.setHeight(static_cast<qreal>(r.height())*checkedProgress); ///< 选中进度矩形
        painter->drawRect(r);                                       ///< 绘制选中效果
    }
}

/**
 * @brief 绘制光晕
 * @param painter 画笔对象
 * @note 内部实现
 */
void QtMaterialFlatButton::paintHalo(QPainter *painter)
{
    Q_D(QtMaterialFlatButton);

    if (!d->haloVisible) {
        return; ///< 光晕不可见
    }

    const qreal opacity = d->stateMachine->haloOpacity(); ///< 光晕透明度
    const qreal s = d->stateMachine->haloScaleFactor()*d->stateMachine->haloSize(); ///< 光晕缩放
    const qreal radius = static_cast<qreal>(width())*s;   ///< 光晕半径

    if (isEnabled() && opacity > 0) {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(foregroundColor()); ///< 光晕颜色
        painter->setOpacity(opacity);
        painter->setBrush(brush);
        painter->setPen(Qt::NoPen);
        const QPointF center = rect().center();
        painter->drawEllipse(center, radius, radius); ///< 绘制光晕圆
    }
}

/** @brief 颜色插值宏，计算源颜色和目标颜色之间的插值 */
#define COLOR_INTERPOLATE(CH) (1-progress)*source.CH() + progress*dest.CH()

/**
 * @brief 绘制前景
 * @param painter 画笔对象
 * @note 内部实现
 */
void QtMaterialFlatButton::paintForeground(QPainter *painter)
{
    Q_D(QtMaterialFlatButton);

    if (isEnabled()) {
        painter->setPen(foregroundColor()); ///< 启用时的前景色
        const qreal progress = d->stateMachine->checkedOverlayProgress(); ///< 选中进度
        if (isCheckable() && progress > 0) {
            QColor source = foregroundColor();
            QColor dest = Qt::TransparentMode == d->bgMode ? Qt::white
                                                           : backgroundColor(); ///< 目标颜色
            if (qFuzzyCompare(1, progress)) {
                painter->setPen(dest); ///< 完全选中时使用目标颜色
            } else {
                painter->setPen(QColor(COLOR_INTERPOLATE(red),
                                       COLOR_INTERPOLATE(green),
                                       COLOR_INTERPOLATE(blue),
                                       COLOR_INTERPOLATE(alpha))); ///< 插值颜色
            }
        }
    } else {
        painter->setPen(disabledForegroundColor()); ///< 禁用时的前景色
    }

    if (icon().isNull())  {
        if (Qt::AlignLeft == d->textAlignment) {
            painter->drawText(rect().adjusted(12, 0, 0, 0), Qt::AlignLeft | Qt::AlignVCenter, text()); ///< 左对齐文本
        } else {
            painter->drawText(rect(), Qt::AlignCenter, text()); ///< 居中文本
        }
        return;
    }

    QSize textSize(fontMetrics().size(Qt::TextSingleLine, text())); ///< 文本尺寸
    QSize base(size()-textSize);                                    ///< 基础尺寸

    const int iw = iconSize().width() + IconPadding;               ///< 图标宽度
    QPoint pos(Qt::AlignLeft == d->textAlignment ? 12 : (base.width()-iw)/2, 0); ///< 起始位置

    QRect textGeometry(pos + QPoint(0, base.height()/2), textSize); ///< 文本区域
    QRect iconGeometry(pos + QPoint(0, (height()-iconSize().height())/2), iconSize()); ///< 图标区域

    if (Material::LeftIcon == d->iconPlacement) {
        textGeometry.translate(iw, 0); ///< 图标在左，文本右移
    } else {
        iconGeometry.translate(textSize.width() + IconPadding, 0); ///< 图标在右
    }

    painter->drawText(textGeometry, Qt::AlignCenter, text()); ///< 绘制文本

    QPixmap pixmap = icon().pixmap(iconSize());
    QPainter icon(&pixmap);
    icon.setCompositionMode(QPainter::CompositionMode_SourceIn);
    icon.fillRect(pixmap.rect(), painter->pen().color()); ///< 设置图标颜色
    painter->drawPixmap(iconGeometry, pixmap);            ///< 绘制图标
}

/**
 * @brief 更新剪裁路径
 * @note 内部实现
 */
void QtMaterialFlatButton::updateClipPath()
{
    Q_D(QtMaterialFlatButton);

    const qreal radius = d->cornerRadius;

    QPainterPath path;
    path.addRoundedRect(rect(), radius, radius); ///< 圆角剪裁路径
    d->rippleOverlay->setClipPath(path);         ///< 设置水波纹剪裁
}