//
// Created by WeiWang on 24-11-14.
//

#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QWidget>

#if defined(COLLECTIONDEVICE_LIBRARY)
#define COLLECTIONDEVICE_EXPORT Q_DECL_EXPORT
#else
#define COLLECTIONDEVICE_EXPORT Q_DECL_IMPORT
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class DeviceWidget; }
QT_END_NAMESPACE

class COLLECTIONDEVICE_EXPORT DeviceWidget : public QWidget {
Q_OBJECT

public:
    explicit DeviceWidget(QWidget *parent = nullptr);
    ~DeviceWidget() override;

private:
    Ui::DeviceWidget *ui;
};


#endif //DEVICEWIDGET_H
