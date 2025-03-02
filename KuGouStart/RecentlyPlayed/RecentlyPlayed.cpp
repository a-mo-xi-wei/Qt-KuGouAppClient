//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RecentlyPlayed.h" resolved

#include "RecentlyPlayed.h"
#include "ui_RecentlyPlayed.h"
#include "logger.hpp"

#include <QButtonGroup>
#include <QFile>
#include <QMouseEvent>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

RecentlyPlayed::RecentlyPlayed(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RecentlyPlayed)
    , m_buttonGroup((std::make_unique<QButtonGroup>(this)))
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/recently.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }
    initUi();
}

RecentlyPlayed::~RecentlyPlayed() {
    delete ui;
}

void RecentlyPlayed::initUi() {
    initStackedWidget();
    initIndexLab();
    ui->single_song_pushButton->clicked();
}

void RecentlyPlayed::initIndexLab() {
    //下标图片
    ui->idx1_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx2_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx3_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx4_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx5_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->single_song_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->idx5_lab->hide();
    //同步状态
    ui->guide_widget1->installEventFilter(this);
    ui->guide_widget2->installEventFilter(this);
    ui->guide_widget3->installEventFilter(this);
    ui->guide_widget4->installEventFilter(this);
    ui->guide_widget5->installEventFilter(this);
}

void RecentlyPlayed::initStackedWidget() {
    initSingleSong();
    initSongList();
    initVideoWidget();
    initSongChannel();
    initMVChannel();
    this->m_buttonGroup->addButton(ui->single_song_pushButton);
    this->m_buttonGroup->addButton(ui->song_list_pushButton);
    this->m_buttonGroup->addButton(ui->video_pushButton);
    this->m_buttonGroup->addButton(ui->song_channel_pushButton);
    this->m_buttonGroup->addButton(ui->MV_channel_pushButton);
    this->m_buttonGroup->setExclusive(true);
}

void RecentlyPlayed::initSingleSong() {
    this->m_singleSong = std::make_unique<RecentlySingleSong>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_singleSong.get());
    ui->stackedWidget->setCurrentWidget(this->m_singleSong.get());
}

void RecentlyPlayed::initSongList() {
    this->m_songList = std::make_unique<RecentlySongList>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_songList.get());
}

void RecentlyPlayed::initVideoWidget() {
    this->m_videoWidget = std::make_unique<RecentlyVideoWidget>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_videoWidget.get());
}

void RecentlyPlayed::initSongChannel() {
    this->m_songChannel = std::make_unique<RecentlySongChannel>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_songChannel.get());
}

void RecentlyPlayed::initMVChannel() {
    this->m_mvChannel = std::make_unique<RecentlyMVChannel>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_mvChannel.get());
}

void RecentlyPlayed::on_single_song_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_singleSong.get())return;
    ui->single_song_pushButton->setChecked(true);
    STREAM_INFO()<<"切换单曲界面";
    ui->stackedWidget->setCurrentWidget(this->m_singleSong.get());
    ui->idx1_lab->show();
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->idx5_lab->hide();
    ui->single_song_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->song_list_number_label->setStyleSheet("");
    ui->video_number_label->setStyleSheet("");
    ui->song_channel_number_label->setStyleSheet("");
    ui->MV_channel_number_label->setStyleSheet("");
    //显示check_box_widget
    ui->check_box_widget->show();
}

void RecentlyPlayed::on_song_list_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_songList.get())return;
    ui->song_list_pushButton->setChecked(true);
    STREAM_INFO()<<"切换歌单界面";
    ui->stackedWidget->setCurrentWidget(this->m_songList.get());
    ui->idx1_lab->hide();
    ui->idx2_lab->show();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->idx5_lab->hide();
    ui->single_song_number_label->setStyleSheet("");
    ui->song_list_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->video_number_label->setStyleSheet("");
    ui->song_channel_number_label->setStyleSheet("");
    ui->MV_channel_number_label->setStyleSheet("");
    //隐藏check_box_widget
    ui->check_box_widget->hide();
}

void RecentlyPlayed::on_video_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_videoWidget.get())return;
    ui->video_pushButton->setChecked(true);
    STREAM_INFO()<<"切换视频界面";
    ui->stackedWidget->setCurrentWidget(this->m_videoWidget.get());
    ui->idx1_lab->hide();
    ui->idx2_lab->hide();
    ui->idx3_lab->show();
    ui->idx4_lab->hide();
    ui->idx5_lab->hide();
    ui->single_song_number_label->setStyleSheet("");
    ui->song_list_number_label->setStyleSheet("");
    ui->video_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->song_channel_number_label->setStyleSheet("");
    ui->MV_channel_number_label->setStyleSheet("");
    //隐藏check_box_widget
    ui->check_box_widget->hide();
}

void RecentlyPlayed::on_song_channel_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_songChannel.get())return;
    ui->song_channel_pushButton->setChecked(true);
    STREAM_INFO()<<"切换音乐频道界面";
    ui->stackedWidget->setCurrentWidget(this->m_songChannel.get());
    ui->idx1_lab->hide();
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->show();
    ui->idx5_lab->hide();
    ui->single_song_number_label->setStyleSheet("");
    ui->song_list_number_label->setStyleSheet("");
    ui->video_number_label->setStyleSheet("");
    ui->song_channel_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->MV_channel_number_label->setStyleSheet("");
    //隐藏check_box_widget
    ui->check_box_widget->hide();
}

void RecentlyPlayed::on_MV_channel_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_mvChannel.get())return;
    ui->MV_channel_pushButton->setChecked(true);
    STREAM_INFO()<<"切换MV频道界面";
    ui->stackedWidget->setCurrentWidget(this->m_mvChannel.get());
    ui->idx1_lab->hide();
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->idx5_lab->show();
    ui->single_song_number_label->setStyleSheet("");
    ui->song_list_number_label->setStyleSheet("");
    ui->video_number_label->setStyleSheet("");
    ui->song_channel_number_label->setStyleSheet("");
    ui->MV_channel_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    //隐藏check_box_widget
    ui->check_box_widget->hide();
}

bool RecentlyPlayed::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->guide_widget1) {
        if (event->type() == QEvent::Enter) {
            ui->single_song_pushButton->setStyleSheet(R"(
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
            if (ui->single_song_pushButton->isChecked())
                ui->single_song_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->single_song_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->single_song_pushButton->setStyleSheet(R"(
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
            if (ui->single_song_pushButton->isChecked())
                ui->single_song_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->single_song_number_label->setStyleSheet("");
        }
    }
    if (watched == ui->guide_widget2) {
        if (event->type() == QEvent::Enter) {
            ui->song_list_pushButton->setStyleSheet(R"(
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
            if (ui->song_list_pushButton->isChecked())
                ui->song_list_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->song_list_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->song_list_pushButton->setStyleSheet(R"(
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
            if (ui->song_list_pushButton->isChecked())
                ui->song_list_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->song_list_number_label->setStyleSheet("");
        }
    }
    if (watched == ui->guide_widget3) {
        if (event->type() == QEvent::Enter) {
            ui->video_pushButton->setStyleSheet(R"(
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
            if (ui->video_pushButton->isChecked())
                ui->video_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->video_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->video_pushButton->setStyleSheet(R"(
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
            if (ui->video_pushButton->isChecked())
                ui->video_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->video_number_label->setStyleSheet("");
        }
    }
    if (watched == ui->guide_widget4) {
        if (event->type() == QEvent::Enter) {
            ui->song_channel_pushButton->setStyleSheet(R"(
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
            if (ui->song_channel_pushButton->isChecked())
                ui->song_channel_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->song_channel_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->song_channel_pushButton->setStyleSheet(R"(
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
            if (ui->song_channel_pushButton->isChecked())
                ui->song_channel_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->song_channel_number_label->setStyleSheet("");
        }
    }
    if (watched == ui->guide_widget5) {
        if (event->type() == QEvent::Enter) {
            ui->MV_channel_pushButton->setStyleSheet(R"(
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
            if (ui->MV_channel_pushButton->isChecked())
                ui->MV_channel_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->MV_channel_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->MV_channel_pushButton->setStyleSheet(R"(
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
            if (ui->MV_channel_pushButton->isChecked())
                ui->MV_channel_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->MV_channel_number_label->setStyleSheet("");
        }
    }
    return QWidget::eventFilter(watched, event);
}

void RecentlyPlayed::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
        if (event->button() == Qt::LeftButton) {
        // 获取 singleSong_number_label 的矩形区域
        const auto labelRect1 = ui->single_song_number_label    ->geometry();
        const auto labelRect2 = ui->song_list_number_label      ->geometry();
        const auto labelRect3 = ui->video_number_label          ->geometry();
        const auto labelRect4 = ui->song_channel_number_label   ->geometry();
        const auto labelRect5 = ui->MV_channel_number_label    ->geometry();
        // 将点击坐标转换为标签父控件的坐标系
        const QPoint clickPos1 = ui->single_song_number_label   ->parentWidget()->mapFrom(this, event->pos());
        const QPoint clickPos2 = ui->song_list_number_label     ->parentWidget()->mapFrom(this, event->pos());
        const QPoint clickPos3 = ui->video_number_label         ->parentWidget()->mapFrom(this, event->pos());
        const QPoint clickPos4 = ui->song_channel_number_label  ->parentWidget()->mapFrom(this, event->pos());
        const QPoint clickPos5 = ui->MV_channel_number_label   ->parentWidget()->mapFrom(this, event->pos());

        if (labelRect1.contains(clickPos1)) {
            //qDebug() << "Label clicked!";
            ui->single_song_pushButton->clicked();
        }
        if (labelRect2.contains(clickPos2)) {
            ui->song_list_pushButton->clicked();
        }
        if (labelRect3.contains(clickPos3)) {
            ui->video_pushButton->clicked();
        }
        if (labelRect4.contains(clickPos4)) {
            ui->song_channel_pushButton->clicked();
        }
        if (labelRect5.contains(clickPos5)) {
            ui->MV_channel_pushButton->clicked();
        }
    }

}

void RecentlyPlayed::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    // 创建一个悬停事件
    //QHoverEvent hoverEvent(QEvent::HoverEnter, QPoint(10, 10), QPoint(-1, -1));
    // 立即发送事件到目标控件
    //QApplication::sendEvent(ui->single_song_pushButton, &hoverEvent);
}
