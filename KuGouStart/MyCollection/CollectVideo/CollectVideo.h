//
// Created by WeiWang on 24-11-15.
//

#ifndef COLLECTVIDEO_H
#define COLLECTVIDEO_H

#include <QWidget>

#if defined(COLLECTVIDEO_LIBRARY)
#define COLLECTVIDEO_EXPORT Q_DECL_EXPORT
#else
#define COLLECTVIDEO_EXPORT Q_DECL_IMPORT
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class CollectVideo; }
QT_END_NAMESPACE

class COLLECTVIDEO_EXPORT CollectVideo : public QWidget {
Q_OBJECT

public:
    explicit CollectVideo(QWidget *parent = nullptr);
    ~CollectVideo() override;

private:
    Ui::CollectVideo *ui;
};


#endif //COLLECTVIDEO_H
