//
// Created by WeiWang on 24-11-14.
//

#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class DeviceWidget;
}

QT_END_NAMESPACE

class DeviceWidget : public QWidget {
    Q_OBJECT

public:
    explicit DeviceWidget(QWidget *parent = nullptr);

    ~DeviceWidget() override;

private slots:
    void on_search_pushButton_clicked();

signals:
    void find_more_music();

private:
    Ui::DeviceWidget *ui;
};


#endif //DEVICEWIDGET_H
