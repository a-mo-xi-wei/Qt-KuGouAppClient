//
// Created by WeiWang on 24-11-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_VideoWidget.h" resolved

#include "VideoWidget.h"
#include "ui_VideoWidget.h"

#include <QButtonGroup>
#include <QFile>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))


VideoWidget::VideoWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoWidget)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
    , m_upBtn(std::make_unique<UpToolButton>(this))
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/video.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
    initButtonGroup();
    initTotalWidget();
    initVector();
    initUi();
}

VideoWidget::~VideoWidget() {
    delete ui;
}

void VideoWidget::initButtonGroup() const {
    this->m_buttonGroup->addButton(ui->recommend_pushButton);
    this->m_buttonGroup->addButton(ui->video_rank_pushButton);
    this->m_buttonGroup->addButton(ui->MV_pushButton);
    this->m_buttonGroup->addButton(ui->site_pushButton);
    this->m_buttonGroup->addButton(ui->cover_pushButton);
    this->m_buttonGroup->addButton(ui->dance_pushButton);
    this->m_buttonGroup->addButton(ui->children_pushButton);
    this->m_buttonGroup->addButton(ui->live_pushButton);
    this->m_buttonGroup->addButton(ui->first_concert_pushButton);
    this->m_buttonGroup->addButton(ui->chinese_language_pushButton);
    this->m_buttonGroup->addButton(ui->South_Korea_pushButton);
    this->m_buttonGroup->addButton(ui->Japan_pushButton);
    this->m_buttonGroup->addButton(ui->American_pushButton);
    this->m_buttonGroup->setExclusive(true);
}

void VideoWidget::initTotalWidget() const {

}

void VideoWidget::initVector() {
}

void VideoWidget::handleWheelValue(const int &value) {
}

void VideoWidget::onUpBtnClicked() {
}

void VideoWidget::onUpBtnShowOrNot() {
}

void VideoWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
}

void VideoWidget::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
}

void VideoWidget::initUi() {
    //设置ToolButton
    ui->all_toolButton->setHoverFontColor(QColor(QStringLiteral("#3AA1FF")));
    ui->all_toolButton->setApproach(true);
    ui->all_toolButton->setIconSize(QSize(10,10));
    ui->all_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-gray.svg")));
    ui->all_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-blue.svg")));
    ui->all_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-gray.svg")));
    //设置title_widget2初始隐藏
    ui->title_widget2->hide();
}

void VideoWidget::on_all_toolButton_toggled() {
    if (ui->all_toolButton->isChecked()) {
        ui->all_toolButton->setText(QStringLiteral("收起"));
        ui->all_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg")));
        ui->all_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-blue.svg")));
        ui->all_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-gray.svg")));
        ui->title_widget2->show();
        //update();
    } else {
        ui->all_toolButton->setText(QStringLiteral("全部"));
        ui->all_toolButton->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-blue.svg")));
        ui->all_toolButton->setEnterIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-blue.svg")));
        ui->all_toolButton->setLeaveIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/up-gray.svg")));
        ui->title_widget2->hide();
        //update();
    }

}
