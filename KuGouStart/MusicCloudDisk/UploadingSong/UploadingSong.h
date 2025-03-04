//
// Created by WeiWang on 24-11-15.
//

#ifndef UPLOADINGSONG_H
#define UPLOADINGSONG_H

#include <QWidget>

#if defined(UPLOADINGSONG_LIBRARY)
#define UPLOADINGSONG_EXPORT Q_DECL_EXPORT
#else
#define UPLOADINGSONG_EXPORT Q_DECL_IMPORT
#endif

QT_BEGIN_NAMESPACE

namespace Ui {
    class UploadingSong;
}

QT_END_NAMESPACE

class UPLOADINGSONG_EXPORT UploadingSong : public QWidget {
    Q_OBJECT

public:
    explicit UploadingSong(QWidget *parent = nullptr);

    ~UploadingSong() override;

private:
    void initUi();

private:
    Ui::UploadingSong *ui;
};


#endif //UPLOADINGSONG_H
