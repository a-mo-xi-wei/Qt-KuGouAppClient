#ifndef SMASKWIDGET_H
#define SMASKWIDGET_H

#include <QWidget>

#if defined(MYMASKWIDGET_LIBRARY)
#define MYMASKWIDGET_EXPORT Q_DECL_EXPORT
#else
#define MYMASKWIDGET_EXPORT Q_DECL_IMPORT
#endif

class QParallelAnimationGroup;
class QPropertyAnimation;

class MYMASKWIDGET_EXPORT SMaskWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int animatedY READ animatedY WRITE setAnimatedY)
    Q_PROPERTY(int alpha READ alpha WRITE setAlpha)

public:
    explicit SMaskWidget(QWidget *parent = nullptr);

    void setDefaultFillCircleColor(const QColor &color);

    void setDefaultFillTriangleColor(const QColor &color);

    void setHoverFillCircleColor(const QColor &color);

    void setHoverFillTriangleColor(const QColor &color);

    void setEnterWidgetChangeCursor(const bool &change);

    void setBorderRadius(const int &radius);

    void setMaskColor(const QColor &color);

    void setStander(const int &stander);

    void setMove(const bool& move);

    bool getMove();

    int animatedY() const { return m_animatedY; }
    int alpha() const { return m_alpha; }

    void animationUp();

    void animationDown();

public slots:
    void setAnimatedY(int y);
    void setAlpha(int alpha);

private:
    void calOnce();

    bool isMouseInCircle(const float &mouseX, const float &mouseY) const;

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

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
    //填充颜色相关
    QColor m_defaultFillCircleColor = QColor();
    QColor m_hoverFillCircleColor = Qt::white;
    QColor m_defaultFillTriangleColor = Qt::white;
    QColor m_hoverFillTriangleColor = QColor();
    //遮罩颜色
    QColor m_maskColor = QColor(0, 0, 0, 160);
    //鼠标一进入widget就变指向样式
    bool m_isEnterWidgetChangeCursor = true;
    bool m_isEnterCircle = false;
    //border-radius
    int m_borderRadius = 8;
    //基准
    int m_stander = 0;
    //动画相关
    int m_animatedY;
    int m_alpha = 0;

    QPropertyAnimation* m_posAnimation;
    QPropertyAnimation* m_alphaAnimation;
    QParallelAnimationGroup* m_aniGroup{};

    bool m_isMove = false; // 控制动画是否启用的开关

};

#endif // SMASKWIDGET_H
