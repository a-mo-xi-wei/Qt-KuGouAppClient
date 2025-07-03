/**
 * @file BubbleFrame.cpp
 * @brief 实现 BubbleFrame 类，提供聊天气泡框架功能
 * @author WeiWang
 * @date 2024-11-12
 * @version 1.0
 */

#include "BubbleFrame.h"
#include <QPainter>
#include <QDateTime>

/** @brief 三角形宽度常量 */
const int WIDTH_SANJIAO = 8;

/**
 * @brief 构造函数，初始化气泡框架
 * @param role 聊天角色（自己、他人、时间）
 * @param parent 父控件指针，默认为 nullptr
 */
BubbleFrame::BubbleFrame(ChatRole role, QWidget *parent)
    : QFrame(parent)
    , m_role(role)
    , m_margin(3)
{
    m_pHLayout = new QHBoxLayout(this);                       ///< 创建水平布局
    if (m_role == ChatRole::Self)
        m_pHLayout->setContentsMargins(m_margin, m_margin, WIDTH_SANJIAO + m_margin, m_margin); ///< 设置自己角色边距
    else
        m_pHLayout->setContentsMargins(WIDTH_SANJIAO + m_margin, m_margin, m_margin, m_margin); ///< 设置他人/时间角色边距
    m_createTime = QDateTime::currentDateTime().toString("MM/dd HH:mm"); ///< 设置当前时间
}

/**
 * @brief 设置气泡内容控件
 * @param w 内容控件指针
 */
void BubbleFrame::setWidget(QWidget *w)
{
    if (m_pHLayout->count() > 0)
        return;                                               ///< 防止重复添加
    m_pHLayout->addWidget(w);                                ///< 添加内容控件
}

/**
 * @brief 绘制事件
 * @param e 绘制事件对象
 * @note 重写基类方法，绘制气泡或时间标签
 */
void BubbleFrame::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform); ///< 启用抗锯齿
    painter.setPen(Qt::NoPen);                               ///< 无边框

    if (m_role == ChatRole::Other)
    {
        // 绘制他人气泡
        QColor bk_color(Qt::white);                           ///< 背景色（白色）
        painter.setBrush(QBrush(bk_color));
        QRect bk_rect = QRect(WIDTH_SANJIAO, 0, this->width() - WIDTH_SANJIAO, this->height()); ///< 气泡矩形
        painter.drawRoundedRect(bk_rect, 5, 5);              ///< 绘制圆角矩形
        // 绘制左侧三角形
        QPointF points[3] = {
            QPointF(bk_rect.x(), 12),
            QPointF(bk_rect.x(), 10 + WIDTH_SANJIAO + 2),
            QPointF(bk_rect.x() - WIDTH_SANJIAO, 10 + WIDTH_SANJIAO - WIDTH_SANJIAO / 2),
        };
        painter.drawPolygon(points, 3);                      ///< 绘制三角形
    }
    else if (m_role == ChatRole::Self)
    {
        // 绘制自己气泡
        QColor bk_color(158, 234, 106);                      ///< 背景色（绿色）
        painter.setBrush(QBrush(bk_color));
        QRect bk_rect = QRect(0, 0, this->width() - WIDTH_SANJIAO, this->height()); ///< 气泡矩形
        painter.drawRoundedRect(bk_rect, 5, 5);              ///< 绘制圆角矩形
        // 绘制右侧三角形
        QPointF points[3] = {
            QPointF(bk_rect.x() + bk_rect.width(), 12),
            QPointF(bk_rect.x() + bk_rect.width(), 12 + WIDTH_SANJIAO + 2),
            QPointF(bk_rect.x() + bk_rect.width() + WIDTH_SANJIAO, 10 + WIDTH_SANJIAO - WIDTH_SANJIAO / 2),
        };
        painter.drawPolygon(points, 3);                      ///< 绘制三角形
    }
    else if (m_role == ChatRole::Time)
    {
        // 绘制时间标签
        QPen penText;
        penText.setColor(QColor(153, 153, 153));             ///< 设置灰色文字
        painter.setPen(penText);
        QTextOption option(Qt::AlignCenter);                 ///< 居中对齐
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere); ///< 自动换行
        QFont te_font = this->font();
        te_font.setFamily("MicrosoftYaHei");                 ///< 设置字体
        te_font.setPointSize(10);                            ///< 设置字号
        painter.setFont(te_font);
        this->setFixedWidth(150);                            ///< 设置固定宽度
        painter.drawText(this->rect(), m_createTime, option); ///< 绘制时间文本
    }

    QFrame::paintEvent(e);                                   ///< 调用基类绘制
}