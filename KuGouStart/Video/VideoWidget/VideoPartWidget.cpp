/**
 * @file VideoPartWidget.cpp
 * @brief 实现 VideoPartWidget 类，管理视频分区界面
 * @author WeiWang
 * @date 2024-12-15
 * @version 1.0
 */

#include "VideoPartWidget.h"

#include <QFile>
#include <QHBoxLayout>
#include <QLabel>

/**
 * @brief 构造函数，初始化视频分区
 * @param parent 父控件指针，默认为 nullptr
 */
VideoPartWidget::VideoPartWidget(QWidget *parent)
    : QWidget(parent)
    , m_titleLab(new QLabel(this))                      ///< 初始化标题标签
    , m_tabWidget(new QWidget(this))                    ///< 初始化视频块容器
{
    initUi();                                            ///< 初始化界面
}

/**
 * @brief 设置标题名称
 * @param title 标题文本
 * @note 更新标题标签
 */
void VideoPartWidget::setTitleName(const QString &title) const
{
    this->m_titleLab->setText(" " + title);             ///< 设置标题文本
}

/**
 * @brief 添加视频块
 * @param x 网格行坐标
 * @param y 网格列坐标
 * @param block 视频块指针
 * @note 将视频块添加到网格布局
 */
void VideoPartWidget::addBlockWidget(const int &x, const int &y, VideoBlockWidget *block) const
{
    auto lay = static_cast<QGridLayout *>(this->m_tabWidget->layout()); ///< 获取网格布局
    lay->addWidget(block, x, y);                        ///< 添加视频块
}

/**
 * @brief 初始化界面
 * @note 设置标题、容器和网格布局
 */
void VideoPartWidget::initUi()
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum); ///< 设置大小策略
    this->setContentsMargins(0, 0, 0, 0);               ///< 设置边距
    this->m_titleLab->setStyleSheet("font-weight: bold;font-size: 16px;color: black;"); ///< 设置标题样式
    auto hlay = new QHBoxLayout;                        ///< 创建水平布局
    hlay->addWidget(this->m_titleLab);                  ///< 添加标题标签
    hlay->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Preferred)); ///< 添加间隔
    auto vlay = new QVBoxLayout(this);                  ///< 创建垂直布局
    vlay->addLayout(hlay);                              ///< 添加水平布局
    vlay->addWidget(this->m_tabWidget);                 ///< 添加容器
    this->m_tabWidget->setContentsMargins(0, 0, 0, 0);  ///< 设置容器边距
    auto lay = new QGridLayout(this->m_tabWidget);      ///< 创建网格布局
    lay->setContentsMargins(0, 0, 0, 0);                ///< 设置布局边距
    lay->setRowStretch(0, 1);                           ///< 设置行拉伸
    lay->setRowStretch(1, 1);
    lay->setColumnStretch(0, 1);                        ///< 设置列拉伸
    lay->setColumnStretch(1, 1);
    lay->setColumnStretch(2, 1);
    lay->setHorizontalSpacing(8);                       ///< 设置水平间距
    lay->setVerticalSpacing(8);                         ///< 设置垂直间距

    // @note 未使用，保留用于调试
    // auto lay = new MyFlowLayout(this->m_tabWidget, 15, -1, -1);
    // auto lay = new MyFlowLayout(this->m_tabWidget, true, 5);
    // lay->setContentsMargins(0, 0, 0, 0);
    // this->m_tabWidget->setLayout(lay);
}

/**
 * @brief 绘制事件
 * @param event 绘制事件
 * @note 默认实现
 */
void VideoPartWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);                         ///< 调用父类处理
}