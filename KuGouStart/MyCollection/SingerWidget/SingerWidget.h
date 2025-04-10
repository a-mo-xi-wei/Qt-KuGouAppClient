//
// Created by WeiWang on 24-11-14.
//

#ifndef SINGERWIDGET_H
#define SINGERWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class SingerWidget;
}

QT_END_NAMESPACE

class SingerWidget : public QWidget {
    Q_OBJECT

public:
    explicit SingerWidget(QWidget *parent = nullptr);

    ~SingerWidget() override;

private slots:
    void on_search_pushButton_clicked();

signals:
    void find_more_music();

private:
    Ui::SingerWidget *ui;
};


#endif //SINGERWIDGET_H
