/**
 * @file CheckBox1.cpp
 * @brief 实现 CheckBox1 类，提供带动画的复选框功能
 * @author [iwxyi]
 * @date 2025-05-15
 * @version 1.0
 */

#include "CheckBox1.h"
#include <QPainter>
#include <QPainterPath>

/**
 * @brief 构造函数，初始化复选框
 * @param parent 父控件指针，默认为 nullptr
 */
CheckBox1::CheckBox1(QWidget *parent) : AniCheckBox(parent)
{
}

/**
 * @brief 设置未选中状态的颜色
 * @param c 颜色值
 */
void CheckBox1::setUncheckedColor(QColor c)
{
    this->uncheckedColor = c;
    update();
}

/**
 * @brief 鼠标进入事件，触发悬浮动画
 * @param e 进入事件
 */
void CheckBox1::enterEvent(QEnterEvent *e)
{
    QCheckBox::enterEvent(e);
    startAnimation("hover_prog", getHoverProg(), 1, 300, QEasingCurve::OutBack);
}

/**
 * @brief 鼠标离开事件，触发离开动画
 * @param e 事件
 */
void CheckBox1::leaveEvent(QEvent *e)
{
    QCheckBox::leaveEvent(e);
    startAnimation("hover_prog", getHoverProg(), 0, 300, QEasingCurve::OutBack);
}

/**
 * @brief 复选框状态变化处理
 * @param state 复选框状态
 */
void CheckBox1::checkStateChanged(const int& state)
{
    if (state == Qt::Unchecked) {
        startAnimation("check_prog", getCheckProg(), 0, 1000, QEasingCurve::OutQuad);
    } else if (state == Qt::PartiallyChecked) {
        // 部分选中状态暂未实现
    } else if (state == Qt::Checked) {
        startAnimation("check_prog", getCheckProg(), 1, 1000, QEasingCurve::Linear);
    }
}

/**
 * @brief 绘制复选框
 * @param painter 绘图对象
 * @param rect 绘制区域
 */
void CheckBox1::drawBox(QPainter &painter, QRectF rect)
{
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(uncheckedColor, qMax(rect.width() / 16, 1.0), Qt::SolidLine, Qt::RoundCap));

    // 绘制边缘方框，与悬浮状态相关
    QPainterPath borderPath;
    double radius = rect.width() / 8;
    double l = rect.left(), t = rect.top(), r = rect.right(), b = rect.bottom(),
           w = rect.width(), h = rect.height();

    // arcTo:从0度开始，逆时针扫，参数为角度制
    double quadDishu = 5;
    borderPath.moveTo(l, t + radius);
    borderPath.arcTo(QRectF(l, t, radius * 2, radius * 2), 180, -90);
    borderPath.quadTo(QPointF(l + w / 2, t + h / quadDishu * hoverProg), QPointF(r - radius, t));
    // borderPath.lineTo(r - radius, t);
    borderPath.arcTo(QRectF(r - radius * 2, t, radius * 2, radius * 2), 90, -90);
    borderPath.quadTo(QPointF(r - w / quadDishu * hoverProg, t + h / 2), QPointF(r, b - radius));
    borderPath.arcTo(QRectF(r - radius * 2, b - radius * 2, radius * 2, radius * 2), 0, -90);
    borderPath.quadTo(QPointF(r - w / 2, b - h / quadDishu * hoverProg), QPointF(l + radius, b));
    borderPath.arcTo(QRectF(l, b - radius * 2, radius * 2, radius * 2), -90, -90);
    borderPath.quadTo(QPointF(l + w / quadDishu * hoverProg, b - h / 2), QPointF(l, t + radius));
    painter.drawPath(borderPath);

    // 绘制选中状态
    painter.setPen(QPen(foreColor, qMax(rect.width() / 20, 1.0), Qt::SolidLine, Qt::RoundCap));
    QPainterPath circlePath;
    circlePath.moveTo(l, t);
    const double outRad = sqrt(w * w + h * h) / 2; // 外接圆半径
    const double threshold = 0.6; // 到什么时刻转完一圈

    // 边框转圈动画
    double borderVal = checkProg;
    if (borderVal > threshold)
        borderVal = threshold;
    borderVal = borderVal / threshold; // 边缘的完成度的比例
    if (borderVal >= 1)
        borderVal = 1;
    circlePath.arcTo(QRectF(l + w / 2 - outRad, t + h / 2 - outRad, outRad * 2, outRad * 2), 135, 360 * borderVal);
    painter.save();
    painter.setClipPath(circlePath);
    painter.drawPath(borderPath);
    painter.restore();

    // 绘制勾号
    if (checkProg > threshold) {
        double gou = checkProg - threshold;
        const double threshold = 0.13;
        const double restThreshold = 0.27;
        // QPointF point0(l, t + radius); // 从边缘连接到√
        const QPointF point1(l + w * 0.30, t + h * 0.45);
        const QPointF point2(l + w * 0.45, t + h * 0.70);
        const QPointF point3(l + w * 0.70, t + h * 0.32);

        // 绘制勾号第一段
        double prop = gou;
        if (prop >= threshold)
            prop = threshold;
        prop = prop / threshold; // 在当前段的比例(偏中心）
        QPainterPath path;
        path.moveTo(point1);
        path.lineTo(point2);
        painter.drawLine(QLineF(point1, path.pointAtPercent(prop)));

        // 绘制勾号第二段
        if (gou > threshold) {
            gou -= threshold;
            double prop = gou / restThreshold; // 在当前段的比例
            QPainterPath path2;
            path2.moveTo(point2);
            path2.lineTo(point3);
            painter.drawLine(QLineF(point2, path2.pointAtPercent(prop)));
        }
    }
}