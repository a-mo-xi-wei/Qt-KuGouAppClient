/**
* @file RefreshMask.h
 * @brief 定义 RefreshMask 类，提供加载遮罩功能
 * @author WeiWang
 * @date 2025-03-22
 * @version 1.0
 */

#ifndef REFRESHMASK_H
#define REFRESHMASK_H

#include "qtmaterialcircularprogress.h"

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(UIWIDGETS_LIBRARY)
#define UIWIDGETS_EXPORT Q_DECL_EXPORT
#else
#define UIWIDGETS_EXPORT Q_DECL_IMPORT
#endif

/**
 * @class RefreshMask
 * @brief 加载遮罩类，显示模糊背景和进度条
 */
class UIWIDGETS_EXPORT RefreshMask : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化加载遮罩
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit RefreshMask(QWidget *parent = nullptr);

    /**
     * @brief 显示加载遮罩
     */
    void showLoading(const int& loadingTime = 2000);

    /**
     * @brief 保持加载遮罩
     */
    void keepLoading();

    /**
     * @brief 隐藏加载遮罩
     */
    void hideLoading(const QString& message = "");

    /**
     * @brief 判断是否正在加显示
     * @return 是否正在显示
     */
    bool isLoading();

private:
    /**
     * @brief 初始化界面
     */
    void initUi();

protected:
    /**
     * @brief 大小调整事件，更新遮罩和进度条位置
     * @param event 大小调整事件
     */
    void resizeEvent(QResizeEvent *event) override;

    void showEvent(QShowEvent *event) override;

signals:
    /**
     * @brief 加载完成信号
     */
    void loadingFinished(const QString&  message = "加载完成");

private:
    QWidget*                    maskWidget{};     ///< 遮罩控件
    QtMaterialCircularProgress* progress{};       ///< 进度条控件
    QTimer*                     m_timer{};        ///< 定时器
};

#endif // REFRESHMASK_H