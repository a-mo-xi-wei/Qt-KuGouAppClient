/**
 * @file qtmaterialflatbutton_internal.h
 * @brief 定义 QtMaterialFlatButtonStateMachine 类，提供扁平按钮状态管理
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALFLATBUTTON_INTERNAL_H
#define QTMATERIALFLATBUTTON_INTERNAL_H

#include <QStateMachine>
#include <QColor>

// 前向声明
class QtMaterialFlatButton;
class QSequentialAnimationGroup;

/**
 * @class QtMaterialFlatButtonStateMachine
 * @brief 扁平按钮状态机类，管理按钮的交互状态（悬停、焦点、按下、选中）
 * @note 内部实现
 */
class QtMaterialFlatButtonStateMachine : public QStateMachine
{
    Q_OBJECT

    Q_PROPERTY(qreal overlayOpacity WRITE setOverlayOpacity READ overlayOpacity) ///< 覆盖层透明度属性
    Q_PROPERTY(qreal checkedOverlayProgress WRITE setCheckedOverlayProgress READ checkedOverlayProgress) ///< 选中覆盖层进度属性
    Q_PROPERTY(qreal haloOpacity WRITE setHaloOpacity READ haloOpacity) ///< 光晕透明度属性
    Q_PROPERTY(qreal haloSize WRITE setHaloSize READ haloSize) ///< 光晕尺寸属性
    Q_PROPERTY(qreal haloScaleFactor WRITE setHaloScaleFactor READ haloScaleFactor) ///< 光晕缩放因子属性

public:
    /**
     * @brief 构造函数，初始化状态机
     * @param parent 指向 QtMaterialFlatButton 的指针
     */
    explicit QtMaterialFlatButtonStateMachine(QtMaterialFlatButton *parent);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialFlatButtonStateMachine();

    /**
     * @brief 设置覆盖层透明度
     * @param opacity 覆盖层透明度
     */
    void setOverlayOpacity(qreal opacity);

    /**
     * @brief 获取覆盖层透明度
     * @return 覆盖层透明度
     */
    inline qreal overlayOpacity() const;

    /**
     * @brief 设置选中覆盖层进度
     * @param progress 选中进度（0 到 1）
     */
    void setCheckedOverlayProgress(qreal progress);

    /**
     * @brief 获取选中覆盖层进度
     * @return 选中进度
     */
    inline qreal checkedOverlayProgress() const;

    /**
     * @brief 设置光晕透明度
     * @param opacity 光晕透明度
     */
    void setHaloOpacity(qreal opacity);

    /**
     * @brief 获取光晕透明度
     * @return 光晕透明度
     */
    inline qreal haloOpacity() const;

    /**
     * @brief 设置光晕尺寸
     * @param size 光晕尺寸
     */
    void setHaloSize(qreal size);

    /**
     * @brief 获取光晕尺寸
     * @return 光晕尺寸
     */
    inline qreal haloSize() const;

    /**
     * @brief 设置光晕缩放因子
     * @param factor 缩放因子
     */
    void setHaloScaleFactor(qreal factor);

    /**
     * @brief 获取光晕缩放因子
     * @return 缩放因子
     */
    inline qreal haloScaleFactor() const;

    /**
     * @brief 启动动画和状态机
     */
    void startAnimations();

    /**
     * @brief 配置状态属性
     */
    void setupProperties();

    /**
     * @brief 更新选中状态
     */
    void updateCheckedStatus();

signals:
    /**
     * @brief 按钮按下信号
     */
    void buttonPressed();

    /**
     * @brief 按钮选中信号
     */
    void buttonChecked();

    /**
     * @brief 按钮未选中信号
     */
    void buttonUnchecked();

protected:
    /**
     * @brief 事件过滤器
     * @param watched 观察对象
     * @param event 事件对象
     * @return 是否处理事件
     * @note 重写基类方法
     */
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(QtMaterialFlatButtonStateMachine) ///< 禁用拷贝

    /**
     * @brief 添加事件转换
     * @param object 触发对象
     * @param eventType 事件类型
     * @param fromState 起始状态
     * @param toState 目标状态
     */
    void addTransition(QObject *object, QEvent::Type eventType, QState *fromState, QState *toState);

    /**
     * @brief 添加抽象转换
     * @param transition 转换对象
     * @param fromState 起始状态
     * @param toState 目标状态
     */
    void addTransition(QAbstractTransition *transition, QState *fromState, QState *toState);

    QtMaterialFlatButton *const m_button;               ///< 关联按钮
    QState               *const m_topLevelState;        ///< 顶级状态
    QState               *const m_configState;          ///< 配置状态
    QState               *const m_checkableState;       ///< 可选中状态
    QState               *const m_checkedState;         ///< 选中状态
    QState               *const m_uncheckedState;       ///< 未选中状态
    QState               *const m_neutralState;         ///< 中立状态
    QState               *const m_neutralFocusedState;  ///< 中立焦点状态
    QState               *const m_hoveredState;         ///< 悬停状态
    QState               *const m_hoveredFocusedState;  ///< 悬停焦点状态
    QState               *const m_pressedState;         ///< 按下状态
    QSequentialAnimationGroup *const m_haloAnimation;   ///< 光晕动画组
    qreal                       m_overlayOpacity;       ///< 覆盖层透明度
    qreal                       m_checkedOverlayProgress; ///< 选中覆盖层进度
    qreal                       m_haloOpacity;          ///< 光晕透明度
    qreal                       m_haloSize;             ///< 光晕尺寸
    qreal                       m_haloScaleFactor;      ///< 光晕缩放因子
    bool                        m_wasChecked;           ///< 之前选中状态
};

inline qreal QtMaterialFlatButtonStateMachine::overlayOpacity() const
{
    return m_overlayOpacity;
}

inline qreal QtMaterialFlatButtonStateMachine::checkedOverlayProgress() const
{
    return m_checkedOverlayProgress;
}

inline qreal QtMaterialFlatButtonStateMachine::haloOpacity() const
{
    return m_haloOpacity;
}

inline qreal QtMaterialFlatButtonStateMachine::haloSize() const
{
    return m_haloSize;
}

inline qreal QtMaterialFlatButtonStateMachine::haloScaleFactor() const
{
    return m_haloScaleFactor;
}

#endif // QTMATERIALFLATBUTTON_INTERNAL_H