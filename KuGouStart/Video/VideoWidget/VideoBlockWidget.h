//
// Created by WeiWang on 24-12-15.
//

#ifndef VIDEOBLOCKWIDGET_H
#define VIDEOBLOCKWIDGET_H

#include <QWidget>
#include <QMouseEvent>

class QLabel;

QT_BEGIN_NAMESPACE

namespace Ui {
    class VideoBlockWidget;
}

QT_END_NAMESPACE

class VideoBlockWidget : public QWidget {
    Q_OBJECT

public:
    explicit VideoBlockWidget(QWidget *parent = nullptr);

    ~VideoBlockWidget() override;

    void setCoverPix(const QString &pixmapPath) const;

    void setVideoName(const QString &name);

    void setIconPix(const QString &pix) const;

    void setAuthor(const QString &author);

    void setShowTip() const;

    void setTipText(const QString &text) const;

private:
    void initUi() const;

    void updateVideoNameText() const;

    void updateVideoAuthorText() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    bool eventFilter(QObject *watched, QEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::VideoBlockWidget *ui;
    //视频名、视频作者
    QString m_videoName;
    QString m_videoAuthor;
};


#endif //VIDEOBLOCKWIDGET_H
