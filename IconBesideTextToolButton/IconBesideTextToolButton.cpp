#include "IconBesideTextToolButton.h"

#include <QPainter>

IconBesideTextToolButton::IconBesideTextToolButton(QWidget *parent)
    : QToolButton(parent)
      , m_enterIconSize(QSize(25, 25))
      , m_leaveIconSize(QSize(20, 20))
      , m_enterFontSize(15)
      , m_leaveFontSize(12) {
}

void IconBesideTextToolButton::setEnterIconSize(const QSize &size) {
    this->m_enterIconSize = size;
}

void IconBesideTextToolButton::setLeaveIconSize(const QSize &size) {
    this->m_leaveIconSize = size;
    this->setIconSize(size);
}

void IconBesideTextToolButton::setEnterFontSize(const int &size) {
    this->m_enterFontSize = size;
}

void IconBesideTextToolButton::setLeaveFontSize(const int &size) {
    this->m_leaveFontSize = size;
}

void IconBesideTextToolButton::setChangeSize(const bool &change) {
    this->m_changeSize = change;
}

void IconBesideTextToolButton::setHoverFontColor(const QColor &color) {
    this->m_hoverFontColor = color;
}

void IconBesideTextToolButton::setPadding(const int &padding) {
    this->m_padding = padding;
}

void IconBesideTextToolButton::setApproach(const bool &approach) {
    this->m_isApproach = approach;
}

void IconBesideTextToolButton::setEnterIcon(const QIcon &icon) {
    this->m_enterIcon = icon;
}

void IconBesideTextToolButton::setLeaveIcon(const QIcon &icon) {
    this->m_leaveIcon = icon;
}

void IconBesideTextToolButton::paintEvent(QPaintEvent *event) {
    // 临时存储图标，然后将按钮的图标清除
    const QIcon originalIcon = this->icon();
    const QString originalText = this->text();
    this->setIcon(QIcon()); // 清除图标
    this->setText(QString()); // 清除文字

    // 调用父类的 paintEvent，绘制按钮的背景和边框
    QToolButton::paintEvent(event);

    // 恢复按钮的图标
    this->setIcon(originalIcon);
    // 恢复按钮的文字
    this->setText(originalText);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if (this->m_isHover)painter.setPen(this->m_hoverFontColor);
    // 获取控件的矩形区域
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
    if (this->m_isApproach) {
        textRect = QRect(left + (width-textWidth-iconWidth+5)/2,
                        top + (height - textHeight) / 2,
                        textWidth, textHeight);
        iconRect = QRect(right - (width-textWidth-iconWidth+5)/2,
                        top + (height - iconHeight) / 2,
                       iconWidth,iconHeight);
    } else {
        textRect = QRect(left + this->m_padding, top + (height - textHeight) / 2,
                       textWidth, textHeight);
        iconRect = QRect(right - iconWidth - this->m_padding,
                       top + (height - iconHeight) / 2,
                       iconWidth,iconHeight);
    }
     // 绘制文字和图标
    painter.drawText(textRect, Qt::AlignLeft, originalText);
    originalIcon.paint(&painter, iconRect);
}

void IconBesideTextToolButton::enterEvent(QEnterEvent *event) {
    this->m_isHover = true;
    if (!this->m_changeSize) {
        if(!this->m_enterIcon.isNull())
            this->setIcon(this->m_enterIcon);
        return QToolButton::enterEvent(event);
    }
    if(!this->m_enterIcon.isNull())
        this->setIcon(this->m_enterIcon);
    QFont font = this->font();
    font.setPointSize(this->m_enterFontSize);
    this->setFont(font);
    this->setIconSize(this->m_enterIconSize);
    QToolButton::enterEvent(event);
}

void IconBesideTextToolButton::leaveEvent(QEvent *event) {
    this->m_isHover = false;
    if (!this->m_changeSize) {
        if(!this->m_leaveIcon.isNull())
            this->setIcon(this->m_leaveIcon);
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
