/**
 * @file PopularLeftWidget.cpp
 * @brief 实现 PopularLeftWidget 类，提供热门左侧控件功能
 * @author WeiWang
 * @date 2025-02-17
 * @version 1.0
 */

#include "PopularLeftWidget.h"
#include "logger.hpp"

#include <QFile>
#include <QGraphicsEffect>
#include <QToolButton>
#include <QMovie>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QLabel>
#include <QPainter>
#include <QTimer>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化热门左侧控件
 * @param parent 父控件指针，默认为 nullptr
 */
PopularLeftWidget::PopularLeftWidget(QWidget *parent)
    : QWidget(parent)
    , m_enterLivBtn(new QToolButton(this))
    , m_opacityEffect(new QGraphicsOpacityEffect(this))
    , m_aniGroup(new QParallelAnimationGroup(this))
    , m_stopLab(new QLabel(this))
    , m_refreshLab(new QLabel(this))
    , m_muteLab(new QLabel(this))
{
    this->setObjectName("leftWidget");                   ///< 设置对象名称
    QFile file(GET_CURRENT_DIR + QStringLiteral("/left.css")); ///< 加载样式表
    if (file.open(QIODevice::ReadOnly))
    {
        this->setStyleSheet(file.readAll());             ///< 应用样式表
    }
    else
    {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";          ///< 记录错误日志
        return;
    }
    initUi();                                            ///< 初始化界面
    this->m_enterLivBtn->hide();                         ///< 隐藏直播按钮
    this->m_stopLab->hide();                             ///< 隐藏停止标签
    this->m_muteLab->hide();                             ///< 隐藏静音标签
    this->m_refreshLab->hide();                          ///< 隐藏刷新标签
    m_posAnimation = new QPropertyAnimation(this, "animatedY"); ///< 创建位置动画
    m_posAnimation->setDuration(300);                    ///< 设置动画时长
    m_posAnimation->setEasingCurve(QEasingCurve::OutQuad); ///< 设置缓动曲线
    m_alphaAnimation = new QPropertyAnimation(this, "alpha"); ///< 创建透明度动画
    m_alphaAnimation->setDuration(300);                  ///< 设置动画时长
    m_alphaAnimation->setEasingCurve(QEasingCurve::InOutQuad); ///< 设置缓动曲线
    this->m_aniGroup->addAnimation(m_posAnimation);      ///< 添加位置动画
    this->m_aniGroup->addAnimation(m_alphaAnimation);    ///< 添加透明度动画
    m_enterLivBtn->setGraphicsEffect(m_opacityEffect);   ///< 设置透明效果
}

/**
 * @brief 设置动画 Y 坐标
 * @param y Y 坐标
 */
void PopularLeftWidget::setAnimatedY(int y)
{
    m_animatedY = y;                                     ///< 设置 Y 坐标
    update();                                            ///< 刷新界面
}

/**
 * @brief 设置透明度
 * @param alpha 透明度值
 */
void PopularLeftWidget::setAlpha(int alpha)
{
    m_alpha = alpha;                                     ///< 设置透明度
    update();                                            ///< 刷新界面
}

/**
 * @brief 初始化界面
 * @note 初始化直播按钮和标签
 */
void PopularLeftWidget::initUi()
{
    this->m_enterLivBtn->setObjectName("enterLiveBtn");  ///< 设置按钮对象名称
    this->m_enterLivBtn->setCursor(Qt::PointingHandCursor); ///< 设置手形光标
    this->m_enterLivBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); ///< 设置文本和图标样式
    this->m_enterLivBtn->setText("点击进入直播间");     ///< 设置按钮文本
    this->m_enterLivBtn->setFixedSize(210, 60);          ///< 设置按钮大小
    this->m_enterLivBtn->setIconSize(QSize(60, 40));     ///< 设置图标大小
    const auto movie = new QMovie(":/Live/Res/live/music.gif"); ///< 创建 GIF 动画
    movie->start();                                      ///< 启动动画
    const auto timer = new QTimer(this);                 ///< 创建定时器
    connect(timer, &QTimer::timeout, [this, movie]() {
        this->m_enterLivBtn->setIcon(QIcon(movie->currentPixmap())); ///< 更新按钮图标
    });
    timer->start(150);                                   ///< 设置定时器间隔（约 30 FPS）
    this->m_stopLab->setObjectName("stopLab");           ///< 设置停止标签对象名称
    this->m_stopLab->setFixedSize(30, 30);               ///< 设置停止标签大小
    this->m_stopLab->setCursor(Qt::PointingHandCursor);  ///< 设置手形光标
    const auto stopLabImgPath = ":/Live/Res/live/stop.png"; ///< 停止图标路径
    this->m_stopLab->setStyleSheet(QString("border-image: url('%1');").arg(stopLabImgPath)); ///< 设置停止图标
    this->m_refreshLab->setObjectName("refreshLab");     ///< 设置刷新标签对象名称
    this->m_refreshLab->setFixedSize(35, 35);            ///< 设置刷新标签大小
    this->m_refreshLab->setCursor(Qt::PointingHandCursor); ///< 设置手形光标
    const auto refreshLabImgPath = ":/Live/Res/live/refresh.png"; ///< 刷新图标路径
    this->m_refreshLab->setStyleSheet(QString("border-image:url('%1');").arg(refreshLabImgPath)); ///< 设置刷新图标
    this->m_muteLab->setObjectName("muteLab");           ///< 设置静音标签对象名称
    this->m_muteLab->setFixedSize(30, 30);               ///< 设置静音标签大小
    this->m_muteLab->setCursor(Qt::PointingHandCursor);  ///< 设置手形光标
    const auto muteLabImgPath = ":/Live/Res/live/mute.png"; ///< 静音图标路径
    this->m_muteLab->setStyleSheet(QString("border-image:url('%1');").arg(muteLabImgPath)); ///< 设置静音图标
}

/**
 * @brief 执行向上动画
 * @note 调整位置和透明度
 */
void PopularLeftWidget::animationUp() const
{
    this->m_aniGroup->stop();                            ///< 停止当前动画
    this->m_posAnimation->setStartValue(this->height() / 2 - this->m_enterLivBtn->height() / 2 + 35); ///< 设置起始位置
    this->m_posAnimation->setEndValue(this->height() / 2 - this->m_enterLivBtn->height() / 2); ///< 设置结束位置
    this->m_alphaAnimation->setStartValue(0);            ///< 设置起始透明度
    this->m_alphaAnimation->setEndValue(255);            ///< 设置结束透明度
    this->m_aniGroup->start();                           ///< 启动动画
}

/**
 * @brief 执行向下动画
 * @note 调整位置和透明度
 */
void PopularLeftWidget::animationDown() const
{
    this->m_aniGroup->stop();                            ///< 停止当前动画
    this->m_posAnimation->setStartValue(this->height() / 2 - this->m_enterLivBtn->height() / 2); ///< 设置起始位置
    this->m_posAnimation->setEndValue(this->height() / 2 - this->m_enterLivBtn->height() / 2 + 35); ///< 设置结束位置
    this->m_alphaAnimation->setStartValue(255);          ///< 设置起始透明度
    this->m_alphaAnimation->setEndValue(0);              ///< 设置结束透明度
    this->m_aniGroup->start();                           ///< 启动动画
}

/**
 * @brief 绘制事件
 * @param event 绘制事件
 * @note 绘制背景遮罩和控件位置
 */
void PopularLeftWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);                          ///< 调用父类绘制
    QPainter painter(this);                              ///< 创建画家
    painter.setRenderHint(QPainter::Antialiasing);       ///< 启用抗锯齿
    painter.setBrush(QColor(0, 0, 0, 160));              ///< 设置半透明黑色背景
    painter.setPen(Qt::NoPen);                           ///< 无边框
    painter.drawRoundedRect(rect(), 10, 10);             ///< 绘制圆角矩形
    this->m_enterLivBtn->move(this->width() / 2 - this->m_enterLivBtn->width() / 2, this->m_animatedY); ///< 设置按钮位置
    m_opacityEffect->setOpacity(m_alpha / 255.0);        ///< 设置按钮透明度
    this->m_stopLab->move(20, this->height() - 50);      ///< 设置停止标签位置
    this->m_refreshLab->move(this->width() - 100, this->height() - 53); ///< 设置刷新标签位置
    this->m_muteLab->move(this->width() - 50, this->height() - 50); ///< 设置静音标签位置
}

/**
 * @brief 进入事件
 * @param event 进入事件
 * @note 显示控件并触发向上动画
 */
void PopularLeftWidget::enterEvent(QEnterEvent *event)
{
    QWidget::enterEvent(event);                          ///< 调用父类事件
    this->m_enterLivBtn->show();                         ///< 显示直播按钮
    this->m_stopLab->show();                             ///< 显示停止标签
    this->m_muteLab->show();                             ///< 显示静音标签
    this->m_refreshLab->show();                          ///< 显示刷新标签
    animationUp();                                       ///< 触发向上动画
}

/**
 * @brief 离开事件
 * @param event 离开事件
 * @note 隐藏控件并触发向下动画
 */
void PopularLeftWidget::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);                          ///< 调用父类事件
    this->m_enterLivBtn->hide();                         ///< 隐藏直播按钮
    this->m_stopLab->hide();                             ///< 隐藏停止标签
    this->m_muteLab->hide();                             ///< 隐藏静音标签
    this->m_refreshLab->hide();                          ///< 隐藏刷新标签
    animationDown();                                     ///< 触发向下动画
}