//
// Created by WeiWang on 24-11-14.
//

#ifndef SINGERWIDGET_H
#define SINGERWIDGET_H

#include <QWidget>

#if defined(COLLECTSINGER_LIBRARY)
#define COLLECTSINGER_EXPORT Q_DECL_EXPORT
#else
#define COLLECTSINGER_EXPORT Q_DECL_IMPORT
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class SingerWidget; }
QT_END_NAMESPACE

class COLLECTSINGER_EXPORT SingerWidget : public QWidget {
Q_OBJECT

public:
    explicit SingerWidget(QWidget *parent = nullptr);
    ~SingerWidget() override;

private:
    Ui::SingerWidget *ui;
};


#endif //SINGERWIDGET_H
