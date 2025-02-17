//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Live.h" resolved

#include "Live.h"
#include "ui_Live.h"
#include "MyScrollArea.h"

#include <QButtonGroup>
#include <QFile>
#include <QScrollBar>
#include <QWheelEvent>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))


Live::Live(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Live)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
    , m_recommendWidget(std::make_unique<LiveCommonPartWidget>(this))
    , m_musicWidget(std::make_unique<LiveMusicPartWidget>(this))
    , m_newStarWidget(std::make_unique<LiveCommonPartWidget>(this))
    , m_appearanceWidget(std::make_unique<LiveBigLeftWidget>(this))
    , m_danceWidget(std::make_unique<LiveCommonPartWidget>(this))
    , m_gameWidget(std::make_unique<LiveBigLeftWidget>(this))
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/live.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
    initButtonGroup();

}

Live::~Live() {
    delete ui;
}

void Live::initButtonGroup() const {
    this->m_buttonGroup->addButton(ui->popular_pushButton);
    this->m_buttonGroup->addButton(ui->attention_pushButton);
    this->m_buttonGroup->addButton(ui->recommend_pushButton);
    this->m_buttonGroup->addButton(ui->music_pushButton);
    this->m_buttonGroup->addButton(ui->new_star_pushButton);
    this->m_buttonGroup->addButton(ui->appearance_pushButton);
    this->m_buttonGroup->addButton(ui->dance_pushButton);
    this->m_buttonGroup->addButton(ui->barrage_game_pushButton);
    this->m_buttonGroup->setExclusive(true);
}

void Live::initUi() {
    //加入布局
    auto lay = dynamic_cast<QVBoxLayout *>(ui->table_widget->layout());
    if (!lay) {
        qWarning() << "布局不存在";
        return;
    }
    lay->insertWidget(lay->count() - 1, this->m_recommendWidget.get());
    lay->insertWidget(lay->count() - 1, this->m_musicWidget.get());
    lay->insertWidget(lay->count() - 1, this->m_newStarWidget.get());
    lay->insertWidget(lay->count() - 1, this->m_appearanceWidget.get());
    lay->insertWidget(lay->count() - 1, this->m_danceWidget.get());
    lay->insertWidget(lay->count() - 1, this->m_gameWidget.get());

    this->m_vScrollBar = ui->scrollArea->verticalScrollBar();
    //处理信号
    connect(ui->recommend_pushButton, &QPushButton::clicked, this, [this] {
        this->m_vScrollBar->setValue(this->m_recommendWidget->mapToParent(QPoint(0, 0)).y());
    });
    connect(ui->music_pushButton, &QPushButton::clicked, this, [this] {
        this->m_vScrollBar->setValue(this->m_musicWidget->mapToParent(QPoint(0, 0)).y());
    });
    connect(ui->new_star_pushButton, &QPushButton::clicked, this, [this] {
        this->m_vScrollBar->setValue(this->m_newStarWidget->mapToParent(QPoint(0, 0)).y());
    });
    connect(ui->appearance_pushButton, &QPushButton::clicked, this, [this] {
        this->m_vScrollBar->setValue(this->m_appearanceWidget->mapToParent(QPoint(0, 0)).y());
    });
    connect(ui->dance_pushButton, &QPushButton::clicked, this, [this] {
        this->m_vScrollBar->setValue(this->m_danceWidget->mapToParent(QPoint(0, 0)).y());
    });
    connect(ui->barrage_game_pushButton, &QPushButton::clicked, this, [this] {
        this->m_vScrollBar->setValue(this->m_gameWidget->mapToParent(QPoint(0, 0)).y());
    });
    connect(ui->scrollArea, &MyScrollArea::wheelValue, this, &Live::handleWheelValue);
    connect(ui->scrollArea, &MyScrollArea::wheelValue, this, &Live::handleWheelValue);

    initPopularWidget();
}

void Live::initPopularWidget() {

}

void Live::handleWheelValue(const int &value) {
    if (value >= this->m_recommendWidget->mapToParent(QPoint(0, 0)).y() &&
    value < this->m_musicWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->recommend_pushButton->setChecked(true);
    }
    else if (value >= this->m_musicWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_newStarWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->music_pushButton->setChecked(true);
    }
    else if (value >= this->m_newStarWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_appearanceWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->new_star_pushButton->setChecked(true);
    }
    else if (value >= this->m_appearanceWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_danceWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->appearance_pushButton->setChecked(true);
    }
    else if (value >= this->m_danceWidget->mapToParent(QPoint(0, 0)).y() &&
               value < this->m_gameWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->dance_pushButton->setChecked(true);
    }
    else if (value >= this->m_gameWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->barrage_game_pushButton->setChecked(true);
    }
}

void Live::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    ui->scrollArea->setFixedHeight(this->window()->height() - 255);
}
