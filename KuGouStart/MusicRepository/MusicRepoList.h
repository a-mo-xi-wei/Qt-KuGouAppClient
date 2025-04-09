//
// Created by WeiWang on 24-11-24.
//

#ifndef MUSICREPOLIST_H
#define MUSICREPOLIST_H

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class MusicRepoList;
}

QT_END_NAMESPACE

class MusicRepoList : public QWidget {
    Q_OBJECT

public:
    explicit MusicRepoList(QWidget *parent = nullptr);

    ~MusicRepoList() override;

    void setCoverPix(const QString &pixmapPath) const;

    void setSongName(const QString &song);

    void setSinger(const QString &singer);

private:
    void initUi();

    void updateSongText() const;

    void updateSingerText() const;

signals:
    void enterList();

    void leaveList();

    void curPlaySongNameChange(const QString& songName);

    void curPlaySingerChange(const QString& singer);

private slots:
    void onPlayAddToolBtn();

    void onLikeToolBtn();

    void onCommentToolBtn();

protected:
    void enterEvent(QEnterEvent *event) override;

    void leaveEvent(QEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MusicRepoList *ui;
    bool m_isEnter = false;
    //歌曲名、歌手
    QString m_songName;
    QString m_singer;
};

#endif //MUSICREPOLIST_H
