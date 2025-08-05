/**
 * @file MusicRepoBlock.cpp
 * @brief 实现 MusicRepoBlock 类，显示音乐仓库块状项
 * @author WeiWang
 * @date 2024-11-30
 * @version 1.0
 */

#include "MusicRepoBlock.h"
#include "ui_MusicRepoBlock.h"
#include "logger.hpp"
#include "ElaToolTip.h"

#include <QFile>
#include <QMouseEvent>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化音乐仓库块状项
 * @param parent 父控件指针，默认为 nullptr
 */
MusicRepoBlock::MusicRepoBlock(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MusicRepoBlock)
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/repoblock.css")); ///< 加载样式表
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
    ui->song_label->setFont(QFont("TaiwanPearl", 11));
    ui->singer_label->setFont(QFont("TaiwanPearl", 9));
}

/**
 * @brief 析构函数，清理资源
 */
MusicRepoBlock::~MusicRepoBlock()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 设置封面图片
 * @param pixmapPath 图片路径
 */
void MusicRepoBlock::setCoverPix(const QString &pixmapPath)
{
    ui->cover_widget->setBorderImage(pixmapPath, 10);    ///< 设置封面图片和圆角
}

/**
 * @brief 设置歌曲名称
 * @param song 歌曲名称
 */
void MusicRepoBlock::setSongName(const QString &song)
{
    this->m_songName = song;                             ///< 设置歌曲名称
    auto song_label_toolTip = new ElaToolTip(ui->song_label); ///< 创建歌曲标签工具提示
    song_label_toolTip->setToolTip(this->m_songName);    ///< 设置歌曲提示
    updateSongText();                                    ///< 更新歌曲文本
}

/**
 * @brief 设置歌手名称
 * @param singer 歌手名称
 */
void MusicRepoBlock::setSinger(const QString &singer)
{
    this->m_singer = singer;                             ///< 设置歌手名称
    auto singer_label_toolTip = new ElaToolTip(ui->singer_label); ///< 创建歌手标签工具提示
    singer_label_toolTip->setToolTip(this->m_singer);    ///< 设置歌手提示
    updateSingerText();                                  ///< 更新歌手文本
}

/**
 * @brief 更新歌曲名称文本
 * @note 根据控件宽度截断文本
 */
void MusicRepoBlock::updateSongText()
{
    auto font = ui->song_label->font();                  ///< 获取字体
    QFontMetrics fm(font);                               ///< 创建字体测量工具
    auto elidedText = fm.elidedText(this->m_songName, Qt::ElideRight, ui->info_widget->width() - 10); ///< 截断文本
    ui->song_label->setText(elidedText);                 ///< 设置截断文本
}

/**
 * @brief 更新歌手名称文本
 * @note 根据控件宽度截断文本
 */
void MusicRepoBlock::updateSingerText()
{
    auto font = ui->singer_label->font();                ///< 获取字体
    QFontMetrics fm(font);                               ///< 创建字体测量工具
    auto elidedText = fm.elidedText(this->m_singer, Qt::ElideRight, ui->info_widget->width() - 10); ///< 截断文本
    ui->singer_label->setText(elidedText);               ///< 设置截断文本
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void MusicRepoBlock::mousePressEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void MusicRepoBlock::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 鼠标双击事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void MusicRepoBlock::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件
 * @note 调整封面高度和更新文本
 */
void MusicRepoBlock::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    ui->cover_widget->setFixedHeight(ui->cover_widget->width()); ///< 同步封面高度
    updateSongText();                                    ///< 更新歌曲文本
    updateSingerText();                                  ///< 更新歌手文本
}