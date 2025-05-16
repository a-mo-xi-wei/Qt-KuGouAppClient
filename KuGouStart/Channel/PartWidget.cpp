/**
 * @file PartWidget.cpp
 * @brief 实现 PartWidget 类，提供频道分区界面功能
 * @author WeiWang
 * @date 2024-12-03
 * @version 1.0
 */

#include "PartWidget.h"
#include "MyFlowLayout.h"

#include <QFile>
#include <QHBoxLayout>
#include <QLabel>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化频道分区界面
 * @param parent 父控件指针，默认为 nullptr
 */
PartWidget::PartWidget(QWidget *parent)
    : QWidget(parent)
    , m_titleLab(new QLabel(this))
    , m_titleDeco(new QLabel(this))
    , m_tabWidget(new QWidget(this))
{
    initUi();                                            ///< 初始化界面
    {
        QFile file(GET_CURRENT_DIR + QStringLiteral("/part.css")); ///< 加载样式表
        if (file.open(QIODevice::ReadOnly))
        {
            this->setStyleSheet(file.readAll());         ///< 应用样式表
        }
        else
        {
            qDebug() << "样式表打开失败QAQ";
            STREAM_ERROR() << "样式表打开失败QAQ";      ///< 记录错误日志
            return;
        }
    }
}

/**
 * @brief 设置标题名称
 * @param title 标题内容
 */
void PartWidget::setTitleName(const QString &title) const
{
    this->m_titleLab->setText(" " + title);              ///< 设置标题文本，添加前置空格
}

/**
 * @brief 添加音乐块控件
 * @param block 音乐块控件指针
 */
void PartWidget::addBlockWidget(ChannelBlock *block) const
{
    auto lay = static_cast<MyFlowLayout *>(this->m_tabWidget->layout()); ///< 获取流式布局
    lay->addWidget(block);                               ///< 添加音乐块到布局
}

/**
 * @brief 初始化界面
 */
void PartWidget::initUi()
{
    this->setContentsMargins(0, 0, 0, 0);                ///< 设置控件边距为 0
    this->m_titleLab->setObjectName("titleLab");         ///< 设置标题标签对象名称
    this->m_titleDeco->setObjectName("titleDeco");       ///< 设置装饰标签对象名称
    this->m_titleDeco->setText(" .MHz");                 ///< 设置装饰标签文本
    auto hlay = new QHBoxLayout;                         ///< 创建水平布局
    hlay->addWidget(this->m_titleLab);                   ///< 添加标题标签
    hlay->addWidget(this->m_titleDeco);                  ///< 添加装饰标签
    hlay->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Preferred)); ///< 添加伸缩项
    auto vlay = new QVBoxLayout(this);                   ///< 创建垂直布局
    vlay->addLayout(hlay);                               ///< 添加水平布局
    vlay->addWidget(this->m_tabWidget);                  ///< 添加容器控件
    auto lay = new MyFlowLayout(this->m_tabWidget, 12, -1, -1); ///< 创建流式布局，间距 12
    lay->setContentsMargins(0, 0, 0, 0);                 ///< 设置布局边距为 0
    this->m_tabWidget->setLayout(lay);                   ///< 设置容器控件布局
}