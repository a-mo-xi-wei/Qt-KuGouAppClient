/**
* @file MySearchLineEdit.h
 * @brief 定义 MySearchLineEdit 类，提供带动画效果的搜索输入框
 * @author WeiWang
 * @date 2024-10-10
 * @version 1.0
 */

#ifndef MYSEARCHLINEEDIT_H
#define MYSEARCHLINEEDIT_H

#include <QLineEdit>

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYSEARCHLINEEDIT_LIBRARY)
#define MYSEARCHLINEEDIT_EXPORT Q_DECL_EXPORT
#else
#define MYSEARCHLINEEDIT_EXPORT Q_DECL_IMPORT
#endif

// 前向声明
class QPropertyAnimation;

/**
 * @class MySearchLineEdit
 * @brief 自定义搜索输入框类，继承自 QLineEdit，支持焦点动画和自定义右键菜单
 */
class MYSEARCHLINEEDIT_EXPORT MySearchLineEdit : public QLineEdit {
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化搜索输入框
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit MySearchLineEdit(QWidget *parent = nullptr);

    /**
     * @brief 设置输入框最大宽度
     * @param width 最大宽度
     */
    void setMaxWidth(const int &width);

protected:
    /**
     * @brief 重写焦点获取事件，触发展开动画
     * @param event 焦点事件
     */
    void focusInEvent(QFocusEvent *event) override;

    /**
     * @brief 重写焦点丢失事件，触发收起动画
     * @param event 焦点事件
     */
    void focusOutEvent(QFocusEvent *event) override;

    /**
     * @brief 重写事件过滤器，处理鼠标点击外部区域的焦点丢失
     * @param watched 监视的对象
     * @param event 事件对象
     * @return 是否处理事件
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    /**
     * @brief 重写按键事件，限制密码模式下的复制粘贴等操作
     * @param event 按键事件
     */
    void keyPressEvent(QKeyEvent *event) override;

    /**
     * @brief 重写右键菜单事件，显示自定义菜单
     * @param event 右键菜单事件
     */
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    QPropertyAnimation *m_animation{}; ///< 宽度动画对象
    int m_originalWidth{};            ///< 原始宽度
    int m_maxWidth{};                 ///< 最大宽度
};

#endif // MYSEARCHLINEEDIT_H