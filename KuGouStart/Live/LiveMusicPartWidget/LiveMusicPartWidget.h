/**
* @file LiveMusicPartWidget.h
 * @brief 定义 LiveMusicPartWidget 类，提供直播音乐部分控件
 * @author WeiWang
 * @date 2025-02-17
 * @version 1.0
 */

#ifndef LIVEMUSICPARTWIDGET_H
#define LIVEMUSICPARTWIDGET_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class LiveMusicPartWidget;
}
QT_END_NAMESPACE

/**
 * @class LiveMusicPartWidget
 * @brief 直播音乐部分控件类，显示封面、标题和描述
 */
class LiveMusicPartWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化直播音乐部分控件
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit LiveMusicPartWidget(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~LiveMusicPartWidget() override;

    /**
     * @brief 设置标题
     * @param name 标题文本
     */
    void setTitleName(const QString &name) const;

private:
    /**
     * @brief 初始化界面
     * @note 设置工具提示、按钮图标并异步加载 JSON 数据
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
     * @note 设置封面、文本和提示
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
    Ui::LiveMusicPartWidget *ui;                    ///< UI 指针
    QList<QString>           m_leftBottomTextVec;   ///< 底部文本列表
};

#endif // LIVEMUSICPARTWIDGET_H