/**
 * @file SongBlock.cpp
 * @brief 实现 SongBlock 类，管理歌曲块界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#include "SongBlock.h"
#include "MyBlockWidget.h"

#include <QLabel>
#include <QMouseEvent>
#include <QRandomGenerator>
#include <QVBoxLayout>

/**
 * @brief 构造函数，初始化歌曲块
 * @param parent 父控件指针，默认为 nullptr
 */
SongBlock::SongBlock(QWidget *parent)
    : QWidget(parent)
    , m_cover(new MyBlockWidget(this))                   ///< 初始化封面控件
    , m_descLab(new QLabel(this))                       ///< 初始化描述标签
{
    initUi();                                            ///< 初始化界面
}

/**
 * @brief 初始化界面
 * @note 设置布局、样式、遮罩和流行按钮
 */
void SongBlock::initUi()
{
    this->m_cover->setCursor(Qt::PointingHandCursor);   ///< 设置封面光标
    this->m_descLab->setCursor(Qt::PointingHandCursor); ///< 设置描述标签光标

    auto *mainLayout = new QVBoxLayout(this);           ///< 创建垂直布局
    mainLayout->setContentsMargins(0, 0, 0, 10);        ///< 设置边距
    mainLayout->setSpacing(3);                          ///< 设置间距
    mainLayout->addWidget(m_cover);                     ///< 添加封面控件
    mainLayout->addWidget(m_descLab);                   ///< 添加描述标签

    m_descLab->setStyleSheet("QLabel{color:black;font-size:14px;}QLabel:hover{color:#3AA1FF;}"); ///< 设置描述标签样式

    auto &mask = this->m_cover->getMask();              ///< 获取遮罩
    mask.setDefaultFillCircleColor(Qt::white);          ///< 设置默认圆形填充颜色
    mask.setHoverFillCircleColor(QColor(QStringLiteral("#26A1FF"))); ///< 设置悬停圆形填充颜色
    mask.setDefaultFillTriangleColor(QColor(QStringLiteral("#666666"))); ///< 设置默认三角形填充颜色
    mask.setHoverFillTriangleColor(QColor(QStringLiteral("#666666"))); ///< 设置悬停三角形填充颜色
    mask.setEnterWidgetChangeCursor(true);              ///< 启用悬停光标
    mask.setStander(130);                               ///< 设置遮罩标准值

    this->m_cover->setPopularDirection(2);              ///< 设置流行按钮方向
    this->m_cover->setHaveNumberUnit(false);            ///< 默认无单位

    auto number = QRandomGenerator::global()->bounded(1, 5000); ///< 生成随机数
    if (number <= 500)
    {
        this->m_cover->setHaveNumberUnit(true);         ///< 设置带单位
        const auto n = QRandomGenerator::global()->generateDouble() * 100; ///< 生成随机播放量
        this->m_cover->setPopularBtnText(QString::number(n, 'f', 2)); ///< 设置播放量
    }
    else
    {
        this->m_cover->setPopularBtnText(QString::number(number)); ///< 设置播放量
    }
    this->m_cover->setFixedSize(160, 160);             ///< 设置封面固定大小
}

/**
 * @brief 设置封面图片
 * @param pixmapPath 图片路径
 * @note 更新封面显示
 */
void SongBlock::setCoverPix(const QString &pixmapPath) const
{
    this->m_cover->setBorderImage(pixmapPath);          ///< 设置封面图片
}

/**
 * @brief 设置描述文本
 * @param text 描述文本
 * @note 更新描述标签并调整换行
 */
void SongBlock::setDescText(const QString &text) const
{
    this->m_descLab->setText(text);                     ///< 设置描述文本
    updateDescLab();                                    ///< 更新描述标签
}

/**
 * @brief 更新描述标签
 * @note 处理文本换行和省略
 */
void SongBlock::updateDescLab() const
{
    this->m_descLab->setFixedWidth(this->width());      ///< 设置描述标签宽度
    auto font = this->m_descLab->font();                ///< 获取字体
    QFontMetrics fm(font);                              ///< 创建字体测量工具

    const int availableWidth = this->m_descLab->width() + 20; ///< 计算可用宽度
    const QString text = this->m_descLab->text();       ///< 获取描述文本
    QString elidedText = text;
    int breakIndex = 0;
    for (int i = 0; i < text.length(); ++i)
    {
        if (fm.horizontalAdvance(text.left(i)) > availableWidth)
        {
            breakIndex = i - 1;                         ///< 找到换行位置
            break;
        }
    }

    const QString firstLine = text.left(breakIndex);    ///< 分割第一行
    const QString secondLine = text.mid(breakIndex);    ///< 分割第二行
    // @note 未使用，保留用于调试
    // qDebug() << "secondLine: " << secondLine;
    const QString secondLineElided = fm.elidedText(secondLine, Qt::ElideRight, availableWidth); ///< 处理第二行省略
    // @note 未使用，保留用于调试
    // qDebug() << "secondLineElided: " << secondLineElided << " secondLineElided.width(): " << fm.horizontalAdvance(secondLineElided) << " availableWidth : " << availableWidth;

    elidedText = firstLine + secondLineElided;          ///< 拼接两行文本
    this->m_descLab->setText(elidedText);               ///< 设置描述文本
    this->m_descLab->setWordWrap(true);                 ///< 启用换行

    const int lineHeight = fm.lineSpacing();            ///< 获取单行高度
    this->m_descLab->setFixedHeight(2 * lineHeight);    ///< 设置两行高度
}

/**
 * @brief 显示提示标签
 * @note 启用封面的提示标签
 */
void SongBlock::setShowTip() const
{
    this->m_cover->setShowTip();                        ///< 显示提示标签
    this->m_cover->setTipStyleSheet(QStringLiteral("border-radius:10px;background-color:#797978;color:white;")); ///< 设置提示样式
}

/**
 * @brief 设置提示文本
 * @param text 提示文本
 * @note 更新封面的提示标签内容
 */
void SongBlock::setTipText(const QString &text) const
{
    this->m_cover->setTipLabText(text);                 ///< 设置提示文本
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件
 * @note 更新描述标签宽度
 */
void SongBlock::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);                        ///< 调用父类处理
    updateDescLab();                                    ///< 更新描述标签
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void SongBlock::mousePressEvent(QMouseEvent *event)
{
    event->ignore();                                    ///< 忽略事件
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void SongBlock::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();                                    ///< 忽略事件
}

/**
 * @brief 鼠标双击事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void SongBlock::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore();                                    ///< 忽略事件
}

/**
 * @brief 显示事件
 * @param event 显示事件
 * @note 更新描述标签
 */
void SongBlock::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);                          ///< 调用父类处理
    updateDescLab();                                    ///< 更新描述标签
    // @note 未使用，保留用于调试
    // qDebug() << "当前大小: " << this->size();
}