//
// Created by WeiWang on 24-11-14.
//

#ifndef SONGLIST_H
#define SONGLIST_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class SongList;
}

QT_END_NAMESPACE

class SongList : public QWidget {
    Q_OBJECT

public:
    explicit SongList(QWidget *parent = nullptr);

    ~SongList() override;

private:
    void initUi();

    //初始化封面库
    void initCoverVector();
    //初始化描述库
    void initDescVector();
    //打乱
    void shuffleVector();
private:
    Ui::SongList *ui;

    std::vector<QPixmap> m_coverVector;//存放图片
    std::vector<QString> m_descVector;//存放描述
};


#endif //SONGLIST_H
