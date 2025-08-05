/**
 * @file MVBlockWidget.cpp
 * @brief 实现 MVBlockWidget 类，提供音乐视频块控件
 * @author WeiWang
 * @date 2024-12-27
 * @version 1.0
 */

#include "MVBlockWidget.h"
#include "ui_MVBlockWidget.h"
#include "logger.hpp"
#include "ElaToolTip.h"
#include <QFile>
#include <QMouseEvent>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化音乐视频块控件
 * @param parent 父控件指针，默认为 nullptr
 */
MVBlockWidget::MVBlockWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::MVBlockWidget)
{
    ui->setupUi(this);
    QFile file(GET_CURRENT_DIR + QStringLiteral("/block.css"));
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
    initUi(); ///< 初始化界面
}

/**
 * @brief 析构函数，清理资源
 */
MVBlockWidget::~MVBlockWidget()
{
    delete ui;
}

/**
 * @brief 设置封面图片
 * @param pixmapPath 图片路径
 */
void MVBlockWidget::setCoverPix(const QString &pixmapPath) const
{
    ui->cover_widget->setBorderImage(pixmapPath, 10); ///< 设置圆角图片
}

/**
 * @brief 设置标题
 * @param title 标题文本
 */
void MVBlockWidget::setTitle(const QString &title)
{
    this->m_title = title;
    auto title_label_toolTip = new ElaToolTip(ui->title_label); ///< 创建标题提示
    title_label_toolTip->setToolTip(this->m_title);            ///< 设置提示文本
    updateTitleText();                                         ///< 更新标题显示
}

/**
 * @brief 设置描述
 * @param description 描述文本
 */
void MVBlockWidget::setDescription(const QString &description)
{
    this->m_description = description;
    auto desc_label_toolTip = new ElaToolTip(ui->desc_label); ///< 创建描述提示
    desc_label_toolTip->setToolTip(this->m_description);      ///< 设置提示文本
    updateDescText();                                         ///< 更新描述显示
}

/**
 * @brief 隐藏描述标签
 */
void MVBlockWidget::hideDesc()
{
    ui->desc_label->hide(); ///< 隐藏描述标签
}

/**
 * @brief 初始化界面
 */
void MVBlockWidget::initUi()
{
    ui->title_label->setFont(QFont("TaiwanPearl", 11));
    ui->desc_label->setFont(QFont("TaiwanPearl", 9));
    auto &mask = ui->cover_widget->getMask(); ///< 获取遮罩
    mask.setDefaultFillCircleColor(Qt::white); ///< 设置默认圆形颜色
    mask.setHoverFillCircleColor(QColor(QStringLiteral("#26A1FF"))); ///< 设置悬停圆形颜色
    mask.setDefaultFillTriangleColor(QColor(QStringLiteral("#666666"))); ///< 设置默认三角形颜色
    mask.setHoverFillTriangleColor(QColor(QStringLiteral("#666666"))); ///< 设置悬停三角形颜色
    mask.setMaskColor(QColor(0, 0, 0, 100)); ///< 设置遮罩颜色
    mask.setStander(120);                    ///< 设置遮罩标准值
    ui->cover_widget->setAspectRatio(2);     ///< 设置宽高比
}

/**
 * @brief 更新标题文本（省略显示）
 */
void MVBlockWidget::updateTitleText() const
{
    auto font = ui->title_label->font();
    QFontMetrics fm(font);
    auto elidedText = fm.elidedText(this->m_title, Qt::ElideRight, this->width() - 10); ///< 省略文本
    ui->title_label->setText(elidedText);                                              ///< 设置显示文本
}

/**
 * @brief 更新描述文本（省略显示）
 */
void MVBlockWidget::updateDescText() const
{
    auto font = ui->desc_label->font();
    QFontMetrics fm(font);
    auto elidedText = fm.elidedText(this->m_description, Qt::ElideRight, this->width() - 10); ///< 省略文本
    ui->desc_label->setText(elidedText);                                                    ///< 设置显示文本
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件对象
 * @note 重写基类方法
 */
void MVBlockWidget::mousePressEvent(QMouseEvent *event)
{
    event->ignore(); ///< 忽略事件
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件对象
 * @note 重写基类方法
 */
void MVBlockWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore(); ///< 忽略事件
}

/**
 * @brief 鼠标双击事件
 * @param event 鼠标事件对象
 * @note 重写基类方法
 */
void MVBlockWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore(); ///< 忽略事件
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件对象
 * @note 重写基类方法
 */
void MVBlockWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    ui->cover_widget->setFixedHeight(ui->cover_widget->width() / 2); ///< 设置封面高度
    updateTitleText();                                              ///< 更新标题
    updateDescText();                                               ///< 更新描述
}