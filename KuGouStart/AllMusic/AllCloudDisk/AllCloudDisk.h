//
// Created by WeiWang on 24-11-15.
//

#ifndef ALLCLOUDDISK_H
#define ALLCLOUDDISK_H

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class AllCloudDisk;
}

QT_END_NAMESPACE

class AllCloudDisk : public QWidget {
    Q_OBJECT

public:
    explicit AllCloudDisk(QWidget *parent = nullptr);

    ~AllCloudDisk() override;

private slots:
    void on_search_pushButton_clicked();

signals:
    void find_more_music();

private:
    Ui::AllCloudDisk *ui;
};


#endif //ALLCLOUDDISK_H
