//
// Created by WeiWang on 24-12-12.
//

#ifndef VIDEOCHANNELBLOCK_H
#define VIDEOCHANNELBLOCK_H

#include <QWidget>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE

namespace Ui {
    class VideoChannelBlock;
}

QT_END_NAMESPACE

class VideoChannelBlock : public QWidget {
    Q_OBJECT

public:
    explicit VideoChannelBlock(QWidget *parent = nullptr);

    ~VideoChannelBlock() override;

    void setCoverPix(const QString &pixmapPath);

    void setDescription(const QString &description);

private:
    void initUi();

    void initCoverTextFont();

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::VideoChannelBlock *ui;
    QString m_coverText;
    QString m_descriptionText;
    QFont m_coverTextFont;
    bool m_isEnter = false;
};


#endif //VIDEOCHANNELBLOCK_H
