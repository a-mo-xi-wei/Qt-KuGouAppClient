//
// Created by WeiWang on 24-11-12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Channel.h" resolved

#include "Channel.h"
#include "ui_Channel.h"
#include "SScrollArea.h"
#include "ChannelBlock.h"

#include<QFile>
#include<QButtonGroup>
#include <QScrollBar>
#include <QWheelEvent>
#include <random>

// 创建一个宏来截取 __FILE__ 宏中的目录部分
#define GET_CURRENT_DIR (QString(__FILE__).first(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

Channel::Channel(QWidget *parent) :
    QWidget(parent)
    , ui(new Ui::Channel)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
    , m_recommendWidget(std::make_unique<PartWidget>(this))
    , m_djWidget(std::make_unique<PartWidget>(this))
    , m_languageWidget(std::make_unique<PartWidget>(this))
    , m_themeWidget(std::make_unique<PartWidget>(this))
    , m_sceneWidget(std::make_unique<PartWidget>(this))
    , m_moodWidget(std::make_unique<PartWidget>(this))
    , m_styleWidget(std::make_unique<PartWidget>(this))
    , m_crowdWidget(std::make_unique<PartWidget>(this))
    , m_childrenWidget(std::make_unique<PartWidget>(this))
    , m_musicalInstrumentWidget(std::make_unique<PartWidget>(this))
    , m_labelWidget(std::make_unique<PartWidget>(this))
    , m_varietyWidget(std::make_unique<PartWidget>(this))
    , m_nationalCustomsWidget(std::make_unique<PartWidget>(this))
    , m_sportsWidget(std::make_unique<PartWidget>(this))
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/channel.css"));
        if (file.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(file.readAll());
        }
        else {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
        ui->title_label->setStyleSheet(QString("border:none;border-image: url(%1/music-channel.png);").arg(GET_CURRENT_DIR));
    }
    initButtonGroup();
    initTotalWidget();
    initVector();
    initUi();
}

Channel::~Channel() {
    delete ui;
}

void Channel::initButtonGroup() {
    this->m_buttonGroup->addButton(ui->recommend_pushButton);
    this->m_buttonGroup->addButton(ui->DJ_pushButton);
    this->m_buttonGroup->addButton(ui->language_pushButton);
    this->m_buttonGroup->addButton(ui->theme_pushButton);
    this->m_buttonGroup->addButton(ui->scene_pushButton);
    this->m_buttonGroup->addButton(ui->mood_pushButton);
    this->m_buttonGroup->addButton(ui->style_pushButton);
    this->m_buttonGroup->addButton(ui->crowd_pushButton);
    this->m_buttonGroup->addButton(ui->children_pushButton);
    this->m_buttonGroup->addButton(ui->musical_instrument_pushButton);
    this->m_buttonGroup->addButton(ui->label_pushButton);
    this->m_buttonGroup->addButton(ui->variety_pushButton);
    this->m_buttonGroup->addButton(ui->national_customs_pushButton);
    this->m_buttonGroup->addButton(ui->sports_pushButton);
    this->m_buttonGroup->setExclusive(true);
}

void Channel::initTotalWidget() {
    this->m_recommendWidget->setTitleName("推荐");
    this->m_djWidget->setTitleName("DJ");
    this->m_languageWidget->setTitleName("语言");
    this->m_themeWidget->setTitleName("主题");
    this->m_sceneWidget->setTitleName("场景");
    this->m_moodWidget->setTitleName("心情");
    this->m_styleWidget->setTitleName("风格");
    this->m_crowdWidget->setTitleName("人群");
    this->m_childrenWidget->setTitleName("儿童");
    this->m_musicalInstrumentWidget->setTitleName("乐器");
    this->m_labelWidget->setTitleName("厂牌");
    this->m_varietyWidget->setTitleName("综艺");
    this->m_nationalCustomsWidget->setTitleName("国风");
    this->m_sportsWidget->setTitleName("运动");
}

void Channel::initUi() {
    {
        //加入布局
        auto lay = dynamic_cast<QVBoxLayout*>(ui->table_widget->layout());
        if (!lay) {
            qWarning()<<"布局不存在";
            return;
        }
        lay->insertWidget(lay->count() - 1, this->m_recommendWidget.get());
        lay->insertWidget(lay->count() - 1, this->m_djWidget.get());
        lay->insertWidget(lay->count() - 1, this->m_languageWidget.get());
        lay->insertWidget(lay->count() - 1, this->m_themeWidget.get());
        lay->insertWidget(lay->count() - 1, this->m_sceneWidget.get());
        lay->insertWidget(lay->count() - 1, this->m_moodWidget.get());
        lay->insertWidget(lay->count() - 1, this->m_styleWidget.get());
        lay->insertWidget(lay->count() - 1, this->m_crowdWidget.get());
        lay->insertWidget(lay->count() - 1, this->m_childrenWidget.get());
        lay->insertWidget(lay->count() - 1, this->m_musicalInstrumentWidget.get());
        lay->insertWidget(lay->count() - 1, this->m_labelWidget.get());
        lay->insertWidget(lay->count() - 1, this->m_varietyWidget.get());
        lay->insertWidget(lay->count() - 1, this->m_nationalCustomsWidget.get());
        lay->insertWidget(lay->count() - 1, this->m_sportsWidget.get());
    }
    {
        //处理信号
        connect(ui->recommend_pushButton,&QPushButton::clicked,this,[this] {
            ui->scrollArea->verticalScrollBar()->setValue(this->m_recommendWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->DJ_pushButton,&QPushButton::clicked,this,[this] {
            ui->scrollArea->verticalScrollBar()->setValue(this->m_djWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->language_pushButton,&QPushButton::clicked,this,[this] {
            ui->scrollArea->verticalScrollBar()->setValue(this->m_languageWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->theme_pushButton,&QPushButton::clicked,this,[this] {
            ui->scrollArea->verticalScrollBar()->setValue(this->m_themeWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->scene_pushButton,&QPushButton::clicked,this,[this] {
            ui->scrollArea->verticalScrollBar()->setValue(this->m_sceneWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->mood_pushButton,&QPushButton::clicked,this,[this] {
            ui->scrollArea->verticalScrollBar()->setValue(this->m_moodWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->style_pushButton,&QPushButton::clicked,this,[this] {
            ui->scrollArea->verticalScrollBar()->setValue(this->m_styleWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->crowd_pushButton,&QPushButton::clicked,this,[this] {
            ui->scrollArea->verticalScrollBar()->setValue(this->m_crowdWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->children_pushButton,&QPushButton::clicked,this,[this] {
            ui->scrollArea->verticalScrollBar()->setValue(this->m_childrenWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->musical_instrument_pushButton,&QPushButton::clicked,this,[this] {
            ui->scrollArea->verticalScrollBar()->setValue(this->m_musicalInstrumentWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->label_pushButton,&QPushButton::clicked,this,[this] {
            ui->scrollArea->verticalScrollBar()->setValue(this->m_labelWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->variety_pushButton,&QPushButton::clicked,this,[this] {
            ui->scrollArea->verticalScrollBar()->setValue(this->m_varietyWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->national_customs_pushButton,&QPushButton::clicked,this,[this] {
            ui->scrollArea->verticalScrollBar()->setValue(this->m_nationalCustomsWidget->mapToParent(QPoint(0, 0)).y());
        });
        connect(ui->sports_pushButton,&QPushButton::clicked,this,[this] {
            ui->scrollArea->verticalScrollBar()->setValue(this->m_sportsWidget->mapToParent(QPoint(0, 0)).y());
        });
        //wheelVaue信号
        connect(ui->scrollArea,&SScrollArea::wheelValue,this,&Channel::handleWheelValue);
    }
    {
        //m_recommendWidget 17 个
        for (int i = 1; i <= 17; ++i) {
            auto block = new ChannelBlock(this);
            block->setCoverPix(this->m_pixPathVector[i]);
           this->m_recommendWidget->addBlockWidget(block);
        }
    }
}

void Channel::initVector() {
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
        this->m_songAndsinger.emplace_back("依然在一起", "马天宇");
        this->m_songAndsinger.emplace_back("你之于我", "丁禹兮");
        this->m_songAndsinger.emplace_back("重写宇宙", "世界之外");
        this->m_songAndsinger.emplace_back("葡萄成熟时", "陈奕迅");
        this->m_songAndsinger.emplace_back("后来遇见他", "善宇");
        this->m_songAndsinger.emplace_back("一叶知心", "巫娜");
        this->m_songAndsinger.emplace_back("好好生活", "旭言");
        this->m_songAndsinger.emplace_back("28TH", "HVOB");
        this->m_songAndsinger.emplace_back("天荒爱未老", "周慧敏");
        this->m_songAndsinger.emplace_back("一生不变", "邓瑞霞");
        this->m_songAndsinger.emplace_back("一暝", "杨静");
        this->m_songAndsinger.emplace_back("客家情怀", "谢忠良");
        this->m_songAndsinger.emplace_back("悬溺", "葛东琪");
        this->m_songAndsinger.emplace_back("还原", "肖战");
        this->m_songAndsinger.emplace_back("有一种爱叫做放手", "阿木");
        this->m_songAndsinger.emplace_back("炽火同行", "曹杨");
        this->m_songAndsinger.emplace_back("暖人", "张磊");
        this->m_songAndsinger.emplace_back("今生的爱前世的债", "相思");
        this->m_songAndsinger.emplace_back("水中花(Live)", "郁可唯");
        this->m_songAndsinger.emplace_back("主视角", "陈思斯");
        this->m_songAndsinger.emplace_back("声声慢(粤语版)", "陈艺鹏");
        this->m_songAndsinger.emplace_back("清静经", "王菲");
        this->m_songAndsinger.emplace_back("Together", "王铮亮");
        this->m_songAndsinger.emplace_back("夏夜心事", "Audrey");
        this->m_songAndsinger.emplace_back("静听流水声", "明月闲人");
        this->m_songAndsinger.emplace_back("桂花香", "七朵组合");
        this->m_songAndsinger.emplace_back("那个女孩", "宝石Gem");
        this->m_songAndsinger.emplace_back("她们唱的爱情", "曾昭玮");
        this->m_songAndsinger.emplace_back("天气预爆", "筷子兄弟");
        this->m_songAndsinger.emplace_back("碎银几两", "轩东");
        this->m_songAndsinger.emplace_back("花轿里面的人", "巨敏");
        this->m_songAndsinger.emplace_back("不可道", "祖娅纳惜");
        this->m_songAndsinger.emplace_back("渴望", "毛阿敏");
        this->m_songAndsinger.emplace_back("冬天里的一把火", "费翔");
        this->m_songAndsinger.emplace_back("说散就散(Live)", "杨耀扬");
        this->m_songAndsinger.emplace_back("如果有来生(Live)", "谭维维");
        this->m_songAndsinger.emplace_back("时光谣", "蔡健雅");
        this->m_songAndsinger.emplace_back("好风(Live)", "布衣乐队");
        this->m_songAndsinger.emplace_back("纸上雪", "许嵩");
    }
    for (int i = 1 ; i <= 60 ; ++i) {
        this->m_pixPathVector.emplace_back(QString(":/BlockCover/Res/blockcover/music-block-cover%1.jpg").arg(i));
    }
    // 使用当前时间作为随机数种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // 随机打乱 QVector
    std::shuffle(this->m_songAndsinger.begin(), this->m_songAndsinger.end(), std::default_random_engine(seed));
    std::shuffle(this->m_pixPathVector.begin(), this->m_pixPathVector.end(), std::default_random_engine(seed));

}

void Channel::handleWheelValue(const int& value) {
    if (value >= this->m_recommendWidget->mapToParent(QPoint(0, 0)).y() &&
        value < this->m_djWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->recommend_pushButton->setChecked(true);
    }
    else if (value >= this->m_djWidget->mapToParent(QPoint(0, 0)).y() &&
        value < this->m_languageWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->DJ_pushButton->setChecked(true);
    }
    else if (value >= this->m_languageWidget->mapToParent(QPoint(0, 0)).y() &&
        value < this->m_themeWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->language_pushButton->setChecked(true);
    }
    else if (value >= this->m_themeWidget->mapToParent(QPoint(0, 0)).y() &&
        value < this->m_sceneWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->theme_pushButton->setChecked(true);
    }
    else if (value >= this->m_sceneWidget->mapToParent(QPoint(0, 0)).y()&&
        value < this->m_moodWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->scene_pushButton->setChecked(true);
    }
    else if (value >= this->m_moodWidget->mapToParent(QPoint(0, 0)).y() &&
        value < this->m_styleWidget->mapToParent(QPoint(0, 0)).y() ) {
        ui->mood_pushButton->setChecked(true);
    }
    else if (value >= this->m_styleWidget->mapToParent(QPoint(0, 0)).y() &&
        value < this->m_childrenWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->style_pushButton->setChecked(true);
    }
    else if (value >= this->m_crowdWidget->mapToParent(QPoint(0, 0)).y() &&
        value < this->m_childrenWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->crowd_pushButton->setChecked(true);
    }
    else if (value >= this->m_childrenWidget->mapToParent(QPoint(0, 0)).y() &&
        value < this->m_musicalInstrumentWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->children_pushButton->setChecked(true);
    }
    else if (value >= this->m_musicalInstrumentWidget->mapToParent(QPoint(0, 0)).y() &&
        value < this->m_labelWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->musical_instrument_pushButton->setChecked(true);
    }
    else if (value >= this->m_labelWidget->mapToParent(QPoint(0, 0)).y() &&
        value < this->m_varietyWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->label_pushButton->setChecked(true);
    }
    else if (value >= this->m_varietyWidget->mapToParent(QPoint(0, 0)).y() &&
        value < this->m_nationalCustomsWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->variety_pushButton->setChecked(true);
    }
    else if (value >= this->m_nationalCustomsWidget->mapToParent(QPoint(0, 0)).y() &&
        value < this->m_sportsWidget->mapToParent(QPoint(0, 0)).y()) {
        ui->national_customs_pushButton->setChecked(true);
    }
    else if (value >= this->m_sportsWidget->mapToParent(QPoint(0, 0)).y()){
        ui->sports_pushButton->setChecked(true);
    }
}

void Channel::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    ui->scrollArea->setFixedHeight(this->window()->height()-305);
}
