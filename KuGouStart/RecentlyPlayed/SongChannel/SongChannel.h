//
// Created by WeiWang on 24-11-15.
//

#ifndef SONGCHANNEL_H
#define SONGCHANNEL_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class SongChannel; }
QT_END_NAMESPACE

class SongChannel : public QWidget {
Q_OBJECT

public:
    explicit SongChannel(QWidget *parent = nullptr);
    ~SongChannel() override;

private:
    Ui::SongChannel *ui;
};


#endif //SONGCHANNEL_H
