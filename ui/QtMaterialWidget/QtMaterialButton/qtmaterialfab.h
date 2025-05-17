/**
 * @file qtmaterialfab.h
 * @brief 定义 QtMaterialFloatingActionButton 类，提供 Material Design 浮动动作按钮
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALFAB_H
#define QTMATERIALFAB_H

#include "qtmaterialraisedbutton.h"

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

// 前向声明
class QtMaterialFloatingActionButtonPrivate;

/**
 * @class QtMaterialFloatingActionButton
 * @brief 浮动动作按钮类，继承自 QtMaterialRaisedButton，支持 Material Design 风格
 */
class QTMATERIALWIDGET_EXPORT QtMaterialFloatingActionButton : public QtMaterialRaisedButton
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化浮动动作按钮
     * @param icon 按钮图标
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit QtMaterialFloatingActionButton(const QIcon &icon, QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialFloatingActionButton();

    /**
     * @brief 获取推荐尺寸
     * @return 按钮尺寸
     * @note 重写基类方法
     */
    QSize sizeHint() const Q_DECL_OVERRIDE;

    /**
     * @brief 设置迷你模式
     * @param state 是否启用迷你模式
     */
    void setMini(bool state);

    /**
     * @brief 检查是否为迷你模式
     * @return 是否为迷你模式
     */
    bool isMini() const;

    /**
     * @brief 设置按钮所在角落
     * @param corner 角落位置
     */
    void setCorner(Qt::Corner corner);

    /**
     * @brief 获取按钮所在角落
     * @return 角落位置
     */
    Qt::Corner corner() const;

    /**
     * @brief 设置按钮偏移
     * @param x X 轴偏移
     * @param y Y 轴偏移
     */
    void setOffset(int x, int y);

    /**
     * @brief 获取按钮偏移
     * @return 偏移尺寸
     */
    QSize offset() const;

    /**
     * @brief 设置 X 轴偏移
     * @param x X 轴偏移
     */
    void setXOffset(int x);

    /**
     * @brief 获取 X 轴偏移
     * @return X 轴偏移
     */
    int xOffset() const;

    /**
     * @brief 设置 Y 轴偏移
     * @param y Y 轴偏移
     */
    void setYOffset(int y);

    /**
     * @brief 获取 Y 轴偏移
     * @return Y 轴偏移
     */
    int yOffset() const;

protected:
    /**
     * @brief 处理事件
     * @param event 事件对象
     * @return 是否处理事件
     * @note 重写基类方法
     */
    bool event(QEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief 事件过滤器
     * @param obj 对象
     * @param event 事件对象
     * @return 是否处理事件
     * @note 重写基类方法
     */
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief 绘制按钮
     * @param event 绘制事件
     * @note 重写基类方法
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief 更新剪裁路径
     * @note 重写基类方法
     */
    void updateClipPath() Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(QtMaterialFloatingActionButton) ///< 禁用拷贝
    Q_DECLARE_PRIVATE(QtMaterialFloatingActionButton) ///< 声明私有类
};

#endif // QTMATERIALFAB_H