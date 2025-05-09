//
// Created by WeiWang on 24-11-15.
//

#ifndef RECENTLYVIDEOWIDGET_H
#define RECENTLYVIDEOWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class RecentlyVideoWidget;
}

QT_END_NAMESPACE

class RecentlyVideoWidget : public QWidget {
    Q_OBJECT

public:
    explicit RecentlyVideoWidget(QWidget *parent = nullptr);

    ~RecentlyVideoWidget() override;

private slots:
    void on_search_pushButton_clicked();

signals:
    void find_more_music();

private:
    Ui::RecentlyVideoWidget *ui;
};


#endif //RECENTLYVIDEOWIDGET_H
