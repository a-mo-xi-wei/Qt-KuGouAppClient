//
// Created by WeiWang on 25-3-22.
//

#include "RefreshMask.h"

#include <QGraphicsDropShadowEffect>
#include <QTimer>
#include <QVBoxLayout>

RefreshMask::RefreshMask(QWidget *parent)
    : QWidget(parent)
    , m_timer(new QTimer(this))
{
    initUi();
    m_timer->setSingleShot(true); // 设置为单次触发
    connect(m_timer, &QTimer::timeout, this, &RefreshMask::hideLoading);
}

void RefreshMask::initUi() {
    this->setCursor(Qt::ForbiddenCursor);
    // 初始化遮罩层（与窗口同级）
    maskWidget = new QWidget(this);
    // 给遮罩层添加圆角阴影
    maskWidget->setStyleSheet(
        "background-color: qlineargradient("
                "x1:0, y1:0, x2:1, y2:1,"
                "stop:0 rgba(196, 243, 255, 50%),"
                "stop:1 rgba(255, 214, 255, 50%)"
        ");"
        "border-radius: 12px;"
        "border: none;"
    );

    // 添加磨砂效果（仅作用于遮罩层）
    auto *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(15); // 模糊半径
    maskWidget->setGraphicsEffect(blur);
    maskWidget->hide();

    // 添加阴影效果
    auto *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 50));
    shadow->setOffset(0, 0);
    maskWidget->setGraphicsEffect(shadow);

    // 初始化进度条（与遮罩层同级，但位于上层）
    progress = new QtMaterialCircularProgress(this);
    progress->hide();

    // 确保Z轴顺序：进度条在遮罩层之上
    progress->raise();
    auto layout = new QVBoxLayout(maskWidget);
    layout->addWidget(progress);
    layout->setAlignment(progress, Qt::AlignCenter);
}

void RefreshMask::showLoading() {
    //this->resize(this->parentWidget()->size());
    this->raise();          // 确保位于父部件最上层
    this->show();           // 显示 RefreshMask 自身
    maskWidget->show();
    progress->show(); // 开始旋转动画
    m_timer->start(2000); // 启动定时器
}

void RefreshMask::hideLoading() {
    if (this->isHidden())return;
    m_timer->stop(); // 停止定时器（避免重复触发）
    this->hide();
    maskWidget->hide();
    progress->hide();
    emit loadingFinished();
}

void RefreshMask::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    maskWidget->resize(size());
    progress->move((width()-progress->width())/2,
                  (height()-progress->height())/2);
}

