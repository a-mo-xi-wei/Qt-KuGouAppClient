/**
 * @file qtmaterialsnackbar.cpp
 * @brief 实现 QtMaterialSnackbar 类，提供 Material Design 提示条
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#include "qtmaterialsnackbar.h"
#include "qtmaterialsnackbar_p.h"
#include "qtmaterialsnackbar_internal.h"
#include "qtmaterialstyle.h"
#include "qtmaterialstatetransition.h"
#include <QApplication>
#include <QPainter>

/**
 * @brief 构造函数，初始化私有类
 * @param q 指向 QtMaterialSnackbar 的指针
 * @note 内部实现
 */
QtMaterialSnackbarPrivate::QtMaterialSnackbarPrivate(QtMaterialSnackbar *q)
    : q_ptr(q)
{
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialSnackbarPrivate::~QtMaterialSnackbarPrivate()
{
}

/**
 * @brief 初始化提示条
 */
void QtMaterialSnackbarPrivate::init()
{
    Q_Q(QtMaterialSnackbar);

    stateMachine   = new QtMaterialSnackbarStateMachine(q); ///< 创建状态机
    bgOpacity      = 0.9;                                  ///< 默认背景透明度
    duration       = 3000;                                 ///< 默认持续时间
    boxWidth       = 300;                                  ///< 默认宽度
    clickDismiss   = false;                                ///< 默认禁用点击隐藏
    useThemeColors = true;                                 ///< 默认使用主题颜色

    q->setAttribute(Qt::WA_TransparentForMouseEvents); ///< 初始透明鼠标事件

    QFont font("Microsoft YaHei Ui", 12, QFont::Medium);
    font.setCapitalization(QFont::AllUppercase); ///< 设置全大写
    q->setFont(font);                           ///< 设置字体

    stateMachine->start(); ///< 启动状态机
    QCoreApplication::processEvents(); ///< 处理事件队列
}

/**
 * @brief 构造函数，初始化提示条
 * @param parent 父控件指针，默认为 nullptr
 */
QtMaterialSnackbar::QtMaterialSnackbar(QWidget *parent)
    : QtMaterialOverlayWidget(parent),
      d_ptr(new QtMaterialSnackbarPrivate(this))
{
    d_func()->init(); ///< 初始化私有类
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialSnackbar::~QtMaterialSnackbar()
{
}

/**
 * @brief 设置自动隐藏持续时间
 * @param duration 持续时间（毫秒）
 */
void QtMaterialSnackbar::setAutoHideDuration(int duration)
{
    Q_D(QtMaterialSnackbar);

    d->duration = duration;
}

/**
 * @brief 获取自动隐藏持续时间
 * @return 持续时间（毫秒）
 */
int QtMaterialSnackbar::autoHideDuration() const
{
    Q_D(const QtMaterialSnackbar);

    return d->duration;
}

/**
 * @brief 设置是否使用主题颜色
 * @param value 是否使用主题颜色
 */
void QtMaterialSnackbar::setUseThemeColors(bool value)
{
    Q_D(QtMaterialSnackbar);

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
bool QtMaterialSnackbar::useThemeColors() const
{
    Q_D(const QtMaterialSnackbar);

    return d->useThemeColors;
}

/**
 * @brief 设置背景颜色
 * @param color 背景颜色
 */
void QtMaterialSnackbar::setBackgroundColor(const QColor &color)
{
    Q_D(QtMaterialSnackbar);

    d->backgroundColor = color;

    MATERIAL_DISABLE_THEME_COLORS ///< 禁用主题颜色宏
    update();                    ///< 刷新界面
}

/**
 * @brief 获取背景颜色
 * @return 背景颜色
 */
QColor QtMaterialSnackbar::backgroundColor() const
{
    Q_D(const QtMaterialSnackbar);

    if (d->useThemeColors || !d->backgroundColor.isValid()) {
        return QtMaterialStyle::instance().themeColor("text"); ///< 使用主题颜色
    } else {
        return d->backgroundColor; ///< 使用自定义颜色
    }
}

/**
 * @brief 设置背景透明度
 * @param opacity 透明度值
 */
void QtMaterialSnackbar::setBackgroundOpacity(qreal opacity)
{
    Q_D(QtMaterialSnackbar);

    d->bgOpacity = opacity;
    update(); ///< 刷新界面
}

/**
 * @brief 获取背景透明度
 * @return 透明度值
 */
qreal QtMaterialSnackbar::backgroundOpacity() const
{
    Q_D(const QtMaterialSnackbar);

    return d->bgOpacity;
}

/**
 * @brief 设置文本颜色
 * @param color 文本颜色
 */
void QtMaterialSnackbar::setTextColor(const QColor &color)
{
    Q_D(QtMaterialSnackbar);

    d->textColor = color;

    MATERIAL_DISABLE_THEME_COLORS ///< 禁用主题颜色宏
    update();                    ///< 刷新界面
}

/**
 * @brief 获取文本颜色
 * @return 文本颜色
 */
QColor QtMaterialSnackbar::textColor() const
{
    Q_D(const QtMaterialSnackbar);

    if (d->useThemeColors || !d->textColor.isValid()) {
        return QtMaterialStyle::instance().themeColor("canvas"); ///< 使用主题颜色
    } else {
        return d->textColor; ///< 使用自定义颜色
    }
}

/**
 * @brief 设置字体大小
 * @param size 字体大小（点）
 */
void QtMaterialSnackbar::setFontSize(qreal size)
{
    QFont f(font());
    f.setPointSizeF(size);
    setFont(f); ///< 更新字体

    update(); ///< 刷新界面
}

/**
 * @brief 获取字体大小
 * @return 字体大小（点）
 */
qreal QtMaterialSnackbar::fontSize() const
{
    return font().pointSizeF();
}

/**
 * @brief 设置提示条宽度
 * @param width 宽度（像素）
 */
void QtMaterialSnackbar::setBoxWidth(int width)
{
    Q_D(QtMaterialSnackbar);

    d->boxWidth = width;
    update(); ///< 刷新界面
}

/**
 * @brief 获取提示条宽度
 * @return 宽度（像素）
 */
int QtMaterialSnackbar::boxWidth() const
{
    Q_D(const QtMaterialSnackbar);

    return d->boxWidth;
}

/**
 * @brief 设置点击隐藏模式
 * @param value 是否启用点击隐藏
 */
void QtMaterialSnackbar::setClickToDismissMode(bool value)
{
    Q_D(QtMaterialSnackbar);

    d->clickDismiss = value;
}

/**
 * @brief 获取点击隐藏模式
 * @return 是否启用点击隐藏
 */
bool QtMaterialSnackbar::clickToDismissMode() const
{
    Q_D(const QtMaterialSnackbar);

    return d->clickDismiss;
}

/**
 * @brief 添加消息到队列
 * @param message 消息内容
 */
void QtMaterialSnackbar::addMessage(const QString &message)
{
    Q_D(QtMaterialSnackbar);

    d->messages.push_back(message); ///< 添加到队列尾部
    d->stateMachine->postEvent(new QtMaterialStateTransitionEvent(SnackbarShowTransition)); ///< 触发显示
    raise(); ///< 提升到顶层
}

/**
 * @brief 立即添加消息
 * @param message 消息内容
 */
void QtMaterialSnackbar::addInstantMessage(const QString &message)
{
    Q_D(QtMaterialSnackbar);

    if (d->messages.isEmpty()) {
        d->messages.push_back(message); ///< 队列为空时添加到尾部
    } else {
        d->messages.insert(1, message); ///< 否则插入到第二位
    }

    d->stateMachine->progress(); ///< 推进状态
}

/**
 * @brief 移除并处理队列中的消息
 */
void QtMaterialSnackbar::dequeue()
{
    Q_D(QtMaterialSnackbar);

    if (d->messages.isEmpty()) {
        return;
    }

    d->messages.removeFirst(); ///< 移除首条消息

    if (!d->messages.isEmpty()) {
        d->stateMachine->postEvent(new QtMaterialStateTransitionEvent(SnackbarNextTransition)); ///< 显示下一条
    } else {
        d->stateMachine->postEvent(new QtMaterialStateTransitionEvent(SnackbarWaitTransition)); ///< 进入等待状态
    }
}

/**
 * @brief 绘制事件
 * @param event 绘制事件对象
 * @note 重写基类方法
 */
void QtMaterialSnackbar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    Q_D(QtMaterialSnackbar);

    if (d->messages.isEmpty()) {
        return; ///< 无消息不绘制
    }

    QString message = d->messages.first(); ///< 获取首条消息

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); ///< 启用抗锯齿

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(backgroundColor()); ///< 设置背景颜色
    painter.setBrush(brush);
    painter.setOpacity(d->bgOpacity); ///< 设置背景透明度

    QRect r(0, 0, d->boxWidth, 40); ///< 初始矩形

    painter.setPen(Qt::white); ///< 临时笔用于计算文本边界
    QRect br = painter.boundingRect(r, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, message); ///< 计算文本边界

    painter.setPen(Qt::NoPen); ///< 无边框
    r = br.united(r).adjusted(-10, -10, 10, 20); ///< 调整矩形边界

    const qreal s = 1 - d->stateMachine->offset(); ///< 计算动画偏移

    painter.translate((width() - (r.width() - 20)) / 2,
                      height() + 10 - s * (r.height())); ///< 居中并应用偏移

    br.moveCenter(r.center()); ///< 文本矩形居中
    painter.drawRoundedRect(r.adjusted(0, 0, 0, 3), 3, 3); ///< 绘制圆角矩形
    painter.setPen(textColor()); ///< 设置文本颜色
    painter.drawText(br, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, message); ///< 绘制文本
}