#ifndef QTMATERIALFAB_H
#define QTMATERIALFAB_H

#include "qtmaterialraisedbutton.h"

#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

class QtMaterialFloatingActionButtonPrivate;

class QTMATERIALWIDGET_EXPORT QtMaterialFloatingActionButton : public QtMaterialRaisedButton
{
    Q_OBJECT

public:
    explicit QtMaterialFloatingActionButton(const QIcon &icon, QWidget *parent = nullptr);
    ~QtMaterialFloatingActionButton();

    QSize sizeHint() const Q_DECL_OVERRIDE;

    void setMini(bool state);
    bool isMini() const;

    void setCorner(Qt::Corner corner);
    Qt::Corner corner() const;

    void setOffset(int x, int y);
    QSize offset() const;

    void setXOffset(int x);
    int xOffset() const;

    void setYOffset(int y);
    int yOffset() const;

protected:
    bool event(QEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    void updateClipPath() Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(QtMaterialFloatingActionButton)
    Q_DECLARE_PRIVATE(QtMaterialFloatingActionButton)

};

#endif // QTMATERIALFAB_H
