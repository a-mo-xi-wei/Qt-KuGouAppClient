#ifndef SMASKWIDGET_H
#define SMASKWIDGET_H

#include <QWidget>
#include<QPainter>
#include<memory>

class SMaskWidget : public QWidget {
    Q_OBJECT

public:
    explicit SMaskWidget(QWidget *parent = nullptr);

    void setDefaultFillCircleColor(const QColor &color);

    void setDefaultFillTriangleColor(const QColor &color);

    void setHoverFillCircleColor(const QColor &color);

    void setHoverFillTriangleColor(const QColor &color);

    void setEnterWidgetChangeCursor(const bool& change);

private:
    void calOnce();

    bool isMouseInCircle(const float &mouseX, const float &mouseY);

protected:
    void paintEvent(QPaintEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void leaveEvent(QEvent *event) override;

    void enterEvent(QEnterEvent *event) override;

    void resizeEvent(QResizeEvent* event) override;

private:
    float m_w;
    float m_h;
    float m_radius;
    float m_centerX;
    float m_centerY;
    //三角形三个点
    QPointF m_ap;
    QPointF m_bp;
    QPointF m_cp;
    //颜色相关
    QColor m_defaultFillCircleColor = QColor();
    QColor m_hoverFillCircleColor = Qt::white;
    QColor m_defaultFillTriangleColor = Qt::white;
    QColor m_hoverFillTriangleColor = QColor();
    //鼠标一进入widget就变指向样式
    bool m_isEnterWidgetChangeCursor = true;
    bool m_isEnterCircle = false;

};

#endif // SMASKWIDGET_H
