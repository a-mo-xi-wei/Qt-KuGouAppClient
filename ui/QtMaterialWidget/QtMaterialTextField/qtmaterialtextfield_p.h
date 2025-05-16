/**
* @file qtmaterialtextfield_p.h
 * @brief 定义 QtMaterialTextFieldPrivate 类，提供 QtMaterialTextField 的私有实现
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALTEXTFIELD_P_H
#define QTMATERIALTEXTFIELD_P_H

#include <QColor>

// 前向声明
class QtMaterialTextField;
class QtMaterialTextFieldStateMachine;
class QtMaterialTextFieldLabel;

/**
 * @class QtMaterialTextFieldPrivate
 * @brief QtMaterialTextField 的私有实现类，管理状态机和属性
 * @note 内部实现
 */
class QtMaterialTextFieldPrivate
{
    Q_DISABLE_COPY(QtMaterialTextFieldPrivate) ///< 禁用拷贝
    Q_DECLARE_PUBLIC(QtMaterialTextField)     ///< 声明公共类

public:
    /**
     * @brief 构造函数，初始化私有类
     * @param q 指向 QtMaterialTextField 的指针
     */
    QtMaterialTextFieldPrivate(QtMaterialTextField *q);

    /**
     * @brief 析构函数，清理资源
     */
    virtual ~QtMaterialTextFieldPrivate();

    /**
     * @brief 初始化文本输入框
     */
    void init();

    QtMaterialTextField             *const q_ptr;           ///< 指向公共类
    QtMaterialTextFieldStateMachine *stateMachine;          ///< 状态机
    QtMaterialTextFieldLabel        *label;                 ///< 标签控件
    QColor                           textColor;             ///< 文本颜色
    QColor                           labelColor;            ///< 标签颜色
    QColor                           inkColor;              ///< 墨水颜色
    QColor                           inputLineColor;        ///< 输入线颜色
    QString                          labelString;           ///< 标签文本
    qreal                            labelFontSize;         ///< 标签字体大小
    bool                             showLabel;             ///< 是否显示标签
    bool                             showInputLine;         ///< 是否显示输入线
    bool                             useThemeColors;        ///< 是否使用主题颜色
};

#endif // QTMATERIALTEXTFIELD_P_H