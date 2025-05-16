/**
* @file qtmaterialsnackbar_p.h
 * @brief 定义 QtMaterialSnackbarPrivate 类，提供 QtMaterialSnackbar 的私有实现
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALSNACKBAR_P_H
#define QTMATERIALSNACKBAR_P_H

// 前向声明
class QtMaterialSnackbar;
class QtMaterialSnackbarStateMachine;

/**
 * @class QtMaterialSnackbarPrivate
 * @brief QtMaterialSnackbar 的私有实现类，管理属性和消息队列
 * @note 内部实现
 */
class QtMaterialSnackbarPrivate
{
    Q_DISABLE_COPY(QtMaterialSnackbarPrivate) ///< 禁用拷贝
    Q_DECLARE_PUBLIC(QtMaterialSnackbar)     ///< 声明公共类

public:
    /**
     * @brief 构造函数，初始化私有类
     * @param q 指向 QtMaterialSnackbar 的指针
     */
    QtMaterialSnackbarPrivate(QtMaterialSnackbar *q);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialSnackbarPrivate();

    /**
     * @brief 初始化提示条
     */
    void init();

    QtMaterialSnackbar             *const q_ptr;          ///< 指向公共类
    QtMaterialSnackbarStateMachine *stateMachine;         ///< 状态机
    QColor                          backgroundColor;      ///< 背景颜色
    QColor                          textColor;            ///< 文本颜色
    qreal                           bgOpacity;            ///< 背景透明度
    QList<QString>                  messages;             ///< 消息队列
    int                             duration;             ///< 持续时间
    int                             boxWidth;             ///< 提示条宽度
    bool                            clickDismiss;         ///< 点击隐藏模式
    bool                            useThemeColors;       ///< 是否使用主题颜色
};

#endif // QTMATERIALSNACKBAR_P_H