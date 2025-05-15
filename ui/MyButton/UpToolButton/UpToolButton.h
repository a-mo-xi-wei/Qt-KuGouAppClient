/**
* @file UpToolButton.h
 * @brief 定义 UpToolButton 类，提供上移按钮功能
 * @author WeiWang
 * @date 2024-10-16
 * @version 1.0
 */

#ifndef UPTOOLBUTTON_H
#define UPTOOLBUTTON_H

#include <QToolButton>

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYBUTTON_LIBRARY)
#define MYBUTTON_EXPORT Q_DECL_EXPORT
#else
#define MYBUTTON_EXPORT Q_DECL_IMPORT
#endif

/**
 * @class UpToolButton
 * @brief 上移按钮类，继承自 QToolButton，支持悬浮效果
 */
class MYBUTTON_EXPORT UpToolButton : public QToolButton {
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化上移按钮
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit UpToolButton(QWidget *parent = nullptr);

    /**
     * @brief 初始化按钮界面
     */
    void initUi();

protected:
    /**
     * @brief 鼠标进入事件，切换悬浮图标
     * @param event 进入事件
     */
    void enterEvent(QEnterEvent *event) override;

    /**
     * @brief 鼠标离开事件，恢复默认图标
     * @param event 事件
     */
    void leaveEvent(QEvent *event) override;
};

#endif // UPTOOLBUTTON_H