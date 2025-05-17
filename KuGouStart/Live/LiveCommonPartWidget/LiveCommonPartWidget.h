/**
 * @file LiveCommonPartWidget.h
 * @brief 定义 LiveCommonPartWidget 类，提供直播通用部分控件
 * @author WeiWang
 * @date 2025-02-17
 * @version 1.0
 */

#ifndef LIVECOMMONPARTWIDGET_H
#define LIVECOMMONPARTWIDGET_H

#include <QWidget>

/**
 * @class LiveBlockWidget
 * @brief 直播块控件类
 */
class LiveBlockWidget;

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class LiveCommonPartWidget;
}
QT_END_NAMESPACE

/**
 * @class LiveCommonPartWidget
 * @brief 直播通用部分控件类，支持多行块控件布局
 */
class LiveCommonPartWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化直播通用部分控件
     * @param parent 父控件指针，默认为 nullptr
     * @param lines 行数，默认为 1
     */
    explicit LiveCommonPartWidget(QWidget *parent = nullptr, int lines = 1);

    /**
     * @brief 析构函数，清理资源
     */
    ~LiveCommonPartWidget() override;

    /**
     * @brief 设置标题
     * @param name 标题文本
     */
    void setTitleName(const QString &name) const;

    /**
     * @brief 隐藏提示标签
     * @note 隐藏所有块控件的提示标签
     */
    void setNoTipLab();

private:
    /**
     * @brief 初始化界面
     * @param lines 行数
     * @note 设置按钮图标并异步加载 JSON 数据
     */
    void initUi(const int &lines);

    /**
     * @brief 初始化第一行
     * @note 创建并布局块控件
     */
    void initLineOne();

    /**
     * @brief 初始化第二行
     * @note 创建并布局块控件
     */
    void initLineTwo();

    /**
     * @brief 获取目录文件数量
     * @param folderPath 目录路径
     * @return 文件数量
     */
    int getFileCount(const QString &folderPath);

private slots:
    /**
     * @brief 全部按钮点击槽函数
     * @note 显示暂无更多提示
     */
    void on_all_pushButton_clicked();

protected:
    /**
     * @brief 调整大小事件
     * @param event 调整大小事件
     * @note 动态显示或隐藏块控件
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief 事件过滤器
     * @param watched 监视的对象
     * @param event 事件
     * @return 是否处理事件
     * @note 处理左右按钮点击事件
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    /**
     * @brief 初始化完成信号
     */
    void initOK();

private:
    Ui::LiveCommonPartWidget *ui;                     ///< UI 指针
    LiveBlockWidget          *m_blockArr[12] = {nullptr};         ///< 块控件数组
    QList<QString>            m_leftBottomTextVec;    ///< 底部文本列表
};

#endif // LIVECOMMONPARTWIDGET_H