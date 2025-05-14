/**
* @file ElaTextPrivate.h
 * @brief 定义 ElaTextPrivate 类，管理文本标签的私有实现
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELATEXTPRIVATE_H
#define ELATEXTPRIVATE_H

#include <QObject>

#include "Def.h"
#include "stdafx.h"

class ElaText;

/**
 * @class ElaTextPrivate
 * @brief 文本标签私有实现类，管理主题和文本属性
 */
class ElaTextPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaText)
    Q_PROPERTY_CREATE_D(ElaTextType::TextStyle, TextStyle)
    Q_PROPERTY_CREATE_D(ElaIconType::IconName,  ElaIcon)

public:
    /**
     * @brief 构造函数，初始化文本标签私有对象
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaTextPrivate(QObject* parent = nullptr);

    /**
     * @brief 析构函数，释放文本标签私有资源
     */
    ~ElaTextPrivate() override;

    /**
     * @brief 主题切换事件处理
     * @param themeMode 主题模式
     */
    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    /**
     * @brief 当前主题模式
     */
    ElaThemeType::ThemeMode  _themeMode;

    /**
     * @brief 是否任意换行
     */
    bool                     _isWrapAnywhere{false};

    /**
     * @brief 文本间距
     */
    qreal                    _textSpacing{0.5};

    /**
     * @brief 更改主题
     */
    void _changeTheme();
};

#endif // ELATEXTPRIVATE_H