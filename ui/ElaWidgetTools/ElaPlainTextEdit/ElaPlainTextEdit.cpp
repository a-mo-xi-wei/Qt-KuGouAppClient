/**
 * @file ElaPlainTextEdit.cpp
 * @brief 实现 ElaPlainTextEdit 类，提供自定义纯文本编辑框功能
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaPlainTextEdit.h"
#include "ElaEventBus.h"
#include "ElaMenu.h"
#include "ElaPlainTextEditPrivate.h"
#include "ElaPlainTextEditStyle.h"
#include "ElaScrollBar.h"
#include "ElaTheme.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QMimeData>
#include <QPainter>
#include <QPropertyAnimation>

/**
 * @brief 构造函数，初始化纯文本编辑框
 * @param parent 父控件指针，默认为 nullptr
 */
ElaPlainTextEdit::ElaPlainTextEdit(QWidget* parent)
    : QPlainTextEdit(parent), d_ptr(new ElaPlainTextEditPrivate())
{
    Q_D(ElaPlainTextEdit);
    d->q_ptr = this;
    setObjectName("ElaPlainTextEdit");
    setStyleSheet("#ElaPlainTextEdit{background-color:transparent;}");
    setHorizontalScrollBar(new ElaScrollBar(this));
    setVerticalScrollBar(new ElaScrollBar(this));
    setMouseTracking(true);
    // 事件总线
    d->_focusEvent = new ElaEvent("WMWindowClicked", "onWMWindowClickedEvent", d);
    d->_focusEvent->registerAndInit();

    d->_style = new ElaPlainTextEditStyle(style());
    setStyle(d->_style);
    d->onThemeChanged(eTheme->getThemeMode());
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaPlainTextEditPrivate::onThemeChanged);
}

/**
 * @brief 构造函数，初始化带文本的纯文本编辑框
 * @param text 初始文本
 * @param parent 父控件指针，默认为 nullptr
 */
ElaPlainTextEdit::ElaPlainTextEdit(const QString& text, QWidget* parent)
    : ElaPlainTextEdit(parent)
{
    setPlainText(text);
}

/**
 * @brief 析构函数，释放纯文本编辑框资源
 */
ElaPlainTextEdit::~ElaPlainTextEdit()
{
}

/**
 * @brief 焦点进入事件处理
 * @param event 焦点事件对象
 */
void ElaPlainTextEdit::focusInEvent(QFocusEvent* event)
{
    Q_D(ElaPlainTextEdit);
    if (event->reason() == Qt::MouseFocusReason)
    {
        QPropertyAnimation* markAnimation = new QPropertyAnimation(d->_style, "pExpandMarkWidth");
        connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            update();
        });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->_style->getExpandMarkWidth());
        markAnimation->setEndValue(width() / 2 - 3);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QPlainTextEdit::focusInEvent(event);
}

/**
 * @brief 焦点离开事件处理
 * @param event 焦点事件对象
 */
void ElaPlainTextEdit::focusOutEvent(QFocusEvent* event)
{
    Q_D(ElaPlainTextEdit);
    if (event->reason() != Qt::PopupFocusReason)
    {
        QPropertyAnimation* markAnimation = new QPropertyAnimation(d->_style, "pExpandMarkWidth");
        connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            update();
        });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->_style->getExpandMarkWidth());
        markAnimation->setEndValue(0);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QPlainTextEdit::focusOutEvent(event);
}

/**
 * @brief 上下文菜单事件处理
 * @param event 上下文菜单事件对象
 */
void ElaPlainTextEdit::contextMenuEvent(QContextMenuEvent* event)
{
    ElaMenu* menu = new ElaMenu(this);
    menu->setMenuItemHeight(27);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;
    if (!isReadOnly())
    {
        action = menu->addElaIconAction(ElaIconType::ArrowRotateLeft, "撤销", QKeySequence::Undo);
        action->setEnabled(isUndoRedoEnabled() ? document()->isUndoAvailable() : false);
        connect(action, &QAction::triggered, this, &ElaPlainTextEdit::undo);

        action = menu->addElaIconAction(ElaIconType::ArrowRotateRight, "恢复", QKeySequence::Redo);
        action->setEnabled(isUndoRedoEnabled() ? document()->isRedoAvailable() : false);
        connect(action, &QAction::triggered, this, &ElaPlainTextEdit::redo);
        menu->addSeparator();
    }
#ifndef QT_NO_CLIPBOARD
    if (!isReadOnly())
    {
        action = menu->addElaIconAction(ElaIconType::KnifeKitchen, "剪切", QKeySequence::Cut);
        action->setEnabled(!isReadOnly() && !textCursor().selectedText().isEmpty());
        connect(action, &QAction::triggered, this, &ElaPlainTextEdit::cut);
    }

    action = menu->addElaIconAction(ElaIconType::Copy, "复制", QKeySequence::Copy);
    action->setEnabled(!textCursor().selectedText().isEmpty());
    connect(action, &QAction::triggered, this, &ElaPlainTextEdit::copy);

    if (!isReadOnly())
    {
        action = menu->addElaIconAction(ElaIconType::Paste, "粘贴", QKeySequence::Paste);
        action->setEnabled(!isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        connect(action, &QAction::triggered, this, &ElaPlainTextEdit::paste);
    }
#endif
    if (!isReadOnly())
    {
        action = menu->addElaIconAction(ElaIconType::DeleteLeft, "删除");
        action->setEnabled(!isReadOnly() && !toPlainText().isEmpty() && !textCursor().selectedText().isEmpty());
        connect(action, &QAction::triggered, this, [=](bool checked) {
            if (!textCursor().selectedText().isEmpty())
            {
                textCursor().deleteChar();
            }
        });
    }
    if (!menu->isEmpty())
    {
        menu->addSeparator();
    }
    action = menu->addAction("全选");
    action->setShortcut(QKeySequence::SelectAll);
    action->setEnabled(!toPlainText().isEmpty() && !(textCursor().selectedText() == toPlainText()));
    connect(action, &QAction::triggered, this, &ElaPlainTextEdit::selectAll);
    menu->popup(event->globalPos());
    this->setFocus();
}