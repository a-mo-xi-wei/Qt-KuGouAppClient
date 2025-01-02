//
// Created by WeiWang on 24-11-30.
//

#ifndef MUSICREPOVIDEO_H
#define MUSICREPOVIDEO_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class MusicRepoVideo;
}

QT_END_NAMESPACE

class MusicRepoVideo : public QWidget {
    Q_OBJECT

public:
    explicit MusicRepoVideo(QWidget *parent = nullptr);

    ~MusicRepoVideo() override;

    void setCoverPix(const QString &pixmapPath) const;

    void setVideoName(const QString &name);

    void setIconPix(const QString &pix) const;

    void setAuthor(const QString &author);

private:
    void initUi();

    void updateVideoNameText() const;

    void updateVideoAuthorText() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    bool eventFilter(QObject *watched, QEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MusicRepoVideo *ui;
    //视频名、视频作者
    QString m_videoName;
    QString m_videoAuthor;
    //是否进入
    bool m_isEnter = false;
};
#endif //MUSICREPOVIDEO_H
