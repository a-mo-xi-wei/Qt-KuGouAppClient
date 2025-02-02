//
// Created by WeiWang on 24-11-15.
//

#ifndef ALLLOCAL_H
#define ALLLOCAL_H

#include <QWidget>


QT_BEGIN_NAMESPACE

namespace Ui {
    class AllLocal;
}

QT_END_NAMESPACE

class AllLocal : public QWidget {
    Q_OBJECT

public:
    explicit AllLocal(QWidget *parent = nullptr);

    ~AllLocal() override;

private:
    void initUi();

private:
    Ui::AllLocal *ui;
};


#endif //ALLLOCAL_H
