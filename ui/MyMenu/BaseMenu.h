/**
 * @file BaseMenu.h
 * @brief 定义 BaseMenu 类，提供菜单基类功能
 * @author WeiWang
 * @date 2025-01-12
 * @version 1.0
 */

#ifndef BASEMENU_H
#define BASEMENU_H

//策略基类

#include "MenuBtn.hpp"
#include <QMenu>

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYMENU_LIBRARY)
#define MYMENU_EXPORT Q_DECL_EXPORT
#else
#define MYMENU_EXPORT Q_DECL_IMPORT
#endif

class DynamicBackgroundInterface;
class QTimeLine;
class QGraphicsOpacityEffect;
class QWidgetAction;

/**
 * @class BaseMenu
 * @brief 菜单基类，继承自 QMenu，提供阴影、动画和悬停状态管理功能
 */
class MYMENU_EXPORT BaseMenu : public QMenu
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化菜单基类
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit BaseMenu(QWidget* parent = nullptr);

    /**
     * @brief 虚析构函数，确保子类正确释放资源
     */
    ~BaseMenu() override = default;

    /**
     * @brief 虚函数，初始化菜单布局和内容，由子类实现
     */
    virtual void initMenu()
    {
    }

    /**
     * @brief 获取当前菜单对象
     * @return 当前菜单对象指针
     */
    virtual const BaseMenu* getMenu() const;

    /**
     * @brief 虚函数，设置菜单阴影效果，由子类实现
     * @param width 阴影宽度
     * @param radius 阴影圆角半径
     * @param color 阴影颜色
     */
    virtual void setShadow(const int& width, const int& radius, const QColor& color)
    {
    }

    /**
     * @brief 设置当前响应项的索引
     * @param idx 索引值
     */
    void setCurIndex(const int& idx);

    /**
     * @brief 设置动画持续时间
     * @param duration 动画持续时间（毫秒）
     */
    void setAniDuration(const int& duration);

protected:
    /**
     * @brief 检查并更新悬停状态
     */
    void checkHover();

    /**
     * @brief 检查并更新选中状态
     */
    void checkSelection() const;

    /**
     * @brief 重写绘制事件，绘制自定义阴影效果
     * @param event 绘制事件
     */
    void paintEvent(QPaintEvent* event) override;

    /**
     * @brief 重写显示事件，触发显示动画
     * @param event 显示事件
     */
    void showEvent(QShowEvent* event) override;

    /**
     * @brief 重写离开事件，清理悬停状态
     * @param event 离开事件
     */
    void leaveEvent(QEvent* event) override;

    /**
     * @brief 连接 QWidgetAction 的悬停事件到按钮
     * @param widgetAction 菜单项动作
     * @param btn 关联的按钮
     */
    void connectAction(const QWidgetAction* widgetAction, MenuBtn* btn);

protected:
    QVector<QWidget*> m_lastHover{}; ///< 上一次悬停的控件列表
    QVector<QWidget*> m_currentHover{}; ///< 当前悬停的控件列表
    MenuBtn* m_lastSelect{}; ///< 上一次选中的按钮
    MenuBtn* m_curSelect{}; ///< 当前选中的按钮
    int m_curIndex = -1; ///< 当前响应项的索引

private:
    QGraphicsOpacityEffect* m_opacityEffect{}; ///< 透明度效果
    QTimeLine* m_timeLine{}; ///< 动画时间线
    DynamicBackgroundInterface* dm_bg; ///< 动态背景
};

#endif // BASEMENU_H
