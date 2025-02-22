//
// Created by WeiWang on 25-2-17.

#include "PopularLeftWidget.h"

#include <QFile>
#include <QGraphicsEffect>
#include <QToolButton>
#include <QMovie>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QLabel>
#include <QPainter>
#include <QTimer>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

PopularLeftWidget::PopularLeftWidget(QWidget *parent)
    : QWidget(parent)
    , m_enterLivBtn(new QToolButton(this))
    , m_opacityEffect(new QGraphicsOpacityEffect(this))
    , m_aniGroup(new QParallelAnimationGroup(this))
    , m_stopLab(new QLabel(this))
    , m_refreshLab(new QLabel(this))
    , m_muteLab(new QLabel(this))
{
    this->setObjectName("leftWidget");
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/left.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
    initUi();
    this->m_enterLivBtn->hide();
    this->m_stopLab->hide();
    this->m_muteLab->hide();
    this->m_refreshLab->hide();
    // 位置动画
    m_posAnimation = new QPropertyAnimation(this, "animatedY");
    m_posAnimation->setDuration(300);
    m_posAnimation->setEasingCurve(QEasingCurve::OutQuad);
    // 创建两个动画，一个从0到255，一个从255到0
    m_alphaAnimation = new QPropertyAnimation(this, "alpha");
    m_alphaAnimation->setDuration(300);
    m_alphaAnimation->setEasingCurve(QEasingCurve::InOutQuad);  // 使用平滑的缓入缓出动画曲线

    this->m_aniGroup->addAnimation(m_posAnimation);
    this->m_aniGroup->addAnimation(m_alphaAnimation);

    m_enterLivBtn->setGraphicsEffect(m_opacityEffect);
}

void PopularLeftWidget::setAnimatedY(int y) {
    m_animatedY = y;
    //qDebug()<<"当前动画高度: "<<m_animatedY;
    update();
}

void PopularLeftWidget::setAlpha(int alpha) {
    m_alpha = alpha;
    update();  // 更新窗口，触发重绘事件
}

void PopularLeftWidget::initUi() {
    //初始化进入直播间按钮
    this->m_enterLivBtn->setObjectName("enterLiveBtn");
    this->m_enterLivBtn->setCursor(Qt::PointingHandCursor);
    this->m_enterLivBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->m_enterLivBtn->setText("点击进入直播间");
    this->m_enterLivBtn->setFixedSize(210,60);
    this->m_enterLivBtn->setIconSize(QSize(60,40));
    const auto movie = new QMovie(GET_CURRENT_DIR + QStringLiteral("/liveRes/music.gif"));

    // Start the movie to begin playing the GIF
    movie->start();

    // Setup a timer to update the button icon with the current frame of the movie
    const auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this,movie](){
        this->m_enterLivBtn->setIcon(QIcon(movie->currentPixmap()));
    });
    timer->start(150); // Adjust interval as necessary, here it's set to 30 FPS

    //初始化下方三个label
    this->m_stopLab->setObjectName("stopLab");
    this->m_stopLab->setFixedSize(30,30);
    this->m_stopLab->setCursor(Qt::PointingHandCursor);
    const auto stopLabImgPath = GET_CURRENT_DIR + QStringLiteral("/liveRes/stop.png");
    this->m_stopLab->setStyleSheet(QString("border-image: url('%1');").arg(stopLabImgPath));
    //this->m_stopLab->setStyleSheet(QString("background-color:red;"));

    this->m_refreshLab->setObjectName("refreshLab");
    this->m_refreshLab->setFixedSize(35,35);
    this->m_refreshLab->setCursor(Qt::PointingHandCursor);
    const auto refreshLabImgPath = GET_CURRENT_DIR + QStringLiteral("/liveRes/refresh.png");
    this->m_refreshLab->setStyleSheet(QString("border-image:url('%1');").arg(refreshLabImgPath));
    //this->m_refreshLab->setStyleSheet(QString("background-color:red;"));

    this->m_muteLab->setObjectName("muteLab");
    this->m_muteLab->setFixedSize(30,30);
    this->m_muteLab->setCursor(Qt::PointingHandCursor);
    const auto muteLabImgPath = GET_CURRENT_DIR + QStringLiteral("/liveRes/mute.png");
    this->m_muteLab->setStyleSheet(QString("border-image:url('%1');").arg(muteLabImgPath));
    //this->m_muteLab->setStyleSheet(QString("background-color:red;"));

    qDebug()<<"当前m_stopLab样式："<<this->m_stopLab->styleSheet()
    <<"当前m_refreshLab样式："<<this->m_refreshLab->styleSheet()
    <<"当前m_muteLab样式："<<this->m_muteLab->styleSheet();
}

void PopularLeftWidget::animationUp() const {
    this->m_aniGroup->stop();

    this->m_posAnimation->setStartValue(this->height()/2-this->m_enterLivBtn->height()/2 + 35);
    this->m_posAnimation->setEndValue(this->height()/2-this->m_enterLivBtn->height()/2);

    this->m_alphaAnimation->setStartValue(0);
    this->m_alphaAnimation->setEndValue(255);

    this->m_aniGroup->start();
}

void PopularLeftWidget::animationDown() const {
    this->m_aniGroup->stop();

    this->m_posAnimation->setStartValue(this->height()/2-this->m_enterLivBtn->height()/2);
    this->m_posAnimation->setEndValue(this->height()/2-this->m_enterLivBtn->height()/2 + 35);

    this->m_alphaAnimation->setStartValue(255);
    this->m_alphaAnimation->setEndValue(0);

    this->m_aniGroup->start();
}

void PopularLeftWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 1. 先绘制背景遮罩（始终绘制）
    painter.setBrush(QColor(0, 0, 0, 160));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 10, 10);

    this->m_enterLivBtn->move(this->width()/2-this->m_enterLivBtn->width()/2,
        this->m_animatedY);
    // 设置按钮的透明度
    m_opacityEffect->setOpacity(m_alpha / 255.0);  // alpha 范围是0到255

    this->m_stopLab->move(20,this->height()-50);
    this->m_refreshLab->move(this->width()-100,this->height()-53);
    this->m_muteLab->move(this->width()-50,this->height()-50);
}

void PopularLeftWidget::enterEvent(QEnterEvent *event) {
    QWidget::enterEvent(event);
    //qDebug()<<"进入";
    this->m_enterLivBtn->show();
    this->m_stopLab->show();
    this->m_muteLab->show();
    this->m_refreshLab->show();
    animationUp();
}

void PopularLeftWidget::leaveEvent(QEvent *event) {
    QWidget::leaveEvent(event);
    this->m_enterLivBtn->hide();
    this->m_stopLab->hide();
    this->m_muteLab->hide();
    this->m_refreshLab->hide();
    //qDebug()<<"离开";
    animationDown();
}

