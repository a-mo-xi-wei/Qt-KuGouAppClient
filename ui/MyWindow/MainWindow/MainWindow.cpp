/**
 * @file MainWindow.cpp
 * @brief 实现 MainWindow 类，提供主窗口功能和交互
 * @author WeiWang
 * @date 2025-03-13
 * @version 1.0
 */

#include "MainWindow.h"
#include "WaterDrop.h"
#include "logger.hpp"
#include "ElaToolTip.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QTimeLine>
#include <QGraphicsOpacityEffect>
#include <QTimer>

/**
 * @brief 阴影宽度常量
 */
constexpr int SHADOW_WIDTH = 5;

/**
 * @brief 圆角半径常量
 */
constexpr int RADIUS = 12;

/**
 * @brief 构造函数，初始化主窗口
 * @param parent 父控件指针，默认为 nullptr
 */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_aboutDialog(std::make_unique<AboutDialog>(this))
{
    this->m_aboutDialog->hide(); ///< 隐藏关于对话框
    connect(m_aboutDialog.get(), &AboutDialog::showDialog, this, [this](const bool &flag) {
        this->m_showDialog = flag;
    });
}

/**
 * @brief 重写绘制事件，绘制窗口阴影
 * @param event 绘制事件
 */
void MainWindow::paintEvent(QPaintEvent *event)
{
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
    //QPainterPath path1;
    //path1.setFillRule(Qt::WindingFill);
    //path1.addRoundedRect(SHADOW_WIDTH,SHADOW_WIDTH, this->width() - SHADOW_WIDTH * 2, this->height() - SHADOW_WIDTH * 2,RADIUS,RADIUS);
    /*
    QColor color(150, 150, 150, 55);
    for (int i = 0; i != SHADOW_WIDTH; ++i) {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRoundedRect(SHADOW_WIDTH - i, SHADOW_WIDTH - i,
                            this->width() - (SHADOW_WIDTH - i) * 2,
                            this->height() - (SHADOW_WIDTH - i) * 2, RADIUS, RADIUS);
        color.setAlpha(240 - static_cast<int>(qSqrt(i) * 80)); ///< 动态调整透明度
        painter.setPen(color);
        painter.drawPath(path);
    }
    */
    // 绘制阴影（从外向内，黑色渐变为透明）
    for (int i = 0; i != SHADOW_WIDTH; ++i) {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRoundedRect(SHADOW_WIDTH - i, SHADOW_WIDTH - i,
                            this->width() - (SHADOW_WIDTH - i) * 2,
                            this->height() - (SHADOW_WIDTH - i) * 2, RADIUS, RADIUS);

        // 关键修改：使用灰色作为基础色，并动态计算透明度
        int maxAlpha = 150;  // 最外层阴影的最大透明度（0-255）
        float progress = static_cast<float>(i) / SHADOW_WIDTH; // 进度 0.0~1.0
        int alpha = static_cast<int>(maxAlpha * (1.0f - progress));

        QColor color(122, 115, 116, alpha); // 灰色 + 动态透明度
        painter.setPen(color);
        painter.drawPath(path);
    }

    // 绘制背景（在阴影上方）
    QBrush brush(QColor(QStringLiteral("#eef2ff")));
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    QPainterPath bgPath;
    bgPath.addRoundedRect(SHADOW_WIDTH, SHADOW_WIDTH,
                          this->width() - SHADOW_WIDTH * 2,
                          this->height() - SHADOW_WIDTH * 2,
                          RADIUS, RADIUS);
    painter.drawPath(bgPath);
}

/**
 * @brief 重写窗口大小调整事件
 * @param event 大小调整事件
 */
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

/**
 * @brief 重写鼠标按下事件，处理窗口拖动和拉伸
 * @param ev 鼠标事件
 */
void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    QWidget::mousePressEvent(ev);
    auto w = new WaterDrop; ///< 创建水滴动画
    w->move(this->mapToGlobal(ev->pos()));
    w->show();
    if (ev->button() == Qt::LeftButton) {
        // qDebug() << "按下 :" << ev->pos(); ///< 调试用
        windowsLastPs = pos(); ///< 记录窗口位置
        mousePs = ev->globalPosition().toPoint(); ///< 记录鼠标全局位置
        isPress = true; ///< 设置按下标志
        mouse_press_region = getMouseRegion(ev->pos().x(), ev->pos().y()); ///< 获取鼠标区域
    }
}

/**
 * @brief 重写鼠标释放事件，结束拖动或拉伸
 * @param event 鼠标事件
 */
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton) {
        isPress = false; ///< 清除按下标志
    }
    setCursor(QCursor{}); ///< 恢复默认光标
}

/**
 * @brief 重写鼠标移动事件，处理窗口拖动和拉伸
 * @param event 鼠标事件
 */
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    setMouseCursor(event->pos().x(), event->pos().y()); ///< 设置光标形状
    this->point_offset = event->globalPosition().toPoint() - mousePs; ///< 计算鼠标偏移

    if (isPress && mouse_press_region != kMousePositionMid) {
        QRect rect = this->geometry(); ///< 获取窗口区域
        switch (mouse_press_region) {
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
        setGeometry(rect); ///< 更新窗口区域
        mousePs = event->globalPosition().toPoint(); ///< 更新鼠标位置
    }
}

void MainWindow::showEvent(QShowEvent *event) {
    static bool firstShow = false;
    if (!firstShow) {
        event->ignore(); ///< 忽略首次开启

        auto effect = new QGraphicsOpacityEffect(this);
        effect->setOpacity(0.0);
        setGraphicsEffect(effect);

        auto timeLine = new QTimeLine(300, this);
        connect(timeLine, &QTimeLine::valueChanged, this, [=](const qreal &value) {
            QLinearGradient gradient(0, 0, 0, height());
            gradient.setColorAt(0, Qt::white);
            gradient.setColorAt(value, Qt::white);
            gradient.setColorAt(1, Qt::transparent);
            effect->setOpacityMask(gradient); ///< 设置渐变遮罩
            effect->setOpacity(value); ///< 调整透明度
        });

        connect(timeLine, &QTimeLine::finished, this, [=] {
            // 必须延迟 setGraphicsEffect(nullptr)，让 effect->deleteLater() 先执行
            QTimer::singleShot(0, this, [=]() {
                setGraphicsEffect(nullptr); // 自动释放，但我们已经 deleteLater 了
            });
            effect->deleteLater();
            firstShow = true;
        });

        timeLine->start(); // 开始淡入动画
    }
    else {
        event->accept();
    }
    QWidget::showEvent(event); ///< 接受显示事件
}

/**
 * @brief 重写关闭事件，执行渐变关闭动画
 * @param event 关闭事件
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    static bool isClosing = false;                      ///< 窗口关闭标志
    if (!isClosing) {
        event->ignore(); ///< 忽略首次关闭

        auto effect = new QGraphicsOpacityEffect(this);
        effect->setOpacity(1.0);
        setGraphicsEffect(effect);

        auto timeLine = new QTimeLine(300, this);
        connect(timeLine, &QTimeLine::valueChanged, this, [=](const qreal &value) {
            QLinearGradient gradient(0, height(), 0, 0);
            gradient.setColorAt(0, Qt::transparent);
            gradient.setColorAt(value, Qt::transparent);
            gradient.setColorAt(1, Qt::white);
            effect->setOpacityMask(gradient); ///< 设置渐变遮罩
            effect->setOpacity(1 - value); ///< 调整透明度
        });

        connect(timeLine, &QTimeLine::finished, this, [=] {
            QTimer::singleShot(0, this, [=] {
                setGraphicsEffect(nullptr); // 自动释放，但我们已经 deleteLater 了
            });
            effect->deleteLater();
            isClosing = true;
            close(); ///< 再次触发关闭
        });

        timeLine->start(); ///< 启动动画
    } else {
        event->accept();
        QWidget::closeEvent(event); ///< 接受关闭事件
    }
}

/**
 * @brief 设置控件的工具提示
 * @param widget 目标控件
 * @param tooltip 提示内容
 */
void MainWindow::setElaToolTip(QWidget *widget, const QString &tooltip)
{
    auto toolTip = new ElaToolTip(widget);
    toolTip->setToolTip(tooltip);
}

/**
 * @brief 设置鼠标光标形状
 * @param x 鼠标 X 坐标
 * @param y 鼠标 Y 坐标
 */
void MainWindow::setMouseCursor(const int &x, const int &y)
{
    Qt::CursorShape cursor = Qt::ArrowCursor;
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

/**
 * @brief 获取鼠标所在区域
 * @param x 鼠标 X 坐标
 * @param y 鼠标 Y 坐标
 * @return 鼠标区域编号
 */
int MainWindow::getMouseRegion(const int &x, const int &y) const
{
    int region_x = 0, region_y = 0;
    if (x < Area::kMouseRegionLeft) {
        region_x = 1; ///< 左侧区域
    } else if (x > (this->width() - Area::kMouseRegionRight)) {
        region_x = 3; ///< 右侧区域
    } else {
        region_x = 2; ///< 中间区域
    }
    if (y < Area::kMouseRegionTop) {
        region_y = 1; ///< 上侧区域
    } else if (y > (this->height() - Area::kMouseRegionBottom)) {
        region_y = 3; ///< 下侧区域
    } else {
        region_y = 2; ///< 中间区域
    }
    return region_y * 10 + region_x; ///< 计算区域编号
}

/**
 * @brief 显示或隐藏关于对话框
 * @param flag 是否显示
 */
void MainWindow::onShowAboutDialog(const bool flag)
{
    STREAM_INFO() << "系统托盘图标点击: " << (flag ? "显示关于对话框" : "隐藏关于对话框");
    this->raise();
    this->activateWindow();
    this->showNormal();
    if (!this->m_showDialog) {
        this->m_aboutDialog->onShowDialog();
        this->m_showDialog = true;
    } else {
        this->m_aboutDialog->onHideDialog();
        this->m_showDialog = false;
    }
}