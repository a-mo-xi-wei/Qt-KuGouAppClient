/**
* @file qtmaterialdialog.h
 * @brief 定义 QtMaterialDialog 类，提供 Material Design 对话框
 * @author Unknown
 * @date 2025-05-16
 * @version 1.0
 */

#ifndef QTMATERIALDIALOG_H
#define QTMATERIALDIALOG_H

#include "qtmaterialoverlaywidget.h"

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

// 前向声明
class QLayout;
class QtMaterialDialogPrivate;

/**
 * @class QtMaterialDialog
 * @brief 对话框类，继承自 QtMaterialOverlayWidget，支持 Material Design 风格
 */
class QTMATERIALWIDGET_EXPORT QtMaterialDialog : public QtMaterialOverlayWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化对话框
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit QtMaterialDialog(QWidget *parent = 0);

    /**
     * @brief 析构函数，清理资源
     */
    ~QtMaterialDialog();

    /**
     * @brief 获取对话框窗口布局
     * @return 布局指针
     */
    QLayout *windowLayout() const;

    /**
     * @brief 设置对话框窗口布局
     * @param layout 布局指针
     */
    void setWindowLayout(QLayout *layout);

    public slots:
        /**
         * @brief 显示对话框
         */
        void showDialog();

    /**
     * @brief 隐藏对话框
     */
    void hideDialog();

protected:
    /**
     * @brief 绘制事件
     * @param event 绘制事件对象
     * @note 重写基类方法
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    const QScopedPointer<QtMaterialDialogPrivate> d_ptr; ///< 私有类指针

private:
    Q_DISABLE_COPY(QtMaterialDialog) ///< 禁用拷贝
    Q_DECLARE_PRIVATE(QtMaterialDialog) ///< 声明私有类
};

#endif // QTMATERIALDIALOG_H