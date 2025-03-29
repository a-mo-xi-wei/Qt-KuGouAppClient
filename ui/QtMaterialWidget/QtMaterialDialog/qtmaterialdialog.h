#ifndef QTMATERIALDIALOG_H
#define QTMATERIALDIALOG_H

#include "qtmaterialoverlaywidget.h"

#if defined(MATERIALDIALOG_LIBRARY)
#define MATERIALDIALOG_EXPORT Q_DECL_EXPORT
#else
#define MATERIALDIALOG_EXPORT Q_DECL_IMPORT
#endif

class QLayout;
class QtMaterialDialogPrivate;

class MATERIALDIALOG_EXPORT QtMaterialDialog : public QtMaterialOverlayWidget
{
    Q_OBJECT

public:
    explicit QtMaterialDialog(QWidget *parent = 0);
    ~QtMaterialDialog();

    QLayout *windowLayout() const;
    void setWindowLayout(QLayout *layout);

public slots:
    void showDialog();
    void hideDialog();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    const QScopedPointer<QtMaterialDialogPrivate> d_ptr;

private:
    Q_DISABLE_COPY(QtMaterialDialog)
    Q_DECLARE_PRIVATE(QtMaterialDialog)
};

#endif // QTMATERIALDIALOG_H
