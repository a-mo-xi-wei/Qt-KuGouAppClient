/**
 * @file LiveMusicPartWidget.cpp
 * @brief 实现 LiveMusicPartWidget 类，提供直播音乐部分控件功能
 * @author WeiWang
 * @date 2025-02-17
 * @version 1.0
 */

#include "LiveMusicPartWidget.h"
#include "ui_LiveMusicPartWidget.h"
#include "Async.h"
#include "logger.hpp"
#include "ElaMessageBar.h"
#include "ElaToolTip.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <random>
#include <QRandomGenerator>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化直播音乐部分控件
 * @param parent 父控件指针，默认为 nullptr
 */
LiveMusicPartWidget::LiveMusicPartWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LiveMusicPartWidget)
{
    ui->setupUi(this);                                   ///< 初始化 UI
    QFile file(GET_CURRENT_DIR + QStringLiteral("/music.css")); ///< 加载样式表
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
}

/**
 * @brief 析构函数，清理资源
 */
LiveMusicPartWidget::~LiveMusicPartWidget()
{
    delete ui;
}

/**
 * @brief 设置标题
 * @param name 标题文本
 */
void LiveMusicPartWidget::setTitleName(const QString &name) const
{
    ui->title_label->setText(name);                      ///< 设置标题文本
}

/**
 * @brief 初始化界面
 * @note 设置工具提示、按钮图标并异步加载 JSON 数据
 */
void LiveMusicPartWidget::initUi()
{
    // 设置工具提示
    auto title_label_1_toolTip = new ElaToolTip(ui->title_label_1); ///< 创建标题1工具提示
    title_label_1_toolTip->setToolTip(ui->title_label_1->text()); ///< 设置标题1提示文本
    auto desc_label_1_toolTip = new ElaToolTip(ui->desc_label_1); ///< 创建描述1工具提示
    desc_label_1_toolTip->setToolTip(ui->desc_label_1->text()); ///< 设置描述1提示文本
    auto title_label_2_toolTip = new ElaToolTip(ui->title_label_2); ///< 创建标题2工具提示
    title_label_2_toolTip->setToolTip(ui->title_label_2->text()); ///< 设置标题2提示文本
    auto desc_label_2_toolTip = new ElaToolTip(ui->desc_label_2); ///< 创建描述2工具提示
    desc_label_2_toolTip->setToolTip(ui->desc_label_2->text()); ///< 设置描述2提示文本
    auto desc_label_3_toolTip = new ElaToolTip(ui->desc_label_3); ///< 创建描述3工具提示
    desc_label_3_toolTip->setToolTip(ui->desc_label_3->text()); ///< 设置描述3提示文本
    auto title_label_4_toolTip = new ElaToolTip(ui->title_label_4); ///< 创建标题4工具提示
    title_label_4_toolTip->setToolTip(ui->title_label_4->text()); ///< 设置标题4提示文本
    auto desc_label_4_toolTip = new ElaToolTip(ui->desc_label_4); ///< 创建描述4工具提示
    desc_label_4_toolTip->setToolTip(ui->desc_label_4->text()); ///< 设置描述4提示文本
    // 设置按钮图标
    const auto leftLabImgPath = ":/Live/Res/live/left.svg"; ///< 左按钮图标
    ui->left_label->setStyleSheet(QString("border-image:url('%1');").arg(leftLabImgPath)); ///< 设置左按钮样式
    const auto rightLabImgPath = ":/Live/Res/live/right.svg"; ///< 右按钮图标
    ui->right_label->setStyleSheet(QString("border-image:url('%1');").arg(rightLabImgPath)); ///< 设置右按钮样式
    ui->left_label->installEventFilter(this);            ///< 安装左按钮事件过滤器
    ui->right_label->installEventFilter(this);           ///< 安装右按钮事件过滤器
    // 异步解析 JSON 文件
    QString jsonPath = GET_CURRENT_DIR + QStringLiteral("/../text.json"); ///< JSON 文件路径
    const auto future = Async::runAsync(QThreadPool::globalInstance(), &LiveMusicPartWidget::parseJsonFile, this, jsonPath); ///< 异步解析 JSON
    Async::onResultReady(future, this, [=](const QList<QString> &texts) {
        if (texts.isEmpty())
        {
            qWarning() << "No valid data parsed from JSON";
            STREAM_WARN() << "No valid data parsed from JSON"; ///< 记录警告日志
            return;
        }
        this->m_leftBottomTextVec = texts;               ///< 更新文本列表
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(this->m_leftBottomTextVec.begin(), this->m_leftBottomTextVec.end(), std::default_random_engine(seed)); ///< 打乱文本顺序
        initBlockWidget();                               ///< 初始化块控件
    });
    // 设置文本可选择
    ui->desc_label_1->setTextInteractionFlags(Qt::TextSelectableByMouse); ///< 描述1可选择
    ui->desc_label_2->setTextInteractionFlags(Qt::TextSelectableByMouse); ///< 描述2可选择
    ui->desc_label_3->setTextInteractionFlags(Qt::TextSelectableByMouse); ///< 描述3可选择
    ui->desc_label_4->setTextInteractionFlags(Qt::TextSelectableByMouse); ///< 描述4可选择
    ui->title_label_1->setTextInteractionFlags(Qt::TextSelectableByMouse); ///< 标题1可选择
    ui->title_label_2->setTextInteractionFlags(Qt::TextSelectableByMouse); ///< 标题2可选择
    ui->title_label_3->setTextInteractionFlags(Qt::TextSelectableByMouse); ///< 标题3可选择
    ui->title_label_4->setTextInteractionFlags(Qt::TextSelectableByMouse); ///< 标题4可选择
    ui->widget_4->hide();                                ///< 初始隐藏控件4
}

/**
 * @brief 解析 JSON 文件
 * @param filePath 文件路径
 * @return 文本列表
 */
QList<QString> LiveMusicPartWidget::parseJsonFile(const QString &filePath)
{
    QList<QString> texts;                                ///< 文本列表
    QFile file(filePath);                                ///< 打开 JSON 文件
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Failed to open JSON file:" << filePath;
        STREAM_WARN() << "Failed to open JSON file:" << filePath.toStdString(); ///< 记录警告日志
        return texts;
    }
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError); ///< 解析 JSON
    if (parseError.error != QJsonParseError::NoError)
    {
        qWarning() << "JSON parse error:" << parseError.errorString();
        STREAM_WARN() << "JSON parse error:" << parseError.errorString().toStdString(); ///< 记录错误日志
        return texts;
    }
    QJsonArray arr = doc.array();
    for (const auto &item : arr)
    {
        QString text = item.toObject().value("text").toString();
        texts.append(text);                              ///< 添加文本
    }
    file.close();
    return texts;
}

/**
 * @brief 初始化块控件
 * @note 设置封面、文本和提示
 */
void LiveMusicPartWidget::initBlockWidget()
{
    ui->cover_label_1->setStyleSheet(QString("border-image: url(':/BlockCover/Res/blockcover/music-block-cover%1.jpg')")
                                         .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/blockcover"))))); ///< 设置封面1
    ui->left_block_widget_1->setLeftBottomText(this->m_leftBottomTextVec[10]); ///< 设置左块1文本
    ui->left_block_widget_1->setTipLabText(QString::number(QRandomGenerator::global()->bounded(5000))); ///< 设置左块1提示
    ui->left_block_widget_1->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg")
                                             .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover"))))); ///< 设置左块1封面
    ui->right_block_widget_1->setLeftBottomText(this->m_leftBottomTextVec[11]); ///< 设置右块1文本
    ui->right_block_widget_1->setTipLabText(QString::number(QRandomGenerator::global()->bounded(5000))); ///< 设置右块1提示
    ui->right_block_widget_1->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg")
                                              .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover"))))); ///< 设置右块1封面
    ui->cover_label_2->setStyleSheet(QString("border-image: url(':/BlockCover/Res/blockcover/music-block-cover%1.jpg')")
                                         .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/blockcover"))))); ///< 设置封面2
    ui->left_block_widget_2->setLeftBottomText(this->m_leftBottomTextVec[20]); ///< 设置左块2文本
    ui->left_block_widget_2->setTipLabText(QString::number(QRandomGenerator::global()->bounded(5000))); ///< 设置左块2提示
    ui->left_block_widget_2->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg")
                                             .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover"))))); ///< 设置左块2封面
    ui->right_block_widget_2->setLeftBottomText(this->m_leftBottomTextVec[21]); ///< 设置右块2文本
    ui->right_block_widget_2->setTipLabText(QString::number(QRandomGenerator::global()->bounded(5000))); ///< 设置右块2提示
    ui->right_block_widget_2->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg")
                                              .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover"))))); ///< 设置右块2封面
    ui->cover_label_3->setStyleSheet(QString("border-image: url(':/BlockCover/Res/blockcover/music-block-cover%1.jpg')")
                                         .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/blockcover"))))); ///< 设置封面3
    ui->left_block_widget_3->setLeftBottomText(this->m_leftBottomTextVec[30]); ///< 设置左块3文本
    ui->left_block_widget_3->setTipLabText(QString::number(QRandomGenerator::global()->bounded(5000))); ///< 设置左块3提示
    ui->left_block_widget_3->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg")
                                             .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover"))))); ///< 设置左块3封面
    ui->right_block_widget_3->setLeftBottomText(this->m_leftBottomTextVec[31]); ///< 设置右块3文本
    ui->right_block_widget_3->setTipLabText(QString::number(QRandomGenerator::global()->bounded(5000))); ///< 设置右块3提示
    ui->right_block_widget_3->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg")
                                              .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover"))))); ///< 设置右块3封面
    ui->cover_label_4->setStyleSheet(QString("border-image: url(':/BlockCover/Res/blockcover/music-block-cover%1.jpg')")
                                         .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/blockcover"))))); ///< 设置封面4
    ui->left_block_widget_4->setLeftBottomText(this->m_leftBottomTextVec[40]); ///< 设置左块4文本
    ui->left_block_widget_4->setTipLabText(QString::number(QRandomGenerator::global()->bounded(5000))); ///< 设置左块4提示
    ui->left_block_widget_4->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg")
                                             .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover"))))); ///< 设置左块4封面
    ui->right_block_widget_4->setLeftBottomText(this->m_leftBottomTextVec[41]); ///< 设置右块4文本
    ui->right_block_widget_4->setTipLabText(QString::number(QRandomGenerator::global()->bounded(5000))); ///< 设置右块4提示
    ui->right_block_widget_4->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg")
                                              .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover"))))); ///< 设置右块4封面
}

/**
 * @brief 获取目录文件数量
 * @param folderPath 目录路径
 * @return 文件数量
 */
int LiveMusicPartWidget::getFileCount(const QString &folderPath)
{
    QDir dir(folderPath);                                ///< 创建目录对象
    if (!dir.exists())
    {
        qWarning("目录不存在: %s", qPrintable(folderPath));
        PRINT_WARN("目录不存在: %s", folderPath.toStdString()); ///< 记录警告日志
        return 0;
    }
    const auto filters = QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot; ///< 设置过滤器
    const int fileCount = static_cast<int>(dir.entryList(filters, QDir::Name).size()); ///< 获取文件数量
    return fileCount;
}

/**
 * @brief 全部按钮点击槽函数
 * @note 显示暂无更多提示
 */
void LiveMusicPartWidget::on_all_pushButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("暂无更多 %1").arg(ui->title_label->text()), 1000, this->window()); ///< 显示提示
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件
 * @note 动态显示或隐藏控件
 */
void LiveMusicPartWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);                         ///< 调用父类事件
    if (this->width() > 1200)
    {
        ui->widget_4->show();                            ///< 显示控件4
    }
    else
    {
        ui->widget_4->hide();                            ///< 隐藏控件4
    }
}

/**
 * @brief 事件过滤器
 * @param watched 监视的对象
 * @param event 事件
 * @return 是否处理事件
 * @note 处理左右按钮点击事件
 */

bool LiveMusicPartWidget::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->left_label) {
        if (event->type() == QEvent::MouseButtonPress) {
            ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                        QString("暂无更多 %1").arg(ui->title_label->text()),1000,this->window()); ///< 显示左按钮提示
        }
    }
    if (watched == ui->right_label) {
        if (event->type() == QEvent::MouseButtonPress) {
            ElaMessageBar::information(ElaMessageBarType::BottomRight,"Info",
                        QString("暂无更多 %1").arg(ui->title_label->text()),1000,this->window()); ///< 显示右按钮提示
    }
    }
    return QWidget::eventFilter(watched, event);
}