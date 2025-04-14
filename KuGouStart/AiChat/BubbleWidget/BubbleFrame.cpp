#include "BubbleFrame.h"
#include <QPainter>
#include <QDateTime>

const int WIDTH_SANJIAO  = 8;  //三角宽

BubbleFrame::BubbleFrame(ChatRole role, QWidget *parent)
    :QFrame(parent)
    ,m_role(role)
    ,m_margin(3)
{
    m_pHLayout = new QHBoxLayout(this);
    if(m_role == ChatRole::Self)
        m_pHLayout->setContentsMargins(m_margin, m_margin, WIDTH_SANJIAO + m_margin, m_margin);
    else
        m_pHLayout->setContentsMargins(WIDTH_SANJIAO + m_margin, m_margin, m_margin, m_margin);
    m_createTime = QDateTime::currentDateTime().toString("MM/dd HH:mm");
}

void BubbleFrame::setWidget(QWidget *w)
{
    if(m_pHLayout->count() > 0)
        return ;
    m_pHLayout->addWidget(w);
}

void BubbleFrame::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);//消锯齿
    painter.setPen(Qt::NoPen);

    if(m_role == ChatRole::Other)
    {
        //画气泡
        QColor bk_color(Qt::white);
        painter.setBrush(QBrush(bk_color));
        QRect bk_rect = QRect(WIDTH_SANJIAO, 0, this->width()-WIDTH_SANJIAO, this->height());
        painter.drawRoundedRect(bk_rect,5,5);
        //画小三角
        QPointF points[3] = {
            QPointF(bk_rect.x(), 12),
            QPointF(bk_rect.x(), 10+WIDTH_SANJIAO +2),
            QPointF(bk_rect.x()-WIDTH_SANJIAO, 10+WIDTH_SANJIAO-WIDTH_SANJIAO/2),
        };
        painter.drawPolygon(points, 3);
    }
    else if (m_role == ChatRole::Self)
    {
        QColor bk_color(158,234,106);
        painter.setBrush(QBrush(bk_color));
        //画气泡
        QRect bk_rect = QRect(0, 0, this->width()-WIDTH_SANJIAO, this->height());
        painter.drawRoundedRect(bk_rect,5,5);
        //画三角
        QPointF points[3] = {
            QPointF(bk_rect.x()+bk_rect.width(), 12),
            QPointF(bk_rect.x()+bk_rect.width(), 12+WIDTH_SANJIAO +2),
            QPointF(bk_rect.x()+bk_rect.width()+WIDTH_SANJIAO, 10+WIDTH_SANJIAO-WIDTH_SANJIAO/2),
        };
        painter.drawPolygon(points, 3);
    }
    else if (m_role == ChatRole::Time) {
        // 时间标签（灰色居中文字，无气泡）
        QPen penText;
        penText.setColor(QColor(153, 153, 153));  // 设置文字颜色为灰色
        painter.setPen(penText);

        // 设置文字对齐和换行模式
        QTextOption option(Qt::AlignCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

        // 设置字体样式
        QFont te_font = this->font();
        te_font.setFamily("MicrosoftYaHei");      // 字体
        te_font.setPointSize(10);                // 字号
        painter.setFont(te_font);

        // 在控件矩形中央绘制时间文本
        this->setFixedWidth(150);
        painter.drawText(this->rect(),m_createTime , option);
    }

    return QFrame::paintEvent(e);
}
