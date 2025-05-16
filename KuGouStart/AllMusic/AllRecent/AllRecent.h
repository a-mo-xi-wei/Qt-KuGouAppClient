/**
* @file AllRecent.h
 * @brief 定义 AllRecent 类，提供最近播放音乐界面
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#ifndef ALLRECENT_H
#define ALLRECENT_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class AllRecent;
}
QT_END_NAMESPACE

/**
 * @class AllRecent
 * @brief 最近播放音乐界面类，支持搜索更多音乐
 */
class AllRecent : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化最近播放音乐界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit AllRecent(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~AllRecent() override;

    private slots:
        /**
         * @brief 处理搜索按钮点击
         */
        void on_search_pushButton_clicked();

    signals:
        /**
         * @brief 搜索更多音乐信号
         */
        void find_more_music();

private:
    Ui::AllRecent *ui; ///< UI 指针
};

#endif // ALLRECENT_H