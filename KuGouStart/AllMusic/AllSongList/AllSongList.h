/**
* @file AllSongList.h
 * @brief 定义 AllSongList 类，提供歌单界面
 * @author WeiWang
 * @date 2024-11-15
 * @version 1.0
 */

#ifndef ALLSONGLIST_H
#define ALLSONGLIST_H

#include <QWidget>

/**
 * @namespace Ui
 * @brief 包含 UI 类的命名空间
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class AllSongList;
}
QT_END_NAMESPACE

/**
 * @class AllSongList
 * @brief 歌单界面类，支持搜索更多音乐
 */
class AllSongList : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化歌单界面
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit AllSongList(QWidget *parent = nullptr);

    /**
     * @brief 析构函数，清理资源
     */
    ~AllSongList() override;

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
    Ui::AllSongList *ui; ///< UI 指针
};

#endif // ALLSONGLIST_H