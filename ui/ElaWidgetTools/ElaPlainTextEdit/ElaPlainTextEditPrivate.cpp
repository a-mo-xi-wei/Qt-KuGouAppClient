/**
 * @file ElaPlainTextEditPrivate.cpp
 * @brief 实现 ElaPlainTextEditPrivate 类，管理纯文本编辑框的私有实现
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaPlainTextEditPrivate.h"
#include "ElaPlainTextEdit.h"

#include <QTimer>

/**
 * @brief 构造函数，初始化纯文本编辑框私有对象
 * @param parent 父对象指针，默认为 nullptr
 */
ElaPlainTextEditPrivate::ElaPlainTextEditPrivate(QObject* parent)
    : QObject{parent}
{
}

/**
 * @brief 析构函数，释放纯文本编辑框私有资源
 */
ElaPlainTextEditPrivate::~ElaPlainTextEditPrivate()
= default;

/**
 * @brief 处理窗口点击事件
 * @param data 事件数据
 */
void ElaPlainTextEditPrivate::onWMWindowClickedEvent(QVariantMap data)
{
    Q_Q(ElaPlainTextEdit);
    ElaAppBarType::WMMouseActionType actionType = data.value("WMClickType").value<ElaAppBarType::WMMouseActionType>();
    if (actionType == ElaAppBarType::WMLBUTTONDOWN)
    {
        if (!q->toPlainText().isEmpty() && q->hasFocus())
        {
            q->clearFocus();
        }
    }
    else if (actionType == ElaAppBarType::WMLBUTTONUP || actionType == ElaAppBarType::WMNCLBUTTONDOWN)
    {
        if (containsCursorToItem(q) || (actionType == ElaAppBarType::WMLBUTTONUP && !q->toPlainText().isEmpty()))
        {
            return;
        }
        if (q->hasFocus())
        {
            q->clearFocus();
        }
    }
}

/**
 * @brief 判断光标是否在控件内
 * @param item 目标控件
 * @return 是否包含光标
 */
bool ElaPlainTextEditPrivate::containsCursorToItem(QWidget* item)
{
    if (!item || !item->isVisible())
    {
        return false;
    }
    auto point = item->window()->mapFromGlobal(QCursor::pos());
    QRectF rect = QRectF(item->mapTo(item->window(), QPoint(0, 0)), item->size());
    if (rect.contains(point))
    {
        return true;
    }
    return false;
}

/**
 * @brief 主题切换事件处理
 * @param themeMode 主题模式
 */
void ElaPlainTextEditPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
    Q_Q(ElaPlainTextEdit);
    _themeMode = themeMode;
    if (q->isVisible())
    {
        _changeTheme();
    }
    else
    {
        QTimer::singleShot(1, this, [=] {
            _changeTheme();
        });
    }
}

/**
 * @brief 更改主题
 */
void ElaPlainTextEditPrivate::_changeTheme()
{
    Q_Q(ElaPlainTextEdit);
    if (_themeMode == ElaThemeType::Light)
    {
        QPalette palette;
        palette.setColor(QPalette::Text, Qt::black);
        palette.setColor(QPalette::PlaceholderText, QColor(0x00, 0x00, 0x00, 128));
        q->setPalette(palette);
    }
    else
    {
        QPalette palette;
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::PlaceholderText, QColor(0xBA, 0xBA, 0xBA));
        q->setPalette(palette);
    }
}