//
// Created by WeiWang on 24-12-3.
//

#ifndef CHANNELBLOCK_H
#define CHANNELBLOCK_H

#include <QMouseEvent>
#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class ChannelBlock;
}

QT_END_NAMESPACE

class ChannelBlock : public QWidget {
    Q_OBJECT

public:
    explicit ChannelBlock(QWidget *parent = nullptr);

    ~ChannelBlock() override;

    void setCoverPix(const QString &pixmapPath);

    void setTitleText(const QString &title);

    void setSingerSongText(const QString &singerSong);

private:
    void initUi();

    void updateTitleText();

    void updateSingerSongText();

protected:
    void enterEvent(QEnterEvent *event) override;

    void leaveEvent(QEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    Ui::ChannelBlock *ui;
    //text
    QString m_titleText;
    QString m_singerSongText;
};


#endif //CHANNELBLOCK_H
