/**
 * @file qtmaterialdialog.cpp
 * @brief 实现 QtMaterialDialog 类，提供 Material Design 对话框
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#include "qtmaterialdialog.h"
#include "qtmaterialdialog_p.h"
#include <QStackedLayout>
#include <QGraphicsDropShadowEffect>
#include <QStateMachine>
#include <QState>
#include <QApplication>
#include <QPropertyAnimation>
#include <QPainter>
#include "qtmaterialdialog_internal.h"
#include "qtmaterialstatetransition.h"

/**
 * @brief 构造函数，初始化私有类
 * @param q 指向 QtMaterialDialog 的指针
 * @note 内部实现
 */
QtMaterialDialogPrivate::QtMaterialDialogPrivate(QtMaterialDialog *q)
    : q_ptr(q)
{
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialDialogPrivate::~QtMaterialDialogPrivate()
{
}

/**
 * @brief 初始化对话框
 */
void QtMaterialDialogPrivate::init()
{
    Q_Q(QtMaterialDialog);

    dialogWindow = new QtMaterialDialogWindow(q); ///< 创建对话框窗口
    proxyStack   = new QStackedLayout;           ///< 创建堆叠布局
    stateMachine = new QStateMachine(q);         ///< 创建状态机
    proxy        = new QtMaterialDialogProxy(dialogWindow, proxyStack, q); ///< 创建代理窗口

    QVBoxLayout *layout = new QVBoxLayout;
    q->setLayout(layout); ///< 设置主布局

    QWidget *widget = new QWidget;
    widget->setLayout(proxyStack); ///< 设置堆叠布局
    widget->setMinimumWidth(400);  ///< 设置最小宽度

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setColor(QColor(0, 0, 0, 200)); ///< 阴影颜色
    effect->setBlurRadius(64);              ///< 阴影模糊半径
    effect->setOffset(0, 13);               ///< 阴影偏移
    widget->setGraphicsEffect(effect);      ///< 应用阴影效果

    layout->addWidget(widget);
    layout->setAlignment(widget, Qt::AlignCenter); ///< 居中对齐

    proxyStack->addWidget(dialogWindow); ///< 添加对话框窗口
    proxyStack->addWidget(proxy);        ///< 添加代理窗口
    proxyStack->setCurrentIndex(1);      ///< 默认显示代理窗口

    q->setAttribute(Qt::WA_TransparentForMouseEvents); ///< 初始透明鼠标事件

    QState *hiddenState = new QState;  ///< 隐藏状态
    QState *visibleState = new QState; ///< 显示状态

    stateMachine->addState(hiddenState);
    stateMachine->addState(visibleState);
    stateMachine->setInitialState(hiddenState); ///< 设置初始状态

    QtMaterialStateTransition *transition;

    // 显示转换
    transition = new QtMaterialStateTransition(DialogShowTransition);
    transition->setTargetState(visibleState);
    hiddenState->addTransition(transition);

    // 隐藏转换
    transition = new QtMaterialStateTransition(DialogHideTransition);
    transition->setTargetState(hiddenState);
    visibleState->addTransition(transition);

    // 设置状态属性
    visibleState->assignProperty(proxy, "opacity", 1);                   ///< 显示状态完全不透明
    visibleState->assignProperty(effect, "color", QColor(0, 0, 0, 200)); ///< 显示状态阴影颜色
    visibleState->assignProperty(dialogWindow, "offset", 0);             ///< 显示状态无偏移
    hiddenState->assignProperty(proxy, "opacity", 0);                    ///< 隐藏状态完全透明
    hiddenState->assignProperty(effect, "color", QColor(0, 0, 0, 0));    ///< 隐藏状态无阴影
    hiddenState->assignProperty(dialogWindow, "offset", 200);            ///< 隐藏状态向下偏移

    QPropertyAnimation *animation;

    // 透明度动画
    animation = new QPropertyAnimation(proxy, "opacity", q);
    animation->setDuration(280); ///< 动画时长
    stateMachine->addDefaultAnimation(animation);

    // 阴影颜色动画
    animation = new QPropertyAnimation(effect, "color", q);
    animation->setDuration(280); ///< 动画时长
    stateMachine->addDefaultAnimation(animation);

    // 偏移动画
    animation = new QPropertyAnimation(dialogWindow, "offset", q);
    animation->setDuration(280);                     ///< 动画时长
    animation->setEasingCurve(QEasingCurve::OutCirc); ///< 缓动曲线
    stateMachine->addDefaultAnimation(animation);

    // 连接状态信号
    QObject::connect(visibleState, SIGNAL(propertiesAssigned()),
                     proxy, SLOT(makeOpaque())); ///< 显示状态完成时切换为不透明
    QObject::connect(hiddenState, SIGNAL(propertiesAssigned()),
                     proxy, SLOT(makeTransparent())); ///< 隐藏状态完成时切换为透明

    stateMachine->start(); ///< 启动状态机
    QCoreApplication::processEvents(); ///< 处理事件队列
}

/**
 * @brief 构造函数，初始化对话框
 * @param parent 父控件指针，默认为 nullptr
 */
QtMaterialDialog::QtMaterialDialog(QWidget *parent)
    : QtMaterialOverlayWidget(parent),
      d_ptr(new QtMaterialDialogPrivate(this))
{
    d_func()->init(); ///< 初始化私有类
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialDialog::~QtMaterialDialog()
{
}

/**
 * @brief 获取对话框窗口布局
 * @return 布局指针
 */
QLayout *QtMaterialDialog::windowLayout() const
{
    Q_D(const QtMaterialDialog);

    return d->dialogWindow->layout();
}

/**
 * @brief 设置对话框窗口布局
 * @param layout 布局指针
 */
void QtMaterialDialog::setWindowLayout(QLayout *layout)
{
    Q_D(QtMaterialDialog);

    d->dialogWindow->setLayout(layout); ///< 设置布局
}

/**
 * @brief 显示对话框
 */
void QtMaterialDialog::showDialog()
{
    Q_D(QtMaterialDialog);

    d->stateMachine->postEvent(new QtMaterialStateTransitionEvent(DialogShowTransition)); ///< 触发显示转换
    raise(); ///< 提升到顶层
}

/**
 * @brief 隐藏对话框
 */
void QtMaterialDialog::hideDialog()
{
    Q_D(QtMaterialDialog);

    d->stateMachine->postEvent(new QtMaterialStateTransitionEvent(DialogHideTransition)); ///< 触发隐藏转换
    setAttribute(Qt::WA_TransparentForMouseEvents); ///< 设置透明鼠标事件
    d->proxyStack->setCurrentIndex(1);              ///< 切换到代理窗口
}

/**
 * @brief 绘制事件
 * @param event 绘制事件对象
 * @note 重写基类方法
 */
void QtMaterialDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    Q_D(QtMaterialDialog);

    QPainter painter(this);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::black); ///< 背景颜色
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    painter.setOpacity(d->proxy->opacity() / 2.4); ///< 动态透明度
    painter.drawRect(rect());                     ///< 绘制背景
}