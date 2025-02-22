//
// Created by WeiWang on 24-9-18.
//

#include "TitleWidget.h"
#include "MyMenu.h"

#include <QGuiApplication>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>


TitleWidget::TitleWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setObjectName("TitleWidget");
    this->setStyleSheet("QWidget#TitleWidget{margin:5px;}");
    auto menu = new MyMenu(MyMenu::MenuKind::TitleOption,this);
    m_titleOptMenu = menu->getMenu<TitleOptionMenu>();
}

void TitleWidget::showMenu(const QPoint &pos) {
    setMenuPosition(pos);
    //qDebug()<<"当前Menu位置: "<<pos<<" 显示菜单";
    this->m_titleOptMenu->move(this->m_menuPosition);
    this->m_titleOptMenu->show();
    //qDebug()<<"显示菜单成功";
}

void TitleWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    QWidget::mouseDoubleClickEvent(event);
    if (event->button() == Qt::LeftButton) {
        emit doubleClicked(); // 发送自定义的双击信号
    }
}

void TitleWidget::mousePressEvent(QMouseEvent *event) {
    // 判断是否为右键点击
    if (event->button() == Qt::RightButton) {
        showMenu(this->mapToGlobal(event->pos()));
    }
    else {
        QWidget::mousePressEvent(event);
    }
}

void TitleWidget::paintEvent(QPaintEvent *ev) {
    QWidget::paintEvent(ev);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    // 创建一个线性渐变
    // 先绘制阴影效果
    QRect shadowRect = rect().adjusted(5, 5, -4, 2);
    QLinearGradient gradient(shadowRect.topLeft(), shadowRect.bottomLeft());
    gradient.setColorAt(0, QColor(QStringLiteral("#87CEFA"))); // 起始颜色
    gradient.setColorAt(1, QColor(QStringLiteral("#eef2ff"))); // 结束颜色
    p.setBrush(gradient);
    //p.drawRoundedRect(shadowRect, 8, 8);
    // 创建一个 QPainterPath，只在左上和右上角有圆角
    QPainterPath path;
    int radius = 8; // 圆角半径

    path.moveTo(shadowRect.topLeft() + QPoint(radius, 0)); // 从左上角的圆角开始
    path.lineTo(shadowRect.topRight() - QPoint(radius, 0)); // 右上角前的直线部分
    path.quadTo(shadowRect.topRight(), shadowRect.topRight() + QPoint(0, radius)); // 右上角的圆角
    path.lineTo(shadowRect.bottomRight()); // 右侧的直线部分到底
    path.lineTo(shadowRect.bottomLeft()); // 底部的直线部分到左侧
    path.lineTo(shadowRect.topLeft() + QPoint(0, radius)); // 左侧的直线部分
    path.quadTo(shadowRect.topLeft(), shadowRect.topLeft() + QPoint(radius, 0)); // 左上角的圆角
    path.closeSubpath();

    p.drawPath(path); // 绘制路径
}

void TitleWidget::setMenuPosition(const QPoint &pos) {
    this->m_menuPosition = pos;
    // 获取屏幕的尺寸
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    // 计算菜单右侧的全局位置
    //int menuLeftPos = pos.x() - m_menu->width();
    //qDebug() << "Menu width:" << m_titleOptMenu->width();
    int menuRightPos  = pos.x() + m_titleOptMenu->width();
    int menuBottomPos = pos.y() + m_titleOptMenu->height();
    //int menuTopPos = pos.y() - m_menu->height();
    // 若菜单左侧超出屏幕左侧 (不存在)
    //if(menuLeftPos < 0) {
    //    // 动态调整菜单位置，使其在屏幕内显示
    //    m_menuPosition.setX(10);
    //}
    // 如果菜单右侧超出屏幕右侧
    if (menuRightPos > screenGeometry.right()) {
        // 动态调整菜单位置，使其在屏幕内显示
        int offset = menuRightPos - screenGeometry.right() + 5;
        m_menuPosition.setX(pos.x() - offset);
    }
    // 如果菜单下侧超出屏幕下侧
    if (menuBottomPos > screenGeometry.bottom()) {
        // 动态调整菜单位置，使其在屏幕内显示
        int offset = menuBottomPos - screenGeometry.bottom() + 5;
        m_menuPosition.setY(pos.y() - offset);
    }
    // 如果菜单下侧超出屏幕下侧（不存在）
    //if(menuTopPos < 0) {
    //    // 动态调整菜单位置，使其在屏幕内显示
    //    m_menuPosition.setY(10);
    //}
}
