//
// Created by WeiWang on 24-11-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MVWidget.h" resolved

#include "MVWidget.h"
#include "ui_MVWidget.h"

#include <QFile>
#include <QMouseEvent>
#include <QButtonGroup>
#include <QPropertyAnimation>
#include <QScrollBar>
#include <QTimer>
#include <random>


// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

MVWidget::MVWidget(QWidget *parent)
    :QWidget(parent)
    , ui(new Ui::MVWidget)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
    , m_upBtn(std::make_unique<UpToolButton>(this))
    , m_scrollBarTimer(new QTimer(this))
{
    ui->setupUi(this);
    this->m_parent = this->window();
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/mv.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        } else {
            qDebug() << "样式表打开失败QAQ";
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
        //插入歌曲和歌手的配对关系
        this->m_titleAndDesc.emplace_back(" ", " ");
        this->m_titleAndDesc.emplace_back("租购", "薛之谦");
        this->m_titleAndDesc.emplace_back("还会再相遇", "时代少年团");
        this->m_titleAndDesc.emplace_back("篇章(郜一菲小号合作曲)", "王赫野");
        this->m_titleAndDesc.emplace_back("先说爱的人为什么先离开", "田园");
        this->m_titleAndDesc.emplace_back("勇气之歌(《海洋奇缘2》中文主题曲)", "张杰");
        this->m_titleAndDesc.emplace_back("小太阳", "时代少年团");
        this->m_titleAndDesc.emplace_back("山友顶峰湖有彼岸", "魏佳艺");
        this->m_titleAndDesc.emplace_back("朝夕相伴两不厌", "门丽");
        this->m_titleAndDesc.emplace_back("冬2025", "王靖雯");
        this->m_titleAndDesc.emplace_back("繁星从远山升起", "怪阿姨、吴瑭");
        this->m_titleAndDesc.emplace_back("我们的时光 + 彩虹下面(live版)", "赵雷");
        this->m_titleAndDesc.emplace_back("别等啦，出发吧", "张碧晨");
        this->m_titleAndDesc.emplace_back("一分钱难倒英雄汉", "王小叶");
        this->m_titleAndDesc.emplace_back("久别离", "七叔 (叶泽浩)");
        this->m_titleAndDesc.emplace_back("我是夜里的光", "郑畅业");
        this->m_titleAndDesc.emplace_back("极度伤感", "马健涛");
        this->m_titleAndDesc.emplace_back("再见白马", "杨小壮");
        this->m_titleAndDesc.emplace_back("秋叶落尽大雪飞(烟嗓版)", "韩小欠");
        this->m_titleAndDesc.emplace_back("心头刺", "小阿七");
        this->m_titleAndDesc.emplace_back("人间数十载", "韩小欠");
        this->m_titleAndDesc.emplace_back("12号", "周柏豪");
        this->m_titleAndDesc.emplace_back("你在的城市下了雪吗", "苍狼");
        this->m_titleAndDesc.emplace_back("我不能", "六哲");
        this->m_titleAndDesc.emplace_back("风中听落雪", "小阿枫");
        this->m_titleAndDesc.emplace_back("野草没有花期", "潘成 (皮卡潘)");
        this->m_titleAndDesc.emplace_back("风雨中的遗憾(粤语金曲)", "阿国歌");
        this->m_titleAndDesc.emplace_back("如何面对这份情", "红蔷薇");
        this->m_titleAndDesc.emplace_back("花香染指尖", "蔷薇团长、笑天");
        this->m_titleAndDesc.emplace_back("临安月", "冯提莫");
        this->m_titleAndDesc.emplace_back("分手的意义", "贺一航");
        this->m_titleAndDesc.emplace_back("你我这也不能称为我们", "大雨点儿");
        this->m_titleAndDesc.emplace_back("我的人生只能自己升级", "肖雨蒙");
        this->m_titleAndDesc.emplace_back("女人的妆男人的谎", "李乐乐");
        this->m_titleAndDesc.emplace_back("乌兰巴托的夜晚", "石头");
        this->m_titleAndDesc.emplace_back("赤裸裸的伤害", "张艺迈");
        this->m_titleAndDesc.emplace_back("戏", "丫蛋");
        this->m_titleAndDesc.emplace_back("心中有座城叫愁", "影子");
        this->m_titleAndDesc.emplace_back("此夜定有鬼", "一只白羊");
        this->m_titleAndDesc.emplace_back("又是一阵秋风过", "谈柒柒");
        this->m_titleAndDesc.emplace_back("雪的来意", "于洋");
        this->m_titleAndDesc.emplace_back("横滨别恋", "周慧敏");
        this->m_titleAndDesc.emplace_back("我快撑不住了", "范茹");
        this->m_titleAndDesc.emplace_back("TOO HARD", "米卡");
        this->m_titleAndDesc.emplace_back("以后的路陪你走", "清唯");
        this->m_titleAndDesc.emplace_back("恰少年", "曹怡");
        this->m_titleAndDesc.emplace_back("欠你一个天下", "梦珂");
        this->m_titleAndDesc.emplace_back("我若消失谁会思念", "乔玲儿");
        this->m_titleAndDesc.emplace_back("你的(live)", "汪苏泷、贺仙人");
        this->m_titleAndDesc.emplace_back("土坡上的狗尾草(双语版)", "卢润泽、宫八");
        this->m_titleAndDesc.emplace_back("凝眸", "丁禹兮");
        this->m_titleAndDesc.emplace_back("心甘情愿做你一生的知己", "基地小虎");
        this->m_titleAndDesc.emplace_back("寄明月", "虞书欣、丁禹兮、祝绪丹");
        this->m_titleAndDesc.emplace_back("都一样", "肖战");
        this->m_titleAndDesc.emplace_back("忘了归期", "王忻辰、袁小葳");
        this->m_titleAndDesc.emplace_back("踹", "尼古拉斯.四哥");
        this->m_titleAndDesc.emplace_back("7月7日晴", "en");
        this->m_titleAndDesc.emplace_back("白鸽乌鸦相爱的戏码", "艾辰");
        this->m_titleAndDesc.emplace_back("重塑", "都智文");
        this->m_titleAndDesc.emplace_back("带我到山顶(Live)", "姚晓棠、海来阿木");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
        this->m_titleAndDesc.emplace_back("富时不忘穷时苦", "佳佳");
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

void MVWidget::handleWheelValue(const int &value) {
    // 启动定时器，延迟处理
    if (!this->m_scrollBarTimer->isActive()) {
        this->m_scrollBarTimer->start(500); // 500ms 延迟，避免过于频繁地触发
    }
}

void MVWidget::onUpBtnClicked() {
    // 标记动画开始
    ui->scrollArea->setAnimating(true); //开始禁用滚轮

    auto animation = new QPropertyAnimation(ui->scrollArea->verticalScrollBar(), "value", this);
    // 设置动画的起始值（当前滚动条位置）和结束值（最顶部）
    animation->setStartValue(ui->scrollArea->verticalScrollBar()->value()); // 当前滚动条位置
    animation->setEndValue(0); // 滚动到顶部（0 表示最上方）
    animation->setDuration(500); // 动画持续时间，500ms
    animation->setEasingCurve(QEasingCurve::OutBounce); // 缓动曲线

    // 在动画结束后标记动画停止
    connect(animation, &QPropertyAnimation::finished, this, [this]() {
        ui->scrollArea->setAnimating(false); //动画结束启用滚轮
    });

    // 启动动画
    animation->start(QAbstractAnimation::DeleteWhenStopped); // 动画结束后自动删除
}

void MVWidget::onUpBtnShowOrNot() {
    if (ui->scrollArea->verticalScrollBar()->value() > 200)this->m_upBtn->show();
    else this->m_upBtn->hide();
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
    //滚动条
    ui->scrollArea->setScrollAreaKind(MyScrollArea::ScrollAreaKind::Inside);
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
    const auto action = new QAction(this);
    action->setIcon(QIcon(QStringLiteral(":/Res/titlebar/search-black.svg")));
    action->setIconVisibleInMenu(false);
    ui->search_lineEdit->addAction(action, QLineEdit::TrailingPosition);
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
    //wheelVaue信号
    connect(ui->scrollArea, &MyScrollArea::wheelValue, this, &MVWidget::handleWheelValue);
    connect(ui->scrollArea->verticalScrollBar(),&QScrollBar::valueChanged,this, &MVWidget::handleWheelValue);
    //回到最顶部信号
    connect(this->m_upBtn.get(), &QToolButton::clicked, this, &MVWidget::onUpBtnClicked);
    connect(this->m_scrollBarTimer, &QTimer::timeout, this, &MVWidget::onUpBtnShowOrNot);
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
    //UpWidget移动
    this->m_upBtn->move(this->m_parent->width() - this->m_upBtn->width() - 206,
                        this->m_parent->height() - this->m_upBtn->height() - 260);
    this->m_upBtn->raise();
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

void MVWidget::on_recommend_pushButton_clicked() {
    ui->new_song_grid_widget->setUpdatesEnabled(false);
    const auto layout = static_cast<QGridLayout*>(ui->new_song_grid_widget->layout());
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int index = row * 3 + col; // 根据行列计算index
            if (index >= this->m_recommendVector.size()) {
                qWarning() << "m_recommendVector out of range!";
                return;
            }
            auto item = layout->itemAtPosition(row, col); // 按行列获取item
            if (!item) {
                qWarning() << "item error at position:" << row << col;
                return;
            }
            auto widget = static_cast<MVBlockWidget *>(item->widget());
            if (!widget) {
                qWarning() << "widget error at position:" << row << col;
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
