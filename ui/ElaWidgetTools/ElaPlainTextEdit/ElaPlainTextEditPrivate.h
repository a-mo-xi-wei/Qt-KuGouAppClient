#ifndef ELAPLAINTEXTEDITPRIVATE_H
#define ELAPLAINTEXTEDITPRIVATE_H

#include "Def.h"

class ElaEvent;
class ElaPlainTextEdit;
class ElaPlainTextEditStyle;

class ElaPlainTextEditPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(ElaPlainTextEdit)

public:
    explicit ElaPlainTextEditPrivate(QObject *parent = nullptr);

    ~ElaPlainTextEditPrivate() override;

    Q_INVOKABLE void onWMWindowClickedEvent(QVariantMap data);

private:
    bool containsCursorToItem(QWidget* item);

public slots:
    void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaPlainTextEditStyle *_style{nullptr};
    ElaEvent *_focusEvent{nullptr};

    void _changeTheme();
};

#endif // ELAPLAINTEXTEDITPRIVATE_H
