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
#include <QPointer>
#include <memory>
#include <array>
#include <QTimer>

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
    void initStackedWidget();

    /**
     * @brief 创建页面
     * @param id 页面索引
     * @return 创建的页面控件
     */
    QWidget* createPage(int id);

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
    void enableButton(bool flag) const;

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

public slots:
    void emitInitialized() {QTimer::singleShot(0, this, [this] {emit initialized();});}

signals:
    void initialized();

private:
    Ui::Search *ui;                              ///< UI 界面指针
    QPushButton *m_currentBtn{};                 ///< 当前选中的按钮
    std::unique_ptr<QButtonGroup> m_buttonGroup; ///< 按钮组，管理互斥按钮
    QList<QPixmap> m_coverVector;                ///< 封面图片列表
    QList<QString> m_descVector;                 ///< 描述文本列表
    std::unique_ptr<QWidget> m_recommendWidget;  ///< 推荐页面
    std::unique_ptr<QWidget> m_rankWidget;       ///< 排行页面
    std::unique_ptr<QWidget> m_specialWidget;    ///< 专题页面
    std::unique_ptr<QWidget> m_channelWidget;    ///< 频道页面
    std::array<QPointer<QWidget>, 4> m_pages{};  ///< 页面数组
    int m_currentIdx{0};                         ///< 当前页面索引
};

#endif // SEARCH_H