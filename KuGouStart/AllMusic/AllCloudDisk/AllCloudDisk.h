//
// Created by WeiWang on 24-11-15.
//

#ifndef ALLCLOUDDISK_H
#define ALLCLOUDDISK_H

#include <QWidget>

#if defined(ALLCLOUDDISK_LIBRARY)
#define ALLCLOUDDISK_EXPORT Q_DECL_EXPORT
#else
#define ALLCLOUDDISK_EXPORT Q_DECL_IMPORT
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class AllCloudDisk; }
QT_END_NAMESPACE

class ALLCLOUDDISK_EXPORT AllCloudDisk : public QWidget {
Q_OBJECT

public:
    explicit AllCloudDisk(QWidget *parent = nullptr);
    ~AllCloudDisk() override;

private:
    Ui::AllCloudDisk *ui;
};


#endif //ALLCLOUDDISK_H
