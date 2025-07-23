/**
 * @file VideoChannelBlock.cpp
 * @brief 实现 VideoChannelBlock 类，提供视频频道块控件
 * @author WeiWang
 * @date 2024-12-12
 * @version 1.0
 */

#include "VideoChannelBlock.h"
#include "ui_VideoChannelBlock.h"
#include "logger.hpp"

#include <QFile>
#include <QRandomGenerator>
#include <QLabel>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化视频频道块控件
 * @param parent 父控件指针，默认为 nullptr
 */
VideoChannelBlock::VideoChannelBlock(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoChannelBlock)
{
    ui->setupUi(this);
    this->setObjectName("videoblock"); ///< 设置对象名称
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/videoblock.css"));
        if (file.open(QIODevice::ReadOnly))
        {
            this->setStyleSheet(file.readAll()); ///< 加载样式表
        }
        else
        {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";
            return;
        }
    }
    initUi();            ///< 初始化界面
}

/**
 * @brief 析构函数，清理资源
 */
VideoChannelBlock::~VideoChannelBlock()
{
    delete ui;
}

/**
 * @brief 设置封面图片
 * @param pixmapPath 图片路径
 */
void VideoChannelBlock::setCoverPix(const QString &pixmapPath) const
{
    ui->cover_widget->setBorderImage(pixmapPath, 10); ///< 设置圆角图片
}

/**
 * @brief 设置描述
 * @param description 描述文本
 */
void VideoChannelBlock::setDescription(const QString &description)
{
    this->m_descriptionText = description; ///< 设置描述文本
}

/**
 * @brief 设置封面文本
 * @param text 文本内容
 */
void VideoChannelBlock::setCoverText(const QString &text) const
{
    auto font = QFont("YouYuan");
    font.setPixelSize(16);                                  ///< 设置像素大小
    ui->coverTextLab->setFont(font); ///< 设置字体
    ui->coverTextLab->setText(text);                        ///< 设置文本
    int yPosition = height() - 95;                          ///< 计算 Y 位置（下方 95 像素）
    int xPosition = (width() - ui->coverTextLab->width()) / 2;            ///< 计算 X 位置（水平居中）
    ui->coverTextLab->move(xPosition, yPosition);     ///< 移动文本标签
    ui->coverTextLab->raise();
}

/**
 * @brief 初始化界面
 */
void VideoChannelBlock::initUi()
{
    auto &mask = ui->cover_widget->getMask();                     ///< 获取遮罩
    mask.setDefaultFillCircleColor(QColor(QStringLiteral("#525759"))); ///< 设置默认圆形颜色
    mask.setHoverFillCircleColor(QColor(QStringLiteral("#525759")));  ///< 设置悬停圆形颜色
    mask.setDefaultFillTriangleColor(Qt::white);                   ///< 设置默认三角形颜色
    mask.setHoverFillTriangleColor(Qt::white);                    ///< 设置悬停三角形颜色
    mask.setMaskColor(QColor(0, 0, 0, 100));                      ///< 设置遮罩颜色
    ui->cover_widget->setAspectRatio(1.5);                        ///< 设置宽高比
    ui->cover_widget->installEventFilter(this);                   ///< 安装事件过滤器

    ui->coverTextLab->setFixedSize(this->width() - 20,40);      ///< 设置固定宽度

    ui->desc_toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); ///< 设置工具按钮样式
    ui->desc_toolButton->setIcon(QIcon(QStringLiteral(":/TabIcon/Res/tabIcon/eye-gray.svg"))); ///< 设置图标
    ui->desc_toolButton->setText(QString::number(QRandomGenerator::global()->bounded(1, 500)) + "人在看"); ///< 设置随机观看人数
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件对象
 * @note 重写基类方法
 */
void VideoChannelBlock::mousePressEvent(QMouseEvent *event)
{
    event->ignore();                                              ///< 忽略事件
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件对象
 * @note 重写基类方法
 */
void VideoChannelBlock::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();                                              ///< 忽略事件
}

/**
 * @brief 鼠标双击事件
 * @param event 鼠标事件对象
 * @note 重写基类方法
 */
void VideoChannelBlock::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore();                                              ///< 忽略事件
}

/**
 * @brief 事件过滤器
 * @param watched 目标对象
 * @param event 事件对象
 * @return 是否处理事件
 * @note 重写基类方法
 */
bool VideoChannelBlock::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->cover_widget)
    {
        if (event->type() == QEvent::Enter)
        {
            ui->coverTextLab->hide();                         ///< 鼠标进入时隐藏文本
        }
        else if (event->type() == QEvent::Leave)
        {
            ui->coverTextLab->show();                         ///< 鼠标离开时显示文本
            ui->coverTextLab->raise();                        ///< 提升层级
        }
    }
    return QWidget::eventFilter(watched, event);
}