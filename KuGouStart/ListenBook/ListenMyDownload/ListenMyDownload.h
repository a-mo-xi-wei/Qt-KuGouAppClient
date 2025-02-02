//
// Created by WeiWang on 25-2-2.
//

#ifndef LISTENMYDOWNLOAD_H
#define LISTENMYDOWNLOAD_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class ListenMyDownload; }
QT_END_NAMESPACE

class ListenMyDownload : public QWidget {
Q_OBJECT

public:
    explicit ListenMyDownload(QWidget *parent = nullptr);
    ~ListenMyDownload() override;

private:
    Ui::ListenMyDownload *ui;
};


#endif //LISTENMYDOWNLOAD_H
