/**
 * @file RecentlyChannelBlock.cpp
 * @brief 实现 RecentlyChannelBlock 类，管理最近频道块
 * @author WeiWang
 * @date 2025-01-31
 * @version 1.0
 */

#include "RecentlyChannelBlock.h"
#include "ui_RecentlyChannelBlock.h"
#include "logger.hpp"
#include "ElaToolTip.h"

#include <QFile>
#include <QMouseEvent>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化最近频道块
 * @param parent 父控件指针，默认为 nullptr
 */
RecentlyChannelBlock::RecentlyChannelBlock(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RecentlyChannelBlock)
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/block.css")); ///< 加载样式表
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
RecentlyChannelBlock::~RecentlyChannelBlock()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 设置封面图片
 * @param pixmapPath 图片路径
 */
void RecentlyChannelBlock::setCoverPix(const QString &pixmapPath) const
{
    ui->cover_widget->setBorderImage(pixmapPath, 10);    ///< 设置封面图片
}

/**
 * @brief 初始化界面
 * @note 初始化标题字体、工具提示和遮罩效果
 */
void RecentlyChannelBlock::initUi() const
{
    auto font = ui->title_label->font();                 ///< 设置标题字体
    font.setWeight(QFont::DemiBold);
    ui->title_label->setFont(font);
    auto singer_song_label_toolTip = new ElaToolTip(ui->singer_song_label); ///< 歌手/歌曲标签工具提示
    singer_song_label_toolTip->setToolTip(ui->singer_song_label->text());
    auto &mask = ui->cover_widget->getMask();            ///< 获取遮罩
    mask.setDefaultFillCircleColor(Qt::white);           ///< 设置默认圆形颜色
    mask.setHoverFillCircleColor(QColor(QStringLiteral("#5192FE"))); ///< 设置悬停圆形颜色
    mask.setDefaultFillTriangleColor(QColor(QStringLiteral("#666666"))); ///< 设置默认三角形颜色
    mask.setHoverFillTriangleColor(QColor(QStringLiteral("#666666"))); ///< 设置悬停三角形颜色
    mask.setEnterWidgetChangeCursor(false);              ///< 禁用悬停光标变化
    mask.setMaskColor(QColor(0, 0, 0, 100));             ///< 设置遮罩颜色
    mask.setStander(100);                                ///< 设置标准值
    ui->cover_widget->setExpandRespond(true);            ///< 扩展响应范围
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void RecentlyChannelBlock::mousePressEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void RecentlyChannelBlock::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 鼠标双击事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void RecentlyChannelBlock::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 进入事件
 * @param event 进入事件
 * @note 显示遮罩效果
 */
void RecentlyChannelBlock::enterEvent(QEnterEvent *event)
{
    QWidget::enterEvent(event);
    ui->cover_widget->onShowMask();                      ///< 显示遮罩
}

/**
 * @brief 离开事件
 * @param event 离开事件
 * @note 隐藏遮罩效果
 */
void RecentlyChannelBlock::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    ui->cover_widget->onHideMask();                      ///< 隐藏遮罩
}