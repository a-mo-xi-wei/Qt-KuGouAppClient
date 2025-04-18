//
// Created by WeiWang on 24-11-15.
//

#ifndef ALLPAID_H
#define ALLPAID_H

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class AllPaid;
}

QT_END_NAMESPACE

class AllPaid : public QWidget {
    Q_OBJECT

public:
    explicit AllPaid(QWidget *parent = nullptr);

    ~AllPaid() override;

private slots:
    void on_search_pushButton_clicked();

signals:
    void find_more_music();

private:
    Ui::AllPaid *ui;
};


#endif //ALLPAID_H
