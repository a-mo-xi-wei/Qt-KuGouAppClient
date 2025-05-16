/**
* @file ListenRecentlyPlay.h
 * @brief 定义 ListenRecentlyPlay 类，提供最近播放界面
 * @author WeiWang
 * @date 2025-02-02
 * @version 1.0
 */

#ifndef LISTENRECENTLYPLAY_H
#define LISTENRECENTLYPLAY_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class ListenRecentlyPlay;
}
QT_END_NAMESPACE

/**
 * @class ListenRecentlyPlay
 * @brief 最近播放界面类，显示播放记录并支持搜索
 */
class ListenRecentlyPlay : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化最近播放界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ListenRecentlyPlay(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~ListenRecentlyPlay() override;

    private slots:
        /**
         * @brief 搜索按钮点击槽函数
         * @note 触发切换到推荐界面的信号
         */
        void on_search_pushButton_clicked();

    signals:
        /**
         * @brief 切换到推荐界面的信号
         */
        void switch_to_listen_recommend();

private:
    Ui::ListenRecentlyPlay *ui; ///< UI 指针
};

#endif // LISTENRECENTLYPLAY_H