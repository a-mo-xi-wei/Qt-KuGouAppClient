/**
* @file ListOptionMenu.h
 * @brief 定义 ListOptionMenu 类，提供歌曲分类选项菜单功能
 * @author WeiWang
 * @date 2025-01-12
 * @version 1.0
 */

#ifndef LISTOPTIONMENU_H
#define LISTOPTIONMENU_H

#include "../BaseMenu.h"

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(UIWIDGETS_LIBRARY)
#define UIWIDGETS_EXPORT Q_DECL_EXPORT
#else
#define UIWIDGETS_EXPORT Q_DECL_IMPORT
#endif

/**
 * @class ListOptionMenu
 * @brief 歌曲分类选项菜单类，继承自 BaseMenu，提供场景、主题等分类选择
 */
class UIWIDGETS_EXPORT ListOptionMenu : public BaseMenu {
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化歌曲分类选项菜单
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ListOptionMenu(QWidget *parent = nullptr);

private:
    /**
     * @brief 初始化菜单布局和内容
     */
    void initMenu() override;

    /**
     * @brief 初始化场景分类窗口
     * @param widget 目标控件
     */
    void initSceneWidget(QWidget* widget);

    /**
     * @brief 初始化主题分类窗口
     * @param widget 目标控件
     */
    void initThemeWidget(QWidget* widget);

    /**
     * @brief 初始化语种分类窗口
     * @param widget 目标控件
     */
    void initLanguageWidget(QWidget* widget);

    /**
     * @brief 初始化风格分类窗口
     * @param widget 目标控件
     */
    void initStyleWidget(QWidget* widget);

    /**
     * @brief 初始化心情分类窗口
     * @param widget 目标控件
     */
    void initMoodWidget(QWidget* widget);

    /**
     * @brief 初始化年代分类窗口
     * @param widget 目标控件
     */
    void initPeriodWidget(QWidget* widget);

    /**
     * @brief 获取当前菜单对象
     * @return 当前菜单对象指针
     */
    const ListOptionMenu* getMenu() const override;

protected:
    /**
     * @brief 事件过滤器，处理鼠标点击事件
     * @param watched 事件目标对象
     * @param event 事件对象
     * @return 是否处理事件
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    /**
     * @brief 菜单项点击信号
     * @param funcName 点击的功能名称
     */
    void clickedFuncName(const QString& funcName);
};

#endif // LISTOPTIONMENU_H