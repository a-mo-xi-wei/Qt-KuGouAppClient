#include "MainWindow.h"
#include "WaterDrop.h"
#include "MyTrayIcon.h"
#include "logger.hpp"
#include "ElaToolTip.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

constexpr int SHADOW_WIDTH = 5;
constexpr int RADIUS = 12;

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_trayIcon(new MyTrayIcon(this))
    , m_aboutDialog(std::make_unique<AboutDialog>(this))
{
    //接收来自 trayIcon 的信号
    connect(m_trayIcon, &MyTrayIcon::noVolume, this, [this](const bool& flag) {
        //qDebug()<<"MainWindow 托盘图标点击: "<<(flag?"静音":"开启声音");
        emit fromTray_noVolume(flag);
    });
    connect(m_trayIcon, &MyTrayIcon::showAboutDialog, this, &MainWindow::onShowAboutDialog);
    this->m_aboutDialog->hide();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    //QBrush brush(QColor(QStringLiteral("#eef2ff")));
    //painter.setBrush(brush);
    //painter.setPen(Qt::NoPen);
    //QPainterPath path;
    //path.addRoundedRect(this->rect(), RADIUS, RADIUS);
    //painter.drawPath(path);
    //------------绘制阴影
    QPainterPath path1;
    path1.setFillRule(Qt::WindingFill);
    path1.addRoundedRect(SHADOW_WIDTH,SHADOW_WIDTH, this->width() - SHADOW_WIDTH * 2, this->height() - SHADOW_WIDTH * 2,RADIUS,RADIUS);
    QColor color(150, 150, 150, 55);
    for (int i = 0; i != SHADOW_WIDTH; ++i)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRoundedRect(SHADOW_WIDTH - i, SHADOW_WIDTH- i, this->width() - (SHADOW_WIDTH- i) * 2,
                            this->height() - (SHADOW_WIDTH- i) * 2, RADIUS, RADIUS);
        color.setAlpha(240 - static_cast<int>(qSqrt(i) * 80));
        painter.setPen(color);
        painter.drawPath(path);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent *ev) {
    QWidget::mousePressEvent(ev);
    auto w = new WaterDrop;
    w->move(this->mapToGlobal(ev->pos()));
    w->show();
    if (ev->button() == Qt::LeftButton) {
        //qDebug()<<"按下 :"<<ev->pos();
        // 如果是鼠标左键
        // 获取当前窗口位置,以窗口左上角
        windowsLastPs = pos();
        // 获取鼠标在屏幕的位置  就是全局的坐标 以屏幕左上角为坐标系
        mousePs = ev->globalPosition().toPoint();
        isPress = true;
        // 获取鼠标在那个区域
        mouse_press_region = getMouseRegion(ev->pos().x(), ev->pos().y());
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    QWidget::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton) {
        isPress = false;
    }
    setCursor(QCursor{});
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    QWidget::mouseMoveEvent(event);
    // 设置鼠标的形状
    setMouseCursor(event->pos().x(), event->pos().y());
// 计算的鼠标移动偏移量, 就是鼠标全局坐标 - 减去点击时鼠标坐标
    this->point_offset = event->globalPosition().toPoint() - mousePs;

    if (isPress) {
        if (mouse_press_region != kMousePositionMid) {
            // 拉伸窗口
            // 获取客户区
            QRect rect = this->geometry();
            switch (mouse_press_region) {
                // 左上角
                case kMousePositionLeftTop:
                    rect.setTopLeft(rect.topLeft() + point_offset);
                break;
                case kMousePositionTop:
                    rect.setTop(rect.top() + point_offset.y());
                break;
                case kMousePositionRightTop:
                    rect.setTopRight(rect.topRight() + point_offset);
                break;
                case kMousePositionRight:
                    rect.setRight(rect.right() + point_offset.x());
                break;
                case kMousePositionRightBottom:
                    rect.setBottomRight(rect.bottomRight() + point_offset);
                break;
                case kMousePositionBottom:
                    rect.setBottom(rect.bottom() + point_offset.y());
                break;
                case kMousePositionLeftBottom:
                    rect.setBottomLeft(rect.bottomLeft() + point_offset);
                break;
                case kMousePositionLeft:
                    rect.setLeft(rect.left() + point_offset.x());
                break;
                default:
                    break;
            }
            setGeometry(rect);
            mousePs = event->globalPosition().toPoint();
        }
    }
}

void MainWindow::setElaToolTip(QWidget *widget, const QString &tooltip) {
    auto toolTip = new ElaToolTip(widget);
    toolTip->setToolTip(tooltip);
}

void MainWindow::setMouseCursor(const int& x, const int& y) {
    // 鼠标形状对象
    Qt::CursorShape cursor{Qt::ArrowCursor};
    int region = getMouseRegion(x, y);
    switch (region) {
        case kMousePositionLeftTop:
        case kMousePositionRightBottom:
            cursor = Qt::SizeFDiagCursor;
        break;
        case kMousePositionRightTop:
        case kMousePositionLeftBottom:
            cursor = Qt::SizeBDiagCursor;
        break;
        case kMousePositionLeft:
        case kMousePositionRight:
            cursor = Qt::SizeHorCursor;
        break;
        case kMousePositionTop:
        case kMousePositionBottom:
            cursor = Qt::SizeVerCursor;
        break;
        default:
            cursor = Qt::ArrowCursor;
        break;
    }
    setCursor(cursor);
}

int MainWindow::getMouseRegion(const int& x, const int& y) const {
    int region_x = 0, region_y = 0;
    // 鼠标的X坐标小于 边界5 说明他在最上层区域 第一区域
    if (x < Area::kMouseRegionLeft) {
        region_x = 1;
    } else if (x > (this->width()/*窗体宽度*/ - Area::kMouseRegionRight/*边界宽度5*/)) {
        // 如果鼠标X坐标 大于 最右侧的边界 说明他在第三区域
        region_x = 3;
    } else {
        region_x = 2;
    }
    if (y < Area::kMouseRegionTop) {
        // 同理 鼠标Y坐标 小于上层边界  说明鼠标在第一区域
        region_y = 1;
    } else if (y > (this->height() - Area::kMouseRegionBottom)) {
        // 鼠标Y坐标的 大于 最下面的坐标,鼠标就在 第三区
        region_y = 3;
    } else {
        region_y = 2;
    }
    // 最后计算 表示区域的 数值 (x=1, y=1) 计算 = 1*10+1 =11
    // x=2,y=3 = 3*10+2 = 32 在图的 3,2 区域
    return region_y * 10 + region_x;
}

void MainWindow::onShowAboutDialog(const bool flag) const {
    STREAM_INFO()<<"系统托盘图标点击: "<<(flag?"显示关于对话框":"隐藏关于对话框");
    if (flag)this->m_aboutDialog->onShowDialog();
    else this->m_aboutDialog->onHideDialog();
}
