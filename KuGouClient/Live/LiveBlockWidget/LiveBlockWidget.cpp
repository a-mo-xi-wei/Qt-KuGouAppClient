/**
 * @file LiveBlockWidget.cpp
 * @brief 实现 LiveBlockWidget 类，提供直播块控件功能
 * @author WeiWang
 * @date 2025-02-17
 * @version 1.0
 */

#include "LiveBlockWidget.h"
#include "ui_LiveBlockWidget.h"
#include "ElaToolTip.h"

#include <QMouseEvent>

/**
 * @brief 构造函数，初始化直播块控件
 * @param parent 父控件指针，默认为 nullptr
 */
LiveBlockWidget::LiveBlockWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LiveBlockWidget)
{
    ui->setupUi(this);                                   ///< 初始化 UI
    initUi();                                            ///< 初始化界面
}

/**
 * @brief 设置封面高度扩展
 * @note 调整边距和大小策略以扩展封面高度
 */
void LiveBlockWidget::setCoverHeightExpanding()
{
    this->setContentsMargins(0, 0, 0, 0);                ///< 设置边距
    ui->cover_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); ///< 设置扩展策略
    ui->cover_widget->setMinimumHeight(0);               ///< 清除最小高度限制
    ui->cover_widget->setMaximumHeight(QWIDGETSIZE_MAX); ///< 清除最大高度限制
    update();                                            ///< 刷新界面
}

/**
 * @brief 析构函数，清理资源
 */
LiveBlockWidget::~LiveBlockWidget()
{
    delete ui;
}

/**
 * @brief 设置封面图片
 * @param pixmapPath 图片路径
 */
void LiveBlockWidget::setCoverPix(const QString &pixmapPath) const
{
    ui->cover_widget->setBorderImage(pixmapPath, 10);    ///< 设置封面图片
}

/**
 * @brief 设置左下角文本
 * @param text 文本内容
 */
void LiveBlockWidget::setLeftBottomText(const QString &text)
{
    ui->cover_widget->setPopularBtnText(text);           ///< 设置流行按钮文本
    auto coverWidget_toolTip = new ElaToolTip(ui->cover_widget); ///< 创建工具提示
    coverWidget_toolTip->setToolTip(text);               ///< 设置工具提示文本
    update();                                            ///< 刷新界面
}

/**
 * @brief 设置左下角流行按钮字体大小
 * @param size 字体大小
 * @param isBold 是否加粗
 */
void LiveBlockWidget::setLeftPopularBtnFontSize(const int &size, const bool &isBold) const
{
    ui->cover_widget->setLeftPopularBtnFontSize(size, isBold); ///< 设置字体大小和加粗
}

/**
 * @brief 设置提示标签文本
 * @param text 文本内容
 */
void LiveBlockWidget::setTipLabText(const QString &text) const
{
    ui->cover_widget->setTipLabText(" " + text + " ");   ///< 设置提示标签文本
}

/**
 * @brief 隐藏提示标签
 */
void LiveBlockWidget::setNoTipLab() const
{
    ui->cover_widget->setShowTip(false);                 ///< 隐藏提示标签
}

/**
 * @brief 设置封面宽高比
 * @param aspectRatio 宽高比
 */
void LiveBlockWidget::setAspectRatio(const float &aspectRatio)
{
    this->m_aspectRatio = aspectRatio;                   ///< 设置成员变量
    ui->cover_widget->setAspectRatio(aspectRatio);       ///< 设置封面宽高比
}

/**
 * @brief 设置圆形标准值
 * @param stander 标准值
 */
void LiveBlockWidget::setCircleStander(const int &stander) const
{
    auto &mask = ui->cover_widget->getMask();            ///< 获取遮罩
    mask.setStander(stander);                            ///< 设置圆形标准值
}

/**
 * @brief 设置提示样式表
 * @param styleSheet 样式表字符串
 */
void LiveBlockWidget::setTipStyleSheet(const QString &styleSheet) const
{
    ui->cover_widget->setTipStyleSheet(styleSheet);      ///< 设置提示样式表
}

/**
 * @brief 初始化界面
 * @note 设置提示数组、遮罩和控件属性
 */
void LiveBlockWidget::initUi() const
{
    QList<QString> tipArr{
        "第一开播", "第二天开播", "第三天开播", "第四天开播", "第五天开播", "第六天开播",
        "德州", "葫芦岛", "淄博", "深圳", "洛阳", "哈尔滨", "临沂",
        "福州", "东莞", "常德", "宝山", "昆明", "长沙", "厦门",
        "合肥", "苏州", "龙岩", "西安", "嘉兴", "上海", "广州",
        "重庆", "三明", "龙岩", "西安", "商丘", "常州", "宁波",
        "威海", "徐州", "十佳工会推荐主播", "宜昌", "廊坊", "菏泽"}; ///< 提示数组
    ui->cover_widget->setPopularDirection(1);            ///< 设置流行按钮方向
    ui->cover_widget->setLeftPopularBtnIcon("");         ///< 清空按钮图标
    ui->cover_widget->setPopularBtnLeftPadding(0);       ///< 设置按钮左边距
    ui->cover_widget->setLeftPopularBtnFontSize(13, true); ///< 设置字体大小和加粗
    ui->cover_widget->setAspectRatio(this->m_aspectRatio); ///< 设置宽高比
    ui->cover_widget->setShowTip();                      ///< 显示提示
    ui->cover_widget->setTipArr(tipArr);                 ///< 设置提示数组
    ui->cover_widget->setTipStyleSheet(QStringLiteral("border:1px solid rgba(255,255,255,50%);border-radius:10px;background-color:rgba(0,0,0,50%);color:white;")); ///< 设置提示样式
    ui->cover_widget->setHaveNumberUnit(false);          ///< 禁用数字单位
    auto &mask = ui->cover_widget->getMask();            ///< 获取遮罩
    mask.setDefaultFillCircleColor(Qt::white);           ///< 设置默认圆形填充颜色
    mask.setDefaultFillTriangleColor(QColor(0, 0, 0, 200)); ///< 设置默认三角形填充颜色
    mask.setHoverFillCircleColor(Qt::white);             ///< 设置悬停圆形填充颜色
    mask.setHoverFillTriangleColor(QColor());            ///< 设置悬停三角形填充颜色
    mask.setStander(100);                                ///< 设置圆形标准值
    mask.setMove(true);                                  ///< 启用遮罩移动
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void LiveBlockWidget::mousePressEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略鼠标按下
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void LiveBlockWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略鼠标释放
}

/**
 * @brief 鼠标双击事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void LiveBlockWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略鼠标双击
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件
 * @note 根据宽高比调整封面高度
 */
void LiveBlockWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);                         ///< 调用父类事件
    ui->cover_widget->setFixedHeight(ui->cover_widget->width() / m_aspectRatio); ///< 设置封面高度
}