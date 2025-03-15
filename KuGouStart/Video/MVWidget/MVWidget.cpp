//
// Created by WeiWang on 24-11-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MVWidget.h" resolved

#include "MVWidget.h"
#include "ui_MVWidget.h"
#include "logger.hpp"

#include <QFile>
#include <QMouseEvent>
#include <QButtonGroup>
#include <QTimer>
#include <random>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

MVWidget::MVWidget(QWidget *parent)
    :QWidget(parent)
    , ui(new Ui::MVWidget)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    this->m_parent = this->window();
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/mv.css"));
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

MVWidget::~MVWidget() {
    delete ui;
}

void MVWidget::initButtonGroup() {
    this->m_buttonGroup->addButton(ui->recommend_pushButton);
    this->m_buttonGroup->addButton(ui->chinese_pushButton);
    this->m_buttonGroup->addButton(ui->west_pushButton);
    this->m_buttonGroup->addButton(ui->koreaAndJapan_pushButton);
    this->m_buttonGroup->setExclusive(true);
}

void MVWidget::initLiveScene() {
    const auto layout = static_cast<QGridLayout*>(ui->live_scene_grid_widget->layout());
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int index = row * 3 + col; // 根据行列计算index
            auto item = layout->itemAtPosition(row, col); // 按行列获取item
            auto widget = static_cast<MVBlockWidget *>(item->widget());
            widget->setCoverPix(this->m_liveSceneVector[index].pixPath);
            widget->setTitle(this->m_liveSceneVector[index].title);
            widget->hideDesc();
        }
    }
}

void MVWidget::initHonorOfKings() {
    const auto layout = static_cast<QGridLayout*>(ui->honor_of_kings_grid_widget->layout());
    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 3; ++col) {
            int index = row * 3 + col; // 根据行列计算index
            auto item = layout->itemAtPosition(row, col); // 按行列获取item
            auto widget = static_cast<MVBlockWidget *>(item->widget());
            widget->setCoverPix(this->m_honorOfKingsVector[index].pixPath);
            widget->setTitle(this->m_honorOfKingsVector[index].title);
            widget->hideDesc();
        }
    }
}

void MVWidget::initAwardCeremony() {
    const auto layout = static_cast<QGridLayout*>(ui->award_ceremony_grid_widget->layout());
    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 3; ++col) {
            int index = row * 3 + col; // 根据行列计算index
            auto item = layout->itemAtPosition(row, col); // 按行列获取item
            auto widget = static_cast<MVBlockWidget *>(item->widget());
            widget->setCoverPix(this->m_awardCeremonyVector[index].pixPath);
            widget->setTitle(this->m_awardCeremonyVector[index].title);
            widget->hideDesc();
        }
    }
}

void MVWidget::initHotMV() {
    const auto layout = static_cast<QGridLayout*>(ui->hot_MV_grid_widget->layout());
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int index = row * 3 + col; // 根据行列计算index
            auto item = layout->itemAtPosition(row, col); // 按行列获取item
            auto widget = static_cast<MVBlockWidget *>(item->widget());
            widget->setCoverPix(this->m_hotMVVector[index].pixPath);
            widget->setTitle(this->m_hotMVVector[index].title);
            widget->hideDesc();
        }
    }
}

void MVWidget::initVector() {
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/title.json"));
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Could not open file for reading title.json";
            STREAM_WARN() << "Could not open file for reading title.json";
            return;
        }
        auto obj = QJsonDocument::fromJson(file.readAll());
        auto arr = obj.array();
        for (const auto &item : arr) {
            QString title = item.toObject().value("title").toString();
            this->m_titleAndDesc.emplace_back(title, parseTitle(title));
        }
        file.close();
        // 对 vector 排序，按 pair 的第一个元素和第二个元素排序
        std::sort(m_titleAndDesc.begin(), m_titleAndDesc.end());

        // 使用 std::unique 去重，并调整容器大小
        auto last = std::unique(m_titleAndDesc.begin(), m_titleAndDesc.end());
        m_titleAndDesc.erase(last, m_titleAndDesc.end());
        //qDebug() << "现在大小："<<this->m_titleAndDesc.size();
    }

    for (int i = 1; i <= 100; i++) {
        this->m_total.emplace_back(
            QString(":/RectCover/Res/rectcover/music-rect-cover%1.jpg").arg(i),
            m_titleAndDesc[i].first,
            m_titleAndDesc[i].second);
    }

    // 使用当前时间作为随机数种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // 随机打乱 QVector
    std::shuffle(this->m_total.begin(), this->m_total.end(), std::default_random_engine(seed));
    //分配给vector
    for (int i = 1; i <= 10; ++i) {
        this->m_recommendVector.emplace_back(this->m_total[i]);
        //qDebug()<<"m_total["<<i<<"] = "<<this->m_total[i].song<<" "<<this->m_total[i].singer;
    }
    for (int i = 11; i <= 20; ++i) {
        this->m_chineseVector.emplace_back(this->m_total[i]);
        //qDebug()<<"m_total["<<i<<"] = "<<this->m_total[i].song<<" "<<this->m_total[i].singer;
    }
    for (int i = 21; i <= 30; ++i) {
        this->m_westVector.emplace_back(this->m_total[i]);
        //qDebug()<<"m_total["<<i<<"] = "<<this->m_total[i].song<<" "<<this->m_total[i].singer;
    }
    for (int i = 31; i <= 40; ++i) {
        this->m_koreaAndJapanVector.emplace_back(this->m_total[i]);
        //qDebug()<<"m_total["<<i<<"] = "<<this->m_total[i].song<<" "<<this->m_total[i].singer;
    }
    for (int i = 41; i <= 50; ++i) {
        this->m_liveSceneVector.emplace_back(this->m_total[i]);
    }
    for (int i = 51; i <= 60; ++i) {
        this->m_honorOfKingsVector.emplace_back(this->m_total[i]);
    }
    for (int i = 61; i <= 70; ++i) {
        this->m_awardCeremonyVector.emplace_back(this->m_total[i]);
    }
    for (int i = 71; i <= 80; ++i) {
        this->m_hotMVVector.emplace_back(this->m_total[i]);
    }
}

const QString MVWidget::parseTitle(const QString &title) {
    QStringList list = title.split(" - ");
    QString str1 = list[0];
    QString str2 = list[1];
    // 查找 '《' 字符的位置
    int indexOfParenthesis = str2.indexOf("》");
    if (indexOfParenthesis != -1) {
        str2 = str2.left(indexOfParenthesis + 1);  // 截取到 '》' 以及之前的部分
        str2 += "MV上线";
        return str1 + " " + str2;
    }
    // 查找 "（" 字符的位置
    indexOfParenthesis = str2.indexOf("（");
    if (indexOfParenthesis != -1) {
        str2 = str2.left(indexOfParenthesis);  // 截取到 "（" 之前的部分
        str2 = "《" + str2 + "》MV上线";
        return str1 + " " + str2;
    }

    // 查找 '(' 字符的位置
    indexOfParenthesis = str2.indexOf('(');

    // 如果找到了 '('，则截取到 '(' 前的部分
    if (indexOfParenthesis != -1) {
        str2 = str2.left(indexOfParenthesis);  // 截取到 '(' 之前的部分
    }
    str2 = "《" + str2 + "》MV上线";
    return str1 + " " + str2;
}

void MVWidget::initUi() {
    //初始化按钮组
    initButtonGroup();
    //初始化容器
    initVector();
    //直播场景
    initLiveScene();
    //初始化王者荣耀
    initHonorOfKings();
    //初始化奖典典礼
    initAwardCeremony();
    //热门MV
    initHotMV();
    //广告
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/1.png")));
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/2.png")));
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/3.png")));
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/4.png")));
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/5.png")));
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/6.png")));
    ui->advertise_widget->setCurrentIndex(0);
    ui->advertise_widget->adjustSize();
    ui->advertise_widget->setAutoSlide(4000);
    ui->advertise_widget->setContentsMargins(0,0,0,0);
    //搜索框
    this->m_searchAction = new QAction(this);
    this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg")));
    this->m_searchAction->setIconVisibleInMenu(false);
    ui->search_lineEdit->addAction(this->m_searchAction, QLineEdit::TrailingPosition);
    QToolButton* searchButton = nullptr;
    foreach (QToolButton* btn, ui->search_lineEdit->findChildren<QToolButton*>()) {
        if (btn->defaultAction() == this->m_searchAction) {
            searchButton = btn;
            break;
        }
    }

    // 安装事件过滤器
    if (searchButton) {
        searchButton->installEventFilter(this);
    }
    //隐藏后四个按钮
    ui->pushButton5->hide();
    ui->pushButton6->hide();
    ui->pushButton7->hide();
    ui->pushButton8->hide();
    ui->pushButton5->setFixedSize(105,30);
    ui->pushButton6->setFixedSize(105,30);
    ui->pushButton7->setFixedSize(105,30);
    ui->pushButton8->setFixedSize(105,30);
    //先按下推荐
    ui->recommend_pushButton->clicked();
}

void MVWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    //qDebug()<<"width = "<<this->width();
    auto w = this->width();
    static int currentState = -1;  // 记录当前的按钮状态
    //按钮变化
    {
        int newState = 0;  // 状态变量来表示当前应该显示哪些按钮

        if (w <= 890) {
            newState = 0;  // 都不可见
        }
        else if (w <= 950) {
            newState = 1;  // 前 1 个按钮可见
        }
        else if (w <= 1060) {
            newState = 2;  // 前 2 个按钮可见
        }
        else if (w <= 1120) {
            newState = 3;  // 前 3 个按钮可见
        }
        else {
            newState = 4;  // 所有按钮可见
        }

        // 仅当按钮状态变化时才更新显示
        if (currentState != newState) {
            currentState = newState;
            ui->pushButton5->setVisible(newState >= 1);
            ui->pushButton6->setVisible(newState >= 2);
            ui->pushButton7->setVisible(newState >= 3);
            ui->pushButton8->setVisible(newState >= 4);
        }
    }
    //滑动板变化
    ui->advertise_widget->setFixedHeight(ui->advertise_widget->width()/5+65);
    //qDebug()<<"ui->advertise_widget.size() : "<<ui->advertise_widget->size();
    ui->scrollArea->setFixedHeight(this->m_parent->height() - 260);
    //qDebug()<<"当前高度："<<ui->scrollArea->height();
}

void MVWidget::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    //qDebug()<<"当前位置："<<this->geometry();
}

void MVWidget::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void MVWidget::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

void MVWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    event->ignore();
}

bool MVWidget::eventFilter(QObject *watched, QEvent *event) {
    const auto button = qobject_cast<QToolButton*>(watched);
    if (button && button->defaultAction() == this->m_searchAction) {
        if (event->type() == QEvent::Enter) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-blue.svg")));
        } else if (event->type() == QEvent::Leave) {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg")));
        }
    }
    return QObject::eventFilter(watched, event);
}

void MVWidget::on_recommend_pushButton_clicked() {
    ui->new_song_grid_widget->setUpdatesEnabled(false);
    const auto layout = static_cast<QGridLayout*>(ui->new_song_grid_widget->layout());
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int index = row * 3 + col; // 根据行列计算index
            if (index >= this->m_recommendVector.size()) {
                qWarning() << "m_recommendVector out of range!";
                STREAM_WARN() << "m_recommendVector out of range!";
                return;
            }
            auto item = layout->itemAtPosition(row, col); // 按行列获取item
            if (!item) {
                qWarning() << "item error at position:" << row << col;
                STREAM_WARN() << "item error at position:" << row << col;
                return;
            }
            auto widget = static_cast<MVBlockWidget *>(item->widget());
            if (!widget) {
                qWarning() << "widget error at position:" << row << col;
                STREAM_WARN() << "widget error at position:" << row << col;
                return;
            }
            // 更新对应的widget内容
            widget->setCoverPix(this->m_recommendVector[index].pixPath);
            widget->setTitle(this->m_recommendVector[index].title);
            widget->setDescription(this->m_recommendVector[index].description);
        }
    }

    ui->new_song_grid_widget->setUpdatesEnabled(true);
}

void MVWidget::on_chinese_pushButton_clicked() {
    ui->new_song_grid_widget->setUpdatesEnabled(false);
    const auto layout = static_cast<QGridLayout*>(ui->new_song_grid_widget->layout());
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int index = row * 3 + col; // 根据行列计算index
            auto item = layout->itemAtPosition(row, col); // 按行列获取item
            auto widget = static_cast<MVBlockWidget *>(item->widget());
            widget->setCoverPix(this->m_chineseVector[index].pixPath);
            widget->setTitle(this->m_chineseVector[index].title);
            widget->setDescription(this->m_chineseVector[index].description);
        }
    }
    ui->new_song_grid_widget->setUpdatesEnabled(true);
}

void MVWidget::on_west_pushButton_clicked() {
    ui->new_song_grid_widget->setUpdatesEnabled(false);
    const auto layout = static_cast<QGridLayout*>(ui->new_song_grid_widget->layout());
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int index = row * 3 + col; // 根据行列计算index
            auto item = layout->itemAtPosition(row, col); // 按行列获取item
            auto widget = static_cast<MVBlockWidget *>(item->widget());
            widget->setCoverPix(this->m_westVector[index].pixPath);
            widget->setTitle(this->m_westVector[index].title);
            widget->setDescription(this->m_westVector[index].description);
        }
    }
    ui->new_song_grid_widget->setUpdatesEnabled(true);
}

void MVWidget::on_koreaAndJapan_pushButton_clicked() {
    ui->new_song_grid_widget->setUpdatesEnabled(false);
    const auto layout = static_cast<QGridLayout*>(ui->new_song_grid_widget->layout());
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int index = row * 3 + col; // 根据行列计算index
            auto item = layout->itemAtPosition(row, col); // 按行列获取item
            auto widget = static_cast<MVBlockWidget *>(item->widget());
            widget->setCoverPix(this->m_koreaAndJapanVector[index].pixPath);
            widget->setTitle(this->m_koreaAndJapanVector[index].title);
            widget->setDescription(this->m_koreaAndJapanVector[index].description);
        }
    }
    ui->new_song_grid_widget->setUpdatesEnabled(true);
}
