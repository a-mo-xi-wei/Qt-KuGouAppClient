//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MusicCloudDisk.h" resolved

#include "MusicCloudDisk.h"
#include "ui_MusicCloudDisk.h"
#include "logger.hpp"

#include <QButtonGroup>
#include <QFile>
#include <QMouseEvent>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

MusicCloudDisk::MusicCloudDisk(QWidget *parent)
    : QWidget(parent), ui(new Ui::MusicCloudDisk)
    ,m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/cloud.css"));
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

MusicCloudDisk::~MusicCloudDisk() {
    delete ui;
}

void MusicCloudDisk::initUi() {
    //初始化下标标签
    initIndexLab();
    //初始化新增歌曲到曲库按钮
    ui->new_add_toolButton->setIconSize(QSize(10,10));
    ui->new_add_toolButton->setIcon(QIcon(QStringLiteral(":Res/menuIcon/right-black.svg")));
    ui->new_add_toolButton->setEnterIcon(QIcon(QStringLiteral(":Res/menuIcon/right-blue.svg")));
    ui->new_add_toolButton->setLeaveIcon(QIcon(QStringLiteral(":Res/menuIcon/right-black.svg")));
    ui->new_add_toolButton->setApproach(true);
    ui->new_add_toolButton->setHoverFontColor(QColor(QStringLiteral("#3AA1FF")));
    //初始化stackedWidget
    initStackedWidget();
    ui->uploaded_song_pushButton->clicked();

}

void MusicCloudDisk::initIndexLab() {
    //下标图片
    ui->idx1_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx2_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    //默认选中uploaded_song_number_label
    ui->uploaded_song_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:14px;font-weight:bold;"));
    ui->idx2_lab->hide();
    //同步状态
    ui->guide_widget1->installEventFilter(this);
    ui->guide_widget2->installEventFilter(this);
}

void MusicCloudDisk::initStackedWidget() {
    initUploadedSong();
    initUploadingSong();
    this->m_buttonGroup->addButton(ui->uploaded_song_pushButton);
    this->m_buttonGroup->addButton(ui->uploading_song_pushButton);
    this->m_buttonGroup->setExclusive(true);
}

void MusicCloudDisk::initUploadedSong() {
    this->m_uploadedSong = std::make_unique<UploadedSong>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_uploadedSong.get());
    ui->stackedWidget->setCurrentWidget(this->m_uploadedSong.get());
}

void MusicCloudDisk::initUploadingSong() {
    this->m_uploadingSong = std::make_unique<UploadingSong>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_uploadingSong.get());
}

void MusicCloudDisk::on_uploaded_song_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_uploadedSong.get()) {
        return;  // 页面已是目标页面，无需切换
    }
    ui->uploaded_song_pushButton->setChecked(true);
    STREAM_INFO()<<"切换已上传歌曲界面";
    ui->stackedWidget->setCurrentWidget(this->m_uploadedSong.get());
    ui->idx1_lab->show();
    ui->idx2_lab->hide();
    ui->uploaded_song_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:14px;font-weight:bold;"));
    ui->uploading_song_number_label->setStyleSheet("");
}

void MusicCloudDisk::on_uploading_song_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_uploadingSong.get()) {
        return;  // 页面已是目标页面，无需切换
    }
    ui->uploading_song_pushButton->setChecked(true);
    STREAM_INFO()<<"切换正在上传界面";
    ui->stackedWidget->setCurrentWidget(this->m_uploadingSong.get());
    ui->idx1_lab->hide();
    ui->idx2_lab->show();
    ui->uploaded_song_number_label->setStyleSheet("");
    ui->uploading_song_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:14px;font-weight:bold;"));
}

bool MusicCloudDisk::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->guide_widget1) {
        if (event->type() == QEvent::Enter) {
            ui->uploaded_song_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:#26a1ff;
                    font-size:15px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QPushButton:checked {
                    color:#26a1ff;
                    font-size:16px;
                    font-weight:bold;
                }
            )");
            if (ui->uploaded_song_pushButton->isChecked())
                ui->uploaded_song_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:14px;font-weight:bold;"));
            else ui->uploaded_song_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->uploaded_song_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:black;
                    font-size:15px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QWidget#guide_widget QPushButton:checked {
                    color:#26a1ff;
                    font-size:16px;
                    font-weight:bold;
                }
            )");
            if (ui->uploaded_song_pushButton->isChecked())
                ui->uploaded_song_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:14px;font-weight:bold;"));
            else ui->uploaded_song_number_label->setStyleSheet("");
        }
    }
    if (watched == ui->guide_widget2) {
        if (event->type() == QEvent::Enter) {
            ui->uploading_song_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:#26a1ff;
                    font-size:15px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QPushButton:checked {
                    color:#26a1ff;
                    font-size:16px;
                    font-weight:bold;
                }
            )");
            if (ui->uploading_song_pushButton->isChecked())
                ui->uploading_song_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:14px;font-weight:bold;"));
            else ui->uploading_song_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->uploading_song_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:black;
                    font-size:15px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QWidget#guide_widget QPushButton:checked {
                    color:#26a1ff;
                    font-size:16px;
                    font-weight:bold;
                }
            )");
            if (ui->uploading_song_pushButton->isChecked())
                ui->uploading_song_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:14px;font-weight:bold;"));
            else ui->uploading_song_number_label->setStyleSheet("");
        }
    }
    return QWidget::eventFilter(watched, event);
}

void MusicCloudDisk::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        // 获取 uploaded_song_number_label 的矩形区域
        const auto labelRect1 = ui->uploaded_song_number_label  ->geometry();
        const auto labelRect2 = ui->uploading_song_number_label ->geometry();
        // 将点击坐标转换为标签父控件的坐标系
        const QPoint clickPos1 = ui->uploaded_song_number_label     ->parentWidget()->mapFrom(this, event->pos());
        const QPoint clickPos2 = ui->uploading_song_number_label    ->parentWidget()->mapFrom(this, event->pos());
        if (labelRect1.contains(clickPos1)) {
            //qDebug() << "Label clicked!";
            ui->uploaded_song_pushButton->clicked();
        }
        if (labelRect2.contains(clickPos2)) {
            ui->uploading_song_pushButton->clicked();
        }
    }
}

