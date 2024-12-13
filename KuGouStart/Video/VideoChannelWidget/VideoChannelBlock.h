//
// Created by WeiWang on 24-12-12.
//

#ifndef VIDEOCHANNELBLOCK_H
#define VIDEOCHANNELBLOCK_H

#include <QWidget>
#include <QMouseEvent>

class QLabel;
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

    void setCoverPix(const QString &pixmapPath)const;

    void setDescription(const QString &description);

    void setCoverText(const QString &text)const;

private:
    void initUi();

    void initCoverTextFont();

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::VideoChannelBlock *ui;
    QLabel* m_coverTextLab{};
    QString m_descriptionText;
    QFont m_coverTextFont;
};


#endif //VIDEOCHANNELBLOCK_H
