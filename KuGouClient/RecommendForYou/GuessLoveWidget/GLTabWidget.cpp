/**
 * @file GLTabWidget.cpp
 * @brief 实现 GLTabWidget 类，管理“猜你喜欢”推荐界面
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#include "GLTabWidget.h"
#include "IconBesideTextToolButton.h"
#include "MyMenu.h"
#include "logger.hpp"
#include "ElaMenu.h"

#include <QFile>
#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QPaintEvent>

/** @brief 获取当前文件所在目录宏 */
#define GET_CURRENT_DIR (QString(__FILE__).left(qMax(QString(__FILE__).lastIndexOf('/'), QString(__FILE__).lastIndexOf('\\'))))

/**
 * @brief 构造函数，初始化推荐界面
 * @param parent 父控件指针，默认为 nullptr
 */
GLTabWidget::GLTabWidget(QWidget *parent)
    : QWidget{parent}
    , m_modelBtn(new IconBesideTextToolButton(this))
    , m_playToolBtn(new QToolButton(this))
    , m_nextToolBtn(new QToolButton(this))
    , m_likeToolBtn(new QToolButton(this))
    , m_dislikeToolBtn(new QToolButton(this))
{
    initUi();                                            ///< 初始化界面
    layoutUi();                                          ///< 布局界面
}

/**
 * @brief 初始化界面
 * @note 初始化标签、按钮和样式表
 */
void GLTabWidget::initUi()
{
    this->m_glLab = new QLabel(QStringLiteral("猜你喜欢"), this); ///< 初始化“猜你喜欢”标签
    this->m_songNameLab = new QLabel(QStringLiteral("青花瓷"), this); ///< 初始化歌曲名标签
    this->m_singerLab = new QLabel(QStringLiteral("周杰伦"), this); ///< 初始化歌手标签

    this->setObjectName("basic_window_widget");          ///< 设置控件对象名
    this->m_glLab->setObjectName("glLab");               ///< 设置标签对象名
    this->m_songNameLab->setObjectName("songNameLab");   ///< 设置歌曲名标签对象名
    this->m_singerLab->setObjectName("singerLab");       ///< 设置歌手标签对象名
    this->m_modelBtn->setObjectName("modelBtn");         ///< 设置模式按钮对象名
    this->m_playToolBtn->setObjectName("playToolBtn");   ///< 设置播放按钮对象名
    this->m_nextToolBtn->setObjectName("nextToolBtn");   ///< 设置下一首按钮对象名
    this->m_likeToolBtn->setObjectName("likeToolBtn");   ///< 设置喜欢按钮对象名
    this->m_dislikeToolBtn->setObjectName("dislikeToolBtn"); ///< 设置不喜欢按钮对象名

    this->m_modelBtn->setCursor(Qt::CursorShape::PointingHandCursor); ///< 设置模式按钮光标
    this->m_playToolBtn->setCursor(Qt::CursorShape::PointingHandCursor); ///< 设置播放按钮光标
    this->m_nextToolBtn->setCursor(Qt::CursorShape::PointingHandCursor); ///< 设置下一首按钮光标
    this->m_likeToolBtn->setCursor(Qt::CursorShape::PointingHandCursor); ///< 设置喜欢按钮光标
    this->m_dislikeToolBtn->setCursor(Qt::CursorShape::PointingHandCursor); ///< 设置不喜欢按钮光标

    this->m_modelBtn->setText(QStringLiteral("模式"));    ///< 设置模式按钮文本
    this->m_modelBtn->setFixedSize(45, 20);              ///< 设置模式按钮大小
    this->m_modelBtn->setHoverFontColor(Qt::white);      ///< 设置模式按钮悬停字体颜色
    this->m_modelBtn->setIcon(QIcon(QStringLiteral(":/ListenBook/Res/listenbook/down-white.svg"))); ///< 设置模式按钮图标
    this->m_modelBtn->setIconSize(QSize(10, 10));        ///< 设置模式按钮图标大小
    this->m_modelBtn->setApproach(true);                 ///< 启用模式按钮接近效果
    connect(this->m_modelBtn, &IconBesideTextToolButton::clicked, this, &GLTabWidget::onModelBtnClicked); ///< 连接模式按钮点击信号

    QFile file(GET_CURRENT_DIR + QStringLiteral("/table.css")); ///< 加载样式表
    if (file.open(QIODevice::ReadOnly))
    {
        this->setStyleSheet(file.readAll());             ///< 应用样式表
    }
    else
    {
        qDebug() << "样式表打开失败QAQ";
        STREAM_ERROR() << "样式表打开失败QAQ";          ///< 记录错误日志
        return;
    }
}

/**
 * @brief 绘制事件
 * @param ev 绘制事件
 * @note 绘制控件背景
 */
void GLTabWidget::paintEvent(QPaintEvent *ev)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this); ///< 绘制控件背景
}

/** @brief 宽高比宏 */
#define AspectRatio 2

/**
 * @brief 调整大小事件
 * @param event 调整大小事件
 * @note 动态调整控件高度
 */
void GLTabWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    this->setFixedHeight(event->size().width() / AspectRatio); ///< 设置固定高度
    update();                                            ///< 更新界面
}

/**
 * @brief 布局界面
 * @note 使用水平和垂直布局排列控件
 */
void GLTabWidget::layoutUi()
{
    const auto hLayout1 = new QHBoxLayout;               ///< 第一水平布局
    hLayout1->setSpacing(5);                             ///< 设置间距
    hLayout1->addSpacerItem(new QSpacerItem(20, 5, QSizePolicy::Fixed, QSizePolicy::Fixed)); ///< 添加固定间隔
    hLayout1->addWidget(this->m_glLab);                  ///< 添加“猜你喜欢”标签
    hLayout1->addWidget(this->m_modelBtn);               ///< 添加模式按钮
    hLayout1->addSpacerItem(new QSpacerItem(20, 30, QSizePolicy::Expanding, QSizePolicy::Fixed)); ///< 添加扩展间隔

    const auto hLayout2 = new QHBoxLayout;               ///< 第二水平布局
    hLayout2->setContentsMargins(0, 0, 0, 0);            ///< 设置边距
    hLayout2->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Fixed, QSizePolicy::Fixed)); ///< 添加固定间隔
    hLayout2->addWidget(this->m_songNameLab);            ///< 添加歌曲名标签
    hLayout2->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Fixed)); ///< 添加扩展间隔

    const auto hLayout3 = new QHBoxLayout;               ///< 第三水平布局
    hLayout3->setContentsMargins(0, 0, 0, 0);            ///< 设置边距
    hLayout3->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Fixed, QSizePolicy::Fixed)); ///< 添加固定间隔
    hLayout3->addWidget(this->m_singerLab);              ///< 添加歌手标签
    hLayout3->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Fixed)); ///< 添加扩展间隔

    const auto vlayout1 = new QVBoxLayout;               ///< 第一垂直布局
    vlayout1->setSpacing(5);                             ///< 设置间距
    vlayout1->addLayout(hLayout2);                       ///< 添加歌曲名布局
    vlayout1->addLayout(hLayout3);                       ///< 添加歌手布局

    const auto vlayout2 = new QVBoxLayout;               ///< 第二垂直布局
    vlayout2->setSpacing(15);                            ///< 设置间距
    vlayout2->addLayout(hLayout1);                       ///< 添加标题和模式按钮布局
    vlayout2->addLayout(vlayout1);                       ///< 添加歌曲信息布局

    const auto hLayout4 = new QHBoxLayout;               ///< 第四水平布局
    hLayout4->addSpacerItem(new QSpacerItem(20, 30, QSizePolicy::Fixed, QSizePolicy::Fixed)); ///< 添加固定间隔
    hLayout4->addWidget(this->m_playToolBtn);            ///< 添加播放按钮
    hLayout4->addStretch();                              ///< 添加拉伸
    hLayout4->addWidget(this->m_nextToolBtn);            ///< 添加下一首按钮
    hLayout4->addStretch();                              ///< 添加拉伸
    hLayout4->addWidget(this->m_likeToolBtn);            ///< 添加喜欢按钮
    hLayout4->addStretch();                              ///< 添加拉伸
    hLayout4->addWidget(this->m_dislikeToolBtn);         ///< 添加不喜欢按钮
    hLayout4->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Preferred)); ///< 添加扩展间隔
    hLayout4->setStretch(0, 0);                          ///< 设置拉伸因子
    hLayout4->setStretch(1, 0);
    hLayout4->setStretch(2, 1);
    hLayout4->setStretch(3, 0);
    hLayout4->setStretch(4, 1);
    hLayout4->setStretch(5, 0);
    hLayout4->setStretch(6, 1);
    hLayout4->setStretch(7, 0);
    hLayout4->setStretch(8, 14);

    const auto vLayout = new QVBoxLayout(this);          ///< 主垂直布局
    vLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Fixed)); ///< 添加固定间隔
    vLayout->addLayout(vlayout2);                        ///< 添加标题和歌曲信息布局
    vLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Preferred)); ///< 添加扩展间隔
    vLayout->addLayout(hLayout4);                        ///< 添加按钮布局
    vLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Preferred)); ///< 添加扩展间隔
}

/**
 * @brief 模式按钮点击槽函数
 * @note 弹出模式选择菜单
 */
void GLTabWidget::onModelBtnClicked()
{
    const QPoint globalPos = this->m_modelBtn->mapToGlobal(
        QPoint(this->m_modelBtn->width() - 45,
               this->m_modelBtn->height() - 5)); ///< 计算菜单弹出位置

    ElaMenu *menu = new ElaMenu(this);                   ///< 创建模式选择菜单
    menu->setOpacity(0.85);                              ///< 设置菜单透明度
    menu->setFixedWidth(60);                             ///< 设置菜单宽度
    menu->setMenuItemHeight(22);                         ///< 设置菜单项高度
    menu->setAttribute(Qt::WA_DeleteOnClose);            ///< 设置关闭时自动删除

    auto action1 = new QAction(menu);                    ///< 创建“发现”选项
    action1->setText("发现");
    connect(action1, &QAction::triggered, this, [this] { onGetModel("发现"); }); ///< 连接“发现”选项信号
    auto action2 = new QAction(menu);                    ///< 创建“小众”选项
    action2->setText("小众");
    connect(action2, &QAction::triggered, this, [this] { onGetModel("小众"); }); ///< 连接“小众”选项信号
    auto action3 = new QAction(menu);                    ///< 创建“30s”选项
    action3->setText("30s");
    connect(action3, &QAction::triggered, this, [this] { onGetModel("30s"); }); ///< 连接“30s”选项信号

    menu->addAction(action1);                            ///< 添加“发现”选项
    menu->addAction(action2);                            ///< 添加“小众”选项
    menu->addAction(action3);                            ///< 添加“30s”选项

    menu->popup(globalPos);                              ///< 弹出菜单
}

/**
 * @brief 处理模式选择
 * @param model 选择的模式
 * @note 更新模式按钮文本
 */
void GLTabWidget::onGetModel(const QString &model)
{
    this->m_modelBtn->setText(model);                    ///< 更新模式按钮文本
}

/**
 * @brief 鼠标按下事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void GLTabWidget::mousePressEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 鼠标释放事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void GLTabWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}

/**
 * @brief 鼠标双击事件
 * @param event 鼠标事件
 * @note 忽略事件
 */
void GLTabWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore();                                     ///< 忽略事件
}