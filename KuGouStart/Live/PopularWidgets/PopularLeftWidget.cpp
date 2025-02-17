//
// Created by WeiWang on 25-2-17.
//

#include "PopularLeftWidget.h"

#include <QFile>
#include <QToolButton>
#include <QMovie>
#include <QPropertyAnimation>
#include <QTimer>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

PopularLeftWidget::PopularLeftWidget(QWidget *parent)
    : QWidget(parent)
    , m_enterLivBtn(new QToolButton(this))
{
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
    // 位置动画
    m_posAnimation = new QPropertyAnimation(this, "animatedY");
    m_posAnimation->setDuration(300);
    m_posAnimation->setEasingCurve(QEasingCurve::OutQuad);
}

void PopularLeftWidget::setAnimatedY(int y) {
    m_animatedY = y;
    //qDebug()<<"当前动画高度: "<<m_animatedY;
    update();
    emit animatedYChanged(y);
}

void PopularLeftWidget::initUi() {
    //初始化进入直播间按钮
    this->m_enterLivBtn->setObjectName("enterLiveBtn");
    this->m_enterLivBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->m_enterLivBtn->setText("点击进入直播间");
    this->m_enterLivBtn->setFixedSize(210,60);
    this->m_enterLivBtn->setIconSize(QSize(60,40));
    const auto movie = new QMovie(GET_CURRENT_DIR + QStringLiteral("/music.gif"));

    // Start the movie to begin playing the GIF
    movie->start();

    // Setup a timer to update the button icon with the current frame of the movie
    const auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this,movie](){
        this->m_enterLivBtn->setIcon(QIcon(movie->currentPixmap()));
    });
    timer->start(150); // Adjust interval as necessary, here it's set to 30 FPS

}

void PopularLeftWidget::animationUp() const {
    this->m_posAnimation->stop();
    this->m_posAnimation->setStartValue(this->height()/2-this->m_enterLivBtn->height()/2 + 35);
    this->m_posAnimation->setEndValue(this->height()/2-this->m_enterLivBtn->height()/2);
    this->m_posAnimation->start();
}

void PopularLeftWidget::animationDown() const {
    this->m_posAnimation->stop();
    this->m_posAnimation->setStartValue(this->height()/2-this->m_enterLivBtn->height()/2);
    this->m_posAnimation->setEndValue(this->height()/2-this->m_enterLivBtn->height()/2 + 35);
    this->m_posAnimation->start();
}

void PopularLeftWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    this->m_enterLivBtn->move(this->width()/2-this->m_enterLivBtn->width()/2,
        this->m_animatedY);
}

void PopularLeftWidget::enterEvent(QEnterEvent *event) {
    QWidget::enterEvent(event);
    //qDebug()<<"进入";
    this->m_enterLivBtn->show();
    animationUp();
}

void PopularLeftWidget::leaveEvent(QEvent *event) {
    QWidget::leaveEvent(event);
    this->m_enterLivBtn->hide();
    //qDebug()<<"离开";
    animationDown();
}

