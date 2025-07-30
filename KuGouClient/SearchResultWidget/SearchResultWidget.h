//
// Created by WeiWang on 25-7-29.
//

#ifndef SEARCHRESULTWIDGET_H
#define SEARCHRESULTWIDGET_H

#include "libhttp.h"
#include "RefreshMask.h"

class MusicItemWidget;

class SearchResultWidget : public QWidget {
    Q_OBJECT

public:
    explicit SearchResultWidget(QWidget *parent = nullptr);

    void handleSuggestion(const QString &suggestText);

    void playNextMusic();

    void playPreviousMusic();

private:
    void initUi();

    /**
     * @brief 异步加载搜索结果里面的封面图片
     * @param item 音乐项
     * @param imageUrl 封面图片的网络路径
     */
    void loadCoverAsync(MusicItemWidget *item, const QString &imageUrl);

    /**
     * @brief 异步加载搜索结果里的音乐路径
     * @param item 音乐项
     * @param songHash 歌曲的网络哈希
     */
    void loadSongUrlAsync(MusicItemWidget *item, const QString &songHash);

protected:
    void resizeEvent(QResizeEvent *event) override;

    void showEvent(QShowEvent *event) override;

signals:
    void playMusic(MusicItemWidget* item);

private:
    QVector<MusicItemWidget *> m_searchMusicItemVector; ///< 音乐项容器
    std::unique_ptr<RefreshMask> m_refreshMask;         ///< 刷新遮罩
    CLibhttp m_libHttp;                                 ///< HTTP 请求库
    MusicItemWidget *m_playingItem = nullptr;           ///< 正在播放的音乐项
};


#endif //SEARCHRESULTWIDGET_H
