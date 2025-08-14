/**
 * @file LiveCommonPartWidget.cpp
 * @brief 实现 LiveCommonPartWidget 类，提供直播通用部分控件功能
 * @author WeiWang
 * @date 2025-02-17
 * @version 1.0
 */

#include "LiveCommonPartWidget.h"
#include "ui_LiveCommonPartWidget.h"
#include "LiveBlockWidget/LiveBlockWidget.h"
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
 * @brief 构造函数，初始化直播通用部分控件
 * @param parent 父控件指针，默认为 nullptr
 * @param lines 行数，默认为 1
 */
LiveCommonPartWidget::LiveCommonPartWidget(QWidget *parent, const int lines)
    : QWidget(parent)
      , ui(new Ui::LiveCommonPartWidget)
{
    ui->setupUi(this);                                           ///< 初始化 UI
    QFile file(GET_CURRENT_DIR + QStringLiteral("/common.css")); ///< 加载样式表
    if (file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll()); ///< 应用样式表
    } else {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ"; ///< 记录错误日志
        return;
    }
    initUi(lines); ///< 初始化界面
}

/**
 * @brief 析构函数，清理资源
 */
LiveCommonPartWidget::~LiveCommonPartWidget()
{
    delete ui;
}

/**
 * @brief 设置标题
 * @param name 标题文本
 */
void LiveCommonPartWidget::setTitleName(const QString &name) const
{
    ui->title_label->setText(name); ///< 设置标题文本
}

/**
 * @brief 隐藏提示标签
 * @note 隐藏所有块控件的提示标签
 */
void LiveCommonPartWidget::setNoTipLab()
{
    connect(this,
            &LiveCommonPartWidget::initOK,
            this,
            [this] {
                for (const auto &val : this->m_blockArr) {
                    if (val) {
                        val->setNoTipLab(); ///< 隐藏提示标签
                    }
                }
            });
}

/**
 * @brief 获取目录文件数量
 * @param folderPath 目录路径
 * @return 文件数量
 */
int LiveCommonPartWidget::getFileCount(const QString &folderPath)
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
 * @brief 全部按钮点击槽函数
 * @note 显示暂无更多提示
 */
void LiveCommonPartWidget::on_all_pushButton_clicked()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight,
                               "Info",
                               QString("暂无更多 %1").arg(ui->title_label->text()),
                               1000,
                               this->window()); ///< 显示提示
}

/**
 * @brief 初始化界面
 * @param lines 行数
 * @note 设置按钮图标并异步加载 JSON 数据
 */
void LiveCommonPartWidget::initUi(const int &lines)
{
    const auto leftLabImgPath = ":/Live/Res/live/left.svg"; ///< 左按钮图标
    ui->left_label->setStyleSheet(QString("border-image:url('%1');").arg(leftLabImgPath));
    ///< 设置左按钮样式
    const auto rightLabImgPath = ":/Live/Res/live/right.svg"; ///< 右按钮图标
    ui->right_label->setStyleSheet(QString("border-image:url('%1');").arg(rightLabImgPath));
    ///< 设置右按钮样式
    ui->left_label->installEventFilter(this);                             ///< 安装左按钮事件过滤器
    ui->right_label->installEventFilter(this);                            ///< 安装右按钮事件过滤器
    QString jsonPath = GET_CURRENT_DIR + QStringLiteral("/../text.json"); ///< JSON 文件路径
    const auto future = Async::runAsync(QThreadPool::globalInstance(),
                                        [jsonPath] {
                                            QList<QString> texts; ///< 文本列表
                                            QFile file(jsonPath); ///< 打开 JSON 文件
                                            if (!file.open(QIODevice::ReadOnly)) {
                                                qWarning() << "Failed to open JSON file:" <<
                                                    jsonPath;
                                                STREAM_WARN() << "Failed to open JSON file:" <<
                                                    jsonPath.toStdString(); ///< 记录警告日志
                                                return texts;
                                            }
                                            QJsonParseError parseError;
                                            const QJsonDocument doc = QJsonDocument::fromJson(
                                                file.readAll(),
                                                &parseError); ///< 解析 JSON
                                            if (parseError.error != QJsonParseError::NoError) {
                                                qWarning() << "JSON parse error:" << parseError.
                                                    errorString();
                                                STREAM_WARN() << "JSON parse error:" << parseError.
                                                    errorString().toStdString(); ///< 记录错误日志
                                                return texts;
                                            }
                                            QJsonArray arr = doc.array();
                                            for (const auto &item : arr) {
                                                QString text = item.toObject().value("text").
                                                    toString();
                                                texts.append(text); ///< 添加文本
                                            }
                                            file.close();
                                            return texts;
                                        });
    Async::onResultReady(future,
                         this,
                         [=](const QList<QString> &texts) {
                             if (texts.isEmpty()) {
                                 qWarning() << "No valid data parsed from JSON";
                                 STREAM_WARN() << "No valid data parsed from JSON"; ///< 记录警告日志
                                 return;
                             }
                             this->m_leftBottomTextVec = texts; ///< 更新文本列表
                             unsigned seed = std::chrono::system_clock::now().time_since_epoch().
                                 count();
                             std::shuffle(this->m_leftBottomTextVec.begin(),
                                          this->m_leftBottomTextVec.end(),
                                          std::default_random_engine(seed)); ///< 打乱文本顺序
                             this->initLineOne();                            ///< 初始化第一行
                             if (lines == 2)
                                 initLineTwo(); ///< 初始化第二行
                         });
}

/**
 * @brief 初始化第一行
 * @note 创建并布局块控件
 */
void LiveCommonPartWidget::initLineOne()
{
    const auto lay1 = new QHBoxLayout(ui->line_widget_1); ///< 创建水平布局
    lay1->setContentsMargins(0, 0, 0, 0);                 ///< 设置边距
    for (int i = 0; i < 6; ++i) {
        const auto w = new LiveBlockWidget(ui->line_widget_1); ///< 创建块控件
        w->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg")
            .arg(QString::number(QRandomGenerator::global()->bounded(
                1,
                getFileCount(GET_CURRENT_DIR + "/../../../Res_Qrc/Res/standcover"))))); ///< 设置随机封面
        w->setLeftBottomText(this->m_leftBottomTextVec[i]);                             ///< 设置底部文本
        lay1->addWidget(w);                                                             ///< 添加到布局
        w->show();                                                                      ///< 显示控件
        if (i == 5)
            w->hide();           ///< 隐藏最后一个控件
        this->m_blockArr[i] = w; ///< 存储控件指针
    }
    ui->line_widget_1->setLayout(lay1); ///< 设置布局
    emit initOK();                      ///< 发出初始化完成信号
}

/**
 * @brief 初始化第二行
 * @note 创建并布局块控件
 */
void LiveCommonPartWidget::initLineTwo()
{
    const auto lay2 = new QHBoxLayout(ui->line_widget_2); ///< 创建水平布局
    lay2->setContentsMargins(0, 0, 0, 0);                 ///< 设置边距
    for (int i = 6; i < 12; ++i) {
        const auto w = new LiveBlockWidget(ui->line_widget_2); ///< 创建块控件
        w->setCoverPix(QString(":/StandCover/Res/standcover/music-stand-cover%1.jpg")
            .arg(QString::number(QRandomGenerator::global()->bounded(
                1,
                getFileCount(GET_CURRENT_DIR + "/../../../Res_Qrc/Res/standcover"))))); ///< 设置随机封面
        w->setLeftBottomText(this->m_leftBottomTextVec[i + 20]);                        ///< 设置底部文本
        lay2->addWidget(w);                                                             ///< 添加到布局
        w->show();                                                                      ///< 显示控件
        if (i == 11)
            w->hide();           ///< 隐藏最后一个控件
        this->m_blockArr[i] = w; ///< 存储控件指针
    }
    ui->line_widget_2->setLayout(lay2); ///< 设置布局
    emit initOK();                      ///< 发出初始化完成信号
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件
 * @note 动态显示或隐藏块控件
 */
void LiveCommonPartWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event); ///< 调用父类事件
    if (this->width() < 1200) {
        if (this->m_blockArr[5])
            this->m_blockArr[5]->hide(); ///< 隐藏第一行最后一个控件
        if (this->m_blockArr[11])
            this->m_blockArr[11]->hide(); ///< 隐藏第二行最后一个控件
    } else {
        if (this->m_blockArr[5])
            this->m_blockArr[5]->show(); ///< 显示第一行最后一个控件
        if (this->m_blockArr[11])
            this->m_blockArr[11]->show(); ///< 显示第二行最后一个控件
    }
}

/**
 * @brief 事件过滤器
 * @param watched 监视的对象
 * @param event 事件
 * @return 是否处理事件
 * @note 处理左右按钮点击事件
 */
bool LiveCommonPartWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->left_label) {
        if (event->type() == QEvent::MouseButtonPress) {
            ElaMessageBar::information(ElaMessageBarType::BottomRight,
                                       "Info",
                                       QString("暂无更多 %1").arg(ui->title_label->text()),
                                       1000,
                                       this->window()); ///< 显示左按钮提示
        }
    }
    if (watched == ui->right_label) {
        if (event->type() == QEvent::MouseButtonPress) {
            ElaMessageBar::information(ElaMessageBarType::BottomRight,
                                       "Info",
                                       QString("暂无更多 %1").arg(ui->title_label->text()),
                                       1000,
                                       this->window()); ///< 显示右按钮提示
        }
    }
    return QWidget::eventFilter(watched, event);
}