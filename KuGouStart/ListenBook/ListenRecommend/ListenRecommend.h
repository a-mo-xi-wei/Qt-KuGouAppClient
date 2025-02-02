//
// Created by WeiWang on 25-2-2.
//

#ifndef LISTENRECOMMEND_H
#define LISTENRECOMMEND_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class ListenRecommend;
}

QT_END_NAMESPACE

class ListenRecommend : public QWidget {
    Q_OBJECT

public:
    explicit ListenRecommend(QWidget *parent = nullptr);

    ~ListenRecommend() override;

private:
    void initUi();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::ListenRecommend *ui;
};


#endif //LISTENRECOMMEND_H
