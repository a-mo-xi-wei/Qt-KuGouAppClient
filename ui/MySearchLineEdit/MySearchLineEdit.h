//
// Created by WeiWang on 24-10-10.
//

#ifndef MYSEARCHLINEEDIT_H
#define MYSEARCHLINEEDIT_H

#include <QLineEdit>

#if defined(MYSEARCHLINEEDIT_LIBRARY)
#define MYSEARCHLINEEDIT_EXPORT Q_DECL_EXPORT
#else
#define MYSEARCHLINEEDIT_EXPORT Q_DECL_IMPORT
#endif

class QPropertyAnimation;

class MYSEARCHLINEEDIT_EXPORT MySearchLineEdit : public QLineEdit {
    Q_OBJECT

public:
    explicit MySearchLineEdit(QWidget *parent = nullptr);

    void setMaxWidth(const int &width);

protected:
    void focusInEvent(QFocusEvent *event) override;

    void focusOutEvent(QFocusEvent *event) override;

    bool eventFilter(QObject *watched, QEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    QPropertyAnimation *m_animation;
    int m_originalWidth;
    int m_maxWidth;
};


#endif //MYSEARCHLINEEDIT_H
