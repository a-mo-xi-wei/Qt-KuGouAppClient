/**
 * @file ChatItemBase.cpp
 * @brief 实现 ChatItemBase 类，提供聊天项基类功能
 * @author WeiWang
 * @date 2024-11-12
 * @version 1.0
 */

#include "ChatItemBase.h"
#include "BubbleFrame.h"

#include <QFont>
#include <QMovie>

/**
 * @brief 构造函数，初始化聊天项
 * @param role 聊天角色（自己、他人、时间）
 * @param parent 父控件指针，默认为 nullptr
 */
ChatItemBase::ChatItemBase(const ChatRole role, QWidget *parent)
    : QWidget(parent)
    , m_role(role)
    , m_pBubble(std::make_unique<QWidget>(this))
{
    auto pGLayout = new QGridLayout(this);                   ///< 创建网格布局
    pGLayout->setVerticalSpacing(3);                        ///< 设置垂直间距
    pGLayout->setHorizontalSpacing(3);                      ///< 设置水平间距
    pGLayout->setContentsMargins(3, 3, 3, 3);               ///< 设置边距
    auto pSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum); ///< 创建弹性空间
    if (m_role == ChatRole::Time)
    {
        auto pSpacerLeft = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum); ///< 左侧弹性空间
        auto pSpacerRight = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum); ///< 右侧弹性空间
        pGLayout->addItem(pSpacerLeft, 0, 0, 1, 1);         ///< 添加左侧空间
        pGLayout->addWidget(m_pBubble.get(), 0, 1, 1, 1, Qt::AlignCenter); ///< 添加气泡（居中）
        pGLayout->addItem(pSpacerRight, 0, 2, 1, 1);        ///< 添加右侧空间
        return; // 提前返回，避免创建无用控件                                        ///< 提前返回
    }
    m_pNameLabel = new QLabel(this);                       ///< 创建用户名标签
    m_pNameLabel->setObjectName("chat_user_name");         ///< 设置对象名称
    QFont font("Microsoft YaHei");                         ///< 设置字体
    font.setPointSize(9);                                  ///< 设置字号
    m_pNameLabel->setFont(font);
    m_pNameLabel->setFixedHeight(20);                      ///< 设置固定高度
    m_pIconLabel = new QLabel(this);                       ///< 创建头像标签
    m_pIconLabel->setScaledContents(true);                 ///< 启用缩放
    m_pIconLabel->setFixedSize(42, 42);                   ///< 设置固定尺寸
    if (m_role == ChatRole::Self)
    {
        m_pNameLabel->setContentsMargins(0, 0, 8, 0);      ///< 设置右对齐边距
        m_pNameLabel->setAlignment(Qt::AlignRight);        ///< 设置右对齐
        pGLayout->addWidget(m_pNameLabel, 0, 1, 1, 1);     ///< 添加用户名
        pGLayout->addWidget(m_pIconLabel, 0, 2, 2, 1, Qt::AlignTop); ///< 添加头像
        pGLayout->addItem(pSpacer, 1, 0, 1, 1);            ///< 添加左侧空间
        pGLayout->addWidget(m_pBubble.get(), 1, 1, 1, 1);  ///< 添加气泡
        pGLayout->setColumnStretch(0, 2);                  ///< 设置列拉伸
        pGLayout->setColumnStretch(1, 3);                  ///< 设置列拉伸
    }
    else if (m_role == ChatRole::Other)
    {
        initMovie();                                       ///< 初始化加载动画
        m_pNameLabel->setContentsMargins(8, 0, 0, 0);      ///< 设置左对齐边距
        m_pNameLabel->setAlignment(Qt::AlignLeft);         ///< 设置左对齐
        QHBoxLayout *nameLayout = new QHBoxLayout;         ///< 创建水平布局
        nameLayout->setContentsMargins(0, 0, 0, 0);        ///< 设置边距
        nameLayout->setSpacing(5);                         ///< 设置间距
        nameLayout->addWidget(m_pNameLabel);               ///< 添加用户名
        nameLayout->addWidget(m_loading);                  ///< 添加加载动画
        nameLayout->addStretch();                          ///< 添加弹性空间
        QWidget *nameWidget = new QWidget;                 ///< 创建容器
        nameWidget->setLayout(nameLayout);                 ///< 设置布局
        pGLayout->addWidget(m_pIconLabel, 0, 0, 2, 1, Qt::AlignTop); ///< 添加头像
        pGLayout->addWidget(nameWidget, 0, 1, 1, 2);       ///< 添加用户名容器
        pGLayout->addWidget(m_pBubble.get(), 1, 1, 1, 1);  ///< 添加气泡
        pGLayout->addItem(pSpacer, 2, 2, 1, 1);            ///< 添加右侧空间
        pGLayout->setColumnStretch(1, 3);                  ///< 设置列拉伸
        pGLayout->setColumnStretch(2, 2);                  ///< 设置列拉伸
    }
}

/**
 * @brief 设置用户名
 * @param name 用户名
 */
void ChatItemBase::setUserName(const QString &name) const
{
    m_pNameLabel->setText(name);                            ///< 设置用户名文本
}

/**
 * @brief 设置用户头像
 * @param icon 头像图片
 */
void ChatItemBase::setUserIcon(const QPixmap &icon) const
{
    m_pIconLabel->setPixmap(icon);                          ///< 设置头像图片
}

/**
 * @brief 设置气泡内容控件
 * @param w 内容控件指针
 */
void ChatItemBase::setWidget(QWidget *w)
{
    if (!w || !layout())
        return;                                             ///< 检查有效性
    auto pGLayout = qobject_cast<QGridLayout *>(layout());
    if (!pGLayout)
        return;                                             ///< 检查布局
    if (m_pBubble)
    {
        pGLayout->removeWidget(m_pBubble.get());           ///< 移除旧控件
        m_pBubble.reset();                                 ///< 释放旧控件
    }
    m_pBubble.reset(w);                                    ///< 接管新控件
    m_pBubble->setParent(this);                            ///< 设置父控件
    if (m_role == ChatRole::Time)
    {
        pGLayout->addWidget(m_pBubble.get(), 0, 1, 1, 1, Qt::AlignCenter); ///< 添加时间气泡
    }
    else
    {
        pGLayout->addWidget(m_pBubble.get(), 1, 1, 1, 1);  ///< 添加普通气泡
    }
}

/**
 * @brief 启动或停止加载动画
 * @param flag 是否启动
 */
void ChatItemBase::startMovie(const bool &flag)
{
    if (flag)
    {
        m_loading->show();                                 ///< 显示动画
        m_loadingMovie->start();                           ///< 启动动画
    }
    else
    {
        m_loading->hide();                                 ///< 隐藏动画
        m_loadingMovie->stop();                            ///< 停止动画
    }
}

/**
 * @brief 初始化加载动画
 */
void ChatItemBase::initMovie()
{
    m_loadingMovie = new QMovie(this);                     ///< 创建动画对象
    m_loadingMovie->setFileName(":/Res/window/loading.gif"); ///< 设置动画文件
    m_loading = new QLabel(this);                          ///< 创建动画标签
    m_loading->setMovie(m_loadingMovie);                   ///< 设置动画
    m_loading->setFixedSize(16, 16);                       ///< 设置固定尺寸
    m_loading->setAttribute(Qt::WA_TranslucentBackground, true); ///< 设置透明背景
    m_loading->setAutoFillBackground(false);               ///< 禁用背景填充
    m_loading->hide();                                     ///< 初始隐藏
    m_loadingMovie->stop();                                ///< 初始停止
}