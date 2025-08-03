#ifndef DYNAMICBACKGROUNDGRADIENT_H
#define DYNAMICBACKGROUNDGRADIENT_H

#include "dynamicbackgroundinterface.h"

#if defined(DYNAMICBACKGROUND_LIBRARY)
#define DYNAMICBACKGROUND_EXPORT Q_DECL_EXPORT
#else
#define DYNAMICBACKGROUND_EXPORT Q_DECL_IMPORT
#endif

class DYNAMICBACKGROUND_EXPORT DynamicBackgroundGradient : public DynamicBackgroundInterface
{
public:
    DynamicBackgroundGradient(QWidget* parent,
        QColor color1 = QColor(0x93D2FB),
        QColor color2 = QColor(0xD9F8FF),
        QColor color3 = QColor(0xFFF7FF),
        QColor color4 = QColor(0xFFE7FF)
    );

    void setColor(QColor color1, QColor color2, QColor color3, QColor color4) override;
    void setColor2(QColor color1, QColor color2) override;
    void draw(QPainter& painter) override;
    void draw(QPainter& painter, const QPainterPath &path) override;
    void setHorizone(bool h) override;
    void accumulate(int x = 1) override;

protected:
    void timeout();

private:
    // 颜色属性
    ColorArray c1, c2, c3, c4, c5, c6, cu, cd, cm;
    ColorArray du, dd, dm;
    ColorDirects au, ad, am;

    // 方向
    bool horizone;

    // 中间颜色
    bool use_mid;
    double prop;
	
    QColor draw_coloru, draw_colord, draw_colorm;
};

#endif // DYNAMICBACKGROUNDGRADIENT_H
