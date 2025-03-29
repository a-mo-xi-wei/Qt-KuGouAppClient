#ifndef CHECKBOX1_H
#define CHECKBOX1_H
/*
 *  此处参考开源项目 ：iwxyi 大神的 [Qt控件] 带动画的 CheckBox
 *  项目地址为 ： https://github.com/iwxyi/Qt-AniCheckBox
 *  非常实用，有兴趣的可以star
 */

#if defined(CHECKBOX1_LIBRARY)
#define CHECKBOX1_EXPORT Q_DECL_EXPORT
#else
#define CHECKBOX1_EXPORT Q_DECL_IMPORT
#endif

#include "AniCheckBox.h"

class CHECKBOX1_EXPORT CheckBox1 : public AniCheckBox
{
public:

    CheckBox1(QWidget* parent = nullptr);

    void setUncheckedColor(QColor c);

protected:
    void enterEvent(QEnterEvent *e) override;

    void leaveEvent(QEvent *e) override;

    void checkStateChanged(const int& state) override;

    void drawBox(QPainter &painter, QRectF rect) override;

protected:
    QColor uncheckedColor = QColor(0x88, 0x88, 0x88, 0x88);
};

#endif // CHECKBOX1_H
