/**
 * @file MVWidget.cpp
 * @brief 实现 MVWidget 类，提供音乐视频分类界面功能
 * @author WeiWang
 * @date 2024-11-12
 * @version 1.0
 */

#include "MVWidget.h"
#include "ui_MVWidget.h"
#include "logger.hpp"
#include "ElaMessageBar.h"
#include "ElaToolTip.h"
#include <QFile>
#include <QMouseEvent>
#include <QButtonGroup>
#include <QTimer>
#include <random>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化音乐视频界面
 * @param parent 父控件指针，默认为 nullptr
 */
MVWidget::MVWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MVWidget)
    , m_buttonGroup(std::make_unique<QButtonGroup>(this))
{
    ui->setupUi(this);
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/mv.css"));
        if (file.open(QIODevice::ReadOnly))
        {
            this->setStyleSheet(file.readAll()); ///< 加载样式表
        }
        else
        {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }

    initUi(); ///< 初始化界面
}

/**
 * @brief 析构函数，清理资源
 */
MVWidget::~MVWidget()
{
    delete ui;
}

/**
 * @brief 初始化按钮组
 */
void MVWidget::initButtonGroup()
{
    this->m_buttonGroup->addButton(ui->recommend_pushButton);      ///< 添加推荐按钮
    this->m_buttonGroup->addButton(ui->chinese_pushButton);        ///< 添加华语按钮
    this->m_buttonGroup->addButton(ui->west_pushButton);           ///< 添加欧美按钮
    this->m_buttonGroup->addButton(ui->koreaAndJapan_pushButton);  ///< 添加日韩按钮
    this->m_buttonGroup->setExclusive(true);                       ///< 设置互斥
}

/**
 * @brief 初始化直播场景分类
 */
void MVWidget::initLiveScene()
{
    const auto layout = static_cast<QGridLayout *>(ui->live_scene_grid_widget->layout());
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            int index = row * 3 + col;
            auto item = layout->itemAtPosition(row, col);
            auto widget = static_cast<MVBlockWidget *>(item->widget());
            widget->setCoverPix(this->m_liveSceneVector[index].pixPath); ///< 设置封面
            widget->setTitle(this->m_liveSceneVector[index].title);      ///< 设置标题
            widget->hideDesc();                                          ///< 隐藏描述
        }
    }
}

/**
 * @brief 初始化王者荣耀分类
 */
void MVWidget::initHonorOfKings()
{
    const auto layout = static_cast<QGridLayout *>(ui->honor_of_kings_grid_widget->layout());
    for (int row = 0; row < 2; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            int index = row * 3 + col;
            auto item = layout->itemAtPosition(row, col);
            auto widget = static_cast<MVBlockWidget *>(item->widget());
            widget->setCoverPix(this->m_honorOfKingsVector[index].pixPath); ///< 设置封面
            widget->setTitle(this->m_honorOfKingsVector[index].title);      ///< 设置标题
            widget->hideDesc();                                             ///< 隐藏描述
        }
    }
}

/**
 * @brief 初始化颁奖典礼分类
 */
void MVWidget::initAwardCeremony()
{
    const auto layout = static_cast<QGridLayout *>(ui->award_ceremony_grid_widget->layout());
    for (int row = 0; row < 2; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            int index = row * 3 + col;
            auto item = layout->itemAtPosition(row, col);
            auto widget = static_cast<MVBlockWidget *>(item->widget());
            widget->setCoverPix(this->m_awardCeremonyVector[index].pixPath); ///< 设置封面
            widget->setTitle(this->m_awardCeremonyVector[index].title);      ///< 设置标题
            widget->hideDesc();                                              ///< 隐藏描述
        }
    }
}

/**
 * @brief 初始化热门 MV 分类
 */
void MVWidget::initHotMV()
{
    const auto layout = static_cast<QGridLayout *>(ui->hot_MV_grid_widget->layout());
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            int index = row * 3 + col;
            auto item = layout->itemAtPosition(row, col);
            auto widget = static_cast<MVBlockWidget *>(item->widget());
            widget->setCoverPix(this->m_hotMVVector[index].pixPath); ///< 设置封面
            widget->setTitle(this->m_hotMVVector[index].title);      ///< 设置标题
            widget->hideDesc();                                      ///< 隐藏描述
        }
    }
}

/**
 * @brief 初始化数据容器
 */
void MVWidget::initVector()
{
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/title.json"));
        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "Could not open file for reading title.json";
            STREAM_WARN() << "Could not open file for reading title.json";
            return;
        }
        auto obj = QJsonDocument::fromJson(file.readAll());
        auto arr = obj.array();
        for (const auto &item : arr)
        {
            QString title = item.toObject().value("title").toString();
            this->m_titleAndDesc.emplace_back(title, parseTitle(title)); ///< 解析标题
        }
        file.close();
        // 对 vector 排序，按 pair 的第一个元素和第二个元素排序
        std::sort(m_titleAndDesc.begin(), m_titleAndDesc.end());

        // 使用 std::unique 去重，并调整容器大小
        auto last = std::unique(m_titleAndDesc.begin(), m_titleAndDesc.end());
        m_titleAndDesc.erase(last, m_titleAndDesc.end());
        //qDebug() << "现在大小："<<this->m_titleAndDesc.size();
    }

    for (int i = 1; i <= 100; i++)
    {
        this->m_total.emplace_back(
            QString(":/RectCover/Res/rectcover/music-rect-cover%1.jpg").arg(i),
            m_titleAndDesc[i].first,
            m_titleAndDesc[i].second); ///< 添加音乐信息
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); ///< 获取随机种子
    std::shuffle(this->m_total.begin(), this->m_total.end(), std::default_random_engine(seed)); ///< 随机打乱

    for (int i = 1; i <= 10; ++i)
        this->m_recommendVector.emplace_back(this->m_total[i]); ///< 分配推荐
    for (int i = 11; i <= 20; ++i)
        this->m_chineseVector.emplace_back(this->m_total[i]); ///< 分配华语
    for (int i = 21; i <= 30; ++i)
        this->m_westVector.emplace_back(this->m_total[i]); ///< 分配欧美
    for (int i = 31; i <= 40; ++i)
        this->m_koreaAndJapanVector.emplace_back(this->m_total[i]); ///< 分配日韩
    for (int i = 41; i <= 50; ++i)
        this->m_liveSceneVector.emplace_back(this->m_total[i]); ///< 分配直播场景
    for (int i = 51; i <= 60; ++i)
        this->m_honorOfKingsVector.emplace_back(this->m_total[i]); ///< 分配王者荣耀
    for (int i = 61; i <= 70; ++i)
        this->m_awardCeremonyVector.emplace_back(this->m_total[i]); ///< 分配颁奖典礼
    for (int i = 71; i <= 80; ++i)
        this->m_hotMVVector.emplace_back(this->m_total[i]); ///< 分配热门 MV
}

/**
 * @brief 解析标题
 * @param title 原始标题
 * @return 格式化后的标题
 */
const QString MVWidget::parseTitle(const QString &title)
{
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

/**
 * @brief 初始化界面
 */
void MVWidget::initUi()
{
    initButtonGroup(); ///< 初始化按钮组
    initVector();     ///< 初始化数据容器
    initLiveScene();  ///< 初始化直播场景
    initHonorOfKings(); ///< 初始化王者荣耀
    initAwardCeremony(); ///< 初始化颁奖典礼
    initHotMV();      ///< 初始化热门 MV

    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/1.png"))); ///< 添加广告图片
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/2.png")));
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/3.png")));
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/4.png")));
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/5.png")));
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/6.png")));
    ui->advertise_widget->setCurrentIndex(0); ///< 设置初始索引
    ui->advertise_widget->adjustSize();       ///< 调整大小
    ui->advertise_widget->setAutoSlide(4000); ///< 设置自动轮播
    ui->advertise_widget->setContentsMargins(0, 0, 0, 0); ///< 设置边距

    this->m_searchAction = new QAction(this); ///< 创建搜索动作
    this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg"))); ///< 设置图标
    this->m_searchAction->setIconVisibleInMenu(false);
    ui->search_lineEdit->addAction(this->m_searchAction, QLineEdit::TrailingPosition); ///< 添加到搜索框
    ui->search_lineEdit->setBorderRadius(10);
    auto font = QFont("AaSongLiuKaiTi");     ///< 设置字体
    font.setWeight(QFont::Bold);
    font.setPixelSize(12);
    ui->search_lineEdit->setFont(font);

    QToolButton *searchButton = nullptr;
    foreach (QToolButton *btn, ui->search_lineEdit->findChildren<QToolButton *>())
    {
        if (btn->defaultAction() == this->m_searchAction)
        {
            searchButton = btn;
            auto search_lineEdit_toolTip = new ElaToolTip(searchButton); ///< 创建提示
            search_lineEdit_toolTip->setToolTip(QStringLiteral("搜索")); ///< 设置提示文本
            break;
        }
    }

    if (searchButton)
        searchButton->installEventFilter(this); ///< 安装事件过滤器

    ui->pushButton5->hide(); ///< 隐藏按钮
    ui->pushButton6->hide();
    ui->pushButton7->hide();
    ui->pushButton8->hide();
    ui->pushButton5->setFixedSize(105, 30); ///< 设置按钮大小
    ui->pushButton6->setFixedSize(105, 30);
    ui->pushButton7->setFixedSize(105, 30);
    ui->pushButton8->setFixedSize(105, 30);

    ui->recommend_pushButton->clicked(); ///< 默认触发推荐按钮
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件对象
 * @note 重写基类方法
 */
void MVWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    auto w = this->width();
    static int currentState = -1;

    {
        int newState = 0;
        if (w <= 890)
            newState = 0;
        else if (w <= 950)
            newState = 1;
        else if (w <= 1060)
            newState = 2;
        else if (w <= 1120)
            newState = 3;
        else
            newState = 4;

        if (currentState != newState)
        {
            currentState = newState;
            ui->pushButton5->setVisible(newState >= 1); ///< 动态显示按钮
            ui->pushButton6->setVisible(newState >= 2);
            ui->pushButton7->setVisible(newState >= 3);
            ui->pushButton8->setVisible(newState >= 4);
        }
    }

    ui->advertise_widget->setFixedHeight(ui->advertise_widget->width() / 5 + 65); ///< 调整广告高度
    ui->scrollArea->setFixedHeight(this->window()->height() - 260); ///< 调整滚动区域高度
}

/**
 * @brief 显示事件
 * @param event 显示事件对象
 * @note 重写基类方法
 */
void MVWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件对象
 * @note 重写基类方法
 */
void MVWidget::mousePressEvent(QMouseEvent *event)
{
    event->ignore(); ///< 忽略事件
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件对象
 * @note 重写基类方法
 */
void MVWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore(); ///< 忽略事件
}

/**
 * @brief 鼠标双击事件
 * @param event 鼠标事件对象
 * @note 重写基类方法
 */
void MVWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore(); ///< 忽略事件
}

/**
 * @brief 事件过滤器
 * @param watched 目标对象
 * @param event 事件对象
 * @return 是否处理事件
 * @note 重写基类方法
 */
bool MVWidget::eventFilter(QObject *watched, QEvent *event)
{
    const auto button = qobject_cast<QToolButton *>(watched);
    if (button && button->defaultAction() == this->m_searchAction)
    {
        if (event->type() == QEvent::Enter)
        {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-blue.svg"))); ///< 悬停图标
        }
        else if (event->type() == QEvent::Leave)
        {
            this->m_searchAction->setIcon(QIcon(QStringLiteral(":/MenuIcon/Res/menuIcon/search-black.svg"))); ///< 默认图标
        }
    }
    return QObject::eventFilter(watched, event);
}

/**
 * @brief 推荐按钮点击槽
 */
void MVWidget::on_recommend_pushButton_clicked()
{
    ui->new_song_grid_widget->setUpdatesEnabled(false);
    const auto layout = static_cast<QGridLayout *>(ui->new_song_grid_widget->layout());
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            int index = row * 3 + col;
            if (index >= this->m_recommendVector.size())
            {
                qWarning() << "m_recommendVector out of range!";
                STREAM_WARN() << "m_recommendVector out of range!";
                return;
            }
            auto item = layout->itemAtPosition(row, col);
            if (!item)
            {
                qWarning() << "item error at position:" << row << col;
                STREAM_WARN() << "item error at position:" << row << col;
                return;
            }
            auto widget = static_cast<MVBlockWidget *>(item->widget());
            if (!widget)
            {
                qWarning() << "widget error at position:" << row << col;
                STREAM_WARN() << "widget error at position:" << row << col;
                return;
            }
            widget->setCoverPix(this->m_recommendVector[index].pixPath); ///< 设置封面
            widget->setTitle(this->m_recommendVector[index].title);      ///< 设置标题
            widget->setDescription(this->m_recommendVector[index].description); ///< 设置描述
        }
    }
    ui->new_song_grid_widget->setUpdatesEnabled(true);
    ui->new_song_grid_widget->update(); ///< 刷新界面
}

/**
 * @brief 华语按钮点击槽
 */
void MVWidget::on_chinese_pushButton_clicked()
{
    ui->new_song_grid_widget->setUpdatesEnabled(false);
    const auto layout = static_cast<QGridLayout *>(ui->new_song_grid_widget->layout());
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            int index = row * 3 + col;
            auto item = layout->itemAtPosition(row, col);
            auto widget = static_cast<MVBlockWidget *>(item->widget());
            widget->setCoverPix(this->m_chineseVector[index].pixPath); ///< 设置封面
            widget->setTitle(this->m_chineseVector[index].title);      ///< 设置标题
            widget->setDescription(this->m_chineseVector[index].description); ///< 设置描述
        }
    }
    ui->new_song_grid_widget->setUpdatesEnabled(true);
}

/**
 * @brief 欧美按钮点击槽
 */
void MVWidget::on_west_pushButton_clicked()
{
    ui->new_song_grid_widget->setUpdatesEnabled(false);
    const auto layout = static_cast<QGridLayout *>(ui->new_song_grid_widget->layout());
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            int index = row * 3 + col;
            auto item = layout->itemAtPosition(row, col);
            auto widget = static_cast<MVBlockWidget *>(item->widget());
            widget->setCoverPix(this->m_westVector[index].pixPath); ///< 设置封面
            widget->setTitle(this->m_westVector[index].title);      ///< 设置标题
            widget->setDescription(this->m_westVector[index].description); ///< 设置描述
        }
    }
    ui->new_song_grid_widget->setUpdatesEnabled(true);
}

/**
 * @brief 日韩按钮点击槽
 */
void MVWidget::on_koreaAndJapan_pushButton_clicked()
{
    ui->new_song_grid_widget->setUpdatesEnabled(false);
    const auto layout = static_cast<QGridLayout *>(ui->new_song_grid_widget->layout());
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            int index = row * 3 + col;
            auto item = layout->itemAtPosition(row, col);
            auto widget = static_cast<MVBlockWidget *>(item->widget());
            widget->setCoverPix(this->m_koreaAndJapanVector[index].pixPath); ///< 设置封面
            widget->setTitle(this->m_koreaAndJapanVector[index].title);      ///< 设置标题
            widget->setDescription(this->m_koreaAndJapanVector[index].description); ///< 设置描述
        }
    }
    ui->new_song_grid_widget->setUpdatesEnabled(true);
}

/**
 * @brief 更多按钮 1 点击槽
 */
void MVWidget::on_more_pushButton1_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("%1 功能未实现 敬请期待").arg(ui->more_pushButton1->text().left(ui->more_pushButton1->text().size() - 2)), 1000, this->window()); ///< 显示提示
}

/**
 * @brief 更多按钮 2 点击槽
 */
void MVWidget::on_more_pushButton2_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("%1 功能未实现 敬请期待").arg(ui->more_pushButton2->text().left(ui->more_pushButton2->text().size() - 2)), 1000, this->window()); ///< 显示提示
}

/**
 * @brief 更多按钮 3 点击槽
 */
void MVWidget::on_more_pushButton3_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("%1 功能未实现 敬请期待").arg(ui->more_pushButton3->text().left(ui->more_pushButton3->text().size() - 2)), 1000, this->window()); ///< 显示提示
}

/**
 * @brief 更多按钮 4 点击槽
 */
void MVWidget::on_more_pushButton4_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("%1 功能未实现 敬请期待").arg(ui->more_pushButton4->text().left(ui->more_pushButton4->text().size() - 2)), 1000, this->window()); ///< 显示提示
}

/**
 * @brief 更多按钮 5 点击槽
 */
void MVWidget::on_more_pushButton5_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("%1 功能未实现 敬请期待").arg(ui->more_pushButton5->text().left(ui->more_pushButton5->text().size() - 2)), 1000, this->window()); ///< 显示提示
}