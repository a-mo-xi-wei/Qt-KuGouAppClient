/**
* @file qtmaterialraisedbutton.h
 * @brief 定义 QtMaterialRaisedButton 类，提供 Material Design 凸起按钮
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALRAISEDBUTTON_H
#define QTMATERIALRAISEDBUTTON_H

#include "qtmaterialflatbutton.h"

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

// 前向声明
class QtMaterialRaisedButtonPrivate;

/**
 * @class QtMaterialRaisedButton
 * @brief 凸起按钮类，继承自 QtMaterialFlatButton，支持 Material Design 风格
 */
class QTMATERIALWIDGET_EXPORT QtMaterialRaisedButton : public QtMaterialFlatButton
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化凸起按钮
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit QtMaterialRaisedButton(QWidget *parent = 0);

    /**
     * @brief 构造函数，初始化凸起按钮
     * @param text 按钮文本
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit QtMaterialRaisedButton(const QString &text, QWidget *parent = 0);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialRaisedButton();

protected:
    /**
     * @brief 构造函数，初始化凸起按钮（私有）
     * @param d 私有类引用
     * @param parent 父控件指针，默认为 nullptr
     * @note 内部实现
     */
    QtMaterialRaisedButton(QtMaterialRaisedButtonPrivate &d, QWidget *parent = 0);

    /**
     * @brief 处理事件
     * @param event 事件对象
     * @return 是否处理事件
     * @note 重写基类方法
     */
    bool event(QEvent *event) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(QtMaterialRaisedButton)      ///< 禁用拷贝
    Q_DECLARE_PRIVATE(QtMaterialRaisedButton)   ///< 声明私有类
};

#endif // QTMATERIALRAISEDBUTTON_H