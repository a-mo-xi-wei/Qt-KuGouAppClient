#include "SpeedDialog.h"

#include <QCoreApplication>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>


SpeedDialog::SpeedDialog(QWidget *parent)
    : QWidget(parent, Qt::Popup)
      , m_effect(std::make_unique<QGraphicsDropShadowEffect>(this)) {
    this->setContentsMargins(0, 0, 0, 10);
    setFixedSize(280, 295);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);  // 关闭后自动delete
    this->setStyleSheet(R"(QWidget{
                                    border:none;
                                    border-radius:8px;
                                })");
    this->m_effect->setColor(QColor(80, 80, 80));
    this->m_effect->setOffset(0, 0);
    this->m_effect->setBlurRadius(30);
    this->setGraphicsEffect(this->m_effect.get());
    qApp->installEventFilter(this); // 安装全局事件过滤器
}

SpeedDialog::~SpeedDialog() {
    qApp->removeEventFilter(this); // 移除事件过滤器
}

bool SpeedDialog::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        // 判断点击位置是否在对话框外
        if (!this->rect().contains(this->mapFromGlobal(mouseEvent->globalPosition()).toPoint())) {
            this->close(); // 触发WA_DeleteOnClose自动销毁
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void SpeedDialog::paintEvent(QPaintEvent *ev) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QColor(Qt::transparent));
    p.setBrush(QColor(QStringLiteral("#edf2ff")));
    p.drawRoundedRect(rect().x(), rect().y(), 280, 287, 8, 8);
    QPainterPath path;
    path.moveTo(rect().x() + 130, rect().bottom() - 8);
    path.lineTo(rect().x() + 140, rect().bottom());
    path.lineTo(rect().x() + 150, rect().bottom() - 8);
    p.drawPath(path);
}
