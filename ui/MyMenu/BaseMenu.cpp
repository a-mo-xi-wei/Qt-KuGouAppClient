//
// Created by WeiWang on 25-1-12.
//

#include "BaseMenu.h"
#include "logger.hpp"

#include <QFile>
#include <QPainter>
#include <QPainterPath>
#include <QCoreApplication>

constexpr int SHADOW_WIDTH = 5;
constexpr int RADIUS = 12;

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

BaseMenu::BaseMenu(QWidget *parent)
    :QMenu(parent)
{
    //背景透明
    this->setAttribute(Qt::WA_TranslucentBackground, true);
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
}

const BaseMenu *BaseMenu::getMenu() const {
    return this;
}

void BaseMenu::setCurIndex(const int &idx) {
    this->m_curIndex = idx;
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
    path.addRoundedRect(SHADOW_WIDTH, SHADOW_WIDTH, this->width() - SHADOW_WIDTH * 2, this->height() - SHADOW_WIDTH * 2,
                        RADIUS, RADIUS);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QColor color(150, 150, 150, 55);
    for (int i = 0; i != SHADOW_WIDTH; ++i) {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRoundedRect(SHADOW_WIDTH - i, SHADOW_WIDTH - i, this->width() - (SHADOW_WIDTH - i) * 2,
                            this->height() - (SHADOW_WIDTH - i) * 2, RADIUS, RADIUS);
        color.setAlpha(180 - static_cast<int>(qSqrt(i) * 80));
        painter.setPen(color);
        painter.drawPath(path);
    }
}

void BaseMenu::showEvent(QShowEvent *event) {
    QMenu::showEvent(event);
    //this->setFocus(); // 强制widget接收焦点
    emit showSelf();
}

void BaseMenu::leaveEvent(QEvent *event) {
    QMenu::leaveEvent(event);
    checkHover();
}


