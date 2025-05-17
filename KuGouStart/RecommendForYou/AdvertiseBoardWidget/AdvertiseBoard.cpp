/**
 * @file AdvertiseBoard.cpp
 * @brief 实现 AdvertiseBoard 类，管理广告轮播界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#include "AdvertiseBoard.h"
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QResizeEvent>

/**
 * @brief NavButton 构造函数，初始化导航按钮
 * @param normalImage 普通状态图片路径
 * @param hoverImage 悬停状态图片路径
 * @param parent 父控件指针，默认为 nullptr
 */
NavButton::NavButton(const QString &normalImage, const QString &hoverImage, QWidget *parent)
    : QLabel(parent)
    , m_normal(normalImage)
    , m_hover(hoverImage)
{
    setAttribute(Qt::WA_Hover);                          ///< 启用悬停事件
    setMouseTracking(true);                      ///< 启用鼠标跟踪
    setPixmap(m_normal);                                 ///< 设置普通状态图片
    setAttribute(Qt::WA_TranslucentBackground);          ///< 设置透明背景
    setAlignment(Qt::AlignCenter);                    ///< 设置图片居中
}

/**
 * @brief 鼠标进入事件
 * @param event 进入事件
 * @note 切换到悬停状态图片
 */
void NavButton::enterEvent(QEnterEvent *event)
{
    setPixmap(m_hover);                                  ///< 设置悬停状态图片
    QLabel::enterEvent(event);                           ///< 调用父类处理
}

/**
 * @brief 鼠标离开事件
 * @param event 离开事件
 * @note 恢复普通状态图片
 */
void NavButton::leaveEvent(QEvent *event)
{
    setPixmap(m_normal);                                 ///< 设置普通状态图片
    QLabel::leaveEvent(event);                           ///< 调用父类处理
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 触发点击信号
 */
void NavButton::mousePressEvent(QMouseEvent *event)
{
    emit clicked();                                      ///< 触发点击信号
    QLabel::mousePressEvent(event);                      ///< 调用父类处理
}

/**
 * @brief AdvertiseBoard 构造函数，初始化广告轮播界面
 * @param parent 父控件指针，默认为 nullptr
 */
AdvertiseBoard::AdvertiseBoard(QWidget *parent)
    : QWidget(parent)
    , m_leftBtn(new NavButton(":/Res/window/left.svg", ":/Res/window/left-pink.svg", this))
    , m_rightBtn(new NavButton(":/Res/window/right.svg", ":/Res/window/right-pink.svg", this))
    , m_timer(new QTimer(this))
{
    m_timer->setInterval(3000);                          ///< 设置定时器间隔
    connect(m_timer, &QTimer::timeout, this, [this] {
        m_currentIndex = (m_currentIndex + 1) % m_posters.size();
        update();
    });                                                  ///< 连接定时器超时信号
    connect(m_leftBtn, &NavButton::clicked, this, [this] {
        m_currentIndex = (m_currentIndex - 1 + m_posters.size()) % m_posters.size();
        update();
        m_timer->start();
    });                                                  ///< 连接左按钮点击信号
    connect(m_rightBtn, &NavButton::clicked, this, [this] {
        m_currentIndex = (m_currentIndex + 1) % m_posters.size();
        update();
        m_timer->start();
    });                                                  ///< 连接右按钮点击信号

    m_leftBtn->hide();                                   ///< 隐藏左按钮
    m_rightBtn->hide();                                  ///< 隐藏右按钮
    updateButtonPosition();                              ///< 更新按钮位置
}

/**
 * @brief 添加海报
 * @param posterPix 海报图片
 * @note 添加海报并在首个海报时启动定时器
 */
void AdvertiseBoard::addPoster(const QPixmap &posterPix)
{
    m_posters.append(posterPix);                         ///< 添加海报
    if (m_posters.size() == 1 && !m_timer->isActive())
    {
        m_timer->start();                                ///< 首个海报时启动定时器
    }
}

/**
 * @brief 设置宽高比
 * @param ratio 宽高比
 * @note 设置有效宽高比，默认为 1.0
 */
void AdvertiseBoard::setAspectRatio(double ratio)
{
    m_aspectRatio = ratio > 0 ? ratio : 1.0;             ///< 设置宽高比
    updateGeometry();                                    ///< 更新几何尺寸
}

/**
 * @brief 设置自动播放间隔
 * @param ms 间隔时间（毫秒）
 * @note 设置有效间隔，默认为 3000ms
 */
void AdvertiseBoard::setAutoPlayInterval(int ms)
{
    m_timer->setInterval(ms > 0 ? ms : 3000);            ///< 设置定时器间隔
}

/**
 * @brief 绘制事件
 * @param ev 绘制事件
 * @note 绘制当前海报和导航圆点
 */
void AdvertiseBoard::paintEvent(QPaintEvent *ev)
{
    QWidget::paintEvent(ev);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform); ///< 启用抗锯齿和平滑变换

    if (!m_posters.isEmpty())
    {
        QPixmap scaled = m_posters[m_currentIndex].scaled(size(), Qt::KeepAspectRatioByExpanding); ///< 缩放当前海报
        QPainterPath path;
        path.addRoundedRect(rect(), 10, 10);             ///< 创建圆角矩形路径
        painter.setClipPath(path);                       ///< 设置剪裁路径
        painter.drawPixmap(rect(), scaled);              ///< 绘制海报
    }

    if (m_posters.size() > 1)
    {
        QList<QPoint> centers;
        int totalWidth;
        calculateDotPositions(centers, totalWidth);      ///< 计算导航圆点位置
        painter.setPen(Qt::NoPen);                       ///< 设置无边框
        for (int i = 0; i < centers.size(); ++i)
        {
            bool isActive = (i == m_currentIndex);
            int radius = isActive ? DOT_RADIUS + ACTIVE_DOT_EXTRA : DOT_RADIUS; ///< 设置圆点半径
            painter.setBrush(isActive ? QColor(255, 100, 100) : QColor(255, 255, 255, 150)); ///< 设置圆点颜色
            painter.drawEllipse(centers[i], radius, radius); ///< 绘制圆点
        }
    }
}

/**
 * @brief 调整大小事件
 * @param ev 调整大小事件
 * @note 更新按钮位置和控件高度
 */
void AdvertiseBoard::resizeEvent(QResizeEvent *ev)
{
    updateButtonPosition();                              ///< 更新按钮位置
    setFixedHeight(ev->size().width() / m_aspectRatio);  ///< 设置固定高度
    QWidget::resizeEvent(ev);                            ///< 调用父类处理
}

/**
 * @brief 鼠标进入事件
 * @param ev 进入事件
 * @note 显示导航按钮
 */
void AdvertiseBoard::enterEvent(QEnterEvent *ev)
{
    m_leftBtn->show();                                   ///< 显示左按钮
    m_rightBtn->show();                                  ///< 显示右按钮
    QWidget::enterEvent(ev);                             ///< 调用父类处理
}

/**
 * @brief 鼠标离开事件
 * @param ev 离开事件
 * @note 隐藏导航按钮并重启定时器
 */
void AdvertiseBoard::leaveEvent(QEvent *ev)
{
    m_leftBtn->hide();                                   ///< 隐藏左按钮
    m_rightBtn->hide();                                  ///< 隐藏右按钮
    if (!m_posters.isEmpty())
    {
        m_timer->start();                                ///< 重启定时器
    }
    QWidget::leaveEvent(ev);                             ///< 调用父类处理
}

/**
 * @brief 更新导航按钮位置
 * @note 设置按钮大小和位置
 */
void AdvertiseBoard::updateButtonPosition()
{
    m_leftBtn->setFixedSize(60, height());               ///< 设置左按钮大小
    m_rightBtn->setFixedSize(60, height());              ///< 设置右按钮大小
    m_leftBtn->move(0, 0);                               ///< 设置左按钮位置
    m_rightBtn->move(width() - m_rightBtn->width(), 0);  ///< 设置右按钮位置
}

/**
 * @brief 计算导航圆点位置
 * @param centers 圆点中心点列表
 * @param totalWidth 总宽度
 * @note 计算圆点位置并返回总宽度
 */
void AdvertiseBoard::calculateDotPositions(QList<QPoint> &centers, int &totalWidth)
{
    const int count = m_posters.size();
    const int maxRadius = DOT_RADIUS + ACTIVE_DOT_EXTRA;
    totalWidth = (count - 1) * (2 * maxRadius + DOT_SPACING) + 2 * maxRadius; ///< 计算总宽度

    int startX = (width() - totalWidth) / 2 + maxRadius; ///< 计算起始 X 坐标
    int yPos = height() - 20;                            ///< 设置 Y 坐标

    for (int i = 0; i < count; ++i)
    {
        centers.append(QPoint(startX, yPos));            ///< 添加圆点中心点
        startX += 2 * maxRadius + DOT_SPACING;           ///< 更新 X 坐标
    }
}