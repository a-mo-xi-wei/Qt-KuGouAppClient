//
// Created by WeiWang on 24-11-11.
//

#ifndef MUSICREPOSITORY_H
#define MUSICREPOSITORY_H

#include"MyBlockWidget.h"

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class MusicRepository; }
QT_END_NAMESPACE

class MusicRepository : public QWidget {
Q_OBJECT

public:
    explicit MusicRepository(QWidget *parent = nullptr);
    ~MusicRepository() override;
private:
    void initUi();

private:
    Ui::MusicRepository *ui;
};


#endif //MUSICREPOSITORY_H
