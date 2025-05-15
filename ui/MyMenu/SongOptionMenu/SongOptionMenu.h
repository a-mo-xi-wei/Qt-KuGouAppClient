/**
* @file SongOptionMenu.h
 * @brief 定义 SongOptionMenu 类，提供歌曲操作选项菜单功能
 * @author WeiWang
 * @date 2025-01-12
 * @version 1.0
 */

#ifndef SONGOPTIONMENU_H
#define SONGOPTIONMENU_H

#include "../BaseMenu.h"

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYMENU_LIBRARY)
#define MYMENU_EXPORT Q_DECL_EXPORT
#else
#define MYMENU_EXPORT Q_DECL_IMPORT
#endif

/**
 * @class SongOptionMenu
 * @brief 歌曲操作选项菜单类，继承自 BaseMenu，提供播放、添加、删除等功能
 */
class MYMENU_EXPORT SongOptionMenu : public BaseMenu {
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化歌曲操作选项菜单
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit SongOptionMenu(QWidget* parent = nullptr);

    /**
     * @brief 获取当前菜单对象
     * @return 当前菜单对象指针
     */
    const SongOptionMenu* getMenu() const override;

private:
    /**
     * @brief 初始化菜单布局和内容
     */
    void initMenu() override;

    signals:
        /**
         * @brief 播放歌曲信号
         */
        void play();

    /**
     * @brief 下一首播放信号
     */
    void nextPlay();

    /**
     * @brief 添加到播放队列信号
     */
    void addToPlayQueue();

    /**
     * @brief 添加到新建歌单信号
     */
    void addToNewSongList();

    /**
     * @brief 添加到“喜欢”列表信号
     */
    void addToLove();

    /**
     * @brief 添加到默认收藏信号
     */
    void addToCollect();

    /**
     * @brief 添加到默认播放列表信号
     */
    void addToPlayList();

    /**
     * @brief 下载歌曲信号
     */
    void download();

    /**
     * @brief 分享歌曲信号
     */
    void share();

    /**
     * @brief 查看评论信号
     */
    void comment();

    /**
     * @brief 查找相似歌曲信号
     */
    void sameSong();

    /**
     * @brief 查看歌曲信息信号
     */
    void songInfo();

    /**
     * @brief 删除歌曲信号
     * @param idx 要删除的歌曲索引
     */
    void deleteSong(const int &idx);

    /**
     * @brief 打开歌曲文件所在目录信号
     */
    void openInFile();

    /**
     * @brief 搜索歌曲信号
     */
    void search();

    /**
     * @brief 上传歌曲到云盘信号
     */
    void upload();
};

#endif // SONGOPTIONMENU_H