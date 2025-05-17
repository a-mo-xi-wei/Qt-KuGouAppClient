/**
 * @file qtmaterialdialog_internal.cpp
 * @brief 实现 QtMaterialDialogProxy 和 QtMaterialDialogWindow 类，提供对话框内部功能
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#include "qtmaterialdialog_internal.h"
#include <QPainter>
#include <QPixmap>
#include <QEvent>
#include <QStackedLayout>
#include "qtmaterialdialog.h"

/**
 * @brief 构造函数，初始化代理窗口
 * @param source 对话框窗口指针
 * @param layout 堆叠布局指针
 * @param dialog 对话框指针
 * @param parent 父控件指针，默认为 nullptr
 * @note 内部实现
 */
QtMaterialDialogProxy::QtMaterialDialogProxy(
        QtMaterialDialogWindow *source,
        QStackedLayout         *layout,
        QtMaterialDialog       *dialog,
        QWidget                *parent)
    : QWidget(parent),
      m_source(source),
      m_layout(layout),
      m_dialog(dialog),
      m_opacity(0),
      m_mode(Transparent)
{
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialDialogProxy::~QtMaterialDialogProxy()
{
}

/**
 * @brief 设置透明度
 * @param opacity 透明度值
 */
void QtMaterialDialogProxy::setOpacity(qreal opacity)
{
    m_opacity = opacity;
    m_mode = SemiTransparent; ///< 设置为半透明模式
    update();                 ///< 刷新代理窗口
    m_dialog->update();       ///< 刷新对话框
}

/**
 * @brief 切换为完全不透明
 */
void QtMaterialDialogProxy::makeOpaque()
{
    m_dialog->setAttribute(Qt::WA_TransparentForMouseEvents, false); ///< 启用鼠标事件
    m_layout->setCurrentIndex(0);                                   ///< 切换到对话框窗口
    m_opacity = 1.0;                                                ///< 设置完全不透明
    m_mode = Opaque;                                                ///< 设置不透明模式
    update();                                                       ///< 刷新界面
}

/**
 * @brief 切换为完全透明
 */
void QtMaterialDialogProxy::makeTransparent()
{
    m_opacity = 0.0;       ///< 设置完全透明
    m_mode = Transparent;  ///< 设置透明模式
    update();              ///< 刷新界面
}

/**
 * @brief 获取推荐尺寸
 * @return 推荐尺寸
 * @note 重写基类方法
 */
QSize QtMaterialDialogProxy::sizeHint() const
{
    return m_source->sizeHint(); ///< 返回对话框窗口的推荐尺寸
}

/**
 * @brief 处理事件
 * @param event 事件对象
 * @return 是否处理事件
 * @note 重写基类方法
 */
bool QtMaterialDialogProxy::event(QEvent *event)
{
    const QEvent::Type type = event->type();

    if (QEvent::Move == type || QEvent::Resize == type) {
        m_source->setGeometry(geometry()); ///< 同步对话框窗口几何
    }
    return QWidget::event(event);
}

/**
 * @brief 绘制事件
 * @param event 绘制事件对象
 * @note 重写基类方法
 */
void QtMaterialDialogProxy::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    if (Transparent == m_mode) {
        return; ///< 透明模式不绘制
    } else if (Opaque != m_mode) {
        painter.setOpacity(m_opacity); ///< 设置半透明
    }
    QPixmap pm = m_source->grab(m_source->rect()); ///< 捕获对话框窗口内容
    painter.drawPixmap(0, 0, pm);                  ///< 绘制内容
}

/**
 * @brief 构造函数，初始化内容窗口
 * @param dialog 对话框指针
 * @param parent 父控件指针，默认为 nullptr
 * @note 内部实现
 */
QtMaterialDialogWindow::QtMaterialDialogWindow(
        QtMaterialDialog *dialog,
        QWidget          *parent)
    : QWidget(parent),
      m_dialog(dialog)
{
}

/**
 * @brief 析构函数，清理资源
 */
QtMaterialDialogWindow::~QtMaterialDialogWindow()
{
}

/**
 * @brief 设置垂直偏移
 * @param offset 偏移量
 */
void QtMaterialDialogWindow::setOffset(int offset)
{
    QMargins margins = m_dialog->layout()->contentsMargins();
    margins.setBottom(offset); ///< 设置底部边距
    m_dialog->layout()->setContentsMargins(margins); ///< 更新边距
}

/**
 * @brief 获取垂直偏移
 * @return 偏移量
 */
int QtMaterialDialogWindow::offset() const
{
    return m_dialog->layout()->contentsMargins().bottom(); ///< 返回底部边距
}

/**
 * @brief 绘制事件
 * @param event 绘制事件对象
 * @note 重写基类方法
 */
void QtMaterialDialogWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::white); ///< 背景颜色
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);
    painter.drawRect(rect());  ///< 绘制白色背景
}