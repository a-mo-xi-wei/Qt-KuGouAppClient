/**
 * @file MySearchLineEdit.cpp
 * @brief 实现 MySearchLineEdit 类，提供带动画效果的搜索输入框
 * @author WeiWang
 * @date 2024-10-10
 * @version 1.0
 */
#include "MySearchLineEdit.h"

#include <QClipboard>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <mutex>

/**
 * @brief 单次初始化标志，确保原始宽度只设置一次
 */
std::once_flag flag;

/**
 * @brief 构造函数，初始化搜索输入框
 * @param parent 父控件指针，默认为 nullptr
 */
MySearchLineEdit::MySearchLineEdit(QWidget *parent)
    : ElaLineEdit(parent)
    , m_maxWidth(250)
{
}

/**
 * @brief 设置输入框最大宽度
 * @param width 最大宽度
 */
void MySearchLineEdit::setMaxWidth(const int &width) {
    this->m_maxWidth = width;
}

/**
 * @brief 重写焦点获取事件，触发展开动画
 * @param event 焦点事件
 */
void MySearchLineEdit::focusInEvent(QFocusEvent* event) {
    // 先调用基类处理焦点事件
    ElaLineEdit::focusInEvent(event);

    if (width() == m_maxWidth) {
        m_expanded = true;
        return;
    }

    // 设置初始值
    std::call_once(flag, [this] { m_originalWidth = width(); });

    startExpandAnimation();
}

/**
 * @brief 重写焦点丢失事件，触发收起动画
 * @param event 焦点事件
 */
void MySearchLineEdit::focusOutEvent(QFocusEvent* event) {
    // 只在已展开状态下执行收缩动画
    if (m_expanded) {
        startCollapseAnimation();
    }

    // 调用基类处理焦点离开
    ElaLineEdit::focusOutEvent(event);
}

/**
 * @brief 重写按键事件，限制密码模式下的复制粘贴等操作
 * @param event 按键事件
 */
void MySearchLineEdit::keyPressEvent(QKeyEvent *event) {
    if (echoMode() == QLineEdit::Password) {
        if (event->matches(QKeySequence::SelectAll)) {
            return; ///< 密码模式禁用全选
        }
        if (event->matches(QKeySequence::Copy)) {
            return; ///< 密码模式禁用复制
        }
        if (event->matches(QKeySequence::Paste)) {
            return; ///< 密码模式禁用粘贴
        }
    }
    QLineEdit::keyPressEvent(event);
}

void MySearchLineEdit::resizeEvent(QResizeEvent* event) {
    ElaLineEdit::resizeEvent(event);

    // 只有在展开状态且宽度变化时才更新标记动画
    if (m_expanded && !m_animatingWidth) {
        updateMarkAnimationTarget();
    }
}

void MySearchLineEdit::startExpandAnimation() {
    // 确保动画对象存在
    if (!m_widthAnimation) {
        m_widthAnimation = new QPropertyAnimation(this, "minimumWidth", this);
        m_widthAnimation->setEasingCurve(QEasingCurve::InOutSine);
        m_widthAnimation->setDuration(200);

        connect(m_widthAnimation, &QPropertyAnimation::valueChanged, [this] {
            m_animatingWidth = true;
            updateMarkAnimationTarget();
        });

        connect(m_widthAnimation, &QPropertyAnimation::finished, [this] {
            m_animatingWidth = false;
            m_expanded = true;
        });
    }

    if (!m_markAnimation) {
        m_markAnimation = new QPropertyAnimation(this, "expandMarkWidth", this);
        m_markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        m_markAnimation->setDuration(300);
    }

    // 开始宽度展开动画
    m_widthAnimation->stop();
    m_widthAnimation->setStartValue(width());
    m_widthAnimation->setEndValue(m_maxWidth);
    m_widthAnimation->start();

    // 开始标记展开动画
    m_markAnimation->stop();
    m_markAnimation->setStartValue(expandMarkWidth());
    m_markAnimation->setEndValue(m_maxWidth / 2 - getBorderRadius() / 2);
    m_markAnimation->start();
}

void MySearchLineEdit::startCollapseAnimation() {
    // 确保动画对象存在
    if (!m_widthAnimation) {
        m_widthAnimation = new QPropertyAnimation(this, "minimumWidth", this);
        m_widthAnimation->setEasingCurve(QEasingCurve::InOutSine);
        m_widthAnimation->setDuration(200);

        connect(m_widthAnimation, &QPropertyAnimation::valueChanged, [this] {
            m_animatingWidth = true;
            updateMarkAnimationTarget();
        });

        connect(m_widthAnimation, &QPropertyAnimation::finished, [this] {
            m_animatingWidth = false;
            m_expanded = false;
        });
    }

    if (!m_markAnimation) {
        m_markAnimation = new QPropertyAnimation(this, "expandMarkWidth", this);
        m_markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        m_markAnimation->setDuration(300);
    }

    // 开始宽度收缩动画
    m_widthAnimation->stop();
    m_widthAnimation->setStartValue(width());
    m_widthAnimation->setEndValue(m_originalWidth);
    m_widthAnimation->start();

    // 开始标记收缩动画（跟随基类行为收缩到0）
    m_markAnimation->stop();
    m_markAnimation->setStartValue(expandMarkWidth());
    m_markAnimation->setEndValue(0);
    m_markAnimation->start();
}

void MySearchLineEdit::updateMarkAnimationTarget() {
    if (!m_markAnimation || m_markAnimation->state() != QAbstractAnimation::Running)
        return;

    // 计算当前应达到的标记宽度
    const int targetValue = width() / 2 - getBorderRadius() / 2;

    // 获取动画当前状态
    const qreal currentValue = m_markAnimation->currentValue().toInt();
    const qreal progress = m_markAnimation->currentTime() / static_cast<qreal>(m_markAnimation->duration());

    // 平滑更新目标值
    m_markAnimation->setEndValue(targetValue);

    // 避免动画初期跳跃
    if (progress > 0.1) {
        const qreal newValue = currentValue + (targetValue - currentValue) * 0.3;
        setExpandMarkWidth(static_cast<int>(newValue));
    }
}
