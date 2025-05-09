#ifndef QTMATERIALTHEME_P_H
#define QTMATERIALTHEME_P_H

#include <QHash>

class QtMaterialTheme;

#if defined(QTMATERIALWIDGET_LIBRARY)
#define QTMATERIALWIDGET_EXPORT Q_DECL_EXPORT
#else
#define QTMATERIALWIDGET_EXPORT Q_DECL_IMPORT
#endif

class QTMATERIALWIDGET_EXPORT QtMaterialThemePrivate
{
    Q_DISABLE_COPY(QtMaterialThemePrivate)
    Q_DECLARE_PUBLIC(QtMaterialTheme)

public:
    QtMaterialThemePrivate(QtMaterialTheme *q);
    ~QtMaterialThemePrivate();

    QColor rgba(int r, int g, int b, qreal a) const;

    QtMaterialTheme *const q_ptr;
    QHash<QString, QColor> colors;
};

#endif // QTMATERIALTHEME_P_H
