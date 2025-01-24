//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MyCollection.h" resolved

#include "MyCollection.h"
#include "ui_MyCollection.h"

#include <QButtonGroup>
#include <QFile>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

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
            return;
        }
    }
    initUi();
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
    ui->stackedWidget->addWidget(this->m_singleSong.get());
    ui->stackedWidget->setCurrentWidget(this->m_singleSong.get());
}

void MyCollection::initSongList() {
    this->m_songList = std::make_unique<SongListWidget>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_songList.get());
}

void MyCollection::initSpecialAlbum() {
    this->m_specialAlbum = std::make_unique<SpecialAlbum>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_specialAlbum.get());
}

void MyCollection::initCollectVideo() {
    this->m_collectVideo = std::make_unique<CollectVideo>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_collectVideo.get());
}

void MyCollection::initSinger() {
    this->m_singerWidget = std::make_unique<SingerWidget>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_singerWidget.get());
}

void MyCollection::initDevice() {
    this->m_deviceWidget = std::make_unique<DeviceWidget>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_deviceWidget.get());
}

bool MyCollection::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->guide_widget1) {
        if (event->type() == QEvent::Enter) {
            ui->singleSong_pushButton->setStyleSheet(R"(
                QPushButton {
                    color:#26a1ff;
                    font-size:15px;
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
                    font-size:15px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QWidget#guide_widget QPushButton:checked {
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
                    font-size:15px;
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
                    font-size:15px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QWidget#guide_widget QPushButton:checked {
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
                    font-size:15px;
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
                    font-size:15px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QWidget#guide_widget QPushButton:checked {
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
                    font-size:15px;
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
                    font-size:15px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QWidget#guide_widget QPushButton:checked {
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
                    font-size:15px;
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
                    font-size:15px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QWidget#guide_widget QPushButton:checked {
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
                    font-size:15px;
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
                    font-size:15px;
                    border: none;
                    padding: 0px;
                    margin: 0px;
                }
                QWidget#guide_widget QPushButton:checked {
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

void MyCollection::on_singleSong_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(this->m_singleSong.get());
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
    ui->stackedWidget->setCurrentWidget(this->m_songList.get());
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
    ui->stackedWidget->setCurrentWidget(this->m_specialAlbum.get());
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
    ui->stackedWidget->setCurrentWidget(this->m_collectVideo.get());
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
    ui->stackedWidget->setCurrentWidget(this->m_singerWidget.get());
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
    ui->stackedWidget->setCurrentWidget(this->m_deviceWidget.get());
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
