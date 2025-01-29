//
// Created by WeiWang on 24-11-15.
//

#ifndef UPLOADEDSONG_H
#define UPLOADEDSONG_H

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class UploadedSong;
}

QT_END_NAMESPACE

class UploadedSong : public QWidget {
    Q_OBJECT

public:
    explicit UploadedSong(QWidget *parent = nullptr);

    ~UploadedSong() override;

private:
    void initUi();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::UploadedSong *ui;
    QAction *m_searchAction; //专门为了设置图片

};


#endif //UPLOADEDSONG_H
