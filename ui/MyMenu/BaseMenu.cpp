//
// Created by WeiWang on 25-1-12.
//

#include "BaseMenu.h"
#include "logger.hpp"

#include <QFile>
#include <QPainter>
#include <QPainterPath>
#include <QCoreApplication>
#include <QWidgetAction>
#include <QGraphicsOpacityEffect>
#include <QTimeLine>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

BaseMenu::BaseMenu(QWidget *parent)
    :QMenu(parent)
{
    //背景透明
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAttribute(Qt::WA_Hover, true);
    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    this->setMouseTracking(true);
    {
        //设置样式
        QFile file(GET_CURRENT_DIR + QStringLiteral("/menu.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            STREAM_INFO() << "样式表打开失败QAQ";
            return;
        }
    }
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    m_opacityEffect->setOpacity(0.0);
    this->setGraphicsEffect(m_opacityEffect);

    m_timeLine = new QTimeLine(300, this);
    connect(m_timeLine, &QTimeLine::valueChanged, this, [=](const qreal &value) {
        QLinearGradient gradient(0, 0, 0, height());
        gradient.setColorAt(0, QColor(255, 255, 255, 255));
        gradient.setColorAt(value, QColor(255, 255, 255, 255));
        gradient.setColorAt(1, QColor(255, 255, 255, 0));
        m_opacityEffect->setOpacityMask(gradient);
        m_opacityEffect->setOpacity(value);
    });
    connect(m_timeLine, &QTimeLine::finished, this, [=]() {
        m_opacityEffect->setOpacity(1); // 确保完全显示
        m_opacityEffect->setOpacityMask(QBrush()); // 清除残留遮罩
    });
}

const BaseMenu *BaseMenu::getMenu() const {
    return this;
}

void BaseMenu::setCurIndex(const int &idx) {
    this->m_curIndex = idx;
}

void BaseMenu::setAniDuration(const int &duration) {
    m_timeLine->setDuration(duration);
}

QString BaseMenu::getStyleSheet() const {
    return this->styleSheet();
}

void BaseMenu::checkHover() {
    if (!this->m_lastHover.isEmpty()) {
        QEvent leaveEvent(QEvent::Leave); // 创建进入事件
        for (const auto val: m_lastHover) {
            QCoreApplication::sendEvent(val, &leaveEvent); // 发送事件
        }
        m_lastHover.clear();
        m_currentHover.clear();
    }
}

void BaseMenu::checkSelection() const {
    if (m_lastSelect) {
        //qDebug()<<"之前指向目标存在";
        if (m_lastSelect == m_curSelect)return;
        if (!m_lastSelect->icon().isNull()) {
            m_lastSelect->setIcon(QIcon());
            //STREAM_WARN()<<"图标设置为空";
        }
    }
}

void BaseMenu::paintEvent(QPaintEvent *event) {
    QMenu::paintEvent(event);
    //绘制阴影
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(m_shadowWidth, m_shadowWidth, this->width() - m_shadowWidth * 2, this->height() - m_shadowWidth * 2,
                        m_shadowRadius, m_shadowRadius);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    for (int i = 0; i != m_shadowWidth; ++i) {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRoundedRect(m_shadowWidth - i, m_shadowWidth - i, this->width() - (m_shadowWidth - i) * 2,
                            this->height() - (m_shadowWidth - i) * 2, m_shadowRadius, m_shadowRadius);
        m_shadowColor.setAlpha(180 - static_cast<int>(qSqrt(i) * 80));
        painter.setPen(m_shadowColor);
        painter.drawPath(path);
    }
}

void BaseMenu::showEvent(QShowEvent *event) {
    QMenu::showEvent(event);
    // 每次展示都重新开始动画
    m_opacityEffect->setOpacity(0.0);
    m_timeLine->stop();
    m_timeLine->setCurrentTime(0); // 重置进度
    m_timeLine->start();
}

void BaseMenu::leaveEvent(QEvent *event) {
    QMenu::leaveEvent(event);
    checkHover();
}

void BaseMenu::connectAction(const QWidgetAction *widgetAction, MenuBtn *btn) {
    connect(widgetAction, &QWidgetAction::hovered, this, [btn,this] {
           checkHover();
           this->m_currentHover.emplace_back(btn);
           this->m_lastHover = this->m_currentHover;
           QEvent enterEvent(QEvent::Enter); // 创建进入事件
           QCoreApplication::sendEvent(btn, &enterEvent); // 发送事件
           // 模拟按钮进入 hover 状态
           btn->setAttribute(Qt::WA_UnderMouse, true);
    });
}


