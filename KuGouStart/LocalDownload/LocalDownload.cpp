//
// Created by WeiWang on 24-10-10.
//

// You may need to build the project (run Qt uic code generator) to get "ui_LocalDownload.h" resolved

#include "LocalDownload.h"
#include "ui_LocalDownload.h"

#include <QFile>
#include <QStringList>
#include <QResizeEvent>
#include <QTimer>
#include <QButtonGroup>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

LocalDownload::LocalDownload(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LocalDownload)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/local.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }

    initUi();
    //中转信号
    connect(this->m_localSong.get(), &LocalSong::addSongInfo, this, [this](const SongInfor& info){emit addSongInfo(info);});
    connect(this->m_localSong.get(), &LocalSong::subSongInfo, this, [this](const SongInfor& info){emit subSongInfo(info);});
    connect(this->m_localSong.get(), &LocalSong::syncSongInfo, this, [this](QVector<SongInfor> &vec){emit syncSongInfo(vec);});
    connect(this->m_localSong.get(), &LocalSong::playMusic, this, [this](const int& index){emit playMusic(index);});
    connect(this->m_localSong.get(), &LocalSong::startPlay, this, [this]{emit startPlay();});
}

LocalDownload::~LocalDownload() {
    delete ui;
}

void LocalDownload::setPlayIndex(const int &index) {
    this->m_localSong->setPlayIndex(index);
}

void LocalDownload::onMaxScreenHandle() {
    this->m_localSong->onMaxScreenHandle();
}

void LocalDownload::initStackedWidget() {
    initLocalSong();
    initDownloadedSong();
    initDownloadedVideo();
    initDownloading();

    //设置互斥
    this->m_buttonGroup->addButton(ui->local_music_pushButton);
    this->m_buttonGroup->addButton(ui->downloaded_music_pushButton);
    this->m_buttonGroup->addButton(ui->downloaded_video_pushButton);
    this->m_buttonGroup->addButton(ui->downloading_pushButton);
    this->m_buttonGroup->setExclusive(true);
}

void LocalDownload::initUi() {
    //下标图片
    ui->idx1_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx2_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx3_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx4_lab->setPixmap(QPixmap(QStringLiteral(":/Res/window/index_lab.svg")));
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();

    initStackedWidget();

    //默认点击本地音乐
    ui->local_music_pushButton->clicked();
}

void LocalDownload::initLocalSong() {
    this->m_localSong = std::make_unique<LocalSong>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_localSong.get());
    ui->stackedWidget->setCurrentWidget(this->m_localSong.get());
}

void LocalDownload::initDownloadedSong() {
    this->m_downloadedSong = std::make_unique<DownloadedSong>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_downloadedSong.get());
}

void LocalDownload::initDownloadedVideo() {
    this->m_downloadedVideo = std::make_unique<DownloadedVideo>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_downloadedVideo.get());
}

void LocalDownload::initDownloading() {
    this->m_downloading = std::make_unique<Downloading>(ui->stackedWidget);
    ui->stackedWidget->addWidget(this->m_downloading.get());
}

void LocalDownload::on_local_music_pushButton_clicked() {
    //切换堆栈界面
    if (ui->stackedWidget->currentWidget() == this->m_localSong.get())return;
    ui->stackedWidget->setCurrentWidget(this->m_localSong.get());
    ui->idx1_lab->show();
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->local_music_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->downloaded_music_number_label->setStyleSheet("");
    ui->downloaded_video_number_label->setStyleSheet("");
    ui->downloading_number_label->setStyleSheet("");
}

void LocalDownload::on_downloaded_music_pushButton_clicked() {
    //切换堆栈界面
    if (ui->stackedWidget->currentWidget() == this->m_downloadedSong.get())return;
    ui->stackedWidget->setCurrentWidget(this->m_downloadedSong.get());
    ui->idx1_lab->hide();
    ui->idx2_lab->show();
    ui->idx3_lab->hide();
    ui->idx4_lab->hide();
    ui->downloaded_music_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->local_music_number_label->setStyleSheet("");
    ui->downloaded_video_number_label->setStyleSheet("");
    ui->downloading_number_label->setStyleSheet("");
}

void LocalDownload::on_downloaded_video_pushButton_clicked() {
    //切换堆栈界面
    if (ui->stackedWidget->currentWidget() == this->m_downloadedVideo.get())return;
    ui->stackedWidget->setCurrentWidget(this->m_downloadedVideo.get());
    ui->idx1_lab->hide();
    ui->idx2_lab->hide();
    ui->idx3_lab->show();
    ui->idx4_lab->hide();
    ui->downloaded_video_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->downloaded_music_number_label->setStyleSheet("");
    ui->local_music_number_label->setStyleSheet("");
    ui->downloading_number_label->setStyleSheet("");
}

void LocalDownload::on_downloading_pushButton_clicked() {
    //切换堆栈界面
    if (ui->stackedWidget->currentWidget() == this->m_downloading.get())return;
    ui->stackedWidget->setCurrentWidget(this->m_downloading.get());
    ui->idx1_lab->hide();
    ui->idx2_lab->hide();
    ui->idx3_lab->hide();
    ui->idx4_lab->show();
    ui->downloading_number_label->setStyleSheet(QStringLiteral("color:#26a1ff;font-size:16px;font-weight:bold;"));
    ui->downloaded_music_number_label->setStyleSheet("");
    ui->downloaded_video_number_label->setStyleSheet("");
    ui->local_music_number_label->setStyleSheet("");
}
