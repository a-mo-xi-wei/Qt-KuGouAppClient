/**
 * @file IconBesideTextToolButton.cpp
 * @brief 实现 IconBesideTextToolButton 类，提供图标与文本并排的工具按钮功能
 * @author [WeiWang]
 * @date 2025-05-15
 * @version 1.0
 */

#include "IconBesideTextToolButton.h"

#include <QPainter>

/**
 * @brief 构造函数，初始化图标与文本并排的工具按钮
 * @param parent 父控件指针，默认为 nullptr
 */
IconBesideTextToolButton::IconBesideTextToolButton(QWidget *parent)
    : QToolButton(parent)
    , m_enterIconSize(QSize(25, 25))
    , m_leaveIconSize(QSize(20, 20))
    , m_enterFontSize(15)
    , m_leaveFontSize(12)
    , m_hoverFontColor(QColor(255, 255, 255))
{
}

/**
 * @brief 设置鼠标进入时的图标大小
 * @param size 图标大小
 */
void IconBesideTextToolButton::setEnterIconSize(const QSize &size)
{
    m_enterIconSize = size;
}

/**
 * @brief 设置鼠标离开时的图标大小
 * @param size 图标大小
 */
void IconBesideTextToolButton::setLeaveIconSize(const QSize &size)
{
    m_leaveIconSize = size;
    setIconSize(size);
}

/**
 * @brief 设置鼠标进入时的字体大小
 * @param size 字体大小
 */
void IconBesideTextToolButton::setEnterFontSize(const int &size)
{
    m_enterFontSize = size;
}

/**
 * @brief 设置鼠标离开时的字体大小
 * @param size 字体大小
 */
void IconBesideTextToolButton::setLeaveFontSize(const int &size)
{
    m_leaveFontSize = size;
}

/**
 * @brief 设置是否在悬停时改变图标或文字大小
 * @param change 是否改变大小
 */
void IconBesideTextToolButton::setChangeSize(const bool &change)
{
    m_changeSize = change;
}

/**
 * @brief 设置悬停时的字体颜色
 * @param color 字体颜色
 */
void IconBesideTextToolButton::setHoverFontColor(const QColor &color)
{
    m_hoverFontColor = color;
}

/**
 * @brief 设置图标和文字的水平边距
 * @param padding 边距值
 */
void IconBesideTextToolButton::setPadding(const int &padding)
{
    m_padding = padding;
}

/**
 * @brief 设置图标和文字是否靠近显示
 * @param approach 是否靠近
 */
void IconBesideTextToolButton::setApproach(const bool &approach)
{
    m_isApproach = approach;
}

/**
 * @brief 设置鼠标进入时的图标
 * @param icon 图标
 */
void IconBesideTextToolButton::setEnterIcon(const QIcon &icon)
{
    m_enterIcon = icon;
}

/**
 * @brief 设置鼠标离开时的图标
 * @param icon 图标
 */
void IconBesideTextToolButton::setLeaveIcon(const QIcon &icon)
{
    m_leaveIcon = icon;
}

/**
 * @brief 绘制事件，自定义按钮的图标和文字绘制
 * @param event 绘图事件对象
 */
void IconBesideTextToolButton::paintEvent(QPaintEvent *event)
{
    // 临时存储图标和文本
    const QIcon originalIcon = icon();
    const QString originalText = text();
    setIcon(QIcon()); // 清除图标
    setText(QString()); // 清除文本

    // 绘制按钮的背景和边框
    QToolButton::paintEvent(event);

    // 恢复图标和文本
    setIcon(originalIcon);
    setText(originalText);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 设置悬停时的字体颜色
    if (m_isHover)
    {
        painter.setPen(m_hoverFontColor);
    }

    // 获取按钮的矩形区域
    QRect buttonRect = rect();

    // 计算文字和图标的大小
    const QFontMetrics fm = painter.fontMetrics();
    const int textWidth = fm.horizontalAdvance(originalText);
    const int textHeight = fm.height();
    const QSize iconSize = this->iconSize();
    const int iconWidth = iconSize.width();
    const int iconHeight = iconSize.height();
    const int left = buttonRect.left();
    const int right = buttonRect.right();
    const int top = buttonRect.top();
    const int height = buttonRect.height();
    const int width = buttonRect.width();
    QRect textRect;
    QRect iconRect;

    // 根据靠近模式计算文字和图标位置
    if (m_isApproach)
    {
        textRect = QRect(left + (width - textWidth - iconWidth + 7) / 2,
                         top + (height - textHeight) / 2,
                         textWidth, textHeight);
        iconRect = QRect(right - (width - textWidth - iconWidth + 7) / 2,
                         top + (height - iconHeight) / 2,
                         iconWidth, iconHeight);
    }
    else
    {
        textRect = QRect(left + m_padding, top + (height - textHeight) / 2,
                         textWidth, textHeight);
        iconRect = QRect(right - iconWidth - m_padding,
                         top + (height - iconHeight) / 2,
                         iconWidth, iconHeight);
    }

    // 绘制文字和图标
    painter.drawText(textRect, Qt::AlignLeft, originalText);
    originalIcon.paint(&painter, iconRect);
}

/**
 * @brief 鼠标进入事件，处理悬停效果
 * @param event 进入事件对象
 */
void IconBesideTextToolButton::enterEvent(QEnterEvent *event)
{
    m_isHover = true;
    if (!m_changeSize)
    {
        if (!m_enterIcon.isNull())
        {
            setIcon(m_enterIcon);
        }
        QToolButton::enterEvent(event);
        return;
    }

    if (!m_enterIcon.isNull())
    {
        setIcon(m_enterIcon);
    }
    QFont font = this->font();
    font.setPointSize(m_enterFontSize);
    setFont(font);
    setIconSize(m_enterIconSize);
    QToolButton::enterEvent(event);
}

/**
 * @brief 鼠标离开事件，恢复默认状态
 * @param event 事件对象
 */
void IconBesideTextToolButton::leaveEvent(QEvent *event)
{
    m_isHover = false;
    if (!m_changeSize)
    {
        if (!m_leaveIcon.isNull())
        {
            setIcon(m_leaveIcon);
        }
        return QToolButton::leaveEvent(event);
    }
    if(!this->m_leaveIcon.isNull())
        this->setIcon(this->m_leaveIcon);
    QFont font = this->font();
    font.setPointSize(this->m_leaveFontSize); // 恢复默认字体大小
    this->setFont(font);
    this->setIconSize(this->m_leaveIconSize);
    QToolButton::leaveEvent(event);
}
