//
// Created by WeiWang on 24-11-15.
//

#ifndef UPLOADEDSONG_H
#define UPLOADEDSONG_H

#include "SortOptionMenu.h"

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

    void getMenuPosition(const QPoint &pos);

private slots:
    void on_cloud_sort_toolButton_clicked();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::UploadedSong *ui;
    QAction *m_searchAction; //专门为了设置图片

    //菜单相关
    SortOptionMenu *m_sortOptMenu{};
    QPoint m_menuPosition;
};


#endif //UPLOADEDSONG_H
