//
// Created by WeiWang on 24-11-30.
//

#ifndef MUSICREPOBLOCK_H
#define MUSICREPOBLOCK_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class MusicRepoBlock;
}

QT_END_NAMESPACE

class MusicRepoBlock : public QWidget {
    Q_OBJECT

public:
    explicit MusicRepoBlock(QWidget *parent = nullptr);

    ~MusicRepoBlock() override;

    void setCoverPix(const QString &pixmapPath);

    void setSongName(const QString &song);

    void setSinger(const QString &singer);

private:
    void updateSongText();

    void updateSingerText();

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MusicRepoBlock *ui;
    //歌曲名、歌手
    QString m_songName;
    QString m_singer;
};


#endif //MUSICREPOBLOCK_H
