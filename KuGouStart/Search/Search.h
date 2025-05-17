/**
 * @file Search.h
 * @brief 搜索界面类的头文件，定义 Search 类
 * @author WeiWang
 * @date 2024-11-14
 * @version 1.0
 */

#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include <memory>
#include <vector>

class QButtonGroup;
class QPushButton;

/**
 * @namespace Ui
 * @brief UI 命名空间，包含搜索界面的 UI 类
 */
QT_BEGIN_NAMESPACE
namespace Ui { class Search; }
QT_END_NAMESPACE

/**
 * @class Search
 * @brief 搜索界面类，管理推荐、排行、专题和频道页面
 */
class Search : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit Search(QWidget *parent = nullptr);

    /**
     * @brief 析构函数
     * @note 释放 UI 资源
     */
    ~Search() override;

private:
    /**
     * @brief 初始化界面
     * @note 设置工具按钮图标、按钮组和下标图片
     */
    void initUi();

    /**
     * @brief 初始化堆栈窗口
     * @note 创建推荐、排行、专题和频道页面
     */
    void initStackWidget();

    /**
     * @brief 初始化封面库
     * @note 加载 60 张封面图片
     */
    void initCoverVector();

    /**
     * @brief 初始化描述库
     * @note 加载描述文本列表
     */
    void initDescVector();

    /**
     * @brief 刷新数据
     * @note 随机打乱封面和描述数据
     */
    void refresh();

    /**
     * @brief 启用或禁用按钮
     * @param flag 是否启用
     * @note 控制按钮交互性
     */
    void enableButton(const bool &flag) const;

protected:
    /**
     * @brief 调整大小事件
     * @param event 调整大小事件
     * @note 调整窗口宽度和触发当前页面刷新
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief 显示事件
     * @param event 显示事件
     * @note 触发当前页面刷新
     */
    void showEvent(QShowEvent *event) override;

private slots:
    /**
     * @brief 推荐按钮点击槽函数
     * @note 切换到推荐页面并加载内容
     */
    void on_recommend_pushButton_clicked();

    /**
     * @brief 排行按钮点击槽函数
     * @note 切换到排行页面并加载内容
     */
    void on_rank_pushButton_clicked();

    /**
     * @brief 专题按钮点击槽函数
     * @note 切换到专题页面并加载内容
     */
    void on_special_pushButton_clicked();

    /**
     * @brief 频道按钮点击槽函数
     * @note 切换到频道页面并加载内容
     */
    void on_channel_pushButton_clicked();

private:
    Ui::Search *ui;                              ///< UI 界面指针
    QPushButton *m_currentBtn{};                 ///< 当前选中的按钮
    std::unique_ptr<QButtonGroup> m_buttonGroup; ///< 按钮组，管理互斥按钮
    std::vector<QPixmap> m_coverVector;          ///< 封面图片列表
    std::vector<QString> m_descVector;           ///< 描述文本列表
    std::unique_ptr<QWidget> m_recommendWidget;  ///< 推荐页面
    std::unique_ptr<QWidget> m_rankWidget;       ///< 排行页面
    std::unique_ptr<QWidget> m_specialWidget;    ///< 专题页面
    std::unique_ptr<QWidget> m_channelWidget;    ///< 频道页面
};

#endif // SEARCH_H