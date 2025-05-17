/**
 * @file qtmaterialtextfield.cpp
 * @brief 实现 QtMaterialTextField 类，提供 Material Design 文本输入框
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#include "qtmaterialtextfield.h"
#include "qtmaterialtextfield_p.h"
#include "qtmaterialtextfield_internal.h"
#include "qtmaterialstyle.h"
#include <QApplication>
#include <QPainter>

/**
 * @brief 构造函数，初始化私有类
 * @param q 指向 QtMaterialTextField 的指针
 * @note 内部实现
 */
QtMaterialTextFieldPrivate::QtMaterialTextFieldPrivate(QtMaterialTextField *q)
    : q_ptr(q)
{
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialTextFieldPrivate::~QtMaterialTextFieldPrivate()
{
}

/**
 * @brief 初始化文本输入框
 */
void QtMaterialTextFieldPrivate::init()
{
    Q_Q(QtMaterialTextField);

    stateMachine   = new QtMaterialTextFieldStateMachine(q); ///< 创建状态机
    label          = nullptr;                               ///< 初始无标签
    labelFontSize  = 9.5;                                  ///< 默认标签字体大小
    showLabel      = false;                                ///< 默认隐藏标签
    showInputLine  = true;                                 ///< 默认显示输入线
    useThemeColors = true;                                 ///< 默认使用主题颜色

    q->setFrame(false);                                    ///< 禁用边框
    q->setStyle(&QtMaterialStyle::instance());             ///< 设置 Material 风格
    q->setAttribute(Qt::WA_Hover);                        ///< 启用悬停
    q->setMouseTracking(true);                            ///< 启用鼠标跟踪
    q->setTextMargins(0, 2, 0, 4);                       ///< 设置文本边距

    q->setFont(QFont("Roboto", 11, QFont::Normal));       ///< 设置默认字体

    stateMachine->start();                                ///< 启动状态机
    QCoreApplication::processEvents();                    ///< 处理事件队列
}

/**
 * @brief 构造函数，初始化文本输入框
 * @param parent 父控件指针，默认为 nullptr
 */
QtMaterialTextField::QtMaterialTextField(QWidget *parent)
    : QLineEdit(parent),
      d_ptr(new QtMaterialTextFieldPrivate(this))
{
    d_func()->init(); ///< 初始化私有类
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialTextField::~QtMaterialTextField()
{
}

/**
 * @brief 设置是否使用主题颜色
 * @param value 是否使用主题颜色
 */
void QtMaterialTextField::setUseThemeColors(bool value)
{
    Q_D(QtMaterialTextField);

    if (d->useThemeColors == value) {
        return;
    }

    d->useThemeColors = value;
    d->stateMachine->setupProperties(); ///< 更新属性
}

/**
 * @brief 获取是否使用主题颜色
 * @return 是否使用主题颜色
 */
bool QtMaterialTextField::useThemeColors() const
{
    Q_D(const QtMaterialTextField);

    return d->useThemeColors;
}

/**
 * @brief 设置是否显示标签
 * @param value 是否显示标签
 */
void QtMaterialTextField::setShowLabel(bool value)
{
    Q_D(QtMaterialTextField);

    if (d->showLabel == value) {
        return;
    }

    d->showLabel = value;

    if (!d->label && value) {
        d->label = new QtMaterialTextFieldLabel(this); ///< 创建标签
        d->stateMachine->setLabel(d->label);          ///< 设置状态机标签
    }

    if (value) {
        setContentsMargins(0, 23, 0, 0); ///< 调整边距以容纳标签
    } else {
        setContentsMargins(0, 0, 0, 0);  ///< 恢复默认边距
    }
}

/**
 * @brief 获取是否显示标签
 * @return 是否显示标签
 */
bool QtMaterialTextField::hasLabel() const
{
    Q_D(const QtMaterialTextField);

    return d->showLabel;
}

/**
 * @brief 设置标签字体大小
 * @param size 字体大小（点）
 */
void QtMaterialTextField::setLabelFontSize(qreal size)
{
    Q_D(QtMaterialTextField);

    d->labelFontSize = size;

    if (d->label)
    {
        QFont font(d->label->font());
        font.setPointSizeF(size);
        d->label->setFont(font); ///< 更新标签字体
        d->label->update();      ///< 刷新标签
    }
}

/**
 * @brief 获取标签字体大小
 * @return 字体大小（点）
 */
qreal QtMaterialTextField::labelFontSize() const
{
    Q_D(const QtMaterialTextField);

    return d->labelFontSize;
}

/**
 * @brief 设置标签文本
 * @param label 标签文本
 */
void QtMaterialTextField::setLabel(const QString &label)
{
    Q_D(QtMaterialTextField);

    d->labelString = label;
    setShowLabel(true);       ///< 启用标签显示
    d->label->update();       ///< 刷新标签
}

/**
 * @brief 获取标签文本
 * @return 标签文本
 */
QString QtMaterialTextField::label() const
{
    Q_D(const QtMaterialTextField);

    return d->labelString;
}

/**
 * @brief 设置文本颜色
 * @param color 文本颜色
 */
void QtMaterialTextField::setTextColor(const QColor &color)
{
    Q_D(QtMaterialTextField);

    d->textColor = color;
    setStyleSheet(QString("QLineEdit { color: %1; }").arg(color.name())); ///< 更新样式表

    MATERIAL_DISABLE_THEME_COLORS ///< 禁用主题颜色宏
    d->stateMachine->setupProperties(); ///< 更新属性
}

/**
 * @brief 获取文本颜色
 * @return 文本颜色
 */
QColor QtMaterialTextField::textColor() const
{
    Q_D(const QtMaterialTextField);

    if (d->useThemeColors || !d->textColor.isValid()) {
        return QtMaterialStyle::instance().themeColor("text"); ///< 使用主题颜色
    } else {
        return d->textColor; ///< 使用自定义颜色
    }
}

/**
 * @brief 设置标签颜色
 * @param color 标签颜色
 */
void QtMaterialTextField::setLabelColor(const QColor &color)
{
    Q_D(QtMaterialTextField);

    d->labelColor = color;

    MATERIAL_DISABLE_THEME_COLORS ///< 禁用主题颜色宏
    d->stateMachine->setupProperties(); ///< 更新属性
}

/**
 * @brief 获取标签颜色
 * @return 标签颜色
 */
QColor QtMaterialTextField::labelColor() const
{
    Q_D(const QtMaterialTextField);

    if (d->useThemeColors || !d->labelColor.isValid()) {
        return QtMaterialStyle::instance().themeColor("accent3"); ///< 使用主题颜色
    } else {
        return d->labelColor; ///< 使用自定义颜色
    }
}

/**
 * @brief 设置墨水颜色
 * @param color 墨水颜色
 */
void QtMaterialTextField::setInkColor(const QColor &color)
{
    Q_D(QtMaterialTextField);

    d->inkColor = color;

    MATERIAL_DISABLE_THEME_COLORS ///< 禁用主题颜色宏
    d->stateMachine->setupProperties(); ///< 更新属性
}

/**
 * @brief 获取墨水颜色
 * @return 墨水颜色
 */
QColor QtMaterialTextField::inkColor() const
{
    Q_D(const QtMaterialTextField);

    if (d->useThemeColors || !d->inkColor.isValid()) {
        return QtMaterialStyle::instance().themeColor("primary1"); ///< 使用主题颜色
    } else {
        return d->inkColor; ///< 使用自定义颜色
    }
}

/**
 * @brief 设置输入线颜色
 * @param color 输入线颜色
 */
void QtMaterialTextField::setInputLineColor(const QColor &color)
{
    Q_D(QtMaterialTextField);

    d->inputLineColor = color;

    MATERIAL_DISABLE_THEME_COLORS ///< 禁用主题颜色宏
    d->stateMachine->setupProperties(); ///< 更新属性
}

/**
 * @brief 获取输入线颜色
 * @return 输入线颜色
 */
QColor QtMaterialTextField::inputLineColor() const
{
    Q_D(const QtMaterialTextField);

    if (d->useThemeColors || !d->inputLineColor.isValid()) {
        return QtMaterialStyle::instance().themeColor("border"); ///< 使用主题颜色
    } else {
        return d->inputLineColor; ///< 使用自定义颜色
    }
}

/**
 * @brief 设置是否显示输入线
 * @param value 是否显示输入线
 */
void QtMaterialTextField::setShowInputLine(bool value)
{
    Q_D(QtMaterialTextField);

    if (d->showInputLine == value) {
        return;
    }

    d->showInputLine = value;
    update(); ///< 刷新界面
}

/**
 * @brief 获取是否显示输入线
 * @return 是否显示输入线
 */
bool QtMaterialTextField::hasInputLine() const
{
    Q_D(const QtMaterialTextField);

    return d->showInputLine;
}

/**
 * @brief 私有构造函数，初始化文本输入框
 * @param d 私有类指针
 * @param parent 父控件指针，默认为 nullptr
 */
QtMaterialTextField::QtMaterialTextField(QtMaterialTextFieldPrivate &d, QWidget *parent)
    : QLineEdit(parent),
      d_ptr(&d)
{
    d_func()->init(); ///< 初始化私有类
}

/**
 * @brief 处理事件
 * @param event 事件对象
 * @return 是否处理事件
 * @note 重写基类方法
 */
bool QtMaterialTextField::event(QEvent *event)
{
    Q_D(QtMaterialTextField);

    switch (event->type())
    {
    case QEvent::Resize:
    case QEvent::Move: {
        if (d->label) {
            d->label->setGeometry(rect()); ///< 同步标签几何
        }
    }
    default:
        break;
    }
    return QLineEdit::event(event);
}

/**
 * @brief 绘制事件
 * @param event 绘制事件对象
 * @note 重写基类方法
 */
void QtMaterialTextField::paintEvent(QPaintEvent *event)
{
    Q_D(QtMaterialTextField);

    QLineEdit::paintEvent(event);

    QPainter painter(this);

    const qreal progress = d->stateMachine->progress(); ///< 获取动画进度

    if (text().isEmpty() && progress < 1)
    {
        painter.setOpacity(1 - progress); ///< 淡出背景
        painter.fillRect(rect(), parentWidget()->palette().color(backgroundRole())); ///< 填充背景
    }

    const int y = height() - 1; ///< 输入线位置
    const int wd = width() - 5; ///< 输入线宽度

    if (d->showInputLine)
    {
        QPen pen;
        pen.setWidth(1);
        pen.setColor(inputLineColor()); ///< 设置输入线颜色

        if (!isEnabled()) 
            pen.setStyle(Qt::DashLine); ///< 禁用时为虚线

        painter.setPen(pen);
        painter.setOpacity(1);
        painter.drawLine(QLineF(2.5, y, wd, y)); ///< 绘制输入线

        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(inkColor()); ///< 设置墨水颜色

        if (progress > 0)
        {
            painter.setPen(Qt::NoPen);
            painter.setBrush(brush);
            const int w = (1 - progress) * static_cast<qreal>(wd / 2); ///< 计算墨水宽度
            painter.drawRect(w + 2.5, height() - 2, wd - w * 2, 2); ///< 绘制墨水效果
        }
    }
}