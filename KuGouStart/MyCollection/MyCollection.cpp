//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MyCollection.h" resolved

#include "MyCollection.h"
#include "ui_MyCollection.h"
#include "logger.hpp"

#include <QButtonGroup>
#include <QFile>
#include <QMouseEvent>

#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

MyCollection::MyCollection(QWidget *parent) :
    QWidget(parent), ui(new Ui::MyCollection)
    ,m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/collection.css"));
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

MyCollection::~MyCollection() {
    delete ui;
}

void MyCollection::initStackedWidget() {
    initSingleSong();
    initSongList();
    initSpecialAlbum();
    initCollectVideo();
    initSinger();
    initDevice();
    //设置互斥
    this->m_buttonGroup->addButton(ui->singleSong_pushButton);
    this->m_buttonGroup->addButton(ui->songList_pushButton);
    this->m_buttonGroup->addButton(ui->specialAlbum_pushButton);
    this->m_buttonGroup->addButton(ui->collectVideo_pushButton);
    this->m_buttonGroup->addButton(ui->singer_pushButton);
    this->m_buttonGroup->addButton(ui->device_pushButton);
    this->m_buttonGroup->setExclusive(true);
}

void MyCollection::initUi() {
    initStackedWidget();
    initIndexLab();
    ui->singleSong_pushButton->clicked();

    ui->stackedWidget->setAnimation(QEasingCurve::Type::OutQuart);
    ui->stackedWidget->setSpeed(400);
    ui->stackedWidget->setContentsMargins(0,0,0,0);
}

void MyCollection::initIndexLab() {
    //下标图片
    ui->idx1_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx2_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx3_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx4_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx5_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx6_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->singleSong_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->idx5_lab->hide();
    ui->idx6_lab->hide();
    //同步状态
    ui->guide_widget1->installEventFilter(this);
    ui->guide_widget2->installEventFilter(this);
    ui->guide_widget3->installEventFilter(this);
    ui->guide_widget4->installEventFilter(this);
    ui->guide_widget5->installEventFilter(this);
    ui->guide_widget6->installEventFilter(this);
}

void MyCollection::initSingleSong() {
    this->m_singleSong = std::make_unique<SingleSong>(ui->stackedWidget);
    connect(this->m_singleSong.get(),&SingleSong::find_more_music,[this]{emit find_more_music();});
    ui->stackedWidget->addWidget(this->m_singleSong.get());
    ui->stackedWidget->setCurrentWidget(this->m_singleSong.get());
}

void MyCollection::initSongList() {
    this->m_songList = std::make_unique<SongListWidget>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_songList.get());
}

void MyCollection::initSpecialAlbum() {
    this->m_specialAlbum = std::make_unique<SpecialAlbum>(ui->stackedWidget);
    connect(this->m_specialAlbum.get(),&SpecialAlbum::find_more_music,[this]{emit find_more_music();});
    ui->stackedWidget->addWidget(this->m_specialAlbum.get());
}

void MyCollection::initCollectVideo() {
    this->m_collectVideo = std::make_unique<CollectVideo>(ui->stackedWidget);
    connect(this->m_collectVideo.get(),&CollectVideo::find_more_music,[this]{emit find_more_music();});
    ui->stackedWidget->addWidget(this->m_collectVideo.get());
}

void MyCollection::initSinger() {
    this->m_singerWidget = std::make_unique<SingerWidget>(ui->stackedWidget);
    connect(this->m_singerWidget.get(),&SingerWidget::find_more_music,[this]{emit find_more_music();});
    ui->stackedWidget->addWidget(this->m_singerWidget.get());
}

void MyCollection::initDevice() {
    this->m_deviceWidget = std::make_unique<DeviceWidget>(ui->stackedWidget);
    connect(this->m_deviceWidget.get(),&DeviceWidget::find_more_music,[this]{emit find_more_music();});
    ui->stackedWidget->addWidget(this->m_deviceWidget.get());
}

void MyCollection::enableButton(const bool &flag) const {
    ui->singleSong_pushButton->setEnabled(flag);
    ui->songList_pushButton->setEnabled(flag);
    ui->specialAlbum_pushButton->setEnabled(flag);
    ui->collectVideo_pushButton->setEnabled(flag);
    ui->singer_pushButton->setEnabled(flag);
    ui->device_pushButton->setEnabled(flag);
}

bool MyCollection::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->guide_widget1) {
        if (event->type() == QEvent::Enter) {
            ui->singleSong_pushButton->setStyleSheet(R"(
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
            if (ui->singleSong_pushButton->isChecked())
                ui->singleSong_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->singleSong_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->singleSong_pushButton->setStyleSheet(R"(
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
            if (ui->singleSong_pushButton->isChecked())
                ui->singleSong_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->singleSong_number_label->setStyleSheet("");
        }
    }
    if (watched == ui->guide_widget2) {
        if (event->type() == QEvent::Enter) {
            ui->songList_pushButton->setStyleSheet(R"(
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
            if (ui->songList_pushButton->isChecked())
                ui->songList_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->songList_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->songList_pushButton->setStyleSheet(R"(
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
            if (ui->songList_pushButton->isChecked())
                ui->songList_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->songList_number_label->setStyleSheet("");
        }
    }
    if (watched == ui->guide_widget3) {
        if (event->type() == QEvent::Enter) {
            ui->specialAlbum_pushButton->setStyleSheet(R"(
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
            if (ui->specialAlbum_pushButton->isChecked())
                ui->specialAlbum_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->specialAlbum_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->specialAlbum_pushButton->setStyleSheet(R"(
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
            if (ui->specialAlbum_pushButton->isChecked())
                ui->specialAlbum_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->specialAlbum_number_label->setStyleSheet("");
        }
    }
    if (watched == ui->guide_widget4) {
        if (event->type() == QEvent::Enter) {
            ui->collectVideo_pushButton->setStyleSheet(R"(
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
            if (ui->collectVideo_pushButton->isChecked())
                ui->collectVideo_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->collectVideo_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->collectVideo_pushButton->setStyleSheet(R"(
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
            if (ui->collectVideo_pushButton->isChecked())
                ui->collectVideo_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->collectVideo_number_label->setStyleSheet("");
        }
    }
    if (watched == ui->guide_widget5) {
        if (event->type() == QEvent::Enter) {
            ui->singer_pushButton->setStyleSheet(R"(
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
            if (ui->singer_pushButton->isChecked())
                ui->singer_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->singer_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->singer_pushButton->setStyleSheet(R"(
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
            if (ui->singer_pushButton->isChecked())
                ui->singer_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->singer_number_label->setStyleSheet("");
        }
    }
    if (watched == ui->guide_widget6) {
        if (event->type() == QEvent::Enter) {
            ui->device_pushButton->setStyleSheet(R"(
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
            if (ui->device_pushButton->isChecked())
                ui->device_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->device_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;"));
        }
        else if (event->type() == QEvent::Leave) {
            ui->device_pushButton->setStyleSheet(R"(
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
            if (ui->device_pushButton->isChecked())
                ui->device_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
            else ui->device_number_label->setStyleSheet("");
        }
    }
    return QWidget::eventFilter(watched, event);
}

void MyCollection::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // 获取 singleSong_number_label 的矩形区域
        const auto labelRect1 = ui->singleSong_number_label     ->geometry();
        const auto labelRect2 = ui->songList_number_label       ->geometry();
        const auto labelRect3 = ui->specialAlbum_number_label   ->geometry();
        const auto labelRect4 = ui->collectVideo_number_label   ->geometry();
        const auto labelRect5 = ui->singer_number_label         ->geometry();
        const auto labelRect6 = ui->device_number_label         ->geometry();
        // 将点击坐标转换为标签父控件的坐标系
        const QPoint clickPos1 = ui->singleSong_number_label    ->parentWidget()->mapFrom(this, event->pos());
        const QPoint clickPos2 = ui->songList_number_label      ->parentWidget()->mapFrom(this, event->pos());
        const QPoint clickPos3 = ui->specialAlbum_number_label  ->parentWidget()->mapFrom(this, event->pos());
        const QPoint clickPos4 = ui->collectVideo_number_label  ->parentWidget()->mapFrom(this, event->pos());
        const QPoint clickPos5 = ui->singer_number_label        ->parentWidget()->mapFrom(this, event->pos());
        const QPoint clickPos6 = ui->device_number_label        ->parentWidget()->mapFrom(this, event->pos());

        //qDebug() << "Label Rect:" << labelRect;
        //qDebug() << "Click Pos (Converted):" << clickPos;

        if (labelRect1.contains(clickPos1)) {
            //qDebug() << "Label clicked!";
            ui->singleSong_pushButton->clicked();
        }
        if (labelRect2.contains(clickPos2)) {
            ui->songList_pushButton->clicked();
        }
        if (labelRect3.contains(clickPos3)) {
            ui->specialAlbum_pushButton->clicked();
        }
        if (labelRect4.contains(clickPos4)) {
            ui->collectVideo_pushButton->clicked();
        }
        if (labelRect5.contains(clickPos5)) {
            ui->singer_pushButton->clicked();
        }
        if (labelRect6.contains(clickPos6)) {
            ui->device_pushButton->clicked();
        }
    }
    QWidget::mousePressEvent(event);
}

void MyCollection::on_singleSong_pushButton_clicked() {
    // 判断当前显示的页面是否是 m_singleSong 页面，如果是则直接返回
    if (ui->stackedWidget->currentWidget() == this->m_singleSong.get()) {
        //qDebug() << "当前页面已经是 Single Song 页面，无需切换";
        return;  // 页面已是目标页面，无需切换
    }
    ui->singleSong_pushButton->setChecked(true);
    STREAM_INFO()<<"切换单曲界面";
    enableButton(false);
    //ui->stackedWidget->setCurrentWidget(this->m_singleSong.get());
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_singleSong.get()));
    ui->idx1_lab->show();
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->idx5_lab->hide();
    ui->idx6_lab->hide();
    ui->singleSong_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->songList_number_label->setStyleSheet("");
    ui->specialAlbum_number_label->setStyleSheet("");
    ui->collectVideo_number_label->setStyleSheet("");
    ui->singer_number_label->setStyleSheet("");
    ui->device_number_label->setStyleSheet("");
}

void MyCollection::on_songList_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_songList.get())return;
    ui->songList_pushButton->setChecked(true);
    STREAM_INFO()<<"切换歌单界面";
    enableButton(false);
    //ui->stackedWidget->setCurrentWidget(this->m_songList.get());
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_songList.get()));
    ui->idx1_lab->hide();
    ui->idx2_lab->show();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->idx5_lab->hide();
    ui->idx6_lab->hide();
    ui->singleSong_number_label->setStyleSheet("");
    ui->songList_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->specialAlbum_number_label->setStyleSheet("");
    ui->collectVideo_number_label->setStyleSheet("");
    ui->singer_number_label->setStyleSheet("");
    ui->device_number_label->setStyleSheet("");
}

void MyCollection::on_specialAlbum_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_specialAlbum.get())return;
    ui->specialAlbum_pushButton->setChecked(true);
    STREAM_INFO()<<"切换专辑界面";
    enableButton(false);
    //ui->stackedWidget->setCurrentWidget(this->m_specialAlbum.get());
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_specialAlbum.get()));
    ui->idx1_lab->hide();
    ui->idx2_lab->hide();
    ui->idx3_lab->show();
    ui->idx4_lab->hide();
    ui->idx5_lab->hide();
    ui->idx6_lab->hide();
    ui->singleSong_number_label->setStyleSheet("");
    ui->songList_number_label->setStyleSheet("");
    ui->specialAlbum_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->collectVideo_number_label->setStyleSheet("");
    ui->singer_number_label->setStyleSheet("");
    ui->device_number_label->setStyleSheet("");
}

void MyCollection::on_collectVideo_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_collectVideo.get())return;
    ui->collectVideo_pushButton->setChecked(true);
    STREAM_INFO()<<"切换视频界面";
    enableButton(false);
    //ui->stackedWidget->setCurrentWidget(this->m_collectVideo.get());
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_collectVideo.get()));
    ui->idx1_lab->hide();
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->show();
    ui->idx5_lab->hide();
    ui->idx6_lab->hide();
    ui->singleSong_number_label->setStyleSheet("");
    ui->songList_number_label->setStyleSheet("");
    ui->specialAlbum_number_label->setStyleSheet("");
    ui->collectVideo_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->singer_number_label->setStyleSheet("");
    ui->device_number_label->setStyleSheet("");
}

void MyCollection::on_singer_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_singerWidget.get())return;
    ui->singer_pushButton->setChecked(true);
    STREAM_INFO()<<"切换歌手界面";
    enableButton(false);
    //ui->stackedWidget->setCurrentWidget(this->m_singerWidget.get());
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_singerWidget.get()));
    ui->idx1_lab->hide();
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->idx5_lab->show();
    ui->idx6_lab->hide();
    ui->singleSong_number_label->setStyleSheet("");
    ui->songList_number_label->setStyleSheet("");
    ui->specialAlbum_number_label->setStyleSheet("");
    ui->collectVideo_number_label->setStyleSheet("");
    ui->singer_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->device_number_label->setStyleSheet("");
}

void MyCollection::on_device_pushButton_clicked() {
    if (ui->stackedWidget->currentWidget() == this->m_deviceWidget.get())return;
    ui->device_pushButton->setChecked(true);
    STREAM_INFO()<<"切换设备界面";
    enableButton(false);
    //ui->stackedWidget->setCurrentWidget(this->m_deviceWidget.get());
    ui->stackedWidget->slideInIdx(ui->stackedWidget->indexOf(this->m_deviceWidget.get()));
    ui->idx1_lab->hide();
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->idx5_lab->hide();
    ui->idx6_lab->show();
    ui->singleSong_number_label->setStyleSheet("");
    ui->songList_number_label->setStyleSheet("");
    ui->specialAlbum_number_label->setStyleSheet("");
    ui->collectVideo_number_label->setStyleSheet("");
    ui->singer_number_label->setStyleSheet("");
    ui->device_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
}
