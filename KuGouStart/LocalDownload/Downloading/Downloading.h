//
// Created by WeiWang on 25-1-27.
//

#ifndef DOWNLOADING_H
#define DOWNLOADING_H

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class Downloading;
}

QT_END_NAMESPACE

class Downloading : public QWidget {
    Q_OBJECT

public:
    explicit Downloading(QWidget *parent = nullptr);

    ~Downloading() override;

private:
    void initUi();

private slots:
    void on_start_toolButton_clicked();

    void on_stop_toolButton_clicked();

    void on_clear_toolButton_clicked();

    void on_setting_toolButton_clicked();

private slots:
    void on_search_pushButton_clicked();

signals:
    void find_more_music();

private:
    Ui::Downloading *ui;
};


#endif //DOWNLOADING_H
