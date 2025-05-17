/**
* @file qtmaterialsnackbar_internal.h
 * @brief 定义 QtMaterialSnackbarStateMachine 类，提供提示条状态管理
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALSNACKBAR_INTERNAL_H
#define QTMATERIALSNACKBAR_INTERNAL_H

#include <QStateMachine>
#include <QTimer>

// 前向声明
class QtMaterialSnackbar;

/**
 * @class QtMaterialSnackbarStateMachine
 * @brief 提示条状态机类，管理显示、隐藏和消息队列
 * @note 内部实现
 */
class QtMaterialSnackbarStateMachine : public QStateMachine
{
    Q_OBJECT

    Q_PROPERTY(qreal offset WRITE setOffset READ offset) ///< 偏移属性

public:
    /**
     * @brief 构造函数，初始化状态机
     * @param parent 指向 QtMaterialSnackbar 的指针
     */
    QtMaterialSnackbarStateMachine(QtMaterialSnackbar *parent);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialSnackbarStateMachine();

    /**
     * @brief 设置垂直偏移
     * @param offset 偏移值
     */
    void setOffset(qreal offset);

    /**
     * @brief 获取垂直偏移
     * @return 偏移值
     */
    inline qreal offset() const;

    public slots:
        /**
         * @brief 推进状态
         */
        void progress();

    protected slots:
        /**
         * @brief 提示条显示完成
         */
        void snackbarShown();

protected:
    /**
     * @brief 事件过滤
     * @param watched 监听对象
     * @param event 事件对象
     * @return 是否处理事件
     * @note 重写基类方法
     */
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(QtMaterialSnackbarStateMachine) ///< 禁用拷贝

    QtMaterialSnackbar *const m_snackbar;         ///< 关联提示条
    QTimer                    m_timer;            ///< 定时器
    qreal                     m_offset;           ///< 垂直偏移
};

inline qreal QtMaterialSnackbarStateMachine::offset() const
{
    return m_offset;
}

#endif // QTMATERIALSNACKBAR_INTERNAL_H