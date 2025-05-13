/**
 * @file ElaMenu.h
 * @brief 定义 ElaMenu 类，提供自定义菜单功能
 * @author [Your Name]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAMENU_H
#define ELAMENU_H

#include <QMenu>
#include <QWidget>

#include "Def.h"
#include "stdafx.h"

class ElaMenuPrivate;

/**
 * @class ElaMenu
 * @brief 自定义菜单类，支持动画、图标和子菜单
 */
class ELA_EXPORT ElaMenu : public QMenu
{
    Q_OBJECT
    Q_Q_CREATE(ElaMenu)

public:
    /**
     * @brief 构造函数，初始化菜单
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaMenu(QWidget* parent = nullptr);

    /**
     * @brief 构造函数，初始化带标题的菜单
     * @param title 菜单标题
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaMenu(const QString& title, QWidget* parent = nullptr);

    /**
     * @brief 析构函数，释放菜单资源
     */
    ~ElaMenu();

    /**
     * @brief 设置菜单透明度
     * @param opacity 透明度值（0.0 到 1.0）
     */
    void setOpacity(qreal opacity);

    /**
     * @brief 设置是否阻止菜单隐藏
     * @param prevent 是否阻止隐藏
     */
    void setPreventHide(bool prevent);

    /**
     * @brief 设置菜单项高度
     * @param menuItemHeight 菜单项高度
     */
    void setMenuItemHeight(int menuItemHeight);

    /**
     * @brief 获取菜单项高度
     * @return 菜单项高度
     */
    int getMenuItemHeight() const;

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

    /**
     * @brief 检查菜单是否包含子菜单
     * @return 是否包含子菜单
     */
    bool isHasChildMenu() const;

    /**
     * @brief 检查菜单是否包含图标
     * @return 是否包含图标
     */
    bool isHasIcon() const;

Q_SIGNALS:
    /**
     * @brief 菜单显示信号
     */
    void menuShow();

protected:
    /**
     * @brief 显示事件处理
     * @param event 显示事件
     */
    void showEvent(QShowEvent* event) override;

    /**
     * @brief 鼠标释放事件处理
     * @param event 鼠标事件
     */
    void mouseReleaseEvent(QMouseEvent* event) override;

    /**
     * @brief 重绘事件，绘制菜单动画
     * @param event 绘图事件
     */
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELAMENU_H