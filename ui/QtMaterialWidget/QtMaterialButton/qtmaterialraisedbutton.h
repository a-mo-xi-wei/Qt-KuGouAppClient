#ifndef QTMATERIALRAISEDBUTTON_H
#define QTMATERIALRAISEDBUTTON_H

#include "qtmaterialflatbutton.h"

#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

class QtMaterialRaisedButtonPrivate;

class QTMATERIALWIDGET_EXPORT QtMaterialRaisedButton : public QtMaterialFlatButton
{
    Q_OBJECT

public:
    explicit QtMaterialRaisedButton(QWidget *parent = 0);
    explicit QtMaterialRaisedButton(const QString &text, QWidget *parent = 0);
    ~QtMaterialRaisedButton();

protected:
    QtMaterialRaisedButton(QtMaterialRaisedButtonPrivate &d, QWidget *parent = 0);

    bool event(QEvent *event) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(QtMaterialRaisedButton)
    Q_DECLARE_PRIVATE(QtMaterialRaisedButton)
};

#endif // QTMATERIALRAISEDBUTTON_H
