//
// Created by WeiWang on 24-11-11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MusicRepository.h" resolved

#include "MusicRepository.h"
#include "ui_MusicRepository.h"
#include "logger.hpp"

#include <QFile>
#include <QButtonGroup>
#include <QMouseEvent>
#include <random>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))


MusicRepository::MusicRepository(QWidget *parent)
    : QWidget(parent)
      , ui(new Ui::MusicRepository)
      , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/musicrepo.css"));
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    } else {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";
        return;
    }
    initUi();
    this->m_topWindow = this->window();
    if (!m_topWindow) {
        qWarning() << "无法获取顶级窗口！";
        STREAM_WARN() << "无法获取顶级窗口！";
        return;
    }
}

MusicRepository::~MusicRepository() {
    delete ui;
}

void MusicRepository::initButtonGroup() {
    this->m_buttonGroup->addButton(ui->chinese_pushButton);
    this->m_buttonGroup->addButton(ui->west_pushButton);
    this->m_buttonGroup->addButton(ui->korea_pushButton);
    this->m_buttonGroup->addButton(ui->japan_pushButton);
    this->m_buttonGroup->setExclusive(true);
}

void MusicRepository::initUi() {
    //设置鼠标指向样式
    ui->ranking_list_widget->setCursor(Qt::PointingHandCursor);
    ui->singer_widget->setCursor(Qt::PointingHandCursor);
    ui->classify_widget->setCursor(Qt::PointingHandCursor);
    //初始化按钮组
    initButtonGroup();
    //初始化容器
    initVector();
    //初始化新碟上架
    initNewDiskWidget();
    //初始化精选视频
    initSelectWidget();

    //先按下华语
    ui->chinese_pushButton->clicked();
}

void MusicRepository::initNewDiskWidget() {
    //先隐藏两个block
    ui->block_widget6->hide();
    ui->block_widget7->hide();

    // 使用当前时间作为随机数种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // 随机打乱 QVector
    std::shuffle(this->m_total.begin(), this->m_total.end(), std::default_random_engine(seed));
    {
        //初始化
        ui->block_widget1->setCoverPix(this->m_total[1].pixPath);
        ui->block_widget1->setSongName(this->m_total[1].song);
        ui->block_widget1->setSinger(this->m_total[1].singer);
        ui->block_widget2->setCoverPix(this->m_total[2].pixPath);
        ui->block_widget2->setSongName(this->m_total[2].song);
        ui->block_widget2->setSinger(this->m_total[2].singer);
        ui->block_widget3->setCoverPix(this->m_total[3].pixPath);
        ui->block_widget3->setSongName(this->m_total[3].song);
        ui->block_widget3->setSinger(this->m_total[3].singer);
        ui->block_widget4->setCoverPix(this->m_total[4].pixPath);
        ui->block_widget4->setSongName(this->m_total[4].song);
        ui->block_widget4->setSinger(this->m_total[4].singer);
        ui->block_widget5->setCoverPix(this->m_total[5].pixPath);
        ui->block_widget5->setSongName(this->m_total[5].song);
        ui->block_widget5->setSinger(this->m_total[5].singer);
        ui->block_widget6->setCoverPix(this->m_total[6].pixPath);
        ui->block_widget6->setSongName(this->m_total[6].song);
        ui->block_widget6->setSinger(this->m_total[6].singer);
        ui->block_widget7->setCoverPix(this->m_total[7].pixPath);
        ui->block_widget7->setSongName(this->m_total[7].song);
        ui->block_widget7->setSinger(this->m_total[7].singer);
    }
}

void MusicRepository::initSelectWidget() {
    //先隐藏两个video
    ui->video_widget4->hide();
    ui->video_widget5->hide();
    ui->video_widget9->hide();
    ui->video_widget10->hide();

    // 使用当前时间作为随机数种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // 随机打乱 QVector
    std::shuffle(this->m_videoVector.begin(), this->m_videoVector.end(), std::default_random_engine(seed));
    {
        //初始化
        ui->video_widget1->setCoverPix  (this->m_videoVector[1].pixPath);
        ui->video_widget1->setVideoName (this->m_videoVector[1].song);
        ui->video_widget1->setIconPix   (this->m_videoVector[1].pixPath);
        ui->video_widget1->setAuthor    (this->m_videoVector[1].singer);

        ui->video_widget2->setCoverPix  (this->m_videoVector[2].pixPath);
        ui->video_widget2->setVideoName (this->m_videoVector[2].song);
        ui->video_widget2->setIconPix   (this->m_videoVector[2].pixPath);
        ui->video_widget2->setAuthor    (this->m_videoVector[2].singer);

        ui->video_widget3->setCoverPix  (this->m_videoVector[3].pixPath);
        ui->video_widget3->setVideoName (this->m_videoVector[3].song);
        ui->video_widget3->setIconPix   (this->m_videoVector[3].pixPath);
        ui->video_widget3->setAuthor    (this->m_videoVector[3].singer);

        ui->video_widget4->setCoverPix  (this->m_videoVector[4].pixPath);
        ui->video_widget4->setVideoName (this->m_videoVector[4].song);
        ui->video_widget4->setIconPix   (this->m_videoVector[4].pixPath);
        ui->video_widget4->setAuthor    (this->m_videoVector[4].singer);

        ui->video_widget5->setCoverPix  (this->m_videoVector[5].pixPath);
        ui->video_widget5->setVideoName (this->m_videoVector[5].song);
        ui->video_widget5->setIconPix   (this->m_videoVector[5].pixPath);
        ui->video_widget5->setAuthor    (this->m_videoVector[5].singer);

        ui->video_widget6->setCoverPix  (this->m_videoVector[6].pixPath);
        ui->video_widget6->setVideoName (this->m_videoVector[6].song);
        ui->video_widget6->setIconPix   (this->m_videoVector[6].pixPath);
        ui->video_widget6->setAuthor    (this->m_videoVector[6].singer);

        ui->video_widget7->setCoverPix  (this->m_videoVector[7].pixPath);
        ui->video_widget7->setVideoName (this->m_videoVector[7].song);
        ui->video_widget7->setIconPix   (this->m_videoVector[7].pixPath);
        ui->video_widget7->setAuthor    (this->m_videoVector[7].singer);

        ui->video_widget8->setCoverPix  (this->m_videoVector[8].pixPath);
        ui->video_widget8->setVideoName (this->m_videoVector[8].song);
        ui->video_widget8->setIconPix   (this->m_videoVector[8].pixPath);
        ui->video_widget8->setAuthor    (this->m_videoVector[8].singer);

        ui->video_widget9->setCoverPix  (this->m_videoVector[9].pixPath);
        ui->video_widget9->setVideoName (this->m_videoVector[9].song);
        ui->video_widget9->setIconPix   (this->m_videoVector[9].pixPath);
        ui->video_widget9->setAuthor    (this->m_videoVector[9].singer);

        ui->video_widget10->setCoverPix  (this->m_videoVector[10].pixPath);
        ui->video_widget10->setVideoName (this->m_videoVector[10].song);
        ui->video_widget10->setIconPix   (this->m_videoVector[10].pixPath);
        ui->video_widget10->setAuthor    (this->m_videoVector[10].singer);
    }
}

void MusicRepository::initVector() {
    {
        //插入歌曲和歌手的配对关系
        this->m_songAndsinger.emplace_back(" ", " ");
        this->m_songAndsinger.emplace_back("租购", "薛之谦");
        this->m_songAndsinger.emplace_back("还会再相遇", "时代少年团");
        this->m_songAndsinger.emplace_back("篇章(郜一菲小号合作曲)", "王赫野");
        this->m_songAndsinger.emplace_back("先说爱的人为什么先离开", "田园");
        this->m_songAndsinger.emplace_back("勇气之歌(《海洋奇缘2》中文主题曲)", "张杰");
        this->m_songAndsinger.emplace_back("小太阳", "时代少年团");
        this->m_songAndsinger.emplace_back("山友顶峰湖有彼岸", "魏佳艺");
        this->m_songAndsinger.emplace_back("朝夕相伴两不厌", "门丽");
        this->m_songAndsinger.emplace_back("冬2025", "王靖雯");
        this->m_songAndsinger.emplace_back("繁星从远山升起", "怪阿姨、吴瑭");
        this->m_songAndsinger.emplace_back("我们的时光 + 彩虹下面(live版)", "赵雷");
        this->m_songAndsinger.emplace_back("别等啦，出发吧", "张碧晨");
        this->m_songAndsinger.emplace_back("一分钱难倒英雄汉", "王小叶");
        this->m_songAndsinger.emplace_back("久别离", "七叔 (叶泽浩)");
        this->m_songAndsinger.emplace_back("我是夜里的光", "郑畅业");
        this->m_songAndsinger.emplace_back("极度伤感", "马健涛");
        this->m_songAndsinger.emplace_back("再见白马", "杨小壮");
        this->m_songAndsinger.emplace_back("秋叶落尽大雪飞(烟嗓版)", "韩小欠");
        this->m_songAndsinger.emplace_back("心头刺", "小阿七");
        this->m_songAndsinger.emplace_back("人间数十载", "韩小欠");
        this->m_songAndsinger.emplace_back("12号", "周柏豪");
        this->m_songAndsinger.emplace_back("你在的城市下了雪吗", "苍狼");
        this->m_songAndsinger.emplace_back("我不能", "六哲");
        this->m_songAndsinger.emplace_back("风中听落雪", "小阿枫");
        this->m_songAndsinger.emplace_back("野草没有花期", "潘成 (皮卡潘)");
        this->m_songAndsinger.emplace_back("风雨中的遗憾(粤语金曲)", "阿国歌");
        this->m_songAndsinger.emplace_back("如何面对这份情", "红蔷薇");
        this->m_songAndsinger.emplace_back("花香染指尖", "蔷薇团长、笑天");
        this->m_songAndsinger.emplace_back("临安月", "冯提莫");
        this->m_songAndsinger.emplace_back("分手的意义", "贺一航");
        this->m_songAndsinger.emplace_back("你我这也不能称为我们", "大雨点儿");
        this->m_songAndsinger.emplace_back("我的人生只能自己升级", "肖雨蒙");
        this->m_songAndsinger.emplace_back("女人的妆男人的谎", "李乐乐");
        this->m_songAndsinger.emplace_back("乌兰巴托的夜晚", "石头");
        this->m_songAndsinger.emplace_back("赤裸裸的伤害", "张艺迈");
        this->m_songAndsinger.emplace_back("戏", "丫蛋");
        this->m_songAndsinger.emplace_back("心中有座城叫愁", "影子");
        this->m_songAndsinger.emplace_back("此夜定有鬼", "一只白羊");
        this->m_songAndsinger.emplace_back("又是一阵秋风过", "谈柒柒");
        this->m_songAndsinger.emplace_back("雪的来意", "于洋");
        this->m_songAndsinger.emplace_back("横滨别恋", "周慧敏");
        this->m_songAndsinger.emplace_back("我快撑不住了", "范茹");
        this->m_songAndsinger.emplace_back("TOO HARD", "米卡");
        this->m_songAndsinger.emplace_back("以后的路陪你走", "清唯");
        this->m_songAndsinger.emplace_back("恰少年", "曹怡");
        this->m_songAndsinger.emplace_back("欠你一个天下", "梦珂");
        this->m_songAndsinger.emplace_back("我若消失谁会思念", "乔玲儿");
        this->m_songAndsinger.emplace_back("你的(live)", "汪苏泷、贺仙人");
        this->m_songAndsinger.emplace_back("土坡上的狗尾草(双语版)", "卢润泽、宫八");
        this->m_songAndsinger.emplace_back("凝眸", "丁禹兮");
        this->m_songAndsinger.emplace_back("心甘情愿做你一生的知己", "基地小虎");
        this->m_songAndsinger.emplace_back("寄明月", "虞书欣、丁禹兮、祝绪丹");
        this->m_songAndsinger.emplace_back("都一样", "肖战");
        this->m_songAndsinger.emplace_back("忘了归期", "王忻辰、袁小葳");
        this->m_songAndsinger.emplace_back("踹", "尼古拉斯.四哥");
        this->m_songAndsinger.emplace_back("7月7日晴", "en");
        this->m_songAndsinger.emplace_back("白鸽乌鸦相爱的戏码", "艾辰");
        this->m_songAndsinger.emplace_back("重塑", "都智文");
        this->m_songAndsinger.emplace_back("带我到山顶(Live)", "姚晓棠、海来阿木");
        this->m_songAndsinger.emplace_back("富时不忘穷时苦", "佳佳");
    }
    for (int i = 1; i <= 60; i++) {
        this->m_total.emplace_back(
            QString(":/BlockCover/Res/blockcover/music-block-cover%1.jpg").arg(i),
            m_songAndsinger[i].first,
            m_songAndsinger[i].second);
    }

    for (int i = 1; i <= 40; ++i) {
        this->m_videoVector.emplace_back(
            QString(":/RectCover/Res/rectcover/music-rect-cover%1.jpg").arg(i),
            m_songAndsinger[i + 10].first,
            m_songAndsinger[i + 10].second);
    }

    // 使用当前时间作为随机数种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // 随机打乱 QVector
    std::shuffle(this->m_total.begin(), this->m_total.end(), std::default_random_engine(seed));
    //分配给四个vector
    for (int i = 1; i <= 10; ++i) {
        this->m_chineseVector.emplace_back(this->m_total[i]);
        //qDebug()<<"m_total["<<i<<"] = "<<this->m_total[i].song<<" "<<this->m_total[i].singer;
    }
    for (int i = 11; i <= 20; ++i) {
        this->m_westVector.emplace_back(this->m_total[i]);
        //qDebug()<<"m_total["<<i<<"] = "<<this->m_total[i].song<<" "<<this->m_total[i].singer;
    }
    for (int i = 21; i <= 30; ++i) {
        this->m_koreaVector.emplace_back(this->m_total[i]);
        //qDebug()<<"m_total["<<i<<"] = "<<this->m_total[i].song<<" "<<this->m_total[i].singer;
    }
    for (int i = 31; i <= 40; ++i) {
        this->m_japanVector.emplace_back(this->m_total[i]);
        //qDebug()<<"m_total["<<i<<"] = "<<this->m_total[i].song<<" "<<this->m_total[i].singer;
    }
}

void MusicRepository::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    //qDebug()<<"MusicRepository::resizeEvent , size = "<<event->size();
    //通过线性分析得出以下结论
    int average = static_cast<int>(160 + (event->size().width() - 900) * 0.15);
    ui->ranking_list_widget->setFixedHeight(average);
    ui->singer_widget->setFixedHeight(average);
    ui->classify_widget->setFixedHeight(average);
    //grid设置
    const auto topLevelWidth = m_topWindow->width();
    average = (topLevelWidth - 290) / 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            ui->gridLayout->itemAtPosition(i, j)->widget()->setFixedWidth(average);
        }
    }
    //BlockWidget设置
    static int lastVisibleState = -1; // 记录上一次的可见状态
    const int currentWidth = this->width();
    int newVisibleState;

    // 根据宽度确定状态
    if (currentWidth < 1045) {
        newVisibleState = 0;
    } else if (currentWidth < 1250) {
        newVisibleState = 1;
    } else {
        newVisibleState = 2;
    }

    // 只有状态变化时才执行显示/隐藏
    if (newVisibleState != lastVisibleState) {
        switch (newVisibleState) {
            case 0: {
                ui->block_widget6->hide();
                ui->block_widget7->hide();
                ui->video_widget4->hide();
                ui->video_widget5->hide();
                ui->video_widget9->hide();
                ui->video_widget10->hide();
                break;
            }
            case 1: {
                ui->block_widget6->show();
                ui->block_widget7->hide();
                ui->video_widget4->show();
                ui->video_widget5->hide();
                ui->video_widget9->show();
                ui->video_widget10->hide();
                break;
            }
            case 2: {
                ui->block_widget6->show();
                ui->block_widget7->show();
                ui->video_widget4->show();
                ui->video_widget5->show();
                ui->video_widget9->show();
                ui->video_widget10->show();
                break;
            }
            default: break;
        }
        lastVisibleState = newVisibleState;
    }

    // 设置scrollArea的高度
    ui->scrollArea->setFixedHeight(this->m_topWindow->height() - 180);

}

void MusicRepository::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void MusicRepository::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

void MusicRepository::mouseDoubleClickEvent(QMouseEvent *event) {
    event->ignore();
}

void MusicRepository::on_chinese_pushButton_clicked() {
    ui->grid_widget->setUpdatesEnabled(false);
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int index = row * 3 + col; // 根据行列计算index
            if (index >= this->m_chineseVector.size()) {
                qWarning() << "m_chineseVector out of range!";
                STREAM_WARN() << "m_chineseVector out of range!";
                return;
            }
            auto item = ui->gridLayout->itemAtPosition(row, col); // 按行列获取item
            if (!item) {
                qWarning() << "item error at position:" << row << col;
                STREAM_WARN() << "item error at position:" << row << col;
                return;
            }
            auto widget = static_cast<MusicRepoList *>(item->widget());
            if (!widget) {
                qWarning() << "widget error at position:" << row << col;
                STREAM_WARN() << "widget error at position:" << row << col;
                return;
            }
            // 更新对应的widget内容
            widget->setCoverPix(this->m_chineseVector[index].pixPath);
            widget->setSongName(this->m_chineseVector[index].song);
            widget->setSinger(this->m_chineseVector[index].singer);
        }
    }

    ui->grid_widget->setUpdatesEnabled(true);
}

void MusicRepository::on_west_pushButton_clicked() {
    ui->grid_widget->setUpdatesEnabled(false);
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int index = row * 3 + col; // 根据行列计算index
            auto item = ui->gridLayout->itemAtPosition(row, col); // 按行列获取item
            auto widget = static_cast<MusicRepoList *>(item->widget());
            widget->setCoverPix(this->m_westVector[index].pixPath);
            widget->setSongName(this->m_westVector[index].song);
            widget->setSinger(this->m_westVector[index].singer);
        }
    }
    ui->grid_widget->setUpdatesEnabled(true);
}

void MusicRepository::on_korea_pushButton_clicked() {
    ui->grid_widget->setUpdatesEnabled(false);
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int index = row * 3 + col; // 根据行列计算index
            auto item = ui->gridLayout->itemAtPosition(row, col); // 按行列获取item
            auto widget = static_cast<MusicRepoList *>(item->widget());
            widget->setCoverPix(this->m_koreaVector[index].pixPath);
            widget->setSongName(this->m_koreaVector[index].song);
            widget->setSinger(this->m_koreaVector[index].singer);
        }
    }
    ui->grid_widget->setUpdatesEnabled(true);
}

void MusicRepository::on_japan_pushButton_clicked() {
    ui->grid_widget->setUpdatesEnabled(false);
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int index = row * 3 + col; // 根据行列计算index
            auto item = ui->gridLayout->itemAtPosition(row, col); // 按行列获取item
            auto widget = static_cast<MusicRepoList *>(item->widget());
            widget->setCoverPix(this->m_japanVector[index].pixPath);
            widget->setSongName(this->m_japanVector[index].song);
            widget->setSinger(this->m_japanVector[index].singer);
        }
    }
    ui->grid_widget->setUpdatesEnabled(true);
}
