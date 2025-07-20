/**
* @file AboutDialog.h
 * @brief 定义 AboutDialog 类，提供关于对话框功能
 * @author WeiWang
 * @date 2025-03-21
 * @version 1.0
 */

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QWidget>
#include "libhttp.h"

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYDIALOG_LIBRARY)
#define MYDIALOG_EXPORT Q_DECL_EXPORT
#else
#define MYDIALOG_EXPORT Q_DECL_IMPORT
#endif

class ElaText;
class QVBoxLayout;
class QtMaterialDialog;

/**
 * @class AboutDialog
 * @brief 关于对话框类，显示项目信息和 Gitee Star 数量
 */
class MYDIALOG_EXPORT AboutDialog : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化关于对话框
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit AboutDialog(QWidget *parent = nullptr);

private:
    /**
     * @brief 初始化对话框布局
     * @param lay 垂直布局指针
     */
    void initDialog(QVBoxLayout *lay);

    /**
     * @brief 获取 Gitee 项目的 Star 数量
     * @param url Gitee API 地址
     */
    void getGiteeProjectStar(const QString &url);

    /**
     * @brief 初始化描述文本
     * @param text ElaText 控件指针
     */
    void initDescText(ElaText* text);

    public slots:
        /**
         * @brief 显示对话框
         */
        void onShowDialog();

    /**
     * @brief 隐藏对话框
     */
    void onHideDialog();

    signals:
        /**
         * @brief 获取到 Star 数量信号
         */
        void gotStars();

    /**
     * @brief 对话框显示状态信号
     * @param flag 是否显示
     */
    void showDialog(const bool& flag);

private:
    QtMaterialDialog *const m_dialog;        ///< 对话框对象
    QWidget*                m_topWidget;     ///< 顶部控件
    QFont                   m_font;          ///< 字体对象
    int                     m_stars = 125;    ///< Gitee Star 数量
    CLibhttp                m_libHttp;       ///< HTTP 请求对象
};

#endif // ABOUTDIALOG_H