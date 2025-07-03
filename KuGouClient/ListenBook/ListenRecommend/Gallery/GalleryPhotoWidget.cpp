/**
 * @file GalleryPhotoWidget.cpp
 * @brief 实现 GalleryPhotoWidget 类，提供照片卡片控件功能
 * @author WeiWang
 * @date 2025-05-16
 * @version 1.0
 */

#include "GalleryPhotoWidget.h"
#include "ElaToolTip.h"

#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 控件固定宽度
 */
int GalleryPhotoWidget::fixed_width = 160;

/**
 * @brief 控件固定高度
 */
int GalleryPhotoWidget::fixed_height = 240;

/**
 * @brief 内容区域宽度
 */
int GalleryPhotoWidget::content_width = 130;

/**
 * @brief 内容区域高度
 */
int GalleryPhotoWidget::content_height = 200;

/**
 * @brief 图片宽度
 */
int GalleryPhotoWidget::pixmap_width = 130;

/**
 * @brief 图片高度
 */
int GalleryPhotoWidget::pixmap_height = 130;

/**
 * @brief 构造函数，初始化照片卡片控件
 * @param parent 父控件指针，默认为 nullptr
 */
GalleryPhotoWidget::GalleryPhotoWidget(QWidget *parent)
    : WaterZoomButton(parent, "")
    , m_coverWidget(new MyBlockWidget(this))
    , m_titleLab(new QLabel(this))
    , m_descLab(new QLabel(this))
    , m_shadowEffect(new QGraphicsDropShadowEffect(this))
{
    setNormalColor(Qt::white);                           ///< 设置正常背景色
    setHoverColor(Qt::white);                            ///< 设置悬停背景色
    setChoking(10);                                      ///< 设置水波效果边界
    setRadius(15, 15);                                   ///< 设置圆角半径
    setFixedSize(fixed_width, fixed_height);             ///< 设置固定尺寸
    initUi();                                            ///< 初始化界面
    {
        if (QFile file(GET_CURRENT_DIR + QStringLiteral("/photo.css")); file.open(QIODevice::ReadOnly))
        {
            this->setStyleSheet(file.readAll());         ///< 加载样式表
        }
        else
        {
            qDebug() << "样式表打开失败QAQ";
            return;
        }
    }
}

/**
 * @brief 设置封面图片
 * @param pixmapPath 图片路径
 */
void GalleryPhotoWidget::setCoverPix(const QString &pixmapPath) const
{
    this->m_coverWidget->setBorderImage(pixmapPath, 10); ///< 设置封面图片和边界
}

/**
 * @brief 设置标题文本
 * @param title 标题文本
 */
void GalleryPhotoWidget::setTitleText(const QString &title)
{
    this->m_titleText = title;                           ///< 存储标题文本
    this->m_titleLab->setText(title);                    ///< 设置标题标签文本
    auto titleLab_toolTip = new ElaToolTip(this->m_titleLab); ///< 创建工具提示
    titleLab_toolTip->setToolTip(title);                 ///< 设置工具提示内容
    updateTitleText();                                   ///< 更新标题显示
}

/**
 * @brief 设置描述文本
 * @param desc 描述文本
 */
void GalleryPhotoWidget::setDescribeText(const QString &desc)
{
    this->m_describeText = desc;                         ///< 存储描述文本
    this->m_descLab->setText(desc);                      ///< 设置描述标签文本
    updateDescText();                                    ///< 更新描述显示
}

/**
 * @brief 设置流行度文本
 * @param text 流行度文本
 */
void GalleryPhotoWidget::setPopularText(const QString &text) const
{
    this->m_coverWidget->setPopularBtnText(text);        ///< 设置封面控件的流行度文本
}

/**
 * @brief 初始化界面
 * @note 设置阴影效果、背景色、遮罩和标签
 */
void GalleryPhotoWidget::initUi()
{
    m_shadowEffect->setBlurRadius(10);                   ///< 设置阴影模糊半径
    m_shadowEffect->setColor(Qt::gray);                  ///< 设置阴影颜色
    m_shadowEffect->setOffset(3, 3);                     ///< 设置阴影偏移
    m_shadowEffect->setEnabled(false);                   ///< 默认禁用阴影
    this->setGraphicsEffect(this->m_shadowEffect);       ///< 应用阴影效果
    this->setBgColor(QColor(QStringLiteral("#F0F8FF"))); ///< 设置默认背景色
    this->setBgColor(QColor(QStringLiteral("#ECF6FF")), this->press_bg); ///< 设置按下背景色
    this->m_titleLab->setObjectName("titleLab");         ///< 设置标题标签对象名称
    this->m_descLab->setObjectName("descLab");           ///< 设置描述标签对象名称
    this->m_titleLab->setMouseTracking(true);            ///< 启用标题标签鼠标跟踪
    auto &mask = this->m_coverWidget->getMask();         ///< 获取封面控件遮罩
    mask.setDefaultFillCircleColor(Qt::white);           ///< 设置默认圆形填充颜色
    mask.setHoverFillCircleColor(QColor(QStringLiteral("#26A1FF"))); ///< 设置悬停圆形填充颜色
    mask.setDefaultFillTriangleColor(QColor(QStringLiteral("#666666"))); ///< 设置默认三角形填充颜色
    mask.setHoverFillTriangleColor(QColor(QStringLiteral("#666666"))); ///< 设置悬停三角形填充颜色
    mask.setMaskColor(QColor(0, 0, 0, 100));             ///< 设置遮罩颜色
    mask.setStander(120);                                ///< 设置遮罩标准值
    this->m_coverWidget->setPopularDirection(1);         ///< 设置流行度按钮方向
    this->m_coverWidget->setHaveNumberUnit(false);       ///< 禁用数字单位
    this->m_coverWidget->setLeftPopularBtnIcon(QStringLiteral(":/TabIcon/Res/tabIcon/play3-white.svg")); ///< 设置左流行度按钮图标
}

/**
 * @brief 更新标题文本
 * @note 处理换行和省略显示
 */
void GalleryPhotoWidget::updateTitleText() const
{
    const auto font = this->m_titleLab->font();          ///< 获取标题标签字体
    const QFontMetrics fm(font);                         ///< 创建字体测量工具
    const int availableWidth = this->m_titleLab->width(); ///< 获取可用宽度
    const QString text = this->m_titleText;              ///< 获取标题文本
    QString elidedText = text;                           ///< 初始化省略文本
    int breakIndex = 0;                                  ///< 换行位置
    for (int i = 0; i < text.length(); ++i)
    {
        if (fm.horizontalAdvance(text.left(i)) > availableWidth)
        {
            breakIndex = i - 1;                          ///< 找到换行点
            break;
        }
    }
    const QString firstLine = text.left(breakIndex);     ///< 第一行文本
    const QString secondLine = text.mid(breakIndex);     ///< 第二行文本
    const QString secondLineElided = fm.elidedText(secondLine, Qt::ElideRight, availableWidth); ///< 第二行省略文本
    elidedText = firstLine + secondLineElided;           ///< 拼接文本
    this->m_titleLab->setText(elidedText);               ///< 设置标题标签文本
    this->m_titleLab->setWordWrap(true);                 ///< 启用自动换行
    const int lineHeight = fm.lineSpacing();             ///< 获取单行高度
    this->m_titleLab->setFixedHeight(2 * lineHeight);    ///< 设置两行高度
}

/**
 * @brief 更新描述文本
 * @note 处理换行和省略显示
 */
void GalleryPhotoWidget::updateDescText() const
{
    const auto font = this->m_descLab->font();           ///< 获取描述标签字体
    const QFontMetrics fm(font);                         ///< 创建字体测量工具
    const int availableWidth = this->m_descLab->width(); ///< 获取可用宽度
    const QString text = this->m_describeText;           ///< 获取描述文本
    QString elidedText = text;                           ///< 初始化省略文本
    int breakIndex = 0;                                  ///< 换行位置
    for (int i = 0; i < text.length(); ++i)
    {
        if (fm.horizontalAdvance(text.left(i)) > availableWidth)
        {
            breakIndex = i - 1;                          ///< 找到换行点
            break;
        }
    }
    const QString firstLine = text.left(breakIndex);     ///< 第一行文本
    const QString secondLine = text.mid(breakIndex);     ///< 第二行文本
    const QString secondLineElided = fm.elidedText(secondLine, Qt::ElideRight, availableWidth); ///< 第二行省略文本
    elidedText = firstLine + secondLineElided;           ///< 拼接文本
    this->m_descLab->setText(elidedText);                ///< 设置描述标签文本
    this->m_descLab->setWordWrap(true);                  ///< 启用自动换行
    const int lineHeight = fm.lineSpacing();             ///< 获取单行高度
    this->m_descLab->setFixedHeight(2 * lineHeight);     ///< 设置两行高度
}

/**
 * @brief 绘制事件，调整封面和标签位置
 * @param event 绘制事件
 */
void GalleryPhotoWidget::paintEvent(QPaintEvent *event)
{
    WaterZoomButton::paintEvent(event);                  ///< 调用父类绘制事件
    QPainter painter(this);                              ///< 创建画家
    painter.setRenderHint(QPainter::Antialiasing, true); ///< 启用抗锯齿
    painter.setRenderHints(QPainter::SmoothPixmapTransform); ///< 启用平滑图像转换
    int c, margin;                                       ///< 定义边界和边距
    if (!hover_progress)
    {
        c = choking;                                     ///< 默认边界
        margin = 15;                                     ///< 默认边距
    }
    else
    {
        c = choking * (1 - getNolinearProg(hover_progress, hovering ? FastSlower : SlowFaster)); ///< 动态边界
        margin = sqrt(125 - hover_progress);             ///< 动态边距
    }
    const QRect rect(c + margin, c + margin, this->width() - c * 2 - margin * 2, (this->width() - c * 2 - margin * 2) * pixmap_height / pixmap_width); ///< 计算封面区域
    this->m_coverWidget->move(rect.left(), rect.top());  ///< 移动封面控件
    this->m_coverWidget->setFixedSize(rect.size());      ///< 设置封面尺寸
    this->m_titleLab->move(this->m_coverWidget->x(), this->m_coverWidget->y() + this->m_coverWidget->height() + 5); ///< 移动标题标签
    this->m_descLab->move(this->m_titleLab->x(), this->m_titleLab->y() + this->m_titleLab->height() + 5); ///< 移动描述标签
    this->m_titleLab->setFixedWidth(this->m_coverWidget->width()); ///< 设置标题标签宽度
    updateTitleText();                                   ///< 更新标题文本
    this->m_descLab->setFixedWidth(this->m_coverWidget->width()); ///< 设置描述标签宽度
    updateDescText();                                    ///< 更新描述文本
}

/**
 * @brief 鼠标移动事件，处理标题颜色变化
 * @param event 鼠标事件
 */
void GalleryPhotoWidget::mouseMoveEvent(QMouseEvent *event)
{
    WaterZoomButton::mouseMoveEvent(event);              ///< 调用父类鼠标移动事件
    if (this->m_titleLab->rect().contains(this->m_titleLab->mapFromParent(event->pos())))
    {
        this->m_titleLab->setStyleSheet("QLabel#titleLab { color: #2291e6; font-size: 15px; }"); ///< 设置悬停样式
    }
    else
    {
        this->m_titleLab->setStyleSheet("QLabel#titleLab { color: black; font-size: 15px; }"); ///< 恢复默认样式
    }
}

/**
 * @brief 鼠标离开事件，恢复标题颜色和阴影
 * @param event 事件
 */
void GalleryPhotoWidget::leaveEvent(QEvent *event)
{
    WaterZoomButton::leaveEvent(event);                  ///< 调用父类鼠标离开事件
    this->m_titleLab->setStyleSheet("QLabel#titleLab { color: black; font-size: 15px; }"); ///< 恢复标题样式
    m_shadowEffect->setEnabled(false);                   ///< 禁用阴影
}

/**
 * @brief 鼠标进入事件，启用阴影效果
 * @param event 进入事件
 */
void GalleryPhotoWidget::enterEvent(QEnterEvent *event)
{
    WaterZoomButton::enterEvent(event);                  ///< 调用父类鼠标进入事件
    m_shadowEffect->setEnabled(true);                    ///< 启用阴影
}