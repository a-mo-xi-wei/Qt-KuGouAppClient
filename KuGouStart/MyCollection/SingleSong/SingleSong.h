//
// Created by WeiWang on 24-11-14.
//

#ifndef SINGLESONG_H
#define SINGLESONG_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class SingleSong; }
QT_END_NAMESPACE

class SingleSong : public QWidget {
Q_OBJECT

public:
    explicit SingleSong(QWidget *parent = nullptr);
    ~SingleSong() override;

private:
    Ui::SingleSong *ui;
};


#endif //SINGLESONG_H
