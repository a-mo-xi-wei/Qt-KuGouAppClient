#ifndef QTMATERIALSTYLE_P_H
#define QTMATERIALSTYLE_P_H

#include <QtGlobal>

class QtMaterialStyle;
class QtMaterialTheme;

#if defined(MATERIALCOMMON_LIBRARY)
#define MATERIALCOMMON_EXPORT Q_DECL_EXPORT
#else
#define MATERIALCOMMON_EXPORT Q_DECL_IMPORT
#endif

class MATERIALCOMMON_EXPORT QtMaterialStylePrivate
{
    Q_DISABLE_COPY(QtMaterialStylePrivate)
    Q_DECLARE_PUBLIC(QtMaterialStyle)

public:
    QtMaterialStylePrivate(QtMaterialStyle *q);
    ~QtMaterialStylePrivate();

    void init();

    QtMaterialStyle *const q_ptr;
    QtMaterialTheme *theme;
};

#endif // QTMATERIALSTYLE_P_H
