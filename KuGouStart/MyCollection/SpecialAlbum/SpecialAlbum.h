//
// Created by WeiWang on 24-11-14.
//

#ifndef SPECIALALBUM_H
#define SPECIALALBUM_H

#include <QWidget>

#if defined(COLLECTSPECIALALBUM_LIBRARY)
#define COLLECTSPECIALALBUM_EXPORT Q_DECL_EXPORT
#else
#define COLLECTSPECIALALBUM_EXPORT Q_DECL_IMPORT
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class SpecialAlbum; }
QT_END_NAMESPACE

class COLLECTSPECIALALBUM_EXPORT SpecialAlbum : public QWidget {
Q_OBJECT

public:
    explicit SpecialAlbum(QWidget *parent = nullptr);
    ~SpecialAlbum() override;

private:
    Ui::SpecialAlbum *ui;
};


#endif //SPECIALALBUM_H
