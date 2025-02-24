//
// Created by WeiWang on 25-1-12.
//

#ifndef SONGOPTIONMENU_H
#define SONGOPTIONMENU_H

#include "../BaseMenu.h"

class SongOptionMenu : public BaseMenu {
    Q_OBJECT

public:
    SongOptionMenu(QWidget* parent = nullptr);

    const SongOptionMenu* getMenu()const override;

private:
    void initMenu() override;

signals:
    //MusicItem设置相关
    void play();

    void nextPlay();

    void addToPlayQueue();

    void addToNewSongList();

    void addToLove();

    void addToCollect();

    void addToPlayList();

    void download();

    void share();

    void comment();

    void sameSong();

    void songInfo();

    void deleteSong(const int &idx);

    void openInFile();

    void search();

    void upload();
};



#endif //SONGOPTIONMENU_H
