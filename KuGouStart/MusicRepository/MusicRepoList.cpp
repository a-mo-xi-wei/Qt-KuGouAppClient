/**
 * @file MusicRepoList.cpp
 * @brief 实现 MusicRepoList 类，显示音乐仓库列表项
 * @author WeiWang
 * @date 2024-11-24
 * @version 1.0
 */

#include "MusicRepoList.h"
#include "ui_MusicRepoList.h"
#include "logger.hpp"
#include "ElaToolTip.h"
#include "ElaMessageBar.h"

#include <QFile>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化音乐仓库列表项
 * @param parent 父控件指针，默认为 nullptr
 */
MusicRepoList::MusicRepoList(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MusicRepoList)
    , m_isEnter(false)
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/repolist.css")); ///< 加载样式表
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
MusicRepoList::~MusicRepoList()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 设置封面图片
 * @param pixmapPath 图片路径
 */
void MusicRepoList::setCoverPix(const QString &pixmapPath) const
{
    ui->cover_widget->setBorderImage(pixmapPath);        ///< 设置封面图片
}

/**
 * @brief 设置歌曲名称
 * @param song 歌曲名称
 */
void MusicRepoList::setSongName(const QString &song)
{
    this->m_songName = song;                             ///< 设置歌曲名称
    emit curPlaySongNameChange(song);                    ///< 触发歌曲名称变化信号
    updateSongText();                                    ///< 更新歌曲文本
}

/**
 * @brief 设置歌手名称
 * @param singer 歌手名称
 */
void MusicRepoList::setSinger(const QString &singer)
{
    this->m_singer = singer;                             ///< 设置歌手名称
    emit curPlaySingerChange(singer);                    ///< 触发歌手名称变化信号
    updateSingerText();                                  ///< 更新歌手文本
}

/**
 * @brief 初始化界面
 * @note 隐藏工具按钮、设置遮罩、扩展响应范围和工具提示
 */
void MusicRepoList::initUi()
{
    ui->play_add_toolButton->hide();                     ///< 隐藏播放添加按钮
    ui->like_toolButton->hide();                         ///< 隐藏收藏按钮
    ui->comment_toolButton->hide();                      ///< 隐藏评论按钮
    auto &mask = ui->cover_widget->getMask();            ///< 获取遮罩
    mask.setDefaultFillCircleColor(Qt::white);           ///< 设置默认圆形填充颜色
    mask.setHoverFillCircleColor(QColor(QStringLiteral("#26A1FF"))); ///< 设置悬停圆形填充颜色
    mask.setDefaultFillTriangleColor(QColor());          ///< 禁用默认三角形填充
    mask.setHoverFillTriangleColor(QColor());            ///< 禁用悬停三角形填充
    mask.setEnterWidgetChangeCursor(false);              ///< 禁用光标变化
    ui->cover_widget->setExpandRespond(true);            ///< 扩展响应范围
    connect(this, &MusicRepoList::enterList, ui->cover_widget, &MyBlockWidget::onShowMask); ///< 连接进入信号
    connect(this, &MusicRepoList::leaveList, ui->cover_widget, &MyBlockWidget::onHideMask); ///< 连接离开信号
    auto song_label_toolTip = new ElaToolTip(ui->song_label); ///< 创建歌曲标签工具提示
    song_label_toolTip->setToolTip(this->m_songName);    ///< 设置歌曲提示
    auto singer_label_toolTip = new ElaToolTip(ui->singer_label); ///< 创建歌手标签工具提示
    singer_label_toolTip->setToolTip(this->m_singer);    ///< 设置歌手提示
    connect(this, &MusicRepoList::curPlaySongNameChange, [this, song_label_toolTip](const QString &songName) {
        song_label_toolTip->setToolTip(songName);        ///< 更新歌曲提示
        ui->song_label->updateGeometry();                ///< 更新几何
        const QFontMetrics fm(ui->song_label->font());   ///< 获取字体测量
        QString elidedText = fm.elidedText(songName, Qt::ElideRight, ui->song_label->width()); ///< 截断文本
        ui->song_label->setText(elidedText);             ///< 设置截断文本
        song_label_toolTip->adjustSize();                ///< 调整提示尺寸
    });
    connect(this, &MusicRepoList::curPlaySingerChange, [this, singer_label_toolTip](const QString &singerName) {
        singer_label_toolTip->setToolTip(singerName);    ///< 更新歌手提示
        ui->singer_label->updateGeometry();              ///< 更新几何
        const QFontMetrics fm(ui->singer_label->font()); ///< 获取字体测量
        ui->singer_label->setText(fm.elidedText(singerName, Qt::ElideRight, ui->singer_label->width())); ///< 截断文本
        singer_label_toolTip->adjustSize();              ///< 调整提示尺寸
    });
    connect(ui->play_add_toolButton, &QToolButton::clicked, this, &MusicRepoList::onPlayAddToolBtn); ///< 连接播放添加槽
    connect(ui->like_toolButton, &QToolButton::clicked, this, &MusicRepoList::onLikeToolBtn); ///< 连接收藏槽
    connect(ui->comment_toolButton, &QToolButton::clicked, this, &MusicRepoList::onCommentToolBtn); ///< 连接评论槽
}

/**
 * @brief 更新歌曲名称文本
 * @note 根据控件宽度截断文本
 */
void MusicRepoList::updateSongText() const
{
    auto font = ui->song_label->font();                  ///< 获取字体
    QFontMetrics fm(font);                               ///< 创建字体测量工具
    auto elidedText = fm.elidedText(this->m_songName, Qt::ElideRight, ui->info_widget->width() - 20); ///< 截断文本
    ui->song_label->setText(elidedText);                 ///< 设置截断文本
}

/**
 * @brief 更新歌手名称文本
 * @note 根据控件宽度截断文本
 */
void MusicRepoList::updateSingerText() const
{
    auto font = ui->singer_label->font();                ///< 获取字体
    QFontMetrics fm(font);                               ///< 创建字体测量工具
    auto elidedText = fm.elidedText(this->m_singer, Qt::ElideRight, ui->info_widget->width() - 20); ///< 截断文本
    ui->singer_label->setText(elidedText);               ///< 设置截断文本
}

/**
 * @brief 播放添加按钮点击槽函数
 * @note 显示未实现提示
 */
void MusicRepoList::onPlayAddToolBtn()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               "下一首播放 功能未实现 敬请期待", 1000, this->window()); ///< 显示提示
}

/**
 * @brief 收藏按钮点击槽函数
 * @note 显示未实现提示
 */
void MusicRepoList::onLikeToolBtn()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               "收藏 功能未实现 敬请期待", 1000, this->window()); ///< 显示提示
}

/**
 * @brief 评论按钮点击槽函数
 * @note 显示未实现提示
 */
void MusicRepoList::onCommentToolBtn()
{
    ElaMessageBar::information(ElaMessageBarType::BottomRight, "Info",
                               "评论 功能未实现 敬请期待", 1000, this->window()); ///< 显示提示
}

/**
 * @brief 进入事件
 * @param event 进入事件
 * @note 显示按钮和遮罩
 */
void MusicRepoList::enterEvent(QEnterEvent *event)
{
    QWidget::enterEvent(event);
    if (!this->m_isEnter)
    {
        this->m_isEnter = true;                          ///< 设置进入状态
        emit enterList();                                ///< 触发进入信号
        ui->play_add_toolButton->show();                 ///< 显示播放添加按钮
        ui->like_toolButton->show();                     ///< 显示收藏按钮
        ui->comment_toolButton->show();                  ///< 显示评论按钮
        update();                                        ///< 更新界面
    }
}

/**
 * @brief 离开事件
 * @param event 事件
 * @note 隐藏按钮和遮罩
 */
void MusicRepoList::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    if (this->m_isEnter)
    {
        this->m_isEnter = false;                         ///< 清除进入状态
        emit leaveList();                                ///< 触发离开信号
        ui->play_add_toolButton->hide();                 ///< 隐藏播放添加按钮
        ui->like_toolButton->hide();                     ///< 隐藏收藏按钮
        ui->comment_toolButton->hide();                  ///< 隐藏评论按钮
        update();                                        ///< 更新界面
    }
}

/**
 * @brief 绘制事件
 * @param event 绘制事件
 * @note 绘制圆角背景
 */
void MusicRepoList::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QStyleOption opt;                                    ///< 创建样式选项
    opt.initFrom(this);                                  ///< 初始化样式
    QPainter p(this);                                    ///< 创建画笔
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this); ///< 绘制控件
    if (this->m_isEnter)
    {
        p.setPen(Qt::NoPen);                             ///< 无边框
        p.setBrush(QColor(QStringLiteral("#E8EAFB")));   ///< 设置背景颜色
        p.drawRoundedRect(rect(), 10, 10);               ///< 绘制圆角矩形
    }
    updateSongText();                                    ///< 更新歌曲文本
    updateSingerText();                                  ///< 更新歌手文本
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void MusicRepoList::mousePressEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void MusicRepoList::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件
 * @note 更新文本
 */
void MusicRepoList::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateSongText();                                    ///< 更新歌曲文本
    updateSingerText();                                  ///< 更新歌手文本
}