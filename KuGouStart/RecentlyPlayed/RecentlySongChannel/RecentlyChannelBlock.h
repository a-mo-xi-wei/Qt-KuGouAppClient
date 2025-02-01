//
// Created by WeiWang on 25-1-31.
//

#ifndef RECENTLYCHANNELBLOCK_H
#define RECENTLYCHANNELBLOCK_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class RecentlyChannelBlock; }
QT_END_NAMESPACE

class RecentlyChannelBlock : public QWidget {
Q_OBJECT

public:
    explicit RecentlyChannelBlock(QWidget *parent = nullptr);

    ~RecentlyChannelBlock() override;

    void setCoverPix(const QString &pixmapPath)const;

private:
    void initUi()const;

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void enterEvent(QEnterEvent *event) override;

    void leaveEvent(QEvent *event) override;

private:
    Ui::RecentlyChannelBlock *ui;
};

#endif //RECENTLYCHANNELBLOCK_H
