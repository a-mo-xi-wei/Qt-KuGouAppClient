#include "IconBesideTextToolButton.h"

#include <QPainter>
#include <QStyleOptionToolButton>

IconBesideTextToolButton::IconBesideTextToolButton(QWidget *parent)
    :QToolButton(parent)
    ,m_enterIconSize(QSize(25,25))
    ,m_leaveIconSize(QSize(20,20))
    ,m_enterFontSize(15)
    ,m_leaveFontSize(12)
{
    
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

void IconBesideTextToolButton::paintEvent(QPaintEvent *event) {

    // 临时存储图标，然后将按钮的图标清除
    const QIcon originalIcon = this->icon();
    const QString originalText = this->text();
    this->setIcon(QIcon());  // 清除图标
    this->setText(QString());  // 清除文字

    // 调用父类的 paintEvent，绘制按钮的背景和边框
    QToolButton::paintEvent(event);

    // 恢复按钮的图标
    this->setIcon(originalIcon);
    // 恢复按钮的文字
    this->setText(originalText);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 获取控件的矩形区域
    QRect buttonRect = rect();

    // 计算文字和图标的大小
    QFontMetrics fm = painter.fontMetrics();
    int textWidth = fm.horizontalAdvance(originalText);
    int textHeight = fm.height();
    QSize iconSize = this->iconSize();
    QRect textRect(buttonRect.left() + 15, buttonRect.top() + (buttonRect.height() - textHeight) / 2, textWidth, textHeight);
    QRect iconRect(buttonRect.right()-iconSize.width()-15, buttonRect.top() + (buttonRect.height() - iconSize.height()) / 2, iconSize.width(), iconSize.height());
    // 绘制文字和图标
    painter.drawText(textRect, Qt::AlignLeft, originalText);
    originalIcon.paint(&painter, iconRect);
}

void IconBesideTextToolButton::enterEvent(QEnterEvent *event) {
    QFont font = this->font();
    font.setPointSize(this->m_enterFontSize);
    this->setFont(font);
    this->setIconSize(this->m_enterIconSize);
    QToolButton::enterEvent(event);
}

void IconBesideTextToolButton::leaveEvent(QEvent *event) {
    QFont font = this->font();
    font.setPointSize(this->m_leaveFontSize);  // 恢复默认字体大小
    this->setFont(font);
    this->setIconSize(this->m_leaveIconSize);

    QToolButton::leaveEvent(event);
}