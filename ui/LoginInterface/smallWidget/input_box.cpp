#include "input_box.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

Input_box::Input_box(const QString& icon, QWidget *parent) : ElaLineEdit{parent} {
    this->icon = QPixmap(icon);
    this->resize(388, 58);
    setTextMargins(10, 0, 25, 0);

    QFont font = this->font();
    font.setPointSize(15);
    setFont(font);

    QPalette palette = this->palette();

    palette.setColor(QPalette::Text, Qt::black);
    this->setPalette(palette);

    palette.setColor(QPalette::Base, Qt::transparent);
    this->setPalette(palette);
    this->setStyleSheet("QLineEdit { selection-background-color: #1D8FC4; }");
}

void Input_box::setIcon(const QString &iconPath) {
    icon = QPixmap(iconPath);
    update(); // 触发重绘
}

void Input_box::openToolTip() {
    m_passwordTip = new ElaToolTip(this);
    m_passwordTip->hide();
}

void Input_box::setIconToolTip(const QString &tip) const {
    this->m_passwordTip->setToolTip(tip);
}

void Input_box::paintEvent(QPaintEvent *event) {
    ElaLineEdit::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    painter.drawPixmap(iconRect(), icon);
}

void Input_box::mousePressEvent(QMouseEvent *event) {
    if (iconRect().contains(event->pos())) {
        emit iconClicked(); // 触发图标点击信号
        event->accept();
    } else {
        ElaLineEdit::mousePressEvent(event); // 其他区域保持默认行为
    }
}

void Input_box::mouseMoveEvent(QMouseEvent *event) {
    ElaLineEdit::mouseMoveEvent(event);
    if (iconRect().contains(event->pos())) {
        setCursor(Qt::PointingHandCursor);
        if (m_passwordTip)m_passwordTip->show();
    } else {
        setCursor(Qt::IBeamCursor);
        if (m_passwordTip)m_passwordTip->hide();
    }
}

QRect Input_box::iconRect() const {
    return {static_cast<int>(width() * 0.85), height() / 4, height() / 2, height() / 2};
}
