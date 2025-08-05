/**
 * @file ChatView.cpp
 * @brief 实现 ChatView 类，提供聊天视图功能
 * @author WeiWang
 * @date 2025-04-14
 * @version 1.0
 */

#include "ChatView.h"
#include "logger.hpp"
#include "MyScrollArea.h"

#include <QScrollBar>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>

/**
 * @brief 构造函数，初始化聊天视图
 * @param parent 父控件指针，默认为 nullptr
 */
ChatView::ChatView(QWidget *parent)
    : QWidget(parent)
    , isAppended(false)
    , m_centerInitWidget(new QWidget(this))
{
    {
        auto logoLab = new QLabel(this);              ///< Logo 标签
        logoLab->setFixedSize(50, 50);              ///< 设置 Logo 尺寸
        logoLab->setPixmap(QPixmap(":/Res/window/deepseek.png").scaled(logoLab->size())); ///< 设置 Logo 图片
        auto helloTextLab = new QLabel(this);
        helloTextLab->setFixedHeight(70);           ///< 设置欢迎文本高度
        helloTextLab->setText("我是DeepSeek, 很高兴见到你!"); ///< 设置欢迎文本
        helloTextLab->setStyleSheet("color: black;font-size: 22px;"); ///< 设置欢迎文本样式
        auto hlay = new QHBoxLayout;                     ///< 创建水平布局
        hlay->setSpacing(20);                            ///< 设置间距
        hlay->setAlignment(Qt::AlignCenter);             ///< 设置居中对齐
        hlay->addWidget(logoLab);                   ///< 添加 Logo
        hlay->addWidget(helloTextLab);              ///< 添加欢迎文本
        auto funcTextLab = new QLabel(this);           ///< 功能描述标签
        funcTextLab->setText("我可以帮你写代码、读文件、写作各种创意内容，请把你的任务交给我吧~"); ///< 设置功能描述
        funcTextLab->setStyleSheet("color: #404040;font-family: 'TaiwanPearl';font-size: 12px;"); ///< 设置功能描述样式
        auto vlay = new QVBoxLayout(this->m_centerInitWidget); ///< 创建垂直布局
        vlay->setAlignment(Qt::AlignCenter);             ///< 设置居中对齐
        vlay->setSpacing(20);                            ///< 设置间距
        vlay->addLayout(hlay);                           ///< 添加水平布局
        vlay->addWidget(funcTextLab);               ///< 添加功能描述
        this->m_centerInitWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); ///< 设置扩展策略
    }

    auto pMainLayout = new QVBoxLayout(this);            ///< 创建主布局
    pMainLayout->setContentsMargins(0, 0, 0, 0);        ///< 设置边距

    m_pScrollArea = new MyScrollArea();                  ///< 创建滚动区域
    m_pScrollArea->setObjectName("scrollArea");
    m_pScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); ///< 设置扩展策略
    m_pScrollArea->setFrameShape(QFrame::NoFrame);       ///< 设置无边框
    pMainLayout->addWidget(m_pScrollArea);               ///< 添加滚动区域

    auto w = new QWidget(this);                          ///< 创建内容控件
    w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); ///< 设置扩展策略
    auto pVLayout_1 = new QVBoxLayout(w);                ///< 创建垂直布局
    pVLayout_1->addStretch();                            ///< 添加底部拉伸
    m_pScrollArea->setWidget(w);                         ///< 设置滚动区域内容

    // 设置滚动区域透明
    w->setAttribute(Qt::WA_TranslucentBackground);
    w->setAutoFillBackground(false);

    m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn); ///< 显示垂直滚动条
    m_pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); ///< 隐藏水平滚动条
    auto pVScrollBar = m_pScrollArea->verticalScrollBar(); ///< 获取垂直滚动条
    connect(pVScrollBar, &QScrollBar::rangeChanged, this, &ChatView::onVScrollBarMoved); ///< 连接滚动条范围变化信号

    m_centerInitWidget->setParent(m_pScrollArea->viewport()); ///< 设置居中部件父对象
    m_centerInitWidget->setAttribute(Qt::WA_TransparentForMouseEvents); ///< 穿透鼠标事件
    m_centerInitWidget->raise();                         ///< 置于顶层
    auto *opacityEffect = new QGraphicsOpacityEffect(m_centerInitWidget); ///< 创建透明度效果
    opacityEffect->setOpacity(1.0);                      ///< 设置初始透明度
    m_centerInitWidget->setGraphicsEffect(opacityEffect); ///< 设置透明度效果

}

/**
 * @brief 尾部插入聊天项
 * @param item 聊天项控件
 */
void ChatView::appendChatItem(QWidget *item)
{
    const auto vl = getLayout();                         ///< 获取垂直布局
    if (vl)
    {
        vl->insertWidget(vl->count() - 1, item);         ///< 插入聊天项（前于拉伸项）
        if (vl->count() == 2)
        {
            startFadeOutAnimation();                     ///< 触发淡出动画
        }
    }
    else
        STREAM_WARN() << "ChatView::appendChatItem(): layout is nullptr"; ///< 记录警告日志
    isAppended = true;                                   ///< 标记追加状态
}

/**
 * @brief 头部插入聊天项
 * @param item 聊天项控件
 */
void ChatView::prependChatItem(QWidget *item)
{
    // 未实现
}

/**
 * @brief 中间插入聊天项
 * @param before 前置控件
 * @param item 聊天项控件
 */
void ChatView::insertChatItem(QWidget *before, QWidget *item)
{
    // 未实现
}

/**
 * @brief 删除最后一个聊天项
 */
void ChatView::removeLastItem()
{
    auto layout = getLayout();                           ///< 获取垂直布局
    if (!layout)
    {
        qWarning() << "ChatView::removeLastItem(): layout is null.";
        STREAM_WARN() << "ChatView::removeLastItem(): layout is null."; ///< 记录警告日志
        return;
    }
    if (layout->count() > 1)                             ///< 检查是否存在聊天项
    {
        const int lastItemIndex = layout->count() - 2;   ///< 最后一个聊天项索引
        if (QLayoutItem *item = layout->takeAt(lastItemIndex))
        {
            if (QWidget *widget = item->widget())
            {
                widget->deleteLater();                   ///< 安全删除控件
            }
            delete item;                                 ///< 删除布局项
        }
    }
    update();                                            ///< 更新视图
}

/**
 * @brief 删除所有聊天项
 */
void ChatView::removeAllItem()
{
    auto layout = getLayout();                           ///< 获取垂直布局
    if (!layout)
        return;
    if (layout->count() == 1)
        return;                                          ///< 仅包含拉伸项时跳过
    QList<QLayoutItem*> itemsToRemove;
    for (int i = 0; i < layout->count(); ++i)
    {
        QLayoutItem *item = layout->itemAt(i);
        if (item->widget() && item->widget() != m_centerInitWidget)
        {
            itemsToRemove.append(item);                  ///< 收集非居中部件
        }
    }
    for (QLayoutItem *item : itemsToRemove)
    {
        layout->removeItem(item);                        ///< 移除布局项
        if (QWidget *widget = item->widget())
        {
            widget->deleteLater();                       ///< 安全删除控件
        }
        delete item;                                     ///< 删除布局项
    }
    if (layout->count() == 1)
    {
        updateCenterWidgetPosition();                    ///< 更新居中部件位置
        startFadeInAnimation();                          ///< 触发淡入动画
    }
    update();                                            ///< 更新视图
}

/**
 * @brief 获取垂直布局
 * @return 垂直布局指针
 */
QVBoxLayout* ChatView::getLayout() const
{
    if (!m_pScrollArea || !m_pScrollArea->widget())
    {
        qWarning() << "ChatView::getLayout(): m_pScrollArea or its widget is null.";
        STREAM_WARN() << "ChatView::getLayout(): m_pScrollArea or its widget is null."; ///< 记录警告日志
        return nullptr;
    }
    auto layout = qobject_cast<QVBoxLayout*>(m_pScrollArea->widget()->layout());
    if (!layout)
    {
        qWarning() << "ChatView::getLayout(): Layout is not a QVBoxLayout.";
        STREAM_WARN() << "ChatView::getLayout(): Layout is not a QVBoxLayout."; ///< 记录警告日志
    }
    return layout;
}

/**
 * @brief 更新居中部件位置
 */
void ChatView::updateCenterWidgetPosition()
{
    if (!m_centerInitWidget || !m_pScrollArea)
        return;
    const QSize containerSize = m_pScrollArea->viewport()->size(); ///< 获取视口尺寸
    const QSize widgetSize = m_centerInitWidget->size();  ///< 获取部件尺寸
    const int x = (containerSize.width() - widgetSize.width()) / 2; ///< 计算 X 坐标
    const int y = (containerSize.height() - widgetSize.height()) / 2; ///< 计算 Y 坐标
    m_centerInitWidget->move(x, y);                      ///< 更新位置
}

/**
 * @brief 启动淡出动画
 */
void ChatView::startFadeOutAnimation()
{
    m_centerInitWidget->show();                          ///< 确保可见
    auto *effect = qobject_cast<QGraphicsOpacityEffect*>(m_centerInitWidget->graphicsEffect());  ///< 获取透明度效果
    QPropertyAnimation *anim = new QPropertyAnimation(effect, "opacity"); ///< 创建动画
    anim->setDuration(300);                              ///< 设置持续时间
    anim->setStartValue(1.0);                            ///< 设置起始透明度
    anim->setEndValue(0.0);                              ///< 设置结束透明度
    anim->setEasingCurve(QEasingCurve::OutQuad);         ///< 设置缓动曲线
    connect(anim, &QPropertyAnimation::finished, [this]()
    {
        m_centerInitWidget->hide();                      ///< 动画完成时隐藏
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);  ///< 启动动画并自动删除
}

/**
 * @brief 启动淡入动画
 */
void ChatView::startFadeInAnimation()
{
    updateCenterWidgetPosition();                        ///< 更新位置
    m_centerInitWidget->show();                          ///< 确保可见
    auto *effect = qobject_cast<QGraphicsOpacityEffect*>(m_centerInitWidget->graphicsEffect());  ///< 获取透明度效果
    effect->setOpacity(0.0);                             ///< 设置初始透明度
    QPropertyAnimation *anim = new QPropertyAnimation(effect, "opacity"); ///< 创建动画
    anim->setDuration(300);                              ///< 设置持续时间
    anim->setStartValue(0.0);                            ///< 设置起始透明度
    anim->setEndValue(1.0);                              ///< 设置结束透明度
    anim->setEasingCurve(QEasingCurve::InQuad);          ///< 设置缓动曲线
    anim->start(QAbstractAnimation::DeleteWhenStopped);  ///< 启动动画并自动删除
}

/**
 * @brief 调整大小事件
 * @param event 调整大小事件对象
 */
void ChatView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateCenterWidgetPosition();                        ///< 更新居中部件位置
}

void ChatView::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    updateCenterWidgetPosition();
}

/**
 * @brief 处理垂直滚动条范围变化
 * @param min 最小值
 * @param max 最大值
 */
void ChatView::onVScrollBarMoved(int min, int max)
{
    if (isAppended)                                      ///< 检查追加状态
    {
        auto pVScrollBar = m_pScrollArea->verticalScrollBar(); ///< 获取滚动条
        pVScrollBar->setSliderPosition(pVScrollBar->maximum()); ///< 滚动到底部
        QTimer::singleShot(500, [this]()
        {
            isAppended = false;                          ///< 重置追加状态
        });
    }
}