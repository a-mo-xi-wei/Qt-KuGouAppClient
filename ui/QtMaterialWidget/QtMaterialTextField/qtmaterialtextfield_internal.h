/**
 * @file qtmaterialtextfield_internal.h
 * @brief 定义 QtMaterialTextFieldStateMachine 和 QtMaterialTextFieldLabel 类，提供文本输入框内部实现
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALTEXTFIELD_INTERNAL_H
#define QTMATERIALTEXTFIELD_INTERNAL_H

#include <QStateMachine>
#include "qtmaterialtextfield.h"

// 前向声明
class QPropertyAnimation;
class QtMaterialTextFieldLabel;

/**
 * @class QtMaterialTextFieldStateMachine
 * @brief 文本输入框状态机类，管理正常和聚焦状态
 * @note 内部实现
 */
class QtMaterialTextFieldStateMachine : public QStateMachine
{
    Q_OBJECT

    Q_PROPERTY(qreal progress WRITE setProgress READ progress) ///< 动画进度属性

public:
    /**
     * @brief 构造函数，初始化状态机
     * @param parent 指向 QtMaterialTextField 的指针
     */
    QtMaterialTextFieldStateMachine(QtMaterialTextField *parent);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialTextFieldStateMachine();

    /**
     * @brief 设置标签控件
     * @param label 标签控件指针
     */
    void setLabel(QtMaterialTextFieldLabel *label);

    /**
     * @brief 设置动画进度
     * @param progress 进度值
     */
    inline void setProgress(qreal progress);

    /**
     * @brief 获取动画进度
     * @return 进度值
     */
    inline qreal progress() const;

public slots:
    /**
     * @brief 更新状态机属性
     */
    void setupProperties();

private:
    Q_DISABLE_COPY(QtMaterialTextFieldStateMachine) ///< 禁用拷贝

    QtMaterialTextField      *const m_textField;       ///< 关联文本输入框
    QState                   *const m_normalState;     ///< 正常状态
    QState                   *const m_focusedState;    ///< 聚焦状态
    QtMaterialTextFieldLabel       *m_label;           ///< 标签控件
    QPropertyAnimation             *m_offsetAnimation; ///< 偏移动画
    QPropertyAnimation             *m_colorAnimation;  ///< 颜色动画
    qreal                           m_progress;        ///< 动画进度
};

inline void QtMaterialTextFieldStateMachine::setProgress(qreal progress)
{
    m_progress = progress;
    m_textField->update(); ///< 刷新界面
}

inline qreal QtMaterialTextFieldStateMachine::progress() const
{
    return m_progress;
}

/**
 * @class QtMaterialTextFieldLabel
 * @brief 文本输入框标签类，管理浮动标签
 * @note 内部实现
 */
class QtMaterialTextFieldLabel : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(qreal scale WRITE setScale READ scale)        ///< 缩放属性
    Q_PROPERTY(QPointF offset WRITE setOffset READ offset)   ///< 偏移属性
    Q_PROPERTY(QColor color WRITE setColor READ color)       ///< 颜色属性

public:
    /**
     * @brief 构造函数，初始化标签控件
     * @param parent 指向 指向 QtMaterialTextField 的指针
     */
    QtMaterialTextFieldLabel(QtMaterialTextField *parent);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialTextFieldLabel();

    /**
     * @brief 设置缩放比例
     * @param scale 缩放比例
     */
    inline void setScale(qreal scale);

    /**
     * @brief 获取缩放比例
     * @return 缩放比例
     */
    inline qreal scale() const;

    /**
     * @brief 设置偏移位置
     * @param pos 偏移位置
     */
    inline void setOffset(const QPointF &pos);

    /**
     * @brief 获取偏移位置
     * @return 偏移位置
     */
    inline QPointF offset() const;

    /**
     * @brief 设置标签颜色
     * @param color 标签颜色
     */
    inline void setColor(const QColor &color);

    /**
     * @brief 获取标签颜色
     * @return 标签颜色
     */
    inline QColor color() const;

protected:
    /**
     * @brief 绘制事件
     * @param event 绘制事件对象
     * @note 重写基类方法
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(QtMaterialTextFieldLabel) ///< 禁用拷贝

    QtMaterialTextField *const m_textField;     ///< 关联文本输入框
    qreal                      m_scale;         ///< 缩放比例
    qreal                      m_posX;          ///< X 偏移
    qreal                      m_posY;          ///< Y 偏移
    QColor                     m_color;         ///< 标签颜色
};

inline void QtMaterialTextFieldLabel::setScale(qreal scale)
{
    m_scale = scale;
    update(); ///< 刷新界面
}

inline qreal QtMaterialTextFieldLabel::scale() const
{
    return m_scale;
}

inline void QtMaterialTextFieldLabel::setOffset(const QPointF &pos)
{
    m_posX = pos.x();
    m_posY = pos.y();
    update(); ///< 刷新界面
}

inline QPointF QtMaterialTextFieldLabel::offset() const
{
    return QPointF(m_posX, m_posY);
}

inline void QtMaterialTextFieldLabel::setColor(const QColor &color)
{
    m_color = color;
    update(); ///< 刷新界面
}

inline QColor QtMaterialTextFieldLabel::color() const
{
    return m_color;
}

#endif // QTMATERIALTEXTFIELD_INTERNAL_H