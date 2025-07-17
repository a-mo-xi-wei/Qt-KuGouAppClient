/**
 * @file MVWidget.cpp
 * @brief 实现 MVWidget 类，提供音乐视频分类界面功能
 * @author WeiWang
 * @date 2024-11-12
 * @version 1.0
 */

#include "MVWidget.h"
#include "ui_MVWidget.h"
#include "MVBlockWidget.h"
#include "logger.hpp"
#include "ElaMessageBar.h"
#include "ElaToolTip.h"
#include "Async.h"
#include "RefreshMask.h"

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
    , m_refreshMask(std::make_unique<RefreshMask>(this))                 ///< 初始化刷新遮罩
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

    connect(ui->stackedWidget, &SlidingStackedWidget::animationFinished, [this] {
        enableButton(true);                              ///< 动画结束时启用按钮
    });
    enableButton(true);
}

/**
 * @brief 析构函数，清理资源
 */
MVWidget::~MVWidget()
{
    delete ui;
}

/**
 * @brief 创建仓库页面
 * @param beg 开始索引
 * @return 创建的页面控件
 */
QWidget* MVWidget::createPage(const int& beg)
{
    auto pageWidget = new QWidget;
    auto mainLayout = new QVBoxLayout(pageWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 0, 10, 0);

    for (int row = 0; row < 3; ++row) {
        auto rowLayout = new QHBoxLayout;
        rowLayout->setSpacing(10);
        for (int col = 0; col < 3; ++col) {
            int index = row * 3 + col + beg;
            if (index >= 9 + beg) break;
            auto item = new MVBlockWidget;
            item->setCoverPix(m_total[index].pixPath);
            item->setTitle(m_total[index].title);
            item->setDescription(m_total[index].description);
            rowLayout->addWidget(item);
            rowLayout->setStretch(col, 1);
        }
        mainLayout->addLayout(rowLayout);
    }

    return pageWidget;
}

/**
 * @brief 初始化按钮组
 */
void MVWidget::initButtonGroup()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(this->m_total.begin(), this->m_total.end(), std::default_random_engine(seed)); ///< 随机打乱

    // 设置按钮组（互斥）
    this->m_buttonGroup->addButton(ui->recommend_pushButton, 0);
    this->m_buttonGroup->addButton(ui->chinese_pushButton, 1);
    this->m_buttonGroup->addButton(ui->koreaAndJapan_pushButton, 2);
    this->m_buttonGroup->addButton(ui->west_pushButton, 3);
    this->m_buttonGroup->setExclusive(true);

    // 初始化占位页面
    for (int i = 0; i < 4; ++i) {
        auto *placeholder = new QWidget;
        auto *layout = new QVBoxLayout(placeholder);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        m_pages[i] = placeholder;
        ui->stackedWidget->insertWidget(i, placeholder);
    }

    m_pages[m_currentIdx]->layout()->addWidget(createPage(1));
    ui->stackedWidget->slideInIdx(0);

    // 响应按钮点击事件
    connect(m_buttonGroup.get(), &QButtonGroup::idClicked, this, [this](const int& id) {
        if (m_currentIdx == id) {
            return;
        }

        enableButton(false);

        QWidget *placeholder = m_pages[m_currentIdx];
        if (!placeholder) {
            qWarning() << "[WARNING] No placeholder for page ID:" << m_currentIdx;
            enableButton(true);
            return;
        }

        // 清理目标 placeholder 内旧的控件
        QLayout *layout = placeholder->layout();
        if (!layout) {
            layout = new QVBoxLayout(placeholder);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->setSpacing(0);
        } else {
            while (QLayoutItem* item = layout->takeAt(0)) {
                if (QWidget* widget = item->widget()) {
                    widget->deleteLater();
                }
                delete item;
            }
        }
        placeholder = m_pages[id];
        layout = placeholder->layout();

        // 创建新页面
        int beginIndex = id * 10 + 1;
        QWidget *realPage = createPage(beginIndex);
        if (!realPage) {
            qWarning() << "[WARNING] Failed to create repo page at index:" << id;
        } else {
            layout->addWidget(realPage);
        }

        ui->stackedWidget->slideInIdx(id);
        m_currentIdx = id;

        STREAM_INFO() << "切换到 " << m_buttonGroup->button(id)->text().toStdString();
    });
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
            int index = row * 3 + col + 41; // 从索引 41 开始
            auto widget = new MVBlockWidget(ui->live_scene_grid_widget);
            widget->setCoverPix(this->m_total[index].pixPath); ///< 设置封面
            widget->setTitle(this->m_total[index].title);      ///< 设置标题
            widget->hideDesc();                                ///< 隐藏描述
            layout->addWidget(widget, row, col);               ///< 插入布局中
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
            int index = row * 3 + col + 51; // 从索引 51 开始
            auto widget = new MVBlockWidget(ui->honor_of_kings_grid_widget);
            widget->setCoverPix(this->m_total[index].pixPath); ///< 设置封面
            widget->setTitle(this->m_total[index].title);      ///< 设置标题
            widget->hideDesc();                                ///< 隐藏描述
            layout->addWidget(widget, row, col);
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
            int index = row * 3 + col + 61; // 从索引 61 开始
            auto widget = new MVBlockWidget(ui->award_ceremony_grid_widget);
            widget->setCoverPix(this->m_total[index].pixPath); ///< 设置封面
            widget->setTitle(this->m_total[index].title);      ///< 设置标题
            widget->hideDesc();                                ///< 隐藏描述
            layout->addWidget(widget, row, col);
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
            int index = row * 3 + col + 71; // 从索引 71 开始
            auto widget = new MVBlockWidget(ui->hot_MV_grid_widget);
            widget->setCoverPix(this->m_total[index].pixPath); ///< 设置封面
            widget->setTitle(this->m_total[index].title);      ///< 设置标题
            widget->hideDesc();                                ///< 隐藏描述
            layout->addWidget(widget, row, col);
        }
    }
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

void MVWidget::enableButton(const bool &flag) const {
    ui->recommend_pushButton->setEnabled(flag);
    ui->chinese_pushButton->setEnabled(flag);
    ui->koreaAndJapan_pushButton->setEnabled(flag);
    ui->west_pushButton->setEnabled(flag);
}

/**
 * @brief 初始化界面
 */
void MVWidget::initUi()
{
    m_refreshMask->keepLoading();
    const auto future = Async::runAsync(QThreadPool::globalInstance(), [this] {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/title.json"));
        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << "Could not open file for reading title.json";
            STREAM_WARN() << "Could not open file for reading title.json";
            return true;
        }
        auto obj = QJsonDocument::fromJson(file.readAll());
        auto arr = obj.array();
        for (const auto &item : arr)
        {
            QString title = item.toObject().value("title").toString();
            this->m_titleAndDesc.emplace_back(title, parseTitle(title)); ///< 解析标题，感觉有点浪费
        }
        file.close();

        std::sort(m_titleAndDesc.begin(), m_titleAndDesc.end());
        auto last = std::unique(m_titleAndDesc.begin(), m_titleAndDesc.end());
        m_titleAndDesc.erase(last, m_titleAndDesc.end());

        for (int i = 1; i <= 100; i++)
        {
            this->m_total.emplace_back(
                QString(":/RectCover/Res/rectcover/music-rect-cover%1.jpg").arg(i),
                m_titleAndDesc[i].first,
                m_titleAndDesc[i].second); ///< 添加音乐信息
        }

        return true;
    });
    Async::onResultReady(future, this, [this](bool flag) {
        QTimer::singleShot(300, this, [this] {initButtonGroup();});         ///< 初始化按钮组
        QTimer::singleShot(400, this, [this] {initLiveScene();});           ///< 初始化直播场景
        QTimer::singleShot(500, this, [this] {initHonorOfKings();});        ///< 初始化王者荣耀
        QTimer::singleShot(600, this, [this] {initAwardCeremony();});       ///< 初始化颁奖典礼
        QTimer::singleShot(700, this, [this] {initHotMV();});               ///< 初始化热门 MV
        QTimer::singleShot(800,this,[this] {m_refreshMask->hideLoading("");});

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

        ui->recommend_pushButton->click(); ///< 默认触发推荐按钮
    });

    initAdvertiseWidget();     ///< 初始化滑动广告
}

void MVWidget::initAdvertiseWidget() const {
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/1.jpg"))); ///< 添加广告图片
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/2.jpg")));
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/3.jpg")));
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/4.jpg")));
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/5.jpg")));
    ui->advertise_widget->addImage(QPixmap(QStringLiteral(":/MVPoster/Res/mvposter/6.jpg")));
    ui->advertise_widget->setCurrentIndex(0); ///< 设置初始索引
    ui->advertise_widget->adjustSize();       ///< 调整大小
    ui->advertise_widget->setAutoSlide(4000); ///< 设置自动轮播
    ui->advertise_widget->setContentsMargins(0, 0, 0, 0); ///< 设置边距
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

void MVWidget::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    ui->advertise_widget->setFixedHeight(ui->advertise_widget->width() / 5 + 65); ///< 调整广告高度
    m_refreshMask->setGeometry(rect());
    m_refreshMask->raise();  // 确保遮罩在最上层
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