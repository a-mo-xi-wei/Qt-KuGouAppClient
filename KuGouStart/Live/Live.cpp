//
// Created by WeiWang on 24-11-14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Live.h" resolved

#include "Live.h"
#include "ui_Live.h"
#include "MyScrollArea.h"

#include <QButtonGroup>
#include <QFile>
#include <QPainter>
#include <QPainterPath>
#include <QScrollBar>
#include <QWheelEvent>

#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

Live::Live(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Live)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
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
    initUi();
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

    this->m_vScrollBar = ui->scrollArea->verticalScrollBar();
    //处理信号
    connect(ui->popular_pushButton, &QPushButton::clicked, this, [this] {
        this->m_vScrollBar->setValue(ui->popular_widget->mapToParent(QPoint(0, 0)).y());
    });
    connect(ui->attention_pushButton, &QPushButton::clicked, this, [this] {
        this->m_vScrollBar->setValue(ui->attention_widget->mapToParent(QPoint(0, 0)).y());
    });
    connect(ui->recommend_pushButton, &QPushButton::clicked, this, [this] {
        this->m_vScrollBar->setValue(this->m_recommendWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y());
    });
    connect(ui->music_pushButton, &QPushButton::clicked, this, [this] {
        this->m_vScrollBar->setValue(this->m_musicWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y());
    });
    connect(ui->new_star_pushButton, &QPushButton::clicked, this, [this] {
        this->m_vScrollBar->setValue(this->m_newStarWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y());
    });
    connect(ui->appearance_pushButton, &QPushButton::clicked, this, [this] {
        this->m_vScrollBar->setValue(this->m_appearanceWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y());
    });
    connect(ui->dance_pushButton, &QPushButton::clicked, this, [this] {
        this->m_vScrollBar->setValue(this->m_danceWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y());
    });
    connect(ui->barrage_game_pushButton, &QPushButton::clicked, this, [this] {
        this->m_vScrollBar->setValue(this->m_gameWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y());
    });
    connect(ui->scrollArea, &MyScrollArea::wheelValue, this, &Live::handleWheelValue);
    connect(this->m_vScrollBar, &QScrollBar::valueChanged,this, &Live::handleWheelValue);

    initPopularWidget();
    initAttentionWidget();
    initRecommendWidget();
    initMusicWidget();
    initNewStarWidget();
    initAppearanceWidget();
    initDanceWidget();
    initGameWidget();

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

}

void Live::initPopularWidget() {
    //设置互斥
    const auto group = new QButtonGroup(this);
    group->addButton(ui->toolButton_1);
    group->addButton(ui->toolButton_2);
    group->addButton(ui->toolButton_3);
    group->setExclusive(true);

    ui->toolButton_1->setLeftBottomText("HS一白月光");
    ui->toolButton_2->setLeftBottomText("cy菜菜");
    ui->toolButton_3->setLeftBottomText("乔希玥");

    ui->toolButton_1->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->toolButton_2->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ui->toolButton_3->setToolButtonStyle(Qt::ToolButtonIconOnly);

    //后面估计要加图片
    ui->toolButton_1->setBackgroundImg(":/RectCover/Res/rectcover/music-rect-cover20.jpg");
    ui->toolButton_2->setBackgroundImg(":/RectCover/Res/rectcover/music-rect-cover21.jpg");
    ui->toolButton_3->setBackgroundImg(":/RectCover/Res/rectcover/music-rect-cover22.jpg");

    //放置label
    ui->index_label_1->setStyleSheet("background-color: rgba(0,0,0,0);border: none;");
    ui->index_label_1->setPixmap(QPixmap(":Live/Res/live/arrow-left.svg"));
    ui->index_label_1->setFixedSize(20,30);
    ui->index_label_1->show();
    ui->index_label_2->setStyleSheet("background-color: rgba(0,0,0,0);border: none;");
    ui->index_label_2->setPixmap(QPixmap(":Live/Res/live/arrow-left.svg"));
    ui->index_label_2->setFixedSize(20,30);
    ui->index_label_2->hide();
    ui->index_label_3->setStyleSheet("background-color: rgba(0,0,0,0);border: none;");
    ui->index_label_3->setPixmap(QPixmap(":Live/Res/live/arrow-left.svg"));
    ui->index_label_3->setFixedSize(20,30);
    ui->index_label_3->hide();

    connect(ui->toolButton_1,&QToolButton::toggled,[this] {
        ui->index_label_1->setPixmap(QPixmap(":Live/Res/live/arrow-left.svg"));
        ui->index_label_2->setPixmap(QPixmap());
        ui->index_label_3->setPixmap(QPixmap());

        ui->index_label_1->show();
        ui->index_label_2->hide();
        ui->index_label_3->hide();
    });
    connect(ui->toolButton_2,&QToolButton::toggled,[this] {
        ui->index_label_1->setPixmap(QPixmap());
        ui->index_label_2->setPixmap(QPixmap(":Live/Res/live/arrow-left.svg"));
        ui->index_label_3->setPixmap(QPixmap());
        ui->index_label_1->hide();
        ui->index_label_2->show();
        ui->index_label_3->hide();
    });
    connect(ui->toolButton_3,&QToolButton::toggled,[this] {
        ui->index_label_1->setPixmap(QPixmap());
        ui->index_label_2->setPixmap(QPixmap());
        ui->index_label_3->setPixmap(QPixmap(":Live/Res/live/arrow-left.svg"));
        ui->index_label_1->hide();
        ui->index_label_2->hide();
        ui->index_label_3->show();
    });

}

QPixmap Live::roundedPixmap(const QPixmap &src, const QSize &size, const int &radius) {
    const QPixmap scaled = src.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPixmap dest(size);
    dest.fill(Qt::transparent);

    QPainter painter(&dest);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(0, 0, size.width(), size.height(), radius, radius);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, scaled);

    return dest;
}

void Live::initAttentionWidget() {
    const QPixmap roundedPix = roundedPixmap(QPixmap(QStringLiteral(":/Res/window/portrait.jpg")),
                                       ui->portrait_label->size(), 15);
    // 设置圆角半径
    ui->portrait_label->setPixmap(roundedPix);
    //设置互斥
    const auto group = new QButtonGroup(this);
    group->addButton(ui->pushButton_1);
    group->addButton(ui->pushButton_2);
    group->addButton(ui->pushButton_3);
    group->addButton(ui->pushButton_4);
    group->setExclusive(true);
    ui->empty_label->setFixedSize(390,230);
    ui->empty_label->setPixmap(QPixmap(":Live/Res/live/empty.png"));

}

void Live::initRecommendWidget() {
    const auto lay = static_cast<QVBoxLayout *>(ui->table_widget->layout());
    this->m_recommendWidget = std::make_unique<LiveCommonPartWidget>(ui->table_widget,2);
    //初始化widget
    this->m_recommendWidget->setTitleName("推荐");
    lay->insertWidget(lay->count() - 1, this->m_recommendWidget.get());
    lay->setStretchFactor(this->m_recommendWidget.get(), 1);
}

void Live::initMusicWidget() {
    const auto lay = static_cast<QVBoxLayout *>(ui->table_widget->layout());
    this->m_musicWidget = std::make_unique<LiveMusicPartWidget>(ui->table_widget);
    //初始化widget
    this->m_musicWidget->setTitleName("音乐");
    lay->insertWidget(lay->count() - 1, this->m_musicWidget.get());
    lay->setStretchFactor(this->m_musicWidget.get(), 1);
}

void Live::initNewStarWidget() {
    const auto lay = static_cast<QVBoxLayout *>(ui->table_widget->layout());
    this->m_newStarWidget = std::make_unique<LiveCommonPartWidget>(ui->table_widget,1);
    //初始化widget
    this->m_newStarWidget->setTitleName("新秀");
    this->m_newStarWidget->setNoTipLab();
    lay->insertWidget(lay->count() - 1, this->m_newStarWidget.get());
    lay->setStretchFactor(this->m_newStarWidget.get(), 1);
}

void Live::initAppearanceWidget() {
    const auto lay = static_cast<QVBoxLayout *>(ui->table_widget->layout());
    this->m_appearanceWidget = std::make_unique<LiveBigLeftWidget>(ui->table_widget);
    //初始化widget
    this->m_appearanceWidget->setTitleName("颜值");
    lay->insertWidget(lay->count() - 1, this->m_appearanceWidget.get());
    lay->setStretchFactor(this->m_appearanceWidget.get(), 1);
}

void Live::initDanceWidget() {
    const auto lay = static_cast<QVBoxLayout *>(ui->table_widget->layout());
    this->m_danceWidget = std::make_unique<LiveCommonPartWidget>(ui->table_widget,1);
    //初始化widget
    this->m_danceWidget->setTitleName("舞蹈");
    this->m_danceWidget->setNoTipLab();
    lay->insertWidget(lay->count() - 1, this->m_danceWidget.get());
    lay->setStretchFactor(this->m_danceWidget.get(), 1);
}

void Live::initGameWidget() {
    const auto lay = static_cast<QVBoxLayout *>(ui->table_widget->layout());
    this->m_gameWidget = std::make_unique<LiveBigLeftWidget>(ui->table_widget);
    //初始化widget
    this->m_gameWidget->setTitleName("弹幕游戏");
    lay->insertWidget(lay->count() - 1, this->m_gameWidget.get());
    lay->setStretchFactor(this->m_gameWidget.get(), 1);
}

void Live::handleWheelValue(const int &value) {
    if (value >= ui->popular_widget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y() &&
    value < ui->attention_widget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y()) {
        ui->popular_pushButton->setChecked(true);
    }
    else if (value >= ui->attention_widget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y() &&
    value < this->m_recommendWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y()) {
        ui->attention_pushButton->setChecked(true);
    }
    else if (value >= this->m_recommendWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y() &&
    value < this->m_musicWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y()) {
        ui->recommend_pushButton->setChecked(true);
    }
    else if (value >= this->m_musicWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y() &&
               value < this->m_newStarWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y()) {
        ui->music_pushButton->setChecked(true);
    }
    else if (value >= this->m_newStarWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y() &&
               value < this->m_appearanceWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y()) {
        ui->new_star_pushButton->setChecked(true);
    }
    else if (value >= this->m_appearanceWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y() &&
               value < this->m_danceWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y()) {
        ui->appearance_pushButton->setChecked(true);
    }
    else if (value >= this->m_danceWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y() &&
               value < this->m_gameWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y()) {
        ui->dance_pushButton->setChecked(true);
    }
    else if (value >= this->m_gameWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y()) {
        ui->barrage_game_pushButton->setChecked(true);
    }
}

void Live::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    ui->scrollArea->setFixedHeight(this->window()->height() - 255);
    ui->popular_widget->setFixedHeight(ui->popular_widget->width()*2/5);
    //qDebug()<<"ui->popular_widget->width(): "<<ui->popular_widget->width();
    ui->table_widget->setFixedWidth(this->window()->width()-50);
    //qDebug()<<"ui->table_widget->width(): "<<ui->table_widget->width();
    //qDebug()<<"ui->scrollArea->width(): "<<ui->scrollArea->width();
    //qDebug()<<"this->window()->width(): "<<this->window()->width();

}

void Live::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    //等比例缩放popular_widget
    ui->popular_widget->setFixedHeight(ui->popular_widget->width()*2/5);
    ui->table_widget->setFixedWidth(this->window()->width()-50);
}