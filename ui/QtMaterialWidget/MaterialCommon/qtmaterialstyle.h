#ifndef QTMATERIALSTYLE_H
#define QTMATERIALSTYLE_H

#include <QCommonStyle>
#include "qtmaterialstyle_p.h"

#if defined(MATERIALCOMMON_LIBRARY)
#define MATERIALCOMMON_EXPORT Q_DECL_EXPORT
#else
#define MATERIALCOMMON_EXPORT Q_DECL_IMPORT
#endif

#define MATERIAL_DISABLE_THEME_COLORS \
    if (d->useThemeColors == true) { d->useThemeColors = false; }

class QtMaterialTheme;

class MATERIALCOMMON_EXPORT QtMaterialStyle : public QCommonStyle
{
    Q_OBJECT

public:
    inline static QtMaterialStyle &instance();
    virtual ~QtMaterialStyle() override;

    void setTheme(QtMaterialTheme *theme);
    QColor themeColor(const QString &key) const;

protected:
    const QScopedPointer<QtMaterialStylePrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(QtMaterialStyle)

    QtMaterialStyle();

    QtMaterialStyle(QtMaterialStyle const &);
    void operator=(QtMaterialStyle const &);
};

inline QtMaterialStyle::~QtMaterialStyle() {
}

inline QtMaterialStyle &QtMaterialStyle::instance()
{
    static QtMaterialStyle instance;
    return instance;
}

#endif // QTMATERIALSTYLE_H
