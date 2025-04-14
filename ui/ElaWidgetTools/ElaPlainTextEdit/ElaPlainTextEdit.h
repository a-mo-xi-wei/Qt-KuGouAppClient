#ifndef ELAPLAINTEXTEDIT_H
#define ELAPLAINTEXTEDIT_H

#include <QPlainTextEdit>

#include "stdafx.h"

class ElaPlainTextEditPrivate;

class ELA_EXPORT ElaPlainTextEdit : public QPlainTextEdit {
    Q_OBJECT
    Q_Q_CREATE(ElaPlainTextEdit)

public:
    explicit ElaPlainTextEdit(QWidget *parent = nullptr);

    explicit ElaPlainTextEdit(const QString &text, QWidget *parent = nullptr);

    ~ElaPlainTextEdit() override;

protected:
    void focusInEvent(QFocusEvent *event) override;

    void focusOutEvent(QFocusEvent *event) override;

    void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // ELAPLAINTEXTEDIT_H
