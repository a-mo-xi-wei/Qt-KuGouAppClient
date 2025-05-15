/**
 * @file MySearchLineEdit.cpp
 * @brief 实现 MySearchLineEdit 类，提供带动画效果的搜索输入框
 * @author WeiWang
 * @date 2024-10-10
 * @version 1.0
 */

#include "MySearchLineEdit.h"
#include "ElaMenu.h"

#include <QApplication>
#include <QClipboard>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <mutex>

/**
 * @brief 单次初始化标志，确保原始宽度只设置一次
 */
std::once_flag flag;

/**
 * @brief 构造函数，初始化搜索输入框
 * @param parent 父控件指针，默认为 nullptr
 */
MySearchLineEdit::MySearchLineEdit(QWidget *parent)
    : QLineEdit(parent)
    , m_animation(new QPropertyAnimation(this, "minimumWidth", this))
    , m_maxWidth(250)
{
    this->installEventFilter(this); ///< 安装事件过滤器
}

/**
 * @brief 设置输入框最大宽度
 * @param width 最大宽度
 */
void MySearchLineEdit::setMaxWidth(const int &width) {
    this->m_maxWidth = width;
}

/**
 * @brief 重写焦点获取事件，触发展开动画
 * @param event 焦点事件
 */
void MySearchLineEdit::focusInEvent(QFocusEvent *event) {
    QLineEdit::focusInEvent(event);

    if (this->width() == this->m_maxWidth) return; ///< 已展开则无需动画
    QApplication::instance()->installEventFilter(this); ///< 安装应用程序级事件过滤器

    std::call_once(flag, [this] { this->m_originalWidth = this->width(); }); ///< 初始化原始宽度

    this->m_animation->setDuration(200); ///< 动画持续 200 毫秒
    this->m_animation->setStartValue(this->m_originalWidth); ///< 从原始宽度开始
    this->m_animation->setEndValue(this->m_maxWidth); ///< 展开到最大宽度
    this->m_animation->start();
}

/**
 * @brief 重写焦点丢失事件，触发收起动画
 * @param event 焦点事件
 */
void MySearchLineEdit::focusOutEvent(QFocusEvent *event) {
    if (event->reason() == Qt::PopupFocusReason) {
        event->ignore(); ///< 忽略由菜单弹出引起的焦点丢失
        return;
    }
    QLineEdit::focusOutEvent(event);
    // qDebug() << "焦点丢失"; ///< 调试用，记录焦点丢失
    QApplication::instance()->installEventFilter(this); ///< 安装应用程序级事件过滤器

    this->m_animation->setDuration(200); ///< 动画持续 200 毫秒
    this->m_animation->setStartValue(this->m_maxWidth); ///< 从最大宽度开始
    this->m_animation->setEndValue(this->m_originalWidth); ///< 收起到原始宽度
    this->m_animation->start();
}

/**
 * @brief 重写事件过滤器，处理鼠标点击外部区域的焦点丢失
 * @param watched 监视的对象
 * @param event 事件对象
 * @return 是否处理事件
 */
bool MySearchLineEdit::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        auto me = static_cast<QMouseEvent *>(event);

        QWidget *activePopup = QApplication::activePopupWidget();
        if (activePopup && activePopup->inherits("QMenu")) {
            return QLineEdit::eventFilter(watched, event); ///< 菜单弹出时不处理
        }

        // qDebug() << "鼠标按下 : " << mapFromGlobal(me->globalPosition().toPoint()); ///< 调试用，记录鼠标位置
        if (!rect().contains(mapFromGlobal(me->globalPosition().toPoint()))) {
            this->clearFocus(); ///< 点击外部区域时清除焦点
            // qDebug() << "清除焦点"; ///< 调试用，记录焦点清除
        }
    }
    return QLineEdit::eventFilter(watched, event);
}

/**
 * @brief 重写按键事件，限制密码模式下的复制粘贴等操作
 * @param event 按键事件
 */
void MySearchLineEdit::keyPressEvent(QKeyEvent *event) {
    if (echoMode() == QLineEdit::Password) {
        if (event->matches(QKeySequence::SelectAll)) {
            return; ///< 密码模式禁用全选
        }
        if (event->matches(QKeySequence::Copy)) {
            return; ///< 密码模式禁用复制
        }
        if (event->matches(QKeySequence::Paste)) {
            return; ///< 密码模式禁用粘贴
        }
    }
    QLineEdit::keyPressEvent(event);
}

/**
 * @brief 重写右键菜单事件，显示自定义菜单
 * @param event 右键菜单事件
 */
void MySearchLineEdit::contextMenuEvent(QContextMenuEvent *event) {
    ElaMenu *menu = new ElaMenu(this);
    menu->setMenuItemHeight(27);
    menu->setAttribute(Qt::WA_DeleteOnClose); ///< 菜单关闭时自动删除
    QAction *action = nullptr;

    if (!isReadOnly()) {
        action = menu->addElaIconAction(ElaIconType::ArrowRotateLeft, tr("撤销"), QKeySequence::Undo);
        action->setEnabled(isUndoAvailable());
        connect(action, &QAction::triggered, this, &MySearchLineEdit::undo);

        action = menu->addElaIconAction(ElaIconType::ArrowRotateRight, tr("恢复"), QKeySequence::Redo);
        action->setEnabled(isRedoAvailable());
        connect(action, &QAction::triggered, this, &MySearchLineEdit::redo);
        menu->addSeparator();
    }

#ifndef QT_NO_CLIPBOARD
    if (!isReadOnly()) {
        action = menu->addElaIconAction(ElaIconType::KnifeKitchen, tr("剪切"), QKeySequence::Cut);
        action->setEnabled(!isReadOnly() && hasSelectedText() && echoMode() == QLineEdit::Normal);
        connect(action, &QAction::triggered, this, &MySearchLineEdit::cut);
    }

    action = menu->addElaIconAction(ElaIconType::Copy, tr("复制"), QKeySequence::Copy);
    action->setEnabled(hasSelectedText() && echoMode() == QLineEdit::Normal);
    connect(action, &QAction::triggered, this, &MySearchLineEdit::copy);

    if (!isReadOnly()) {
        action = menu->addElaIconAction(ElaIconType::Paste, tr("粘贴"), QKeySequence::Paste);
        action->setEnabled(!isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        connect(action, &QAction::triggered, this, &MySearchLineEdit::paste);
    }
#endif

    if (!isReadOnly()) {
        action = menu->addElaIconAction(ElaIconType::DeleteLeft, tr("删除"));
        action->setEnabled(!isReadOnly() && !text().isEmpty() && hasSelectedText());
        connect(action, &QAction::triggered, this, [=](bool checked) {
            if (hasSelectedText()) {
                int startIndex = selectionStart();
                int endIndex = selectionEnd();
                setText(text().remove(startIndex, endIndex - startIndex)); ///< 删除选中文本
            }
        });
    }

    if (!menu->isEmpty()) {
        menu->addSeparator();
    }

    action = menu->addAction(tr("全选"));
    action->setShortcut(QKeySequence::SelectAll);
    action->setEnabled(!text().isEmpty() && !(selectedText() == text()));
    connect(action, &QAction::triggered, this, &MySearchLineEdit::selectAll);

    menu->popup(event->globalPos()); ///< 显示右键菜单
    this->setFocus(); ///< 保持输入框焦点
}