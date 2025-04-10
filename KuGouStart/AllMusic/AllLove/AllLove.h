//
// Created by WeiWang on 24-11-15.
//

#ifndef ALLLOVE_H
#define ALLLOVE_H

#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
    class AllLove;
}

QT_END_NAMESPACE

class AllLove : public QWidget {
    Q_OBJECT

public:
    explicit AllLove(QWidget *parent = nullptr);

    ~AllLove() override;

private slots:
    void on_search_pushButton_clicked();

signals:
    void find_more_music();

private:
    Ui::AllLove *ui;
};


#endif //ALLLOVE_H
