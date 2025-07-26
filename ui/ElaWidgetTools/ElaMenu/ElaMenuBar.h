/**
* @file ElaMenuBar.h
 * @brief 定义 ElaMenuBar 类，提供自定义菜单栏功能
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAMENUBAR_H
#define ELAMENUBAR_H

#include <QMenuBar>

#include "Def.h"
#include "stdafx.h"

class ElaMenu;

/**
 * @class ElaMenuBar
 * @brief 自定义菜单栏类，支持图标、文本和快捷键的菜单项添加
 */
class ELA_EXPORT ElaMenuBar : public QMenuBar
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化菜单栏
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaMenuBar(QWidget* parent = nullptr);

    /**
     * @brief 析构函数，释放菜单栏资源
     */
    ~ElaMenuBar() override;

    /**
     * @brief 添加现有菜单
     * @param menu 菜单指针
     * @return 菜单对应的动作
     */
    QAction* addMenu(QMenu* menu);

    /**
     * @brief 添加新菜单（仅标题）
     * @param title 菜单标题
     * @return 新创建的菜单指针
     */
    ElaMenu* addMenu(const QString& title);

    /**
     * @brief 添加新菜单（带图标）
     * @param icon 菜单图标
     * @param title 菜单标题
     * @return 新创建的菜单指针
     */
    ElaMenu* addMenu(const QIcon& icon, const QString& title);

    /**
     * @brief 添加新菜单（带 Ela 图标）
     * @param icon Ela 图标类型
     * @param title 菜单标题
     * @return 新创建的菜单指针
     */
    ElaMenu* addMenu(ElaIconType::IconName icon, const QString& title);

    /**
     * @brief 添加带 Ela 图标的动作
     * @param icon Ela 图标类型
     * @param text 动作文本
     * @return 新创建的动作指针
     */
    QAction* addElaIconAction(ElaIconType::IconName icon, const QString& text);

    /**
     * @brief 添加带 Ela 图标和快捷键的动作
     * @param icon Ela 图标类型
     * @param text 动作文本
     * @param shortcut 快捷键
     * @return 新创建的动作指针
     */
    QAction* addElaIconAction(ElaIconType::IconName icon, const QString& text, const QKeySequence& shortcut);
};

#endif // ELAMENUBAR_H