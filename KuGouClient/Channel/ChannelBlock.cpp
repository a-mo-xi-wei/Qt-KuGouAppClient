/**
 * @file ChannelBlock.cpp
 * @brief 实现 ChannelBlock 类，提供音乐频道块界面功能
 * @author WeiWang
 * @date 2024-12-03
 * @version 1.0
 */

#include "ChannelBlock.h"
#include "ui_ChannelBlock.h"
#include "ElaToolTip.h"

#include <QFile>
#include <QRandomGenerator>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化音乐频道块界面
 * @param parent 父控件指针，默认为 nullptr
 */
ChannelBlock::ChannelBlock(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChannelBlock)
    , m_titleText("")
    , m_singerSongText("")
{
    ui->setupUi(this);                                   ///< 初始化 UI
    {
        this->setObjectName("block");                    ///< 设置对象名称
        QFile file(GET_CURRENT_DIR + QStringLiteral("/channelblock.css")); ///< 加载样式表
        if (file.open(QIODevice::ReadOnly))
        {
            this->setStyleSheet(file.readAll());         ///< 应用样式表
        }
        else
        {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";      ///< 记录错误日志
            return;
        }
    }
    initUi();                                            ///< 初始化界面
}

/**
 * @brief 析构函数，清理资源
 */
ChannelBlock::~ChannelBlock()
{
    delete ui;
}

/**
 * @brief 设置封面图片
 * @param pixmapPath 图片路径
 */
void ChannelBlock::setCoverPix(const QString &pixmapPath) const
{
    ui->cover_widget->setBorderImage(pixmapPath, 10);    ///< 设置封面图片，圆角半径为 10
}

/**
 * @brief 初始化界面
 * @note 设置遮罩、响应范围、随机播放数等
 */
void ChannelBlock::initUi() const
{
    ui->title_label->setFont(QFont("TaiwanPearl", 11));
    ui->singe_song_label->setFont(QFont("TaiwanPearl", 9));
    auto &mask = ui->cover_widget->getMask();            ///< 获取遮罩对象
    mask.setDefaultFillCircleColor(Qt::white);           ///< 设置默认圆形填充颜色
    mask.setHoverFillCircleColor(QColor(QStringLiteral("#5192FE"))); ///< 设置悬停圆形填充颜色
    mask.setDefaultFillTriangleColor(QColor(QStringLiteral("#666666"))); ///< 设置默认三角形填充颜色
    mask.setHoverFillTriangleColor(QColor(QStringLiteral("#666666"))); ///< 设置悬停三角形填充颜色
    mask.setStander(120);                                ///< 设置遮罩标准值
    ui->cover_widget->setExpandRespond(true);            ///< 扩展响应范围
    ui->cover_widget->setPopularDirection(1);            ///< 设置流行方向
    ui->cover_widget->setHaveNumberUnit(false);          ///< 禁用数字单位
    ui->cover_widget->setPopularBtnText(QString::number(QRandomGenerator::global()->bounded(1, 1000))); ///< 设置随机播放数
}

/**
 * @brief 设置标题文本
 * @param title 标题内容
 */
void ChannelBlock::setTitleText(const QString &title)
{
    this->m_titleText = title;                           ///< 存储标题文本
    auto title_label_toolTip = new ElaToolTip(ui->title_label); ///< 创建标题工具提示
    title_label_toolTip->setToolTip(this->m_titleText);  ///< 设置工具提示内容
    updateTitleText();                                   ///< 更新标题显示
}

/**
 * @brief 更新标题文本，处理文本截断
 */
void ChannelBlock::updateTitleText() const
{
    auto font = ui->title_label->font();                 ///< 获取标题标签字体
    QFontMetrics fm(font);                               ///< 创建字体测量工具
    auto elidedText = fm.elidedText(this->m_titleText, Qt::ElideRight, this->width() - 10); ///< 截断文本
    ui->title_label->setText(elidedText);                ///< 设置截断后的文本
}

/**
 * @brief 设置歌手歌曲文本
 * @param singerSong 歌手歌曲内容
 */
void ChannelBlock::setSingerSongText(const QString &singerSong)
{
    this->m_singerSongText = singerSong;                 ///< 存储歌手歌曲文本
    auto singe_song_label_toolTip = new ElaToolTip(ui->singe_song_label); ///< 创建歌手歌曲工具提示
    singe_song_label_toolTip->setToolTip(this->m_singerSongText); ///< 设置工具提示内容
    updateSingerSongText();                              ///< 更新歌手歌曲显示
}

/**
 * @brief 更新歌手歌曲文本，处理文本截断
 */
void ChannelBlock::updateSingerSongText() const
{
    auto font = ui->singe_song_label->font();            ///< 获取歌手歌曲标签字体
    QFontMetrics fm(font);                               ///< 创建字体测量工具
    auto elidedText = fm.elidedText(this->m_singerSongText, Qt::ElideRight, this->width() - 10); ///< 截断文本
    ui->singe_song_label->setText(elidedText);           ///< 设置截断后的文本
}

/**
 * @brief 鼠标进入事件，显示遮罩和更改标题颜色
 * @param event 进入事件
 */
void ChannelBlock::enterEvent(QEnterEvent *event)
{
    QWidget::enterEvent(event);                          ///< 调用父类进入事件
    ui->cover_widget->onShowMask();                      ///< 显示遮罩
    ui->title_label->setStyleSheet(QStringLiteral("color:#2FAEFF;")); ///< 设置标题颜色为蓝色
}

/**
 * @brief 鼠标离开事件，隐藏遮罩和恢复标题颜色
 * @param event 事件
 */
void ChannelBlock::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);                          ///< 调用父类离开事件
    ui->cover_widget->onHideMask();                      ///< 隐藏遮罩
    ui->title_label->setStyleSheet(QStringLiteral("color:black;")); ///< 恢复标题颜色为黑色
}

/**
 * @brief 鼠标按下事件，忽略事件
 * @param event 鼠标事件
 */
void ChannelBlock::mousePressEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略鼠标按下事件
}

/**
 * @brief 鼠标释放事件，忽略事件
 * @param event 鼠标事件
 */
void ChannelBlock::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略鼠标释放事件
}

/**
 * @brief 鼠标双击事件，忽略事件
 * @param event 鼠标事件
 */
void ChannelBlock::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略鼠标双击事件
}