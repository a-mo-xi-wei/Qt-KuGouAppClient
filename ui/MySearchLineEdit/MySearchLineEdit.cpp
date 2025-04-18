//
// Created by WeiWang on 24-10-10.
//

#include "MySearchLineEdit.h"
#include "ElaMenu.h"

#include <QApplication>
#include <QClipboard>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <mutex>

std::once_flag flag;

MySearchLineEdit::MySearchLineEdit(QWidget *parent)
    :QLineEdit(parent)
    ,m_animation(new QPropertyAnimation(this,"minimumWidth",this))
    ,m_maxWidth(250)
{
    this->installEventFilter(this);
}

void MySearchLineEdit::setMaxWidth(const int &width) {
    this->m_maxWidth = width;
}

void MySearchLineEdit::focusInEvent(QFocusEvent *event) {
    QLineEdit::focusInEvent(event);

    if (this->width() == this->m_maxWidth)return;
    //使用应用程序级的事件过滤器，在输入框获取焦点时，鼠标点击其他控件时，输入框会丢失焦点，导致输入框收起。
    QApplication::instance()->installEventFilter(this);

    std::call_once(flag,[this]{this->m_originalWidth = this->width();});

    // 设置动画的起始和结束状态
    this->m_animation->setDuration(200);  // 动画时长 0.2 秒
    this->m_animation->setStartValue(this->m_originalWidth);  // 动画的起始值
    this->m_animation->setEndValue(this->m_maxWidth);      // 动画的结束值
    this->m_animation->start();
}

void MySearchLineEdit::focusOutEvent(QFocusEvent *event) {
    // 检查焦点丢失原因是否为菜单弹出
    if (event->reason() == Qt::PopupFocusReason) {
        event->ignore();  // 忽略该事件，不触发父类处理
        return;
    }
    QLineEdit::focusOutEvent(event);
    //qDebug()<<"焦点丢失";
    //使用应用程序级的事件过滤器，在输入框获取焦点时，鼠标点击其他控件时，输入框会丢失焦点，导致输入框收起。
    QApplication::instance()->installEventFilter(this);

    // 设置动画的起始和结束状态
    this->m_animation->setDuration(200);  // 动画时长 0.2 秒
    this->m_animation->setStartValue(this->m_maxWidth);  // 动画的起始值
    this->m_animation->setEndValue(this->m_originalWidth);      // 动画的结束值
    this->m_animation->start();
}

bool MySearchLineEdit::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        auto me = static_cast<QMouseEvent*>(event);

        // 判断当前是否有活动的菜单弹出（如右键菜单）
        QWidget *activePopup = QApplication::activePopupWidget();
        if (activePopup && activePopup->inherits("QMenu")) {
            // 如果有菜单弹出，直接返回，不处理点击外部区域的逻辑
            return QLineEdit::eventFilter(watched, event);
        }

        //qDebug()<<"鼠标按下 : "<<mapFromGlobal(me->globalPosition().toPoint());
        if (!rect().contains(mapFromGlobal(me->globalPosition().toPoint()))) {
            this->clearFocus();
            //qDebug()<<"清除焦点";
        }
    }
    return QLineEdit::eventFilter(watched, event);
}

void MySearchLineEdit::keyPressEvent(QKeyEvent *event) {
    if (echoMode() == QLineEdit::Password)
    {
        if (event->matches(QKeySequence::SelectAll))
        {
            return; //如果lineEdit设置成密码的形式，不可以全选
        }
        if (event->matches(QKeySequence::Copy))
        {
            return; //如果lineEdit设置成密码的形式，不可以拷贝
        }
        if (event->matches(QKeySequence::Paste))
        {
            return; //如果lineEdit设置成密码的形式，不可以粘贴
        }
    }
    return QLineEdit::keyPressEvent(event);
}

void MySearchLineEdit::contextMenuEvent(QContextMenuEvent *event)
{
    ElaMenu* menu = new ElaMenu(this);
    menu->setMenuItemHeight(27);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;
    if (!isReadOnly())
    {
        action = menu->addElaIconAction(ElaIconType::ArrowRotateLeft, tr("撤销"), QKeySequence::Undo);
        action->setEnabled(isUndoAvailable());
        connect(action, &QAction::triggered, this, &MySearchLineEdit::undo);

        action = menu->addElaIconAction(ElaIconType::ArrowRotateRight, tr("恢复"), QKeySequence::Redo);
        action->setEnabled(isRedoAvailable());
        connect(action, &QAction::triggered, this, &MySearchLineEdit::redo);
        menu->addSeparator();
    }
#ifndef QT_NO_CLIPBOARD
    if (!isReadOnly())
    {
        action = menu->addElaIconAction(ElaIconType::KnifeKitchen, tr("剪切"), QKeySequence::Cut);
        action->setEnabled(!isReadOnly() && hasSelectedText() && echoMode() == QLineEdit::Normal);
        connect(action, &QAction::triggered, this, &MySearchLineEdit::cut);
    }

    action = menu->addElaIconAction(ElaIconType::Copy, tr("复制"), QKeySequence::Copy);
    action->setEnabled(hasSelectedText() && echoMode() == QLineEdit::Normal);
    connect(action, &QAction::triggered, this, &MySearchLineEdit::copy);

    if (!isReadOnly())
    {
        action = menu->addElaIconAction(ElaIconType::Paste, tr("粘贴"), QKeySequence::Paste);
        action->setEnabled(!isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        connect(action, &QAction::triggered, this, &MySearchLineEdit::paste);
    }
#endif
    if (!isReadOnly())
    {
        action = menu->addElaIconAction(ElaIconType::DeleteLeft, tr("删除"));
        action->setEnabled(!isReadOnly() && !text().isEmpty() && hasSelectedText());
        connect(action, &QAction::triggered, this, [=](bool checked) {
            if (hasSelectedText())
            {
                int startIndex = selectionStart();
                int endIndex = selectionEnd();
                setText(text().remove(startIndex, endIndex - startIndex));
            }
        });
    }
    if (!menu->isEmpty())
    {
        menu->addSeparator();
    }
    action = menu->addAction(tr("全选"));
    action->setShortcut(QKeySequence::SelectAll);
    action->setEnabled(!text().isEmpty() && !(selectedText() == text()));
    connect(action, &QAction::triggered, this, &MySearchLineEdit::selectAll);
    menu->popup(event->globalPos());
    this->setFocus();
}

