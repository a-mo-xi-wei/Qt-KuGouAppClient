//
// Created by WeiWang on 24-11-23.
//
#include "MyBlockWidget.h"

#include<QToolButton>
#include<QLabel>
#include <QRandomGenerator>
#include <QResizeEvent>
#include <QStyleOption>

MyBlockWidget::MyBlockWidget(QWidget *parent) :
    QWidget(parent)
    , m_bacWidget(new QWidget(this))
    , m_mask(std::make_unique<SMaskWidget>( this))
    , m_tipLab(new QLabel(this))
    , m_rightPopularBtn(new QToolButton(this))
    , m_leftPopularBtn(new QToolButton(this))
{
    initUi();
    this->m_mask->setParent(this->m_bacWidget);
    this->m_mask->move(this->m_bacWidget->pos());
    this->m_mask->setFixedSize(this->m_bacWidget->size());
    this->m_mask->hide();
    //先隐藏流行人数按钮
    this->m_leftPopularBtn->hide();
    this->m_rightPopularBtn->hide();
    //默认隐藏tipLab
    this->m_tipLab->hide();
}

void MyBlockWidget::initUi() {
    this->setFixedSize(200,200);
    this->setCursor(Qt::PointingHandCursor);
    initTipArr();
    this->m_tipLab->setFixedSize(50, 20);
    this->setTipLabText(m_tipArr[QRandomGenerator::global()->bounded(0, static_cast<int>(m_tipArr.size()))]);
    this->m_tipLab->setAlignment(Qt::AlignCenter);
    this->m_tipLab->setStyleSheet(QStringLiteral("border-radius:10px;background-color:black;color:white;"));
    this->m_tipLab->move(6, 6);
    //两个流行人数按钮都初始化
    this->m_rightPopularBtn->setFixedSize(70, 20);
    this->m_rightPopularBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->m_rightPopularBtn->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/popular-white.svg")));
    this->setPopularBtnText(QString::number(QRandomGenerator::global()->generateDouble() * 1000, 'f', 1));
    this->m_rightPopularBtn->setContentsMargins(5, 0, 5, 0);
    this->m_rightPopularBtnStyle = "color:white;border:none;border-radius:10px;";
    this->m_rightPopularBtn->setStyleSheet(this->m_rightPopularBtnStyle+"background-color: rgba(128, 128, 128, 127);");

    this->m_leftPopularBtn->setFixedSize(70, 20);
    this->m_leftPopularBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->m_leftPopularBtn->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/person-white.svg")));
    this->m_leftPopularBtn->setContentsMargins(5, 0, 5, 0);
    this->m_leftPopularBtnStyle = "color:white;border:none;border-radius:10px;background-color: rgba(255,255,255,0);";
    this->m_leftPopularBtn->setStyleSheet(this->m_leftPopularBtnStyle);

}

void MyBlockWidget::initTipArr() {
    m_tipArr = {QStringLiteral("流行"),QStringLiteral("经典"),
                QStringLiteral("轻音乐"),QStringLiteral("影视"),
                QStringLiteral("爵士"),QStringLiteral("轻松"),
                QStringLiteral("日语"),QStringLiteral("中国风"),
                QStringLiteral("英语"),QStringLiteral("电子"),
                QStringLiteral("80后"),QStringLiteral("90后"),
                QStringLiteral("70后"),QStringLiteral("励志"),
                QStringLiteral("乐器演奏"),QStringLiteral("国语"),
                QStringLiteral("民谣"),QStringLiteral("校园"),
                QStringLiteral("安静"),QStringLiteral("寂寞"),
                QStringLiteral("网络"),QStringLiteral("法语"),
                QStringLiteral("ACG"),QStringLiteral("兴奋"),
                QStringLiteral("快乐"),QStringLiteral("金属"),
                QStringLiteral("说唱"),QStringLiteral("DJ热碟"),
                QStringLiteral("甜蜜"),QStringLiteral("广场舞"),};
}

void MyBlockWidget::setBorderImage(const QString &path,const int& border) {
    QString style = QString("border-radius:%1px;border-image:url(%2);").arg(border).arg(path);
    //qDebug()<<"当前样式："<<style;
    this->m_bacWidget->setStyleSheet(style);
    if (border != 8) {
        this->m_mask->setBorderRadius(border);
    }
}

void MyBlockWidget::setTipLabText(const QString &text) {
    this->m_tipLab->setText(text);
}

void MyBlockWidget::setPopularDirection(const int &direction) {
    this->m_popularDirection = direction;
    if(direction == 1) {
        this->m_leftPopularBtn->show();
    }
    else if(direction == 2) {
        this->m_rightPopularBtn->show();
    }
    else {
        this->m_leftPopularBtn->hide();
        this->m_rightPopularBtn->hide();
    }
}

void MyBlockWidget::setPopularBtnText(const QString &text) {
    if(!this->m_popularDirection)return;

    if(this->m_popularDirection == 1) {
        this->m_leftPopularBtn->setText(QStringLiteral(" ") + text + QStringLiteral("万"));
    }
    else if(this->m_popularDirection == 2) {
        this->m_rightPopularBtn->setText(QStringLiteral(" ") + text + QStringLiteral("万"));
    }
}

void MyBlockWidget::setShowTip(const bool &show) {
    if(show)this->m_tipLab->show();
}

void MyBlockWidget::setExpandRespond(const bool &expandRespond) {
    this->m_isExpandRespond = expandRespond;
}

void MyBlockWidget::setRightPopularBtnIcon(const QString &icon) {
    this->m_rightPopularBtn->setIcon(QIcon(icon));
}

void MyBlockWidget::setLeftPopularBtnIcon(const QString &icon) {
    this->m_leftPopularBtn->setIcon(QIcon(icon));
}

void MyBlockWidget::setAspectRatio(const int &aspectRatio) {
    this->m_aspectRatio = aspectRatio;
}

void MyBlockWidget::setPopularBtnLeftPadding(const int &leftPadding) {
    if(!this->m_popularDirection)return;

    if(this->m_popularDirection == 1) {
        this->m_leftPopularBtnStyle += QString("padding-left:%1;").arg(leftPadding);
        this->m_leftPopularBtn->setStyleSheet(this->m_leftPopularBtnStyle);
    }
    else if(this->m_popularDirection == 2) {
        this->m_rightPopularBtnStyle += QString("padding-left:%1;").arg(leftPadding);
        this->m_rightPopularBtn->setStyleSheet(this->m_rightPopularBtnStyle+"background-color: rgba(128, 128, 128, 127);");
    }
}

SMaskWidget& MyBlockWidget::getMask() {
    return *m_mask;
}

void MyBlockWidget::onShowMask() {
    this->m_mask->show();
    this->m_mask->raise();
    QEvent enter(QEvent::Enter);
    this->leaveEvent(&enter);
    update();
}

void MyBlockWidget::onHideMask() {
    this->m_mask->hide();
    QEvent leave(QEvent::Leave);
    this->leaveEvent(&leave);
    update();
}

void MyBlockWidget::paintEvent(QPaintEvent *ev) {
    QWidget::paintEvent(ev);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void MyBlockWidget::enterEvent(QEnterEvent *ev) {
    QWidget::enterEvent(ev);
    if(!this->m_isExpandRespond) {
        if(!this->m_isHoverCover) {//如果之前是离开
            this->m_isHoverCover = true;
            this->m_mask->show();
            this->m_mask->raise();
            this->m_rightPopularBtn->setStyleSheet(this->m_rightPopularBtnStyle + "background-color: rgba(60,60,60, 127);");
            update();
        }
    }

}

void MyBlockWidget::leaveEvent(QEvent *ev) {
    QWidget::leaveEvent(ev);
    if(!this->m_isExpandRespond) {
        if(this->m_isHoverCover) {
            this->m_isHoverCover = false;
            this->m_mask->hide();
            this->m_rightPopularBtn->setStyleSheet(this->m_rightPopularBtnStyle + "background-color: rgba(60,60,60, 127);");
            update();
        }
    }
}

void MyBlockWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    this->m_bacWidget->setFixedSize(event->size().width() / 1.01, event->size().width() / (1.01 * this->m_aspectRatio));
    this->m_mask->setFixedSize(this->m_bacWidget->size());
    this->m_rightPopularBtn->move(this->m_bacWidget->width() - this->m_rightPopularBtn->width() - 5,
                             this->m_bacWidget->height() - this->m_rightPopularBtn->height() - 5);
    this->m_leftPopularBtn->move(5,this->m_bacWidget->height() - this->m_rightPopularBtn->height() - 5);
}

void MyBlockWidget::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void MyBlockWidget::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}
