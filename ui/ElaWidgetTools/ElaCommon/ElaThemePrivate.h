#ifndef ELATHEMEPRIVATE_H
#define ELATHEMEPRIVATE_H

#include <QColor>
#include <QMap>

#include "Def.h"

class ElaTheme;

class ElaThemePrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(ElaTheme)

public:
    explicit ElaThemePrivate(QObject *parent = nullptr);

    ~ElaThemePrivate() override;

private:
    ElaThemeType::ThemeMode _themeMode{ElaThemeType::Light};
    QColor _lightThemeColorList[40];
    QColor _darkThemeColorList[40];

    void _initThemeColor();
};

#endif // ELATHEMEPRIVATE_H
