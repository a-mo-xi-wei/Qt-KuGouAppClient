/**
 * @file RefreshMask.cpp
 * @brief 实现 RefreshMask 类，提供加载遮罩功能
 * @author WeiWang
 * @date 2025-03-22
 * @version 1.0
 */

#include "RefreshMask.h"
#include <QGraphicsDropShadowEffect>
#include <QTimer>
#include <QVBoxLayout>

/**
 * @brief 构造函数，初始化加载遮罩
 * @param parent 父控件指针，默认为 nullptr
 */
RefreshMask::RefreshMask(QWidget *parent)
    : QWidget(parent)
    , m_timer(new QTimer(this))
{
    initUi();
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, [this] {hideLoading("刷新成功");});
}

/**
 * @brief 初始化界面
 */
void RefreshMask::initUi()
{
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

    // 确保进度条在遮罩层之上
    progress->raise();
    auto layout = new QVBoxLayout(maskWidget);
    layout->addWidget(progress);
    layout->setAlignment(progress, Qt::AlignCenter);
}

/**
 * @brief 显示加载遮罩
 */
void RefreshMask::showLoading(const int& loadingTime)
{
    if (!maskWidget->isHidden() || !progress->isHidden())return;
    //this->resize(this->parentWidget()->size());
    this->raise();          // 确保位于父部件最上层
    this->show();           // 显示 RefreshMask 自身
    maskWidget->show();
    progress->show(); // 开始旋转动画
    m_timer->start(loadingTime); // 启动定时器
}

/**
 * @brief 保持加载遮罩
 */
void RefreshMask::keepLoading()
{
    if (!maskWidget->isHidden() || !progress->isHidden())return;
    // 确保遮罩位于最上层并显示
    this->raise();
    this->show();
    maskWidget->show();
    progress->show();

    // 停止定时器以防止自动隐藏
    m_timer->stop();
}

/**
 * @brief 隐藏加载遮罩
 */
void RefreshMask::hideLoading(const QString& message)
{
    if (this->isHidden()) return;
    m_timer->stop();
    this->hide();
    maskWidget->hide();
    progress->hide();
    emit loadingFinished(message);
}

bool RefreshMask::isLoading() {
    return !this->isHidden();
}

/**
 * @brief 大小调整事件，更新遮罩和进度条位置
 * @param event 大小调整事件
 */
void RefreshMask::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    maskWidget->resize(size());
    progress->move((width() - progress->width()) / 2,
                   (height() - progress->height()) / 2);
}