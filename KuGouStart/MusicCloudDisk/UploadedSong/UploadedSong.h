//
// Created by WeiWang on 24-11-15.
//

#ifndef UPLOADEDSONG_H
#define UPLOADEDSONG_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class UploadedSong; }
QT_END_NAMESPACE

class UploadedSong : public QWidget {
Q_OBJECT

public:
    explicit UploadedSong(QWidget *parent = nullptr);
    ~UploadedSong() override;

private:
    Ui::UploadedSong *ui;
};


#endif //UPLOADEDSONG_H
