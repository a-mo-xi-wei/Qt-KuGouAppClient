/**
* @file ElaPlainTextEdit.h
 * @brief 定义 ElaPlainTextEdit 类，提供自定义纯文本编辑框功能
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAPLAINTEXTEDIT_H
#define ELAPLAINTEXTEDIT_H

#include <QPlainTextEdit>

#include "stdafx.h"

class ElaPlainTextEditPrivate;

/**
 * @class ElaPlainTextEdit
 * @brief 自定义纯文本编辑框类，支持主题切换和上下文菜单
 */
class ELA_EXPORT ElaPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
    Q_Q_CREATE(ElaPlainTextEdit)

public:
    /**
     * @brief 构造函数，初始化纯文本编辑框
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaPlainTextEdit(QWidget* parent = nullptr);

    /**
     * @brief 构造函数，初始化带文本的纯文本编辑框
     * @param text 初始文本
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaPlainTextEdit(const QString& text, QWidget* parent = nullptr);

    /**
     * @brief 析构函数，释放纯文本编辑框资源
     */
    ~ElaPlainTextEdit() override;

protected:
    /**
     * @brief 焦点进入事件处理
     * @param event 焦点事件对象
     */
    void focusInEvent(QFocusEvent* event) override;

    /**
     * @brief 焦点离开事件处理
     * @param event 焦点事件对象
     */
    void focusOutEvent(QFocusEvent* event) override;

    /**
     * @brief 上下文菜单事件处理
     * @param event 上下文菜单事件对象
     */
    void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // ELAPLAINTEXTEDIT_H