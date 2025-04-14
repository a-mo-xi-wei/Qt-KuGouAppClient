//
// Created by WeiWang on 25-2-6.
//

#ifndef WATERZOOMBUTTON_H
#define WATERZOOMBUTTON_H

#if defined(MYBUTTON_LIBRARY)
#define MYBUTTON_EXPORT Q_DECL_EXPORT
#else
#define MYBUTTON_EXPORT Q_DECL_IMPORT
#endif

#include "InteractiveButtonBase.h"

class MYBUTTON_EXPORT WaterZoomButton : public InteractiveButtonBase {
    Q_OBJECT

public:
    explicit WaterZoomButton(QWidget *parent = nullptr, const QString& text = "");

    void setChoking(const int& c);

    void setChokingProp(const double& p);

    void setRadiusZoom(const int& radius);

    void setRadius(const int& x, const int& x2);

protected:
    QPainterPath getBgPainterPath() override;

    void resizeEvent(QResizeEvent *event) override;

    int choking; // 向里缩的长度
    double choking_prop;
    int radius_zoom; // 收缩时的半径
};


#endif //WATERZOOMBUTTON_H
