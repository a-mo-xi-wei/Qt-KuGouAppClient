#include "AdvertiseBoard.h"
#include<QPixmap>
#include<QTimer>
#include<QPaintevent>
#include<QPainter>
#include<QPainterPath>
#include<QMouseEvent>

static constexpr int G_Radius = 4;  //圆球半径
static constexpr int G_Space = 2 * G_Radius;   //圆球之间的间隔
static int posterIndex = 0;

AdvertiseBoard::AdvertiseBoard(QWidget *parent)
    : QWidget{parent}
    ,m_leftLab(new MyLLabel(this))
    ,m_rightLab(new MyRLabel(this))
    ,m_timer(new QTimer(this))
{

    connect(this->m_timer,&QTimer::timeout,this,[this]{posterIndex = (++posterIndex) % static_cast<int>(this->m_posters.size());update();});
    this->m_timer->start(3000);

    this->m_leftLab->setObjectName("leftLab");
    this->m_rightLab->setObjectName("rightLab");

    this->m_leftLab->setPixmap(QPixmap(QStringLiteral(":/Res/window/left.svg")));
    this->m_rightLab->setPixmap(QPixmap(QStringLiteral(":/Res/window/right.svg")));

    this->m_leftLab->hide();
    this->m_rightLab->hide();

    this->m_leftLab->setFixedWidth(60);
    this->m_rightLab->setFixedWidth(60);
    // 设置标签背景透明
    this->m_leftLab->setAttribute(Qt::WA_TranslucentBackground);
    this->m_rightLab->setAttribute(Qt::WA_TranslucentBackground);
    // 设置居中
    this->m_leftLab->setAlignment(Qt::AlignCenter);
    this->m_rightLab->setAlignment(Qt::AlignCenter);

    connect(this->m_leftLab, &MyLLabel::clicked, [this] {
        posterIndex = (posterIndex - 1 + static_cast<int>(this->m_posters.size())) % static_cast<int>(this->m_posters.size());
        this->update();
        this->m_timer->start(3000);
    });
    connect(this->m_rightLab, &MyRLabel::clicked, [this] {
        posterIndex = (posterIndex + 1) % static_cast<int>(this->m_posters.size());
        this->update();
        this->m_timer->start(3000);
    });

    updateLabPosition();
}

AdvertiseBoard::~AdvertiseBoard()
{
    for(const auto& pix : this->m_posters){
        delete pix;
    }
    this->m_posters.clear();
}

void AdvertiseBoard::addPoster(const QPixmap &posterPix)
{
    const auto pix = new QPixmap(posterPix);
    this->m_posters.append(pix);
}

void AdvertiseBoard::updateLabPosition()
{
    this->m_leftLab->setFixedHeight(this->height());
    this->m_rightLab->setFixedHeight(this->height());
    
    this->m_leftLab->move(0, 0);
    this->m_rightLab->move(this->width() - this->m_rightLab->width(), 0);
}

void AdvertiseBoard::paintEvent(QPaintEvent *ev)
{
    QWidget::paintEvent(ev);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    if(this->m_posters.isEmpty())return;

    const QPixmap img = this->m_posters[posterIndex]->scaled(this->size(),Qt::KeepAspectRatio);
    //将图片变成圆角
    QPainterPath path;
    path.addRoundedRect(this->rect(),10,10);
    painter.setClipPath(path);

    //绘制广告图
    painter.drawPixmap(this->rect(),img);
    painter.setPen(QColor(255,255,255,0));
    //绘制广告播放进度
    const auto s = static_cast<int>(this->m_posters.size());
    auto drawStartPos = QPoint((this->width() - (s-1) * (G_Radius * 2 + G_Space)) / 2,this->height() - 10);
    for(auto i = 0 ; i < this->m_posters.size() ; ++i){
        if(i == posterIndex) {
            painter.setBrush(Qt::cyan);
            painter.drawEllipse(drawStartPos,G_Radius + 2,G_Radius + 2);
            drawStartPos.setX(drawStartPos.x() + G_Space + 2 * (G_Radius + 1));
        }
        else {
            painter.setBrush(Qt::white);
            painter.drawEllipse(drawStartPos,G_Radius,G_Radius);
            drawStartPos.setX(drawStartPos.x() + G_Space + 2 * G_Radius);
        }
    }
}

#define AspectRatio 2
void AdvertiseBoard::resizeEvent(QResizeEvent* ev)
{
    QWidget::resizeEvent(ev);
    updateLabPosition();
    this->setFixedHeight(ev->size().width() / AspectRatio);
    update();
}

void AdvertiseBoard::enterEvent(QEnterEvent* ev)
{
    this->m_leftLab->show();
    this->m_rightLab->show();
}

void AdvertiseBoard::leaveEvent(QEvent* ev)
{
    this->m_leftLab->hide();
    this->m_rightLab->hide();
}


