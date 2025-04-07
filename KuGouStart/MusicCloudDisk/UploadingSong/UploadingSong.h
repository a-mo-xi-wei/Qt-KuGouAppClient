//
// Created by WeiWang on 24-11-15.
//

#ifndef UPLOADINGSONG_H
#define UPLOADINGSONG_H

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class UploadingSong;
}

QT_END_NAMESPACE

class UploadingSong : public QWidget {
    Q_OBJECT

public:
    explicit UploadingSong(QWidget *parent = nullptr);

    ~UploadingSong() override;

private:
    void initUi();

private slots:
    void on_cloud_upload_toolButton_clicked();

private:
    Ui::UploadingSong *ui;
};


#endif //UPLOADINGSONG_H
