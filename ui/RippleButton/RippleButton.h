#ifndef RIPPLEBUTTON_H
#define RIPPLEBUTTON_H

#include <QWidget>

#include<QToolButton>
#include<QGraphicsDropShadowEffect>

#if defined(RIPPLEBUTTON_LIBRARY)
#define RIPPLEBUTTON_EXPORT Q_DECL_EXPORT
#else
#define RIPPLEBUTTON_EXPORT Q_DECL_IMPORT
#endif

class QPointF;
class QTimer;
class QIcon;

class RIPPLEBUTTON_EXPORT RippleButton : public QToolButton {
public:
    explicit RippleButton(QWidget *parent = Q_NULLPTR); // 构造函数

    void setSpeed(const int &timeInterval) const; // 设置定时器时间间隔，控制填充速度

    void setFillColor(const QColor &fillcolor); // 设置填充颜色

    void setRadius(int radius_); // 设置圆角半径

    void setMyIcon(const QIcon &ico); //设置按钮图标

protected:
    void enterEvent(QEnterEvent *event) override; // 重写鼠标进入事件

    void leaveEvent(QEvent *) override; // 重写鼠标离开事件

    void paintEvent(QPaintEvent *event) override; // 重写绘图事件

    void resizeEvent(QResizeEvent *event) override; // 重写改变大小事件

private:
    std::unique_ptr<QGraphicsDropShadowEffect> m_effect{};
    int timeInterval = 10; // 定时器时间间隔，单位：ms
    QPointF mouse_point; // 记录鼠标进入和离开时的坐标
    QTimer *timer = Q_NULLPTR; // 定时器对象
    QTimer *checkTimer = Q_NULLPTR; // 解决不触发leaveEvent的Bug
    int max_radius; // 最大半径
    int radius = 0; // 绘制半径
    int radius_var = 1; // 半径每次改变的值（增大或减小）
    QColor fill_color; // 填充颜色
    int frame_radius = 0; // 绘制路径的圆角半径
    QIcon m_ico; //按钮图标
};

#endif // RIPPLEBUTTON_H
