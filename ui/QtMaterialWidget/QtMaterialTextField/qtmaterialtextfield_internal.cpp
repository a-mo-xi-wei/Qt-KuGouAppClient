/**
 * @file qtmaterialtextfield_internal.cpp
 * @brief 实现 QtMaterialTextFieldStateMachine 和 QtMaterialTextFieldLabel 类，提供文本输入框内部功能
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#include "qtmaterialtextfield_internal.h"
#include "qtmaterialtextfield.h"
#include <QPropertyAnimation>
#include <QEventTransition>
#include <QPainter>

/**
 * @brief 构造函数，初始化状态机
 * @param parent 指向 QtMaterialTextField 的指针
 * @note 内部实现
 */
QtMaterialTextFieldStateMachine::QtMaterialTextFieldStateMachine(QtMaterialTextField *parent)
    : QStateMachine(parent),
      m_textField(parent),
      m_normalState(new QState),
      m_focusedState(new QState),
      m_label(nullptr),
      m_offsetAnimation(nullptr),
      m_colorAnimation(nullptr),
      m_progress(0.0)
{
    Q_ASSERT(parent);

    addState(m_normalState);  ///< 添加正常状态
    addState(m_focusedState); ///< 添加聚焦状态

    setInitialState(m_normalState); ///< 设置初始状态

    QEventTransition *transition;
    QPropertyAnimation *animation;

    // 聚焦转换
    transition = new QEventTransition(parent, QEvent::FocusIn);
    transition->setTargetState(m_focusedState);
    m_normalState->addTransition(transition);

    animation = new QPropertyAnimation(this, "progress", this);
    animation->setEasingCurve(QEasingCurve::InCubic); ///< 聚焦动画曲线
    animation->setDuration(310);                      ///< 动画时长
    transition->addAnimation(animation);

    // 失焦转换
    transition = new QEventTransition(parent, QEvent::FocusOut);
    transition->setTargetState(m_normalState);
    m_focusedState->addTransition(transition);

    animation = new QPropertyAnimation(this, "progress", this);
    animation->setEasingCurve(QEasingCurve::OutCubic); ///< 失焦动画曲线
    animation->setDuration(310);                       ///< 动画时长
    transition->addAnimation(animation);

    m_normalState->assignProperty(this, "progress", 0);  ///< 正常状态进度
    m_focusedState->assignProperty(this, "progress", 1); ///< 聚焦状态进度

    setupProperties(); ///< 初始化属性

    connect(m_textField, SIGNAL(textChanged(QString)), this, SLOT(setupProperties())); ///< 文本变化时更新
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialTextFieldStateMachine::~QtMaterialTextFieldStateMachine()
{
}

/**
 * @brief 设置标签控件
 * @param label 标签控件指针
 */
void QtMaterialTextFieldStateMachine::setLabel(QtMaterialTextFieldLabel *label)
{
    if (m_label) {
        delete m_label; ///< 删除旧标签
    }

    if (m_offsetAnimation) {
        removeDefaultAnimation(m_offsetAnimation);
        delete m_offsetAnimation; ///< 删除旧偏移动画
    }

    if (m_colorAnimation) {
        removeDefaultAnimation(m_colorAnimation);
        delete m_colorAnimation; ///< 删除旧颜色动画
    }

    m_label = label;

    if (m_label)
    {
        m_offsetAnimation = new QPropertyAnimation(m_label, "offset", this);
        m_offsetAnimation->setDuration(210);                     ///< 动画时长
        m_offsetAnimation->setEasingCurve(QEasingCurve::OutCubic); ///< 动画曲线
        addDefaultAnimation(m_offsetAnimation);

        m_colorAnimation = new QPropertyAnimation(m_label, "color", this);
        m_colorAnimation->setDuration(210);                      ///< 动画时长
        addDefaultAnimation(m_colorAnimation);
    }

    setupProperties(); ///< 更新属性
}

/**
 * @brief 更新状态机属性
 */
void QtMaterialTextFieldStateMachine::setupProperties()
{
    if (m_label)
    {
        const int m = m_textField->textMargins().top(); ///< 获取顶部边距

        if (m_textField->text().isEmpty()) {
            m_normalState->assignProperty(m_label, "offset", QPointF(0, 26)); ///< 空文本时偏移
        } else {
            m_normalState->assignProperty(m_label, "offset", QPointF(0, -m));  ///< 非空时上移
        }

        m_focusedState->assignProperty(m_label, "offset", QPointF(0, -m));    ///< 聚焦时上移
        m_focusedState->assignProperty(m_label, "color", m_textField->inkColor()); ///< 聚焦时墨水颜色
        m_normalState->assignProperty(m_label, "color", m_textField->labelColor()); ///< 正常时标签颜色

        if (0 != m_label->offset().y() && !m_textField->text().isEmpty()) {
            m_label->setOffset(QPointF(0, -m)); ///< 非空文本时调整偏移
        } else if (!m_textField->hasFocus() && m_label->offset().y() <= 0 && m_textField->text().isEmpty()) {
            m_label->setOffset(QPointF(0, 26)); ///< 空文本且失焦时下移
        }
    }

    m_textField->update(); ///< 刷新界面
}

/**
 * @brief 构造函数，初始化标签控件
 * @param parent 指向 QtMaterialTextField 的指针
 * @note 内部实现
 */
QtMaterialTextFieldLabel::QtMaterialTextFieldLabel(QtMaterialTextField *parent)
    : QWidget(parent),
      m_textField(parent),
      m_scale(1),
      m_posX(0),
      m_posY(26),
      m_color(parent->labelColor())
{
    Q_ASSERT(parent);

    QFont font("Roboto", parent->labelFontSize(), QFont::Medium);
    font.setLetterSpacing(QFont::PercentageSpacing, 102); ///< 设置字间距
    setFont(font);                                       ///< 设置字体
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialTextFieldLabel::~QtMaterialTextFieldLabel()
{
}

/**
 * @brief 绘制事件
 * @param event 绘制事件对象
 * @note 重写基类方法
 */
void QtMaterialTextFieldLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    if (!m_textField->hasLabel()) {
        return; ///< 无标签不绘制
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); ///< 启用抗锯齿
    painter.scale(m_scale, m_scale);              ///< 应用缩放
    painter.setPen(m_color);                     ///< 设置颜色
    painter.setOpacity(1);                       ///< 设置不透明

    QPointF pos(2 + m_posX, height() - 36 + m_posY); ///< 计算绘制位置
    painter.drawText(pos.x(), pos.y(), m_textField->label()); ///< 绘制标签
}