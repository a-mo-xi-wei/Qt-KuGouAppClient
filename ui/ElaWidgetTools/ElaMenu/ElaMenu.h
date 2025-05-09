#ifndef ELAMENU_H
#define ELAMENU_H

#include <QMenu>
#include <QWidget>

#include "Def.h"
#include "stdafx.h"

class ElaMenuPrivate;

class ELA_EXPORT ElaMenu : public QMenu {
    Q_OBJECT
    Q_Q_CREATE(ElaMenu)

public:
    explicit ElaMenu(QWidget *parent = nullptr);

    explicit ElaMenu(const QString &title, QWidget *parent = nullptr);

    ~ElaMenu();

    void setOpacity(qreal opacity);

    void setPreventHide(bool prevent);

    void setMenuItemHeight(int menuItemHeight);

    int getMenuItemHeight() const;

    QAction *addMenu(QMenu *menu);

    ElaMenu *addMenu(const QString &title);

    ElaMenu *addMenu(const QIcon &icon, const QString &title);

    ElaMenu *addMenu(ElaIconType::IconName icon, const QString &title);

    QAction *addElaIconAction(ElaIconType::IconName icon, const QString &text);

    QAction *addElaIconAction(ElaIconType::IconName icon, const QString &text, const QKeySequence &shortcut);

    bool isHasChildMenu() const;

    bool isHasIcon() const;

Q_SIGNALS:
    Q_SIGNAL void menuShow();

protected:
    void showEvent(QShowEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;
};

#endif // ELAMENU_H
