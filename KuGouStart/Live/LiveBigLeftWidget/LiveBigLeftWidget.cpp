/**
 * @file LiveBigLeftWidget.cpp
 * @brief 实现 LiveBigLeftWidget 类，提供直播左侧大控件功能
 * @author WeiWang
 * @date 2025-02-17
 * @version 1.0
 */

#include "LiveBigLeftWidget.h"
#include "ui_LiveBigLeftWidget.h"
#include "Async.h"
#include "logger.hpp"
#include "ElaMessageBar.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <random>
#include <QDir>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化直播左侧大控件
 * @param parent 父控件指针，默认为 nullptr
 */
LiveBigLeftWidget::LiveBigLeftWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LiveBigLeftWidget)
{
    ui->setupUi(this);                                   ///< 初始化 UI
    QFile file(GET_CURRENT_DIR + QStringLiteral("/bigleft.css")); ///< 加载样式表
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
LiveBigLeftWidget::~LiveBigLeftWidget()
{
    delete ui;
}

/**
 * @brief 设置标题
 * @param name 标题文本
 */
void LiveBigLeftWidget::setTitleName(const QString &name) const
{
    ui->title_label->setText(name);                      ///< 设置标题文本
}

/**
 * @brief 设置无提示标签
 * @note 隐藏所有控件的提示标签
 */
void LiveBigLeftWidget::setNoTipLab() const
{
    ui->left_widget->setNoTipLab();                      ///< 隐藏左控件提示
    ui->widget_1->setNoTipLab();                         ///< 隐藏控件1提示
    ui->widget_2->setNoTipLab();                         ///< 隐藏控件2提示
    ui->widget_3->setNoTipLab();                         ///< 隐藏控件3提示
    ui->widget_4->setNoTipLab();                         ///< 隐藏控件4提示
    ui->widget_5->setNoTipLab();                         ///< 隐藏控件5提示
    ui->widget_6->setNoTipLab();                         ///< 隐藏控件6提示
    ui->widget_7->setNoTipLab();                         ///< 隐藏控件7提示
    ui->widget_8->setNoTipLab();                         ///< 隐藏控件8提示
}

/**
 * @brief 初始化界面
 * @note 设置按钮图标并异步加载 JSON 数据
 */
void LiveBigLeftWidget::initUi()
{
    const auto leftLabImgPath = ":/Live/Res/live/left.svg"; ///< 左按钮图标
    ui->left_label->setStyleSheet(QString("border-image:url('%1');").arg(leftLabImgPath)); ///< 设置左按钮样式
    const auto rightLabImgPath = ":/Live/Res/live/right.svg"; ///< 右按钮图标
    ui->right_label->setStyleSheet(QString("border-image:url('%1');").arg(rightLabImgPath)); ///< 设置右按钮样式
    ui->left_label->installEventFilter(this);            ///< 安装左按钮事件过滤器
    ui->right_label->installEventFilter(this);           ///< 安装右按钮事件过滤器
    QString jsonPath = GET_CURRENT_DIR + QStringLiteral("/../text.json"); ///< JSON 文件路径
    const auto future = Async::runAsync(QThreadPool::globalInstance(), &LiveBigLeftWidget::parseJsonFile, this, jsonPath); ///< 异步解析 JSON
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
    ui->widget_4->hide();                                ///< 初始隐藏控件4
    ui->widget_8->hide();                                ///< 初始隐藏控件8
}

/**
 * @brief 解析 JSON 文件
 * @param filePath 文件路径
 * @return 文本列表
 */
QList<QString> LiveBigLeftWidget::parseJsonFile(const QString &filePath)
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
 * @note 设置封面比例、文本和随机图片
 */
void LiveBigLeftWidget::initBlockWidget()
{
    ui->left_widget->setCoverHeightExpanding();          ///< 设置封面高度扩展
    ui->left_widget->setAspectRatio(ui->left_widget->width() * 1.0 / ui->left_widget->height()); ///< 设置宽高比
    ui->left_widget->setCircleStander(120);              ///< 设置圆形标准
    ui->left_widget->setLeftPopularBtnFontSize(14, true); ///< 设置流行按钮字体
    ui->left_widget->setLeftBottomText(this->m_leftBottomTextVec[5]); ///< 设置底部文本
    ui->left_widget->setTipStyleSheet(QStringLiteral("font-size: 12px;border-radius:10px;background-color:black;color:white;height: 30px;")); ///< 设置提示样式
    ui->left_widget->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg")
                                     .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover"))))); ///< 设置随机封面
    ui->widget_1->setCoverHeightExpanding();
    ui->widget_1->setAspectRatio(0.78);
    ui->widget_1->setLeftBottomText(this->m_leftBottomTextVec[10]);
    ui->widget_1->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg")
                                  .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover")))));
    ui->widget_2->setCoverHeightExpanding();
    ui->widget_2->setAspectRatio(0.78);
    ui->widget_2->setLeftBottomText(this->m_leftBottomTextVec[15]);
    ui->widget_2->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg")
                                  .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover")))));
    ui->widget_3->setCoverHeightExpanding();
    ui->widget_3->setAspectRatio(0.78);
    ui->widget_3->setLeftBottomText(this->m_leftBottomTextVec[20]);
    ui->widget_3->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg")
                                  .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover")))));
    ui->widget_4->setCoverHeightExpanding();
    ui->widget_4->setAspectRatio(0.78);
    ui->widget_4->setLeftBottomText(this->m_leftBottomTextVec[25]);
    ui->widget_4->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg")
                                  .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover")))));
    ui->widget_5->setCoverHeightExpanding();
    ui->widget_5->setAspectRatio(0.78);
    ui->widget_5->setLeftBottomText(this->m_leftBottomTextVec[30]);
    ui->widget_5->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg")
                                  .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover")))));
    ui->widget_6->setCoverHeightExpanding();
    ui->widget_6->setAspectRatio(0.78);
    ui->widget_6->setLeftBottomText(this->m_leftBottomTextVec[35]);
    ui->widget_6->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg")
                                  .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover")))));
    ui->widget_7->setCoverHeightExpanding();
    ui->widget_7->setAspectRatio(0.78);
    ui->widget_7->setLeftBottomText(this->m_leftBottomTextVec[40]);
    ui->widget_7->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg")
                                  .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover")))));
    ui->widget_8->setCoverHeightExpanding();
    ui->widget_8->setAspectRatio(0.78);
    ui->widget_8->setLeftBottomText(this->m_leftBottomTextVec[45]);
    ui->widget_8->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg")
                                  .arg(QString::number(QRandomGenerator::global()->bounded(1, getFileCount(GET_CURRENT_DIR + "/../../Res_Qrc/Res/standcover")))));
}

/**
 * @brief 获取目录文件数量
 * @param folderPath 目录路径
 * @return 文件数量
 */
int LiveBigLeftWidget::getFileCount(const QString &folderPath)
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
void LiveBigLeftWidget::on_all_pushButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               QString("暂无更多 %1").arg(ui->title_label->text()), 1000, this->window()); ///< 显示提示
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件
 * @note 动态显示或隐藏控件
 */
void LiveBigLeftWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);                         ///< 调用父类事件
    if (this->width() > 1200)
    {
        ui->widget_4->show();                            ///< 显示控件4
        ui->widget_8->show();                            ///< 显示控件8
    }
    else
    {
        ui->widget_4->hide();                            ///< 隐藏控件4
        ui->widget_8->hide();                            ///< 隐藏控件8
    }
}

/**
 * @brief 事件过滤器
 * @param watched 监视的对象
 * @param event 事件
 * @return 是否处理事件
 * @note 处理左右按钮点击事件
 */
bool LiveBigLeftWidget::eventFilter(QObject *watched, QEvent *event) {
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