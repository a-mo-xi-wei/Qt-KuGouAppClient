/**
 * @file LiveBigLeftWidget.h
 * @brief 定义 LiveBigLeftWidget 类，提供直播左侧大控件
 * @author WeiWang
 * @date 2025-02-17
 * @version 1.0
 */

#ifndef LIVEBIGLEFTWIDGET_H
#define LIVEBIGLEFTWIDGET_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class LiveBigLeftWidget;
}
QT_END_NAMESPACE

/**
 * @class LiveBigLeftWidget
 * @brief 直播左侧大控件类，显示封面和文本内容
 */
class LiveBigLeftWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化直播左侧大控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit LiveBigLeftWidget(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~LiveBigLeftWidget() override;

    /**
     * @brief 设置标题
     * @param name 标题文本
     */
    void setTitleName(const QString &name) const;

    /**
     * @brief 设置无提示标签
     * @note 隐藏所有控件的提示标签
     */
    void setNoTipLab() const;

private:
    /**
     * @brief 初始化界面
     * @note 设置按钮图标并异步加载 JSON 数据
     */
    void initUi();

    /**
     * @brief 解析 JSON 文件
     * @param filePath 文件路径
     * @return 文本列表
     */
    QList<QString> parseJsonFile(const QString &filePath);

    /**
     * @brief 初始化块控件
     * @note 设置封面比例、文本和随机图片
     */
    void initBlockWidget();

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
     * @note 动态显示或隐藏控件
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

private:
    Ui::LiveBigLeftWidget *ui;                    ///< UI 指针
    QList<QString>         m_leftBottomTextVec;   ///< 底部文本列表
};

#endif // LIVEBIGLEFTWIDGET_H