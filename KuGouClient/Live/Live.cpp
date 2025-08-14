/**
 * @file Live.cpp
 * @brief 实现 Live 类，提供直播主界面功能
 * @author WeiWang
 * @date 2025-02-17
 * @version 1.0
 */

#include "Live.h"
#include "ui_Live.h"
#include "MyScrollArea.h"
#include "logger.hpp"
#include "ElaMessageBar.h"
#include "RefreshMask.h"

#include <QButtonGroup>
#include <QDir>
#include <QFile>
#include <QPainter>
#include <QPainterPath>
#include <QQueue>
#include <QRandomGenerator>
#include <QScrollBar>
#include <QTimer>
#include <QWheelEvent>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 获取目录文件数量
 * @param folderPath 目录路径
 * @return 文件数量
 */
static int getFileCount(const QString &folderPath)
{
    QDir dir(folderPath); ///< 创建目录对象
    if (!dir.exists()) {
        qWarning("目录不存在: %s", qPrintable(folderPath));
        PRINT_WARN("目录不存在: %s", folderPath.toStdString()); ///< 记录警告日志
        return 0;
    }
    const auto filters = QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot;        ///< 设置过滤器
    const int fileCount = static_cast<int>(dir.entryList(filters, QDir::Name).size()); ///< 获取文件数量
    return fileCount;
}

/**
 * @brief 构造函数，初始化直播主界面
 * @param parent 父控件指针，默认为 nullptr
 */
Live::Live(QWidget *parent)
    : QWidget(parent)
      , ui(new Ui::Live)
      , m_buttonGroup(std::make_unique<QButtonGroup>(this))
      , m_refreshMask(std::make_unique<RefreshMask>(this)) ///< 初始化刷新遮罩
{
    ui->setupUi(this);                                         ///< 初始化 UI
    QFile file(GET_CURRENT_DIR + QStringLiteral("/live.css")); ///< 加载样式表
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll()); ///< 应用样式表
    } else {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ"; ///< 记录错误日志
        return;
    }
    QTimer::singleShot(0, this, [this] { initButtonGroup(); }); ///< 初始化按钮组
    QTimer::singleShot(100, this, [this] { initUi(); });
}

/**
 * @brief 析构函数，清理资源
 */
Live::~Live()
{
    delete ui;
}

/**
 * @brief 初始化按钮组
 * @note 设置按钮互斥
 */
void Live::initButtonGroup() const
{
    this->m_buttonGroup->addButton(ui->popular_pushButton);      ///< 添加热门按钮
    this->m_buttonGroup->addButton(ui->attention_pushButton);    ///< 添加关注按钮
    this->m_buttonGroup->addButton(ui->recommend_pushButton);    ///< 添加推荐按钮
    this->m_buttonGroup->addButton(ui->music_pushButton);        ///< 添加音乐按钮
    this->m_buttonGroup->addButton(ui->new_star_pushButton);     ///< 添加新秀按钮
    this->m_buttonGroup->addButton(ui->appearance_pushButton);   ///< 添加颜值按钮
    this->m_buttonGroup->addButton(ui->dance_pushButton);        ///< 添加舞蹈按钮
    this->m_buttonGroup->addButton(ui->barrage_game_pushButton); ///< 添加弹幕游戏按钮
    this->m_buttonGroup->setExclusive(true);                     ///< 设置互斥
}

/**
 * @brief 初始化界面
 * @note 初始化子控件和布局
 */
void Live::initUi()
{
    ui->guide_widget->setStyleSheet("font-family: 'TaiwanPearl';");
    ui->attention_guide_widget->setStyleSheet("font-family: 'TaiwanPearl';");
    ui->empty_text_label->setStyleSheet("font-family: 'TaiwanPearl';");
    m_refreshMask->keepLoading();

    using Task = std::function<void()>;
    QVector<Task> tasks;

    tasks << [this] { initPopularWidget(); };
    tasks << [this] { initAttentionWidget(); };
    tasks << [this] { initRecommendWidget(); };
    tasks << [this] { initMusicWidget(); };
    tasks << [this] { initNewStarWidget(); };
    tasks << [this] { initAppearanceWidget(); };
    tasks << [this] { initDanceWidget(); };
    tasks << [this] { initGameWidget(); };
    tasks << [this] {
        // 绑定按钮和信号槽
        auto vScrollBar = ui->scrollArea->verticalScrollBar();

        auto connectButton1 = [this](const QPushButton *button, QWidget *targetWidget) {
            connect(button,
                    &QPushButton::clicked,
                    this,
                    [this, targetWidget] {
                        ui->scrollArea->smoothScrollTo(targetWidget->mapToParent(QPoint(0, 0)).y());
                    });
        };
        auto connectButton2 = [this](const QPushButton *button, QWidget *targetWidget) {
            if (!targetWidget) {
                qWarning() << "targetWidget is null for button" << button->objectName();
                return;
            }
            connect(button,
                    &QPushButton::clicked,
                    this,
                    [this, targetWidget] {
                        ui->scrollArea->smoothScrollTo(
                            targetWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y());
                    });
        };

        connectButton1(ui->popular_pushButton, ui->popular_widget);
        connectButton1(ui->attention_pushButton, ui->attention_widget);
        connectButton2(ui->recommend_pushButton, this->m_recommendWidget.get());
        connectButton2(ui->music_pushButton, this->m_musicWidget.get());
        connectButton2(ui->new_star_pushButton, this->m_newStarWidget.get());
        connectButton2(ui->appearance_pushButton, this->m_appearanceWidget.get());
        connectButton2(ui->dance_pushButton, this->m_danceWidget.get());
        connectButton2(ui->barrage_game_pushButton, this->m_gameWidget.get());

        connect(ui->scrollArea, &MyScrollArea::wheelValue, this, &Live::handleWheelValue);
        connect(vScrollBar, &QScrollBar::valueChanged, this, &Live::handleWheelValue);

        auto lay = dynamic_cast<QVBoxLayout *>(ui->table_widget->layout());
        if (!lay) {
            qWarning() << "布局不存在";
            STREAM_WARN() << "布局不存在";
            return;
        }
        lay->insertWidget(lay->count() - 1, this->m_recommendWidget.get());
        lay->insertWidget(lay->count() - 1, this->m_musicWidget.get());
        lay->insertWidget(lay->count() - 1, this->m_newStarWidget.get());
        lay->insertWidget(lay->count() - 1, this->m_appearanceWidget.get());
        lay->insertWidget(lay->count() - 1, this->m_danceWidget.get());
        lay->insertWidget(lay->count() - 1, this->m_gameWidget.get());

        m_refreshMask->hideLoading("");
        QMetaObject::invokeMethod(this, "emitInitialized", Qt::QueuedConnection);
    };

    auto queue = std::make_shared<QQueue<Task>>();
    for (const auto &task : tasks)
        queue->enqueue(task);

    auto runner = std::make_shared<std::function<void()>>();
    *runner = [queue, runner]() {
        if (queue->isEmpty())
            return;

        auto task = queue->dequeue();
        QTimer::singleShot(0,
                           nullptr,
                           [task, runner]() {
                               task();
                               (*runner)();
                           });
    };

    (*runner)();
}

/**
 * @brief 初始化热门控件
 * @note 设置按钮和标签
 */
void Live::initPopularWidget()
{
    const auto group = new QButtonGroup(this); ///< 创建按钮组
    group->addButton(ui->toolButton_1);        ///< 添加按钮1
    group->addButton(ui->toolButton_2);        ///< 添加按钮2
    group->addButton(ui->toolButton_3);        ///< 添加按钮3
    group->setExclusive(true);                 ///< 设置互斥
    const QString descArr[] = {"HS一白月光", "cy菜菜", "乔希玥",
                               "涉外北北同学", "优优luck", "多肉小甜豆", "ZY佳美", "露娜6",
                               "滚滚d", "YE茜茜", "Msn新人星语", "Mor阿满", "BE佳琳y",
                               "jy十一", "优优luck", "小圆OO", "90卿卿", "新人富贵", "90清清",
                               "初夏y2", "ke乐乐", "驴十三", "姜妧", "紫霞", "驴鹏", "刘诗诗v"
    };                                                                            ///< 文本数组
    const auto idx = QRandomGenerator::global()->bounded(0, descArr->size() - 3); ///< 随机索引
    ui->toolButton_1->setLeftBottomText(descArr[idx]);                            ///< 设置按钮1文本
    ui->toolButton_2->setLeftBottomText(descArr[idx + 1]);                        ///< 设置按钮2文本
    ui->toolButton_3->setLeftBottomText(descArr[idx + 2]);                        ///< 设置按钮3文本
    ui->toolButton_1->setToolButtonStyle(Qt::ToolButtonIconOnly);                 ///< 设置图标样式
    ui->toolButton_2->setToolButtonStyle(Qt::ToolButtonIconOnly);                 ///< 设置图标样式
    ui->toolButton_3->setToolButtonStyle(Qt::ToolButtonIconOnly);                 ///< 设置图标样式
    ui->toolButton_1->setBackgroundImg(QString(":/RectCover/Res/rectcover/music-rect-cover%1.jpg")
        .arg(QString::number(
            QRandomGenerator::global()->bounded(1,
                                                getFileCount(
                                                    GET_CURRENT_DIR +
                                                    "/../../Res_Qrc/Res/rectcover")))));
    ///< 设置按钮1背景
    ui->toolButton_2->setBackgroundImg(QString(":/RectCover/Res/rectcover/music-rect-cover%1.jpg")
        .arg(QString::number(
            QRandomGenerator::global()->bounded(1,
                                                getFileCount(
                                                    GET_CURRENT_DIR +
                                                    "/../../Res_Qrc/Res/rectcover")))));
    ///< 设置按钮2背景
    ui->toolButton_3->setBackgroundImg(QString(":/RectCover/Res/rectcover/music-rect-cover%1.jpg")
        .arg(QString::number(
            QRandomGenerator::global()->bounded(1,
                                                getFileCount(
                                                    GET_CURRENT_DIR +
                                                    "/../../Res_Qrc/Res/rectcover")))));
    ///< 设置按钮3背景
    ui->index_label_1->setStyleSheet("background-color: rgba(0,0,0,0);border: none;"); ///< 设置标签1样式
    ui->index_label_1->setPixmap(QPixmap(":Live/Res/live/arrow-left.svg"));            ///< 设置标签1图标
    ui->index_label_1->setFixedSize(20, 30);                                           ///< 设置标签1大小
    ui->index_label_1->show();                                                         ///< 显示标签1
    ui->index_label_2->setStyleSheet("background-color: rgba(0,0,0,0);border: none;"); ///< 设置标签2样式
    ui->index_label_2->setPixmap(QPixmap(":Live/Res/live/arrow-left.svg"));            ///< 设置标签2图标
    ui->index_label_2->setFixedSize(20, 30);                                           ///< 设置标签2大小
    ui->index_label_2->hide();                                                         ///< 隐藏标签2
    ui->index_label_3->setStyleSheet("background-color: rgba(0,0,0,0);border: none;"); ///< 设置标签3样式
    ui->index_label_3->setPixmap(QPixmap(":Live/Res/live/arrow-left.svg"));            ///< 设置标签3图标
    ui->index_label_3->setFixedSize(20, 30);                                           ///< 设置标签3大小
    ui->index_label_3->hide();                                                         ///< 隐藏标签3
    connect(ui->toolButton_1,
            &QToolButton::toggled,
            [this] {
                ui->index_label_1->setPixmap(QPixmap(":Live/Res/live/arrow-left.svg")); ///< 更新标签1图标
                ui->index_label_2->setPixmap(QPixmap());                                ///< 清空标签2图标
                ui->index_label_3->setPixmap(QPixmap());                                ///< 清空标签3图标
                ui->index_label_1->show();                                              ///< 显示标签1
                ui->index_label_2->hide();                                              ///< 隐藏标签2
                ui->index_label_3->hide();                                              ///< 隐藏标签3
            });
    connect(ui->toolButton_2,
            &QToolButton::toggled,
            [this] {
                ui->index_label_1->setPixmap(QPixmap());                                ///< 清空标签1图标
                ui->index_label_2->setPixmap(QPixmap(":Live/Res/live/arrow-left.svg")); ///< 更新标签2图标
                ui->index_label_3->setPixmap(QPixmap());                                ///< 清空标签3图标
                ui->index_label_1->hide();                                              ///< 隐藏标签1
                ui->index_label_2->show();                                              ///< 显示标签2
                ui->index_label_3->hide();                                              ///< 隐藏标签3
            });
    connect(ui->toolButton_3,
            &QToolButton::toggled,
            [this] {
                ui->index_label_1->setPixmap(QPixmap());                                ///< 清空标签1图标
                ui->index_label_2->setPixmap(QPixmap());                                ///< 清空标签2图标
                ui->index_label_3->setPixmap(QPixmap(":Live/Res/live/arrow-left.svg")); ///< 更新标签3图标
                ui->index_label_1->hide();                                              ///< 隐藏标签1
                ui->index_label_2->hide();                                              ///< 隐藏标签2
                ui->index_label_3->show();                                              ///< 显示标签3
            });
}

/**
 * @brief 生成圆角图片
 * @param src 原始图片
 * @param size 目标大小
 * @param radius 圆角半径
 * @return 圆角图片
 */
QPixmap Live::roundedPixmap(const QPixmap &src, const QSize &size, const int &radius)
{
    const QPixmap scaled = src.scaled(size,
                                      Qt::KeepAspectRatioByExpanding,
                                      Qt::SmoothTransformation);            ///< 缩放图片
    QPixmap dest(size);                                                     ///< 创建目标图片
    dest.fill(Qt::transparent);                                             ///< 填充透明背景
    QPainter painter(&dest);                                                ///< 创建画家
    painter.setRenderHint(QPainter::Antialiasing);                          ///< 启用抗锯齿
    QPainterPath path;                                                      ///< 创建路径
    path.addRoundedRect(0, 0, size.width(), size.height(), radius, radius); ///< 添加圆角矩形
    painter.setClipPath(path);                                              ///< 设置裁剪路径
    painter.drawPixmap(0, 0, scaled);                                       ///< 绘制图片
    return dest;
}

/**
 * @brief 初始化关注控件
 * @note 设置头像和按钮
 */
void Live::initAttentionWidget()
{
    const QPixmap roundedPix = roundedPixmap(QPixmap(QStringLiteral(":/Res/window/portrait.jpg")),
                                             ui->portrait_label->size(),
                                             15);                    ///< 生成圆角头像
    ui->portrait_label->setPixmap(roundedPix);                       ///< 设置头像
    const auto group = new QButtonGroup(this);                       ///< 创建按钮组
    group->addButton(ui->pushButton_1);                              ///< 添加按钮1
    group->addButton(ui->pushButton_2);                              ///< 添加按钮2
    group->addButton(ui->pushButton_3);                              ///< 添加按钮3
    group->addButton(ui->pushButton_4);                              ///< 添加按钮4
    group->setExclusive(true);                                       ///< 设置互斥
    ui->empty_label->setFixedSize(390, 230);                         ///< 设置空标签大小
    ui->empty_label->setPixmap(QPixmap(":Live/Res/live/empty.png")); ///< 设置空标签图标
}

/**
 * @brief 初始化推荐控件
 * @note 设置推荐内容
 */
void Live::initRecommendWidget()
{
    const auto lay = static_cast<QVBoxLayout *>(ui->table_widget->layout()); ///< 获取布局
    this->m_recommendWidget = std::make_unique<LiveCommonPartWidget>(ui->table_widget, 2);
    ///< 创建推荐控件
    this->m_recommendWidget->setTitleName("推荐");                        ///< 设置标题
    lay->insertWidget(lay->count() - 1, this->m_recommendWidget.get()); ///< 添加控件
    lay->setStretchFactor(this->m_recommendWidget.get(), 1);            ///< 设置伸缩因子
}

/**
 * @brief 初始化音乐控件
 * @note 设置音乐内容
 */
void Live::initMusicWidget()
{
    const auto lay = static_cast<QVBoxLayout *>(ui->table_widget->layout());       ///< 获取布局
    this->m_musicWidget = std::make_unique<LiveMusicPartWidget>(ui->table_widget); ///< 创建音乐控件
    this->m_musicWidget->setTitleName("音乐");                                       ///< 设置标题
    lay->insertWidget(lay->count() - 1, this->m_musicWidget.get());                ///< 添加控件
    lay->setStretchFactor(this->m_musicWidget.get(), 1);                           ///< 设置伸缩因子
}

/**
 * @brief 初始化新秀控件
 * @note 设置新秀内容
 */
void Live::initNewStarWidget()
{
    const auto lay = static_cast<QVBoxLayout *>(ui->table_widget->layout());             ///< 获取布局
    this->m_newStarWidget = std::make_unique<LiveCommonPartWidget>(ui->table_widget, 1); ///< 创建新秀控件
    this->m_newStarWidget->setTitleName("新秀");                                           ///< 设置标题
    this->m_newStarWidget->setNoTipLab();                                                ///< 隐藏提示标签
    lay->insertWidget(lay->count() - 1, this->m_newStarWidget.get());                    ///< 添加控件
    lay->setStretchFactor(this->m_newStarWidget.get(), 1);                               ///< 设置伸缩因子
}

/**
 * @brief 初始化颜值控件
 * @note 设置颜值内容
 */
void Live::initAppearanceWidget()
{
    const auto lay = static_cast<QVBoxLayout *>(ui->table_widget->layout());          ///< 获取布局
    this->m_appearanceWidget = std::make_unique<LiveBigLeftWidget>(ui->table_widget); ///< 创建颜值控件
    this->m_appearanceWidget->setTitleName("颜值");                                     ///< 设置标题
    lay->insertWidget(lay->count() - 1, this->m_appearanceWidget.get());              ///< 添加控件
    lay->setStretchFactor(this->m_appearanceWidget.get(), 1);                         ///< 设置伸缩因子
}

/**
 * @brief 初始化舞蹈控件
 * @note 设置舞蹈内容
 */
void Live::initDanceWidget()
{
    const auto lay = static_cast<QVBoxLayout *>(ui->table_widget->layout());           ///< 获取布局
    this->m_danceWidget = std::make_unique<LiveCommonPartWidget>(ui->table_widget, 1); ///< 创建舞蹈控件
    this->m_danceWidget->setTitleName("舞蹈");                                           ///< 设置标题
    this->m_danceWidget->setNoTipLab();                                                ///< 隐藏提示标签
    lay->insertWidget(lay->count() - 1, this->m_danceWidget.get());                    ///< 添加控件
    lay->setStretchFactor(this->m_danceWidget.get(), 1);                               ///< 设置伸缩因子
}

/**
 * @brief 初始化游戏控件
 * @note 设置游戏内容
 */
void Live::initGameWidget()
{
    const auto lay = static_cast<QVBoxLayout *>(ui->table_widget->layout());    ///< 获取布局
    this->m_gameWidget = std::make_unique<LiveBigLeftWidget>(ui->table_widget); ///< 创建游戏控件
    this->m_gameWidget->setTitleName("弹幕游戏");                                   ///< 设置标题
    lay->insertWidget(lay->count() - 1, this->m_gameWidget.get());              ///< 添加控件
    lay->setStretchFactor(this->m_gameWidget.get(), 1);                         ///< 设置伸缩因子
}

/**
 * @brief 处理滚动值
 * @param value 滚动值
 * @note 更新按钮状态
 */
void Live::handleWheelValue(const int &value)
{
    const QVector<QPair<QWidget *, QPushButton *>> sectionMappings =
    {
        {ui->popular_widget, ui->popular_pushButton},
        {ui->attention_widget, ui->attention_pushButton},
        {this->m_recommendWidget.get(), ui->recommend_pushButton},
        {this->m_musicWidget.get(), ui->music_pushButton},
        {this->m_newStarWidget.get(), ui->new_star_pushButton},
        {this->m_appearanceWidget.get(), ui->appearance_pushButton},
        {this->m_danceWidget.get(), ui->dance_pushButton},
        {this->m_gameWidget.get(), ui->barrage_game_pushButton}
    };

    // 重置所有按钮的选中状态
    for (const auto &mapping : sectionMappings) {
        mapping.second->setChecked(false);
    }

    // 查找当前滚动位置对应的按钮
    for (int i = 0; i < sectionMappings.size(); ++i) {
        QWidget *currentWidget = sectionMappings[i].first;
        QWidget *nextWidget = (i + 1 < sectionMappings.size())
                                  ? sectionMappings[i + 1].first
                                  : nullptr;

        int currentY = currentWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y();
        int nextY = nextWidget
                        ? nextWidget->mapTo(ui->scrollArea->widget(), QPoint(0, 0)).y()
                        : INT_MAX;

        if (value >= currentY && value < nextY) {
            sectionMappings[i].second->setChecked(true);
            break;
        }
    }
}

/**
 * @brief 全部按钮点击槽函数
 * @note 显示未实现提示
 */
void Live::on_all_toolButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight,
                               "Info",
                               QString("%1 功能未实现 敬请期待").arg(
                                   ui->all_toolButton->text().left(
                                       ui->all_toolButton->text().size() - 2)),
                               1000,
                               this->window()); ///< 显示提示
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件
 * @note 动态调整控件尺寸
 */
void Live::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);                                             ///< 调用父类事件
    ui->popular_widget->setFixedHeight(ui->popular_widget->width() * 2 / 5); ///< 设置热门控件高度
    ui->table_widget->setFixedWidth(this->window()->width() - 50);           ///< 设置表格宽度
    QRect r = rect();
    r.setLeft(r.left() + 10); // 向右移动左边界
    m_refreshMask->setGeometry(r);
    m_refreshMask->raise(); // 确保遮罩在最上层
}

/**
 * @brief 显示事件
 * @param event 显示事件
 * @note 调整控件尺寸
 */
void Live::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);                                               ///< 调用父类事件
    ui->popular_widget->setFixedHeight(ui->popular_widget->width() * 2 / 5); ///< 设置热门控件高度
    ui->table_widget->setFixedWidth(this->window()->width() - 50);           ///< 设置表格宽度
    QRect r = rect();
    r.setLeft(r.left() + 10); // 向右移动左边界
    m_refreshMask->setGeometry(r);
    m_refreshMask->raise(); // 确保遮罩在最上层
}