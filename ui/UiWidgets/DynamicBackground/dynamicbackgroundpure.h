#ifndef DYNAMICBACKGROUNDPURE_H
#define DYNAMICBACKGROUNDPURE_H

#include "dynamicbackgroundinterface.h"

#if defined(UIWIDGETS_LIBRARY)
#define UIWIDGETS_EXPORT Q_DECL_EXPORT
#else
#define UIWIDGETS_EXPORT Q_DECL_IMPORT
#endif

class UIWIDGETS_EXPORT DynamicBackgroundPure : public DynamicBackgroundInterface
{
public:
    DynamicBackgroundPure(QWidget *parent,
    	QColor color1=QColor(255, 250, 240),/*花卉白*/
    	QColor color2=QColor(253, 245, 230)/*舊蕾絲色*/
    );

    void setColor(QColor color1, QColor color2);
    void draw(QPainter& painter);

protected:
    void timeout();

private:
    ColorArray c1, c2, c;
    ColorArray d;
    ColorDirects a;

    QColor draw_color;
};

#endif // DYNAMICBACKGROUNDPURE_H
