/**
 * @file SongListBlockWidget.cpp
 * @brief 实现 SongListBlockWidget 类，显示歌单块
 * @author WeiWang
 * @date 2025-01-25
 * @version 1.0
 */

#include "SongListBlockWidget.h"
#include "ui_SongListBlockWidget.h"

#include <QMouseEvent>

/**
 * @brief 构造函数，初始化歌单块
 * @param parent 父控件指针，默认为 nullptr
 */
SongListBlockWidget::SongListBlockWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SongListBlockWidget)
{
    ui->setupUi(this);
    initUi();                                            ///< 初始化界面
}

/**
 * @brief 析构函数，清理资源
 */
SongListBlockWidget::~SongListBlockWidget()
{
    delete ui;                                           ///< 删除 UI
}

/**
 * @brief 设置封面图片
 * @param pixmapPath 图片路径
 */
void SongListBlockWidget::setCoverPix(const QString &pixmapPath) const
{
    ui->cover_widget->setBorderImage(pixmapPath, 10);    ///< 设置封面图片和圆角
}

/**
 * @brief 设置标题文本
 * @param title 标题文本
 */
void SongListBlockWidget::setTitleText(const QString &title) const
{
    ui->title_label->setText(title);                     ///< 设置标题文本
}

/**
 * @brief 初始化界面
 * @note 设置标题字体、封面控件和遮罩样式
 */
void SongListBlockWidget::initUi() const
{
    ui->title_label->setFont(QFont("TaiwanPearl", 13));
    ui->cover_widget->setExpandRespond(false);          ///< 禁用扩展响应
    ui->cover_widget->setPopularDirection(1);           ///< 设置热门方向
    ui->cover_widget->setHaveNumberUnit(false);         ///< 禁用单位显示
    ui->cover_widget->setLeftPopularBtnIcon("");        ///< 设置空图标
    ui->cover_widget->setLeftPopularBtnWidth(30);       ///< 设置按钮宽度
    ui->cover_widget->setPopularBtnText("0首");         ///< 设置文本
    ui->cover_widget->setPopularBtnLeftPadding(0);      ///< 设置左填充
    auto &mask = ui->cover_widget->getMask();           ///< 获取遮罩
    mask.setDefaultFillCircleColor(Qt::white);          ///< 设置默认圆形填充颜色
    mask.setHoverFillCircleColor(Qt::white);            ///< 设置悬停圆形填充颜色
    mask.setDefaultFillTriangleColor(QColor(QStringLiteral("#666666"))); ///< 设置默认三角形填充颜色
    mask.setHoverFillTriangleColor(QColor(QStringLiteral("#666666")));   ///< 设置悬停三角形填充颜色
    mask.setMaskColor(QColor(0, 0, 0, 100));            ///< 设置遮罩颜色
    mask.setEnterWidgetChangeCursor(false);             ///< 禁用光标变化
    mask.setCursor(Qt::ArrowCursor);                    ///< 设置箭头光标
}

/**
 * @brief 进入事件
 * @param event 进入事件
 * @note 遮罩显示（已注释）
 */
void SongListBlockWidget::enterEvent(QEnterEvent *event)
{
    QWidget::enterEvent(event);
    // ui->cover_widget->onShowMask();                  ///< 遮罩显示（已注释）
}

/**
 * @brief 离开事件
 * @param event 离开事件
 * @note 遮罩隐藏（已注释）
 */
void SongListBlockWidget::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    // ui->cover_widget->onHideMask();                  ///< 遮罩隐藏（已注释）
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void SongListBlockWidget::mousePressEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void SongListBlockWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 鼠标双击事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void SongListBlockWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}