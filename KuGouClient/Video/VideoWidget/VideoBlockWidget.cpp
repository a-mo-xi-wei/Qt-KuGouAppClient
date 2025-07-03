/**
 * @file VideoBlockWidget.cpp
 * @brief 实现 VideoBlockWidget 类，管理视频块界面
 * @author WeiWang
 * @date 2024-12-15
 * @version 1.0
 */

#include "VideoBlockWidget.h"
#include "ui_VideoBlockWidget.h"
#include "logger.hpp"
#include "ElaToolTip.h"

#include <QFile>
#include <QPainter>
#include <QPainterPath>
#include <QRandomGenerator>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/** @brief 视频封面宽高比常量 */
constexpr float AspectRation = 1.6;

/**
 * @brief 构造函数，初始化视频块
 * @param parent 父控件指针，默认为 nullptr
 */
VideoBlockWidget::VideoBlockWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoBlockWidget)
{
    ui->setupUi(this);                                   ///< 设置 UI 布局
    QFile file(GET_CURRENT_DIR + QStringLiteral("/block.css")); ///< 加载样式表
    if (file.open(QIODevice::ReadOnly))
    {
        this->setStyleSheet(file.readAll());             ///< 应用样式表
    }
    else
    {
        // @note 未使用，保留用于调试
        // qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";          ///< 记录错误日志
        return;
    }
    initUi();                                            ///< 初始化界面
}

/**
 * @brief 析构函数
 * @note 释放 UI 资源
 */
VideoBlockWidget::~VideoBlockWidget()
{
    delete ui;                                           ///< 释放 UI 界面
}

/**
 * @brief 设置封面图片
 * @param pixmapPath 图片路径
 * @note 更新封面显示，应用圆角
 */
void VideoBlockWidget::setCoverPix(const QString &pixmapPath) const
{
    ui->cover_widget->setBorderImage(pixmapPath, 10);   ///< 设置封面图片，圆角半径 10
}

/**
 * @brief 设置视频名称
 * @param name 视频名称
 * @note 更新名称标签并添加工具提示
 */
void VideoBlockWidget::setVideoName(const QString &name)
{
    this->m_videoName = name;                           ///< 存储视频名称
    auto video_name_label_toolTip = new ElaToolTip(ui->video_name_label); ///< 创建工具提示
    video_name_label_toolTip->setToolTip(this->m_videoName); ///< 设置完整名称为提示
    updateVideoNameText();                              ///< 更新名称文本
}

/**
 * @brief 设置图标图片
 * @param pix 图片路径
 * @note 更新图标显示，应用圆角效果
 */
void VideoBlockWidget::setIconPix(const QString &pix) const
{
    auto src = QPixmap(pix);                            ///< 加载图片
    auto size = ui->ico_label->size();                  ///< 获取图标标签大小
    auto len = size.width();
    QPixmap scaled = src.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation); ///< 缩放图片
    QPixmap dest(size);
    dest.fill(Qt::transparent);                         ///< 设置透明背景
    QPainter painter(&dest);
    painter.setRenderHint(QPainter::Antialiasing);      ///< 启用抗锯齿
    QPainterPath path;
    path.addRoundedRect(0, 0, len, len, len / 2, len / 2); ///< 创建圆角矩形路径
    painter.setClipPath(path);                          ///< 设置裁剪路径
    painter.drawPixmap(0, 0, scaled);                   ///< 绘制图片
    ui->ico_label->setPixmap(dest);                     ///< 设置图标
}

/**
 * @brief 设置作者
 * @param author 作者名称
 * @note 更新作者标签并添加工具提示
 */
void VideoBlockWidget::setAuthor(const QString &author)
{
    this->m_videoAuthor = author;                       ///< 存储作者名称
    auto video_author_label_toolTip = new ElaToolTip(ui->video_author_label); ///< 创建工具提示
    video_author_label_toolTip->setToolTip(this->m_videoAuthor); ///< 设置完整作者为提示
    updateVideoAuthorText();                            ///< 更新作者文本
}

/**
 * @brief 显示提示标签
 * @note 启用封面的提示标签
 */
void VideoBlockWidget::setShowTip() const
{
    ui->cover_widget->setShowTip();                     ///< 显示提示标签
    ui->cover_widget->setTipStyleSheet(QStringLiteral("border-radius:10px;background-color:#797978;color:white;")); ///< 设置提示样式
}

/**
 * @brief 设置提示文本
 * @param text 提示文本
 * @note 更新封面的提示标签内容
 */
void VideoBlockWidget::setTipText(const QString &text) const
{
    ui->cover_widget->setTipLabText(text);              ///< 设置提示文本
}

/**
 * @brief 初始化界面
 * @note 设置遮罩、播放按钮、时长和随机播放量
 */
void VideoBlockWidget::initUi() const
{
    // 设置遮罩
    auto &mask = ui->cover_widget->getMask();           ///< 获取遮罩
    mask.setDefaultFillCircleColor(Qt::white);          ///< 设置默认圆形填充颜色
    mask.setHoverFillCircleColor(QColor(QStringLiteral("#26A1FF"))); ///< 设置悬停圆形填充颜色
    mask.setDefaultFillTriangleColor(QColor(QStringLiteral("#666666"))); ///< 设置默认三角形填充颜色
    mask.setHoverFillTriangleColor(QColor(QStringLiteral("#666666"))); ///< 设置悬停三角形填充颜色
    mask.setMaskColor(QColor(0, 0, 0, 20));             ///< 设置遮罩颜色
    ui->cover_widget->setLeftPopularBtnIcon(QStringLiteral(":/TabIcon/Res/tabIcon/play3-white.svg")); ///< 设置播放按钮图标
    ui->cover_widget->setPopularDirection(1);           ///< 设置流行按钮方向
    ui->cover_widget->setAspectRatio(AspectRation);     ///< 设置宽高比
    ui->cover_widget->setHaveNumberUnit(false);         ///< 默认无单位
    ui->cover_widget->setDurationBtnShow();             ///< 显示时长按钮

    // 设置随机时长
    QString durationText = "0";                         ///< 初始化时长
    auto number = QRandomGenerator::global()->bounded(1, 10); ///< 生成分钟数
    durationText += QString::number(number) + ":";
    number = QRandomGenerator::global()->bounded(1, 60); ///< 生成秒数
    if (number < 10)
    {
        durationText += "0";                            ///< 补零
    }
    durationText += QString::number(number);
    ui->cover_widget->setDurationBtnText(durationText); ///< 设置时长文本

    // 设置随机播放量
    number = QRandomGenerator::global()->bounded(1, 5000); ///< 生成随机播放量
    if (number <= 500)
    {
        ui->cover_widget->setHaveNumberUnit(true);      ///< 设置带单位
        auto n = QRandomGenerator::global()->generateDouble() * 100; ///< 生成浮点播放量
        ui->cover_widget->setPopularBtnText(QString::number(n, 'f', 2)); ///< 设置播放量
    }
    else
    {
        ui->cover_widget->setPopularBtnText(QString::number(number)); ///< 设置播放量
    }
}

/**
 * @brief 更新视频名称文本
 * @note 处理名称的省略显示
 */
void VideoBlockWidget::updateVideoNameText() const
{
    auto font = ui->video_name_label->font();           ///< 获取字体
    QFontMetrics fm(font);                              ///< 创建字体测量工具
    auto elidedText = fm.elidedText(this->m_videoName, Qt::ElideRight, ui->info_widget->width() - 20); ///< 计算省略文本
    ui->video_name_label->setText(elidedText);          ///< 设置省略文本
}

/**
 * @brief 更新作者文本
 * @note 处理作者的省略显示
 */
void VideoBlockWidget::updateVideoAuthorText() const
{
    auto font = ui->video_author_label->font();         ///< 获取字体
    QFontMetrics fm(font);                              ///< 创建字体测量工具
    auto elidedText = fm.elidedText(this->m_videoAuthor, Qt::ElideRight, ui->info_widget->width() - 20); ///< 计算省略文本
    ui->video_author_label->setText(elidedText);        ///< 设置省略文本
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void VideoBlockWidget::mousePressEvent(QMouseEvent *event)
{
    event->ignore();                                    ///< 忽略事件
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void VideoBlockWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();                                    ///< 忽略事件
}

/**
 * @brief 鼠标双击事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void VideoBlockWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore();                                    ///< 忽略事件
}

/**
 * @brief 事件过滤器
 * @param watched 监控对象
 * @param event 事件
 * @return 是否处理事件
 * @note 默认实现
 */
bool VideoBlockWidget::eventFilter(QObject *watched, QEvent *event)
{
    return QWidget::eventFilter(watched, event);        ///< 调用父类处理
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件
 * @note 更新封面高度和文本省略
 */
void VideoBlockWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);                        ///< 调用父类处理
    // @note 未使用，保留用于调试
    // this->setFixedHeight(event->size().width() / 1.1);
    ui->cover_widget->setFixedHeight(ui->cover_widget->width() / AspectRation); ///< 设置封面高度
    updateVideoNameText();                              ///< 更新名称文本
    updateVideoAuthorText();                            ///< 更新作者文本
}