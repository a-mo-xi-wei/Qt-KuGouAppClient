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
    , m_arrowLab(new QLabel(this))
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
    ui->toolButton_1->clicked();
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
    this->m_arrowLab->setObjectName("arrowLab");
    this->m_arrowLab->setStyleSheet("background-color: rgba(0,0,0,0);border: none;");
    this->m_arrowLab->setPixmap(GET_CURRENT_DIR + QStringLiteral("/PopularWidgets/liveRes/arrow-left.svg"));
    this->m_arrowLab->setFixedSize(30,30);
    connect(ui->toolButton_1,&QToolButton::toggled,[this] {
        // 1. 获取按钮在屏幕上的全局坐标
        const QPoint globalButtonPos = ui->toolButton_1->mapToGlobal(QPoint(0, 0));
        //qDebug()<<"globalButtonPos: " << globalButtonPos;
        // 2. 将全局坐标转换为当前窗口的局部坐标
        const QPoint localButtonPos = this->mapFromGlobal(globalButtonPos);
        //qDebug()<<"localButtonPos: " << localButtonPos;

        // 3. 计算三角形位置（按钮左侧中间偏移）
        const QPoint startPoint = localButtonPos + QPoint(-20, ui->toolButton_1->height() / 2 - 15);

        this->m_arrowLab->move(startPoint);
        this->m_arrowLab->raise();
        //qDebug()<<"lab的位置："<<lab->pos()<<"按钮的位置："<<ui->toolButton_1->mapFromParent(QPoint(0,0))+ui->right_widget->mapFromParent(QPoint(0,0));
    });
    connect(ui->toolButton_2,&QToolButton::toggled,[this] {

        // 1. 获取按钮在屏幕上的全局坐标
        const QPoint globalButtonPos = ui->toolButton_2->mapToGlobal(QPoint(0, 0));

        // 2. 将全局坐标转换为当前窗口的局部坐标
        const QPoint localButtonPos = this->mapFromGlobal(globalButtonPos);

        // 3. 计算三角形位置（按钮左侧中间偏移）
        const QPoint startPoint = localButtonPos + QPoint(-20, ui->toolButton_1->height() / 2 - 15);

        this->m_arrowLab->move(startPoint);
        this->m_arrowLab->raise();
        //qDebug()<<"lab的位置："<<lab->pos();
    });
    connect(ui->toolButton_3,&QToolButton::toggled,[this] {

        // 1. 获取按钮在屏幕上的全局坐标
        const QPoint globalButtonPos = ui->toolButton_3->mapToGlobal(QPoint(0, 0));

        // 2. 将全局坐标转换为当前窗口的局部坐标
        const QPoint localButtonPos = this->mapFromGlobal(globalButtonPos);

        // 3. 计算三角形位置（按钮左侧中间偏移）
        const QPoint startPoint = localButtonPos + QPoint(-20, ui->toolButton_1->height() / 2 - 15);

        this->m_arrowLab->move(startPoint);
        this->m_arrowLab->raise();
        //qDebug()<<"lab的位置："<<lab->pos();
    });

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
    // 查找被选中的按钮
    QToolButton* selectedButton = nullptr;
    if (ui->toolButton_1->isChecked()) {
        selectedButton = ui->toolButton_1;
    } else if (ui->toolButton_2->isChecked()) {
        selectedButton = ui->toolButton_2;
    } else if (ui->toolButton_3->isChecked()) {
        selectedButton = ui->toolButton_3;
    }

    if (selectedButton) {
        // 1. 获取按钮在屏幕上的全局坐标
        const QPoint globalButtonPos = selectedButton->mapToGlobal(QPoint(0, 0));
        //qDebug()<<"globalButtonPos: " << globalButtonPos;
        // 2. 将全局坐标转换为当前窗口的局部坐标
        const QPoint localButtonPos = this->mapFromGlobal(globalButtonPos);
        //qDebug()<<"localButtonPos: " << localButtonPos;

        // 3. 计算三角形位置（按钮左侧中间偏移）
        const QPoint startPoint = localButtonPos + QPoint(-20, selectedButton->height() / 2 - 15);

        this->m_arrowLab->move(startPoint);
        this->m_arrowLab->raise();
    }

}