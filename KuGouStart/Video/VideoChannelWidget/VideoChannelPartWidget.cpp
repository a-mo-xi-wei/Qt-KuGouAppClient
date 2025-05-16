/**
 * @file VideoChannelPartWidget.cpp
 * @brief 实现 VideoChannelPartWidget 类，提供视频频道分类部件功能
 * @author WeiWang
 * @date 2024-12-12
 * @version 1.0
 */

#include "VideoChannelPartWidget.h"
#include "MyFlowLayout.h"

#include <QFile>
#include <QHBoxLayout>
#include <QLabel>

/**
 * @brief 构造函数，初始化视频频道分类部件
 * @param parent 父控件指针，默认为 nullptr
 */
VideoChannelPartWidget::VideoChannelPartWidget(QWidget *parent)
    : QWidget(parent)
    , m_titleLab(new QLabel(this))
    , m_tabWidget(new QWidget(this))
{
    initUi();                                                     ///< 初始化界面
}

/**
 * @brief 设置标题名称
 * @param title 标题文本
 */
void VideoChannelPartWidget::setTitleName(const QString &title) const
{
    this->m_titleLab->setText(" " + title);                       ///< 设置标题文本
}

/**
 * @brief 添加视频块控件
 * @param block 视频块控件指针
 */
void VideoChannelPartWidget::addBlockWidget(VideoChannelBlock *block) const
{
    auto lay = static_cast<MyFlowLayout *>(this->m_tabWidget->layout()); ///< 获取流式布局
    lay->addWidget(block);                                        ///< 添加视频块
}

/**
 * @brief 初始化界面
 */
void VideoChannelPartWidget::initUi()
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum); ///< 设置大小策略
    this->setContentsMargins(0, 0, 0, 0);                         ///< 设置边距
    this->m_titleLab->setStyleSheet("font-weight: bold; font-size: 16px; color: black;"); ///< 设置标题样式

    auto hlay = new QHBoxLayout;                                  ///< 创建水平布局
    hlay->addWidget(this->m_titleLab);                            ///< 添加标题标签
    hlay->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Preferred)); ///< 添加弹性空间

    auto vlay = new QVBoxLayout(this);                            ///< 创建垂直布局
    vlay->addLayout(hlay);                                        ///< 添加水平布局
    vlay->addWidget(this->m_tabWidget);                           ///< 添加容器

    auto lay = new MyFlowLayout(this->m_tabWidget, true, 5);      ///< 创建流式布局
    lay->setContentsMargins(0, 0, 0, 0);                          ///< 设置布局边距
    this->m_tabWidget->setLayout(lay);                            ///< 设置容器布局
    this->m_tabWidget->setContentsMargins(0, 0, 0, 0);           ///< 设置容器边距
}

/**
 * @brief 绘制事件
 * @param event 绘制事件对象
 * @note 重写基类方法，当前为空实现
 */
void VideoChannelPartWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}