//
// Created by WeiWang on 25-3-22.
//

#ifndef REFRESHMASK_H
#define REFRESHMASK_H

#include "qtmaterialcircularprogress.h"

#if defined(REFRESHMASK_LIBRARY)
#define REFRESHMASK_EXPORT Q_DECL_EXPORT
#else
#define REFRESHMASK_EXPORT Q_DECL_IMPORT
#endif

class REFRESHMASK_EXPORT RefreshMask : public QWidget {
    Q_OBJECT

public:
    explicit RefreshMask(QWidget *parent = nullptr);

private:
    void initUi();

private slots:
    void onShowLoading();

    void onHideLoading();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QWidget *maskWidget{};
    QtMaterialCircularProgress *progress{};
};


#endif //REFRESHMASK_H
