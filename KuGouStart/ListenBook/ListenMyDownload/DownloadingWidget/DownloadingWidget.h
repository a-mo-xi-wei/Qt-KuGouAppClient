//
// Created by WeiWang on 25-2-4.
//

#ifndef DOWNLOADINGWIDGET_H
#define DOWNLOADINGWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class DownloadingWidget;
}

QT_END_NAMESPACE

class DownloadingWidget : public QWidget {
    Q_OBJECT

public:
    explicit DownloadingWidget(QWidget *parent = nullptr);

    ~DownloadingWidget() override;

private:
    void initUi();

private slots:
    void on_search_pushButton_clicked();

signals:
    void find_more_audio_book();

private:
    Ui::DownloadingWidget *ui;
};


#endif //DOWNLOADINGWIDGET_H
