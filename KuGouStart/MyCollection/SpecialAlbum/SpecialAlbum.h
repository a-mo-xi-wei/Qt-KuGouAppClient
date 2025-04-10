//
// Created by WeiWang on 24-11-14.
//

#ifndef SPECIALALBUM_H
#define SPECIALALBUM_H

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class SpecialAlbum;
}

QT_END_NAMESPACE

class SpecialAlbum : public QWidget {
    Q_OBJECT

public:
    explicit SpecialAlbum(QWidget *parent = nullptr);

    ~SpecialAlbum() override;

private slots:
    void on_search_pushButton_clicked();

signals:
    void find_more_music();

private:
    Ui::SpecialAlbum *ui;
};


#endif //SPECIALALBUM_H
