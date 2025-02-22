//
// Created by WeiWang on 24-11-23.
//
#include "MyBlockWidget.h"

#include <QToolButton>
#include <QLabel>
#include <QPainter>
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
    , m_durationBtn(new QToolButton(this))
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
    //默认隐藏时长按钮
    this->m_durationBtn->hide();
}

void MyBlockWidget::initUi() {
    this->setFixedSize(200,200);
    this->setCursor(Qt::PointingHandCursor);
    this->setMouseTracking(true);
    initTipArr();
    this->m_tipLab->setFixedHeight(20);
    this->m_tipLab->setScaledContents(true);
    this->m_tipLab->setContentsMargins(5,2,5,2);
    this->setTipLabText(m_tipArr[QRandomGenerator::global()->bounded(0, static_cast<int>(m_tipArr.size()))]);
    this->m_tipLab->setAlignment(Qt::AlignCenter);
    this->m_tipLab->setStyleSheet(QStringLiteral("border-radius:10px;background-color:black;color:white;"));
    this->m_tipLab->move(6, 6);
    //两个流行人数按钮都初始化
    this->m_rightPopularBtn->setEnabled(false);
    this->m_rightPopularBtn->setFixedSize(80, 20);
    this->m_rightPopularBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->m_rightPopularBtn->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/popular-white.svg")));
    this->setPopularBtnText(QString::number(QRandomGenerator::global()->generateDouble() * 1000, 'f', 1));
    this->m_rightPopularBtn->setContentsMargins(5, 0, 5, 0);
    this->m_rightPopularBtnStyle = "color:white;border:none;border-radius:10px;";
    this->m_rightPopularBtn->setStyleSheet(this->m_rightPopularBtnStyle+"background-color: rgba(128, 128, 128, 127);");

    this->m_leftPopularBtn->setEnabled(false);
    this->m_leftPopularBtn->setFixedSize(80, 20);
    this->m_leftPopularBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->m_leftPopularBtn->setIcon(QIcon(QStringLiteral(":/Res/tabIcon/person-white.svg")));
    this->m_leftPopularBtn->setContentsMargins(5, 0, 5, 0);
    this->m_leftPopularBtnStyle = "color:white;border:none;border-radius:10px;background-color: rgba(255,255,255,0);";
    this->m_leftPopularBtn->setStyleSheet(this->m_leftPopularBtnStyle);

    this->m_durationBtn->setEnabled(false);
    this->m_durationBtn->setFixedSize(50,20);
    this->m_leftPopularBtn->setContentsMargins(8, 0, 0, 0);
    this->m_durationBtn->setStyleSheet("color:white;border:none;background-color: rgba(255,255,255,0);");

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

void MyBlockWidget::setBorderImage(const QString &path,const int& border)const {
    QString style = QString("border-radius:%1px;border-image:url(%2);").arg(border).arg(path);
    //qDebug()<<"当前样式："<<style;
    this->m_bacWidget->setStyleSheet(style);
    if (border != 8) {
        this->m_mask->setBorderRadius(border);
    }
}

void MyBlockWidget::setTipLabText(const QString &text)const {
    this->m_tipLab->setText(text);
    this->m_tipLab->adjustSize();
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

void MyBlockWidget::setPopularBtnText(const QString &text)const {
    if(!this->m_popularDirection)return;

    if(this->m_popularDirection == 1) {
        if (this->m_haveUnit)
            this->m_leftPopularBtn->setText(QStringLiteral(" ") + text + QStringLiteral("万"));
        else
            this->m_leftPopularBtn->setText(QStringLiteral(" ") + text);
        if (this->m_leftPopularBtn->icon().isNull()) {
            this->m_leftPopularBtn->setFixedWidth(this->m_leftPopularBtn->fontMetrics().horizontalAdvance(m_leftPopularBtn->text()) +
            this->m_leftPopularBtn->contentsMargins().left() + this->m_leftPopularBtn->contentsMargins().right());
        }
        else {
            this->m_leftPopularBtn->setFixedWidth(this->m_leftPopularBtn->fontMetrics().horizontalAdvance(m_leftPopularBtn->text()) +
            this->m_leftPopularBtn->contentsMargins().left() + this->m_leftPopularBtn->contentsMargins().right() +
            this->m_leftPopularBtn->iconSize().width());
        }
    }
    else if(this->m_popularDirection == 2) {
        if (this->m_haveUnit)
            this->m_rightPopularBtn->setText(QStringLiteral(" ") + text + QStringLiteral("万"));
        else
            this->m_rightPopularBtn->setText(QStringLiteral(" ") + text);
        if (this->m_rightPopularBtn->icon().isNull()) {
            this->m_rightPopularBtn->setFixedWidth(this->m_rightPopularBtn->fontMetrics().horizontalAdvance(this->m_rightPopularBtn->text()) +
            this->m_rightPopularBtn->contentsMargins().left() + this->m_rightPopularBtn->contentsMargins().right());
        }
        else {
            this->m_rightPopularBtn->setFixedWidth(this->m_rightPopularBtn->fontMetrics().horizontalAdvance(this->m_rightPopularBtn->text()) +
            this->m_rightPopularBtn->contentsMargins().left() + this->m_rightPopularBtn->contentsMargins().right() +
            this->m_rightPopularBtn->iconSize().width());
        }
    }
}

void MyBlockWidget::setShowTip() const{
    this->m_tipLab->show();
}

void MyBlockWidget::setExpandRespond(const bool &expandRespond) {
    this->m_isExpandRespond = expandRespond;
}

void MyBlockWidget::setRightPopularBtnIcon(const QString &icon)const {
    if (icon.isEmpty()) {
        //qDebug()<<"icon为空";
        this->m_rightPopularBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
        this->m_rightPopularBtn->setFixedWidth(30);
        return;
    }
    this->m_rightPopularBtn->setIcon(QIcon(icon));
}

void MyBlockWidget::setLeftPopularBtnIcon(const QString &icon) {
    if (icon.isEmpty()) {
        this->m_leftPopularBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
        // 1. 移除固定大小限制      没有效果，不知道为什么
        this->m_leftPopularBtn->setMinimumSize(0, 0);
        this->m_leftPopularBtn->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        this->m_leftPopularBtn->setIconSize(QSize(0,0));
        return;
    }
    this->m_leftPopularBtn->setIcon(QIcon(icon));
}

void MyBlockWidget::setRightPopularBtnWidth(const int &width) const {
    this->m_rightPopularBtn->setFixedWidth(width);
}

void MyBlockWidget::setLeftPopularBtnWidth(const int &width) const {
    this->m_leftPopularBtn->setFixedWidth(width);
}

void MyBlockWidget::setRightPopularBtnFontSize(const int &size) const {
    auto font = this->m_leftPopularBtn->font();
    font.setPixelSize(size);
    this->m_leftPopularBtn->setFont(font);
}

void MyBlockWidget::setLeftPopularBtnFontSize(const int &size,const bool& isBold) const {
    auto font = this->m_leftPopularBtn->font();
    font.setPixelSize(size);
    font.setBold(isBold);
    this->m_leftPopularBtn->setFont(font);
}

void MyBlockWidget::setAspectRatio(const float &aspectRatio) {
    this->m_aspectRatio = aspectRatio;
}

void MyBlockWidget::setPopularBtnLeftPadding(const int &leftPadding) {
    if(!this->m_popularDirection)return;

    if(this->m_popularDirection == 1) {
        if (leftPadding == 0) {
            this->m_leftPopularBtnStyle += "text-align: left;";
            this->m_leftPopularBtn->setStyleSheet(this->m_leftPopularBtnStyle);
            return;
        }
        this->m_leftPopularBtnStyle += QString("padding-left:%1;").arg(leftPadding);
        this->m_leftPopularBtn->setStyleSheet(this->m_leftPopularBtnStyle);
    }
    else if(this->m_popularDirection == 2) {
        if (leftPadding == 0) {
            this->m_rightPopularBtnStyle += "text-align : left;";
            this->m_leftPopularBtn->setStyleSheet(this->m_leftPopularBtnStyle);
            return;
        }
        this->m_rightPopularBtnStyle += QString("padding-left:%1;").arg(leftPadding);
        this->m_rightPopularBtn->setStyleSheet(this->m_rightPopularBtnStyle+"background-color: rgba(128, 128, 128, 127);");
    }
}

void MyBlockWidget::setHaveNumberUnit(const bool &haveNumberUnit) {
    this->m_haveUnit = haveNumberUnit;
}

void MyBlockWidget::setTipArr(const std::vector<QString> &tipArr) {
    this->m_tipArr = tipArr;
}

void MyBlockWidget::setTipStyleSheet(const QString &style) const {
    this->m_tipLab->setStyleSheet(style);
}

void MyBlockWidget::setDurationBtnShow()const {
    this->m_durationBtn->show();
}

void MyBlockWidget::setDurationBtnText(const QString &text)const {
    this->m_durationBtn->setText(text);
}

SMaskWidget& MyBlockWidget::getMask()const {
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
            if (this->m_mask->getMove())
                this->m_mask->animationUp();
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
            if (this->m_mask->getMove())
                this->m_mask->animationDown();
            this->m_mask->hide();
            this->m_rightPopularBtn->setStyleSheet(this->m_rightPopularBtnStyle + "background-color: rgba(60,60,60, 127);");
            update();
        }
    }
}

void MyBlockWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    this->m_bacWidget->setFixedSize(static_cast<int>(event->size().width() / 1.01), static_cast<int>(event->size().width() / (1.01 * this->m_aspectRatio)));

    this->m_mask->setFixedSize(this->m_bacWidget->size());

    this->m_rightPopularBtn->move(this->m_bacWidget->width() - this->m_rightPopularBtn->width() - 5,
                             this->m_bacWidget->height() - this->m_rightPopularBtn->height() - 5);

    this->m_leftPopularBtn->move(5,this->m_bacWidget->height() - this->m_leftPopularBtn->height() - 5);

    this->m_durationBtn->move(this->m_bacWidget->width() - this->m_durationBtn->width() - 5,
                             this->m_bacWidget->height() - this->m_durationBtn->height() - 5);
}

void MyBlockWidget::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void MyBlockWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    event->ignore();
}

void MyBlockWidget::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}
