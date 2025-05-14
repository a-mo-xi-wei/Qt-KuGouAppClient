/**
* @file ElaTextPrivate.cpp
 * @brief 实现 ElaTextPrivate 类，管理文本标签的私有实现
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaTextPrivate.h"
#include "ElaText.h"

#include <QTimer>

/**
 * @brief 构造函数，初始化文本标签私有对象
 * @param parent 父对象指针，默认为 nullptr
 */
ElaTextPrivate::ElaTextPrivate(QObject* parent)
    : QObject{parent}
{
}

/**
 * @brief 析构函数，释放文本标签私有资源
 */
ElaTextPrivate::~ElaTextPrivate()
{
}

/**
 * @brief 主题切换事件处理
 * @param themeMode 主题模式
 */
void ElaTextPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
    Q_Q(ElaText);
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
void ElaTextPrivate::_changeTheme()
{
    Q_Q(ElaText);
    QPalette palette = q->palette();
    palette.setColor(QPalette::WindowText, _themeMode == ElaThemeType::Light ? Qt::black : Qt::white);
    q->setPalette(palette);
}