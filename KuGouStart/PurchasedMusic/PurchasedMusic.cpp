//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_PurchasedMusic.h" resolved

#include "PurchasedMusic.h"
#include "ui_PurchasedMusic.h"
#include "logger.hpp"

#include <QButtonGroup>
#include <QEasingCurve>
#include <QFile>
#include <QMouseEvent>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

PurchasedMusic::PurchasedMusic(QWidget *parent) :
    QWidget(parent)
    , ui(new Ui::PurchasedMusic)
    ,m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/purchased.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }
    initUi();

    //动画结束，恢复可交互
    connect(ui->stackedWidget,&SlidingStackedWidget::animationFinished,[this]{enableButton(true);});
    enableButton(true);
}

PurchasedMusic::~PurchasedMusic() {
    delete ui;
}

void PurchasedMusic::initUi() {
    initIndexLab();
    initStackedWidget();
    ui->paid_single_pushButton->clicked();

    ui->stackedWidget->setAnimation(QEasingCurve::Type::OutQuart);
    ui->stackedWidget->setSpeed(400);
}

void PurchasedMusic::initIndexLab() {
    //下标图片
    ui->idx1_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx2_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx3_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->paid_single_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    //同步状态
    ui->guide_widget1->installEventFilter(this);
    ui->guide_widget2->installEventFilter(this);
    ui->guide_widget3->installEventFilter(this);
}

void PurchasedMusic::initStackedWidget() {
    initPaidSingle();
    initPurchasedAlbums();
    initPurchasedVideos();
    this->m_buttonGroup->addButton(ui->paid_single_pushButton);
    this->m_buttonGroup->addButton(ui->purchased_albums_pushButton);
    this->m_buttonGroup->addButton(ui->purchased_video_pushButton);
    this->m_buttonGroup->setExclusive(true);
}

void PurchasedMusic::initPaidSingle() {
    this->m_paidSingle = std::make_unique<PaidSingle>(ui->stackedWidget);
    auto testWidget = new QWidget(this);
    testWidget->hide();
    ui->stackedWidget->addWidget(testWidget);
    ui->stackedWidget->setCurrentWidget(testWidget);
    ui->stackedWidget->addWidget(this->m_paidSingle.get());
    ui->stackedWidget->setCurrentWidget(this->m_paidSingle.get());
}

void PurchasedMusic::initPurchasedAlbums() {
    this->m_purchasedAlbums = std::make_unique<PurchasedAlbums>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_purchasedAlbums.get());
}

void PurchasedMusic::initPurchasedVideos() {
    this->m_purchasedVideos = std::make_unique<PurchasedVideos>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_purchasedVideos.get());
}

void PurchasedMusic::enableButton(const bool &flag) const {
    ui->paid_single_pushButton->setEnabled(flag);
    ui->purchased_albums_pushButton->setEnabled(flag);
    ui->purchased_video_pushButton->setEnabled(flag);
}

void PurchasedMusic::on_paid_single_pushButton_clicked() {
    // 判断当前显示的页面是否是 m_paidSingle 页面，如果是则直接返回
    if (ui->stackedWidget->currentWidget() == this->m_paidSingle.get()) {
        //qDebug() << "当前页面已经是 Single Song 页面，无需切换";
        return;  // 页面已是目标页面，无需切换
    }
    ui->paid_single_pushButton->setChecked(true);
    STREAM_INFO()<<"切换付费单曲界面";
    enableButton(false);
    //ui->stackedWidget->setCurrentWidget(this->m_paidSingle.get());
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_paidSingle.get()));
    ui->idx1_lab->show();
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->paid_single_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->purchased_albums_number_label->setStyleSheet("");
    ui->purchased_video_number_label->setStyleSheet("");
}

void PurchasedMusic::on_purchased_albums_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_purchasedAlbums.get()) {
        return;  // 页面已是目标页面，无需切换
    }
    ui->purchased_albums_pushButton->setChecked(true);
    STREAM_INFO()<<"切换已购专辑界面";
    enableButton(false);
    //ui->stackedWidget->setCurrentWidget(this->m_purchasedAlbums.get());
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_purchasedAlbums.get()));
    ui->idx1_lab->hide();
    ui->idx2_lab->show();
    ui->idx3_lab->hide();
    ui->paid_single_number_label->setStyleSheet("");
    ui->purchased_albums_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->purchased_video_number_label->setStyleSheet("");
}

void PurchasedMusic::on_purchased_video_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_purchasedVideos.get()) {
        return;  // 页面已是目标页面，无需切换
    }
    ui->purchased_video_pushButton->setChecked(true);
    STREAM_INFO()<<"切换已购视频界面";
    enableButton(false);
    //ui->stackedWidget->setCurrentWidget(this->m_purchasedVideos.get());
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_purchasedVideos.get()));
    ui->idx1_lab->hide();
    ui->idx2_lab->hide();
    ui->idx3_lab->show();
    ui->paid_single_number_label->setStyleSheet("");
    ui->purchased_albums_number_label->setStyleSheet("");
    ui->purchased_video_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
}

bool PurchasedMusic::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->guide_widget1) {
        if (event->type() == QEvent::Enter) {
            ui->paid_single_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:#26a1ff;
                    font-size:16px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QPushButton:checked {
                    color:#26a1ff;
                    font-size:18px;
                    font-weight:bold;
                }
            )");
            if (ui->paid_single_pushButton->isChecked())
                ui->paid_single_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->paid_single_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->paid_single_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:black;
                    font-size:16px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QPushButton:checked {
                    color:#26a1ff;
                    font-size:18px;
                    font-weight:bold;
                }
            )");
            if (ui->paid_single_pushButton->isChecked())
                ui->paid_single_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->paid_single_number_label->setStyleSheet("");
        }
    }
    if (watched == ui->guide_widget2) {
        if (event->type() == QEvent::Enter) {
            ui->purchased_albums_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:#26a1ff;
                    font-size:16px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QPushButton:checked {
                    color:#26a1ff;
                    font-size:18px;
                    font-weight:bold;
                }
            )");
            if (ui->purchased_albums_pushButton->isChecked())
                ui->purchased_albums_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->purchased_albums_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->purchased_albums_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:black;
                    font-size:16px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QPushButton:checked {
                    color:#26a1ff;
                    font-size:18px;
                    font-weight:bold;
                }
            )");
            if (ui->purchased_albums_pushButton->isChecked())
                ui->purchased_albums_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->purchased_albums_number_label->setStyleSheet("");
        }
    }
    if (watched == ui->guide_widget3) {
        if (event->type() == QEvent::Enter) {
            ui->purchased_video_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:#26a1ff;
                    font-size:16px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QPushButton:checked {
                    color:#26a1ff;
                    font-size:18px;
                    font-weight:bold;
                }
            )");
            if (ui->purchased_video_pushButton->isChecked())
                ui->purchased_video_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->purchased_video_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->purchased_video_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:black;
                    font-size:16px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QPushButton:checked {
                    color:#26a1ff;
                    font-size:18px;
                    font-weight:bold;
                }
            )");
            if (ui->purchased_video_pushButton->isChecked())
                ui->purchased_video_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->purchased_video_number_label->setStyleSheet("");
        }
    }
    return QWidget::eventFilter(watched, event);
}

void PurchasedMusic::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        const auto labelRect1 = ui->paid_single_number_label        ->geometry();
        const auto labelRect2 = ui->purchased_albums_number_label   ->geometry();
        const auto labelRect3 = ui->purchased_video_number_label    ->geometry();
        // 将点击坐标转换为标签父控件的坐标系
        const QPoint clickPos1 = ui->paid_single_number_label       ->parentWidget()->mapFrom(this, event->pos());
        const QPoint clickPos2 = ui->purchased_albums_number_label  ->parentWidget()->mapFrom(this, event->pos());
        const QPoint clickPos3 = ui->purchased_video_number_label   ->parentWidget()->mapFrom(this, event->pos());
        if (labelRect1.contains(clickPos1)) {
            //qDebug() << "Label clicked!";
            ui->paid_single_pushButton->clicked();
        }
        if (labelRect2.contains(clickPos2)) {
            ui->purchased_albums_pushButton->clicked();
        }
        if (labelRect3.contains(clickPos3)) {
            ui->purchased_video_pushButton->clicked();
        }
    }
    QWidget::mousePressEvent(event);
}
