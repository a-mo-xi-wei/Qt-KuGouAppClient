/**
 * @file MusicRepository.cpp
 * @brief 实现 MusicRepository 类，管理音乐仓库（歌曲和视频）界面
 * @author WeiWang
 * @date 2024-11-11
 * @version 1.0
 */

#include "MusicRepository.h"
#include "ui_MusicRepository.h"
#include "logger.hpp"
#include "ElaMessageBar.h"

#include <QFile>
#include <QButtonGroup>
#include <QMouseEvent>
#include <random>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化音乐仓库界面
 * @param parent 父控件指针，默认为 nullptr
 */
MusicRepository::MusicRepository(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MusicRepository)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/musicrepo.css")); ///< 加载样式表
    if (file.open(QIODevice::ReadOnly))
    {
        this->setStyleSheet(file.readAll());             ///< 应用样式表
    }
    else
    {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";          ///< 记录错误日志
        return;
    }
    initUi();                                            ///< 初始化界面
    if (!this->window())
    {
        qWarning() << "无法获取顶级窗口！";
        STREAM_WARN() << "无法获取顶级窗口！";          ///< 记录警告日志
        return;
    }
}

/**
 * @brief 析构函数，清理资源
 */
MusicRepository::~MusicRepository()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 初始化按钮组
 * @note 设置按钮互斥
 */
void MusicRepository::initButtonGroup()
{
    this->m_buttonGroup->addButton(ui->chinese_pushButton); ///< 添加华语按钮
    this->m_buttonGroup->addButton(ui->west_pushButton);    ///< 添加欧美按钮
    this->m_buttonGroup->addButton(ui->korea_pushButton);   ///< 添加韩国按钮
    this->m_buttonGroup->addButton(ui->japan_pushButton);   ///< 添加日本按钮
    this->m_buttonGroup->setExclusive(true);                ///< 设置互斥
}

/**
 * @brief 初始化界面
 * @note 设置鼠标样式、初始化按钮组、容器、新碟上架和精选视频
 */
void MusicRepository::initUi()
{
    ui->ranking_list_widget->setCursor(Qt::PointingHandCursor); ///< 设置排行榜鼠标样式
    ui->singer_widget->setCursor(Qt::PointingHandCursor);       ///< 设置歌手鼠标样式
    ui->classify_widget->setCursor(Qt::PointingHandCursor);     ///< 设置分类鼠标样式
    initButtonGroup();                                          ///< 初始化按钮组
    initVector();                                               ///< 初始化容器
    initNewDiskWidget();                                        ///< 初始化新碟上架
    initSelectWidget();                                         ///< 初始化精选视频
    ui->chinese_pushButton->clicked();                          ///< 默认点击华语按钮
}

/**
 * @brief 初始化新碟上架控件
 * @note 随机打乱并隐藏部分块
 */
void MusicRepository::initNewDiskWidget()
{
    ui->block_widget6->hide();                                  ///< 隐藏块 6
    ui->block_widget7->hide();                                  ///< 隐藏块 7
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); ///< 获取时间种子
    std::shuffle(this->m_total.begin(), this->m_total.end(), std::default_random_engine(seed)); ///< 随机打乱
    ui->block_widget1->setCoverPix(this->m_total[1].pixPath);   ///< 设置块 1 封面
    ui->block_widget1->setSongName(this->m_total[1].song);      ///< 设置块 1 歌曲
    ui->block_widget1->setSinger(this->m_total[1].singer);      ///< 设置块 1 歌手
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

/**
 * @brief 初始化精选视频控件
 * @note 随机打乱并隐藏部分视频
 */
void MusicRepository::initSelectWidget()
{
    ui->video_widget4->hide();                                  ///< 隐藏视频 4
    ui->video_widget5->hide();                                  ///< 隐藏视频 5
    ui->video_widget9->hide();                                  ///< 隐藏视频 9
    ui->video_widget10->hide();                                 ///< 隐藏视频 10
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); ///< 获取时间种子
    std::shuffle(this->m_videoVector.begin(), this->m_videoVector.end(), std::default_random_engine(seed)); ///< 随机打乱
    ui->video_widget1->setCoverPix(this->m_videoVector[1].pixPath); ///< 设置视频 1 封面
    ui->video_widget1->setVideoName(this->m_videoVector[1].song);   ///< 设置视频 1 名称
    ui->video_widget1->setIconPix(this->m_videoVector[1].pixPath);  ///< 设置视频 1 图标
    ui->video_widget1->setAuthor(this->m_videoVector[1].singer);    ///< 设置视频 1 作者
    ui->video_widget2->setCoverPix(this->m_videoVector[2].pixPath);
    ui->video_widget2->setVideoName(this->m_videoVector[2].song);
    ui->video_widget2->setIconPix(this->m_videoVector[2].pixPath);
    ui->video_widget2->setAuthor(this->m_videoVector[2].singer);
    ui->video_widget3->setCoverPix(this->m_videoVector[3].pixPath);
    ui->video_widget3->setVideoName(this->m_videoVector[3].song);
    ui->video_widget3->setIconPix(this->m_videoVector[3].pixPath);
    ui->video_widget3->setAuthor(this->m_videoVector[3].singer);
    ui->video_widget4->setCoverPix(this->m_videoVector[4].pixPath);
    ui->video_widget4->setVideoName(this->m_videoVector[4].song);
    ui->video_widget4->setIconPix(this->m_videoVector[4].pixPath);
    ui->video_widget4->setAuthor(this->m_videoVector[4].singer);
    ui->video_widget5->setCoverPix(this->m_videoVector[5].pixPath);
    ui->video_widget5->setVideoName(this->m_videoVector[5].song);
    ui->video_widget5->setIconPix(this->m_videoVector[5].pixPath);
    ui->video_widget5->setAuthor(this->m_videoVector[5].singer);
    ui->video_widget6->setCoverPix(this->m_videoVector[6].pixPath);
    ui->video_widget6->setVideoName(this->m_videoVector[6].song);
    ui->video_widget6->setIconPix(this->m_videoVector[6].pixPath);
    ui->video_widget6->setAuthor(this->m_videoVector[6].singer);
    ui->video_widget7->setCoverPix(this->m_videoVector[7].pixPath);
    ui->video_widget7->setVideoName(this->m_videoVector[7].song);
    ui->video_widget7->setIconPix(this->m_videoVector[7].pixPath);
    ui->video_widget7->setAuthor(this->m_videoVector[7].singer);
    ui->video_widget8->setCoverPix(this->m_videoVector[8].pixPath);
    ui->video_widget8->setVideoName(this->m_videoVector[8].song);
    ui->video_widget8->setIconPix(this->m_videoVector[8].pixPath);
    ui->video_widget8->setAuthor(this->m_videoVector[8].singer);
    ui->video_widget9->setCoverPix(this->m_videoVector[9].pixPath);
    ui->video_widget9->setVideoName(this->m_videoVector[9].song);
    ui->video_widget9->setIconPix(this->m_videoVector[9].pixPath);
    ui->video_widget9->setAuthor(this->m_videoVector[9].singer);
    ui->video_widget10->setCoverPix(this->m_videoVector[10].pixPath);
    ui->video_widget10->setVideoName(this->m_videoVector[10].song);
    ui->video_widget10->setIconPix(this->m_videoVector[10].pixPath);
    ui->video_widget10->setAuthor(this->m_videoVector[10].singer);
}

/**
 * @brief 初始化容器
 * @note 初始化歌曲/歌手配对和分配音乐/视频向量
 */
void MusicRepository::initVector()
{
    this->m_songAndsinger.emplace_back(" ", " ");         ///< 插入占位配对
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
    for (int i = 1; i <= 60; i++)
    {
        this->m_total.emplace_back(
            QString(":/BlockCover/Res/blockcover/music-block-cover%1.jpg").arg(i),
            m_songAndsinger[i].first,
            m_songAndsinger[i].second);                  ///< 插入全部音乐信息
    }
    for (int i = 1; i <= 40; ++i)
    {
        this->m_videoVector.emplace_back(
            QString(":/RectCover/Res/rectcover/music-rect-cover%1.jpg").arg(i),
            m_songAndsinger[i + 10].first,
            m_songAndsinger[i + 10].second);             ///< 插入视频信息
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); ///< 获取时间种子
    std::shuffle(this->m_total.begin(), this->m_total.end(), std::default_random_engine(seed)); ///< 随机打乱
    for (int i = 1; i <= 10; ++i)
    {
        this->m_chineseVector.emplace_back(this->m_total[i]); ///< 分配华语音乐
    }
    for (int i = 11; i <= 20; ++i)
    {
        this->m_westVector.emplace_back(this->m_total[i]);    ///< 分配欧美音乐
    }
    for (int i = 21; i <= 30; ++i)
    {
        this->m_koreaVector.emplace_back(this->m_total[i]);   ///< 分配韩国音乐
    }
    for (int i = 31; i <= 40; ++i)
    {
        this->m_japanVector.emplace_back(this->m_total[i]);   ///< 分配日本音乐
    }
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件
 * @note 动态调整控件高度/宽度和显示/隐藏块
 */
void MusicRepository::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    int average = static_cast<int>(160 + (event->size().width() - 900) * 0.15); ///< 计算控件高度
    ui->ranking_list_widget->setFixedHeight(average);       ///< 设置排行榜高度
    ui->singer_widget->setFixedHeight(average);             ///< 设置歌手高度
    ui->classify_widget->setFixedHeight(average);           ///< 设置分类高度
    const auto topLevelWidth = this->window()->width();        ///< 获取顶级窗口宽度
    average = (topLevelWidth - 290) / 3;                    ///< 计算网格宽度
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ui->gridLayout->itemAtPosition(i, j)->widget()->setFixedWidth(average); ///< 设置网格控件宽度
        }
    }
    static int lastVisibleState = -1;                       ///< 记录上一次可见状态
    const int currentWidth = this->width();                 ///< 获取当前宽度
    int newVisibleState;
    if (currentWidth < 1045)
    {
        newVisibleState = 0;                                ///< 状态 0：隐藏较多块
    }
    else if (currentWidth < 1250)
    {
        newVisibleState = 1;                                ///< 状态 1：显示部分块
    }
    else
    {
        newVisibleState = 2;                                ///< 状态 2：显示全部块
    }
    if (newVisibleState != lastVisibleState)
    {
        switch (newVisibleState)
        {
            case 0:
                ui->block_widget6->hide();                  ///< 隐藏块 6
                ui->block_widget7->hide();                  ///< 隐藏块 7
                ui->video_widget4->hide();                  ///< 隐藏视频 4
                ui->video_widget5->hide();                  ///< 隐藏视频 5
                ui->video_widget9->hide();                  ///< 隐藏视频 9
                ui->video_widget10->hide();                 ///< 隐藏视频 10
                break;
            case 1:
                ui->block_widget6->show();                  ///< 显示块 6
                ui->block_widget7->hide();
                ui->video_widget4->show();                  ///< 显示视频 4
                ui->video_widget5->hide();
                ui->video_widget9->show();                  ///< 显示视频 9
                ui->video_widget10->hide();
                break;
            case 2:
                ui->block_widget6->show();                  ///< 显示块 6
                ui->block_widget7->show();                  ///< 显示块 7
                ui->video_widget4->show();                  ///< 显示视频 4
                ui->video_widget5->show();                  ///< 显示视频 5
                ui->video_widget9->show();                  ///< 显示视频 9
                ui->video_widget10->show();                 ///< 显示视频 10
                break;
            default:
                break;
        }
        lastVisibleState = newVisibleState;                 ///< 更新可见状态
    }
    ui->scrollArea->setFixedHeight(this->window()->height() - 180); ///< 设置滚动区域高度
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void MusicRepository::mousePressEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void MusicRepository::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 鼠标双击事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void MusicRepository::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 华语按钮点击槽函数
 * @note 更新网格列表为华语歌曲
 */
void MusicRepository::on_chinese_pushButton_clicked()
{
    ui->grid_widget->setUpdatesEnabled(false);           ///< 禁用更新
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            int index = row * 3 + col;                   ///< 计算网格索引
            if (index >= this->m_chineseVector.size())
            {
                qWarning() << "m_chineseVector out of range!";
                STREAM_WARN() << "m_chineseVector out of range!"; ///< 记录警告日志
                return;
            }
            auto item = ui->gridLayout->itemAtPosition(row, col); ///< 获取网格项
            if (!item)
            {
                qWarning() << "item error at position:" << row << col;
                STREAM_WARN() << "item error at position:" << row << col; ///< 记录警告日志
                return;
            }
            auto widget = static_cast<MusicRepoList *>(item->widget()); ///< 获取控件
            if (!widget)
            {
                qWarning() << "widget error at position:" << row << col;
                STREAM_WARN() << "widget error at position:" << row << col; ///< 记录警告日志
                return;
            }
            widget->setCoverPix(this->m_chineseVector[index].pixPath); ///< 设置封面
            widget->setSongName(this->m_chineseVector[index].song);    ///< 设置歌曲
            widget->setSinger(this->m_chineseVector[index].singer);    ///< 设置歌手
        }
    }
    ui->grid_widget->setUpdatesEnabled(true);            ///< 启用更新
}

/**
 * @brief 欧美按钮点击槽函数
 * @note 更新网格列表为欧美歌曲
 */
void MusicRepository::on_west_pushButton_clicked()
{
    ui->grid_widget->setUpdatesEnabled(false);           ///< 禁用更新
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            int index = row * 3 + col;                   ///< 计算网格索引
            auto item = ui->gridLayout->itemAtPosition(row, col); ///< 获取网格项
            auto widget = static_cast<MusicRepoList *>(item->widget()); ///< 获取控件
            widget->setCoverPix(this->m_westVector[index].pixPath);    ///< 设置封面
            widget->setSongName(this->m_westVector[index].song);       ///< 设置歌曲
            widget->setSinger(this->m_westVector[index].singer);       ///< 设置歌手
        }
    }
    ui->grid_widget->setUpdatesEnabled(true);            ///< 启用更新
}

/**
 * @brief 韩国按钮点击槽函数
 * @note 更新网格列表为韩国歌曲
 */
void MusicRepository::on_korea_pushButton_clicked()
{
    ui->grid_widget->setUpdatesEnabled(false);           ///< 禁用更新
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            int index = row * 3 + col;                   ///< 计算网格索引
            auto item = ui->gridLayout->itemAtPosition(row, col); ///< 获取网格项
            auto widget = static_cast<MusicRepoList *>(item->widget()); ///< 获取控件
            widget->setCoverPix(this->m_koreaVector[index].pixPath);   ///< 设置封面
            widget->setSongName(this->m_koreaVector[index].song);      ///< 设置歌曲
            widget->setSinger(this->m_koreaVector[index].singer);      ///< 设置歌手
        }
    }
    ui->grid_widget->setUpdatesEnabled(true);            ///< 启用更新
}

/**
 * @brief 日本按钮点击槽函数
 * @note 更新网格列表为日本歌曲
 */
void MusicRepository::on_japan_pushButton_clicked()
{
    ui->grid_widget->setUpdatesEnabled(false);           ///< 禁用更新
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            int index = row * 3 + col;                   ///< 计算网格索引
            auto item = ui->gridLayout->itemAtPosition(row, col); ///< 获取网格项
            auto widget = static_cast<MusicRepoList *>(item->widget()); ///< 获取控件
            widget->setCoverPix(this->m_japanVector[index].pixPath);   ///< 设置封面
            widget->setSongName(this->m_japanVector[index].song);      ///< 设置歌曲
            widget->setSinger(this->m_japanVector[index].singer);      ///< 设置歌手
        }
    }
    ui->grid_widget->setUpdatesEnabled(true);            ///< 启用更新
}

/**
 * @brief 更多按钮 1 点击槽函数
 * @note 显示未实现提示
 */
void MusicRepository::on_more_pushButton1_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("%1 功能未实现 敬请期待").arg(ui->more_pushButton1->text().left(ui->more_pushButton1->text().size() - 2)),
                               1000, this->window());    ///< 显示提示
}

/**
 * @brief 更多按钮 2 点击槽函数
 * @note 显示未实现提示
 */
void MusicRepository::on_more_pushButton2_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("%1 功能未实现 敬请期待").arg(ui->more_pushButton2->text().left(ui->more_pushButton2->text().size() - 2)),
                               1000, this->window());    ///< 显示提示
}

/**
 * @brief 更多按钮 3 点击槽函数
 * @note 显示未实现提示
 */
void MusicRepository::on_more_pushButton3_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("%1 功能未实现 敬请期待").arg(ui->more_pushButton3->text().left(ui->more_pushButton3->text().size() - 2)),
                               1000, this->window());    ///< 显示提示
}