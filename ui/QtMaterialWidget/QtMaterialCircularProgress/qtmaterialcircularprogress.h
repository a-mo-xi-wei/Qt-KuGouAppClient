#ifndef QTMATERIALCIRCULARPROGRESS_H
#define QTMATERIALCIRCULARPROGRESS_H

#include <QProgressBar>
#include "qtmaterialtheme.h"

#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

class QtMaterialCircularProgressPrivate;

class QTMATERIALWIDGET_EXPORT QtMaterialCircularProgress : public QProgressBar {
    Q_OBJECT

    Q_PROPERTY(qreal lineWidth WRITE setLineWidth READ lineWidth)
    Q_PROPERTY(qreal size WRITE setSize READ size)
    Q_PROPERTY(QColor color WRITE setColor READ color)

public:
    explicit QtMaterialCircularProgress(QWidget *parent = 0);

    ~QtMaterialCircularProgress();

    void setProgressType(Material::ProgressType type);

    Material::ProgressType progressType() const;

    void setUseThemeColors(bool value);

    bool useThemeColors() const;

    void setLineWidth(qreal width);

    qreal lineWidth() const;

    void setSize(int size);

    int size() const;

    void setColor(const QColor &color);

    QColor color() const;

    QSize sizeHint() const Q_DECL_OVERRIDE;

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    const QScopedPointer<QtMaterialCircularProgressPrivate> d_ptr;

private:
    Q_DISABLE_COPY(QtMaterialCircularProgress)

    Q_DECLARE_PRIVATE(QtMaterialCircularProgress)
};

#endif // QTMATERIALCIRCULARPROGRESS_H
