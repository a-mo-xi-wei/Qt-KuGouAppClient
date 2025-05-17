/**
 * @file qtmaterialdialog_internal.h
 * @brief 定义 QtMaterialDialogProxy 和 QtMaterialDialogWindow 类，提供对话框内部实现
 * @author laserpants
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALDIALOG_INTERNAL_H
#define QTMATERIALDIALOG_INTERNAL_H

#include <QWidget>

// 前向声明
class QStackedLayout;
class QtMaterialDialog;
class QtMaterialDialogWindow;

/**
 * @class QtMaterialDialogProxy
 * @brief 对话框代理类，管理透明度过渡和内容同步
 * @note 内部实现
 */
class QtMaterialDialogProxy : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(qreal opacity WRITE setOpacity READ opacity) ///< 透明度属性

    /**
     * @enum TransparencyMode
     * @brief 透明度模式
     */
    enum TransparencyMode {
        Transparent,     ///< 完全透明
        SemiTransparent, ///< 半透明
        Opaque           ///< 完全不透明
    };

public:
    /**
     * @brief 构造函数，初始化代理窗口
     * @param source 对话框窗口指针
     * @param layout 堆叠布局指针
     * @param dialog 对话框指针
     * @param parent 父控件指针，默认为 nullptr
     */
    QtMaterialDialogProxy(QtMaterialDialogWindow *source,
                          QStackedLayout *layout,
                          QtMaterialDialog *dialog,
                          QWidget *parent = 0);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialDialogProxy();

    /**
     * @brief 设置透明度
     * @param opacity 透明度值
     */
    void setOpacity(qreal opacity);

    /**
     * @brief 获取透明度
     * @return 透明度值
     */
    inline qreal opacity() const;

protected slots:
    /**
     * @brief 切换为完全不透明
     */
    void makeOpaque();

    /**
     * @brief 切换为完全透明
     */
    void makeTransparent();

    /**
     * @brief 获取推荐尺寸
     * @return 推荐尺寸
     * @note 重写基类方法
     */
    QSize sizeHint() const Q_DECL_OVERRIDE;

protected:
    /**
     * @brief 处理事件
     * @param event 事件对象
     * @return 是否处理事件
     * @note 重写基类方法
     */
    bool event(QEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief 绘制事件
     * @param event 绘制事件对象
     * @note 重写基类方法
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(QtMaterialDialogProxy) ///< 禁用拷贝

    QtMaterialDialogWindow *const m_source;   ///< 对话框窗口
    QStackedLayout         *const m_layout;   ///< 堆叠布局
    QtMaterialDialog       *const m_dialog;   ///< 对话框
    qreal                         m_opacity;  ///< 透明度
    TransparencyMode              m_mode;     ///< 透明度模式
};

inline qreal QtMaterialDialogProxy::opacity() const
{
    return m_opacity;
}

/**
 * @class QtMaterialDialogWindow
 * @brief 对话框内容窗口类，管理内容和偏移
 * @note 内部实现
 */
class QtMaterialDialogWindow : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int offset WRITE setOffset READ offset) ///< 偏移属性

public:
    /**
     * @brief 构造函数，初始化内容窗口
     * @param dialog 对话框指针
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit QtMaterialDialogWindow(QtMaterialDialog *dialog, QWidget *parent = 0);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialDialogWindow();

    /**
     * @brief 设置垂直偏移
     * @param offset 偏移量
     */
    void setOffset(int offset);

    /**
     * @brief 获取垂直偏移
     * @return 偏移量
     */
    int offset() const;

protected:
    /**
     * @brief 绘制事件
     * @param event 绘制事件对象
     * @note 重写基类方法
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(QtMaterialDialogWindow) ///< 禁用拷贝

    QtMaterialDialog *const m_dialog; ///< 对话框
};

#endif // QTMATERIALDIALOG_INTERNAL_H