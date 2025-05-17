/**
 * @file MusicRepoVideo.cpp
 * @brief 实现 MusicRepoVideo 类，显示音乐仓库视频项
 * @author WeiWang
 * @date 2024-11-30
 * @version 1.0
 */

#include "MusicRepoVideo.h"
#include "ui_MusicRepoVideo.h"
#include "logger.hpp"
#include "ElaToolTip.h"

#include <QFile>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QRandomGenerator>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化音乐仓库视频项
 * @param parent 父控件指针，默认为 nullptr
 */
MusicRepoVideo::MusicRepoVideo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MusicRepoVideo)
    , m_isEnter(false)
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/repovideo.css")); ///< 加载样式表
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
MusicRepoVideo::~MusicRepoVideo()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 设置封面图片
 * @param pixmapPath 图片路径
 */
void MusicRepoVideo::setCoverPix(const QString &pixmapPath) const
{
    ui->cover_widget->setBorderImage(pixmapPath, 10);    ///< 设置封面图片和圆角
}

/**
 * @brief 设置视频名称
 * @param name 视频名称
 */
void MusicRepoVideo::setVideoName(const QString &name)
{
    this->m_videoName = name;                            ///< 设置视频名称
    auto video_name_label_tooTip = new ElaToolTip(ui->video_name_label); ///< 创建视频名称标签工具提示
    video_name_label_tooTip->setToolTip(this->m_videoName); ///< 设置视频名称提示
    updateVideoNameText();                               ///< 更新视频名称文本
}

/**
 * @brief 设置图标
 * @param pix 图标路径
 */
void MusicRepoVideo::setIconPix(const QString &pix) const
{
    auto src = QPixmap(pix);                             ///< 加载图标
    auto size = ui->ico_label->size();                   ///< 获取图标标签大小
    auto len = size.width();                             ///< 获取宽度
    QPixmap scaled = src.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation); ///< 缩放图标
    QPixmap dest(size);                                  ///< 创建目标图像
    dest.fill(Qt::transparent);                          ///< 填充透明背景
    QPainter painter(&dest);                             ///< 创建画笔
    painter.setRenderHint(QPainter::Antialiasing);       ///< 启用抗锯齿
    QPainterPath path;                                   ///< 创建路径
    path.addRoundedRect(0, 0, len, len, len / 2, len / 2); ///< 添加圆形矩形
    painter.setClipPath(path);                           ///< 设置裁剪路径
    painter.drawPixmap(0, 0, scaled);                    ///< 绘制缩放图像
    ui->ico_label->setPixmap(dest);                      ///< 设置图标
}

/**
 * @brief 设置作者名称
 * @param author 作者名称
 */
void MusicRepoVideo::setAuthor(const QString &author)
{
    this->m_videoAuthor = author;                        ///< 设置作者名称
    auto video_author_label_tooTip = new ElaToolTip(ui->video_author_label); ///< 创建作者标签工具提示
    video_author_label_tooTip->setToolTip(this->m_videoAuthor); ///< 设置作者提示
    updateVideoAuthorText();                             ///< 更新作者文本
}

/**
 * @brief 初始化界面
 * @note 设置封面控件（图标、方向、宽高比、播放量、左填充）和事件过滤器
 */
void MusicRepoVideo::initUi()
{
    ui->cover_widget->setRightPopularBtnIcon(QStringLiteral(":/Res/tabIcon/video-white.svg")); ///< 设置播放图标
    ui->cover_widget->setPopularDirection(2);            ///< 设置播放量方向
    ui->cover_widget->setAspectRatio(2);                 ///< 设置宽高比
    ui->cover_widget->setPopularBtnText(QString::number(QRandomGenerator::global()->generateDouble() * 10, 'f', 1)); ///< 设置随机播放量
    ui->cover_widget->setPopularBtnLeftPadding(8);       ///< 设置左填充
    ui->cover_widget->installEventFilter(this);          ///< 安装事件过滤器
}

/**
 * @brief 更新视频名称文本
 * @note 根据控件宽度截断文本
 */
void MusicRepoVideo::updateVideoNameText() const
{
    auto font = ui->video_name_label->font();            ///< 获取字体
    QFontMetrics fm(font);                               ///< 创建字体测量工具
    auto elidedText = fm.elidedText(this->m_videoName, Qt::ElideRight, ui->info_widget->width() - 20); ///< 截断文本
    ui->video_name_label->setText(elidedText);           ///< 设置截断文本
}

/**
 * @brief 更新作者名称文本
 * @note 根据控件宽度截断文本
 */
void MusicRepoVideo::updateVideoAuthorText() const
{
    auto font = ui->video_author_label->font();          ///< 获取字体
    QFontMetrics fm(font);                               ///< 创建字体测量工具
    auto elidedText = fm.elidedText(this->m_videoAuthor, Qt::ElideRight, ui->info_widget->width() - 20); ///< 截断文本
    ui->video_author_label->setText(elidedText);         ///< 设置截断文本
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void MusicRepoVideo::mousePressEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void MusicRepoVideo::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 鼠标双击事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void MusicRepoVideo::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 事件过滤器
 * @param watched 监听对象
 * @param event 事件
 * @return 是否处理事件
 * @note 动态切换播放量方向（悬停/离开）
 */
bool MusicRepoVideo::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->cover_widget)
    {
        if (event->type() == QEvent::Enter)
        {
            if (!this->m_isEnter)
            {
                this->m_isEnter = true;                  ///< 设置进入状态
                ui->cover_widget->setPopularDirection(0); ///< 设置悬停方向
            }
        }
        else if (event->type() == QEvent::Leave)
        {
            if (this->m_isEnter)
            {
                this->m_isEnter = false;                 ///< 清除进入状态
                ui->cover_widget->setPopularDirection(2); ///< 设置离开方向
            }
        }
    }
    return QWidget::eventFilter(watched, event);         ///< 调用父类过滤器
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件
 * @note 调整封面高度和更新文本
 */
void MusicRepoVideo::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    ui->cover_widget->setFixedHeight(ui->cover_widget->width() / 2); ///< 设置封面高度（宽高比 1:2）
    updateVideoNameText();                               ///< 更新视频名称文本
    updateVideoAuthorText();                             ///< 更新作者文本
}