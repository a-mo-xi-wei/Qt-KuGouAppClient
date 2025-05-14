/**
* @file ElaPlainTextEditPrivate.h
 * @brief 定义 ElaPlainTextEditPrivate 类，管理纯文本编辑框的私有实现
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAPLAINTEXTEDITPRIVATE_H
#define ELAPLAINTEXTEDITPRIVATE_H

#include "Def.h"

class ElaEvent;
class ElaPlainTextEdit;
class ElaPlainTextEditStyle;

/**
 * @class ElaPlainTextEditPrivate
 * @brief 纯文本编辑框私有实现类，管理事件和主题切换
 */
class ElaPlainTextEditPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaPlainTextEdit)

public:
    /**
     * @brief 构造函数，初始化纯文本编辑框私有对象
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaPlainTextEditPrivate(QObject* parent = nullptr);

    /**
     * @brief 析构函数，释放纯文本编辑框私有资源
     */
    ~ElaPlainTextEditPrivate() override;

    /**
     * @brief 处理窗口点击事件
     * @param data 事件数据
     */
    Q_INVOKABLE void onWMWindowClickedEvent(QVariantMap data);

private:
    /**
     * @brief 判断光标是否在控件内
     * @param item 目标控件
     * @return 是否包含光标
     */
    bool containsCursorToItem(QWidget* item);

    public slots:
        /**
         * @brief 主题切换事件处理
         * @param themeMode 主题模式
         */
        void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    /**
     * @brief 当前主题模式
     */
    ElaThemeType::ThemeMode  _themeMode;

    /**
     * @brief 自定义样式对象
     */
    ElaPlainTextEditStyle*   _style{nullptr};

    /**
     * @brief 焦点事件对象
     */
    ElaEvent*                _focusEvent{nullptr};

    /**
     * @brief 更改主题
     */
    void _changeTheme();
};

#endif // ELAPLAINTEXTEDITPRIVATE_H