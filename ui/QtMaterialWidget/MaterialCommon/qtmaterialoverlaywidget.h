#ifndef QTMATERIALOVERLAYWIDGET_H
#define QTMATERIALOVERLAYWIDGET_H

#include <QWidget>

#if defined(MATERIALCOMMON_LIBRARY)
#define MATERIALCOMMON_EXPORT Q_DECL_EXPORT
#else
#define MATERIALCOMMON_EXPORT Q_DECL_IMPORT
#endif

class MATERIALCOMMON_EXPORT QtMaterialOverlayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QtMaterialOverlayWidget(QWidget *parent = 0);
    ~QtMaterialOverlayWidget();

protected:
    bool event(QEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

    virtual QRect overlayGeometry() const;

private:
    Q_DISABLE_COPY(QtMaterialOverlayWidget)
};

#endif // QTMATERIALOVERLAYWIDGET_H
