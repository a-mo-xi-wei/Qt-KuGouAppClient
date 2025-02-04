//
// Created by WeiWang on 25-2-4.
//

#ifndef DOWNLOADEDWIDGET_H
#define DOWNLOADEDWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class DownloadedWidget;
}

QT_END_NAMESPACE

class DownloadedWidget : public QWidget {
    Q_OBJECT

public:
    explicit DownloadedWidget(QWidget *parent = nullptr);

    ~DownloadedWidget() override;

private:
    Ui::DownloadedWidget *ui;
};


#endif //DOWNLOADEDWIDGET_H
