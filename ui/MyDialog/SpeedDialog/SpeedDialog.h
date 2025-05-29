//
// Created by WeiWang on 25-5-27.
//

#ifndef SPEEDDIALOG_H
#define SPEEDDIALOG_H

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYDIALOG_LIBRARY)
#define MYDIALOG_EXPORT Q_DECL_EXPORT
#else
#define MYDIALOG_EXPORT Q_DECL_IMPORT
#endif


#include "QtMaterialSlider/qtmaterialslider.h"

class SnapSlider : public QtMaterialSlider {
    Q_OBJECT
public:
    using QtMaterialSlider::QtMaterialSlider;

private:
    void snapToPosition() {
        // 计算最近的十分之一位置
        int value = this->value();
        int min = this->minimum();
        int max = this->maximum();

        // 计算步长
        double step = (max - min) / 10.0;

        // 找到最近的十分之一点
        int closestPoint = min;
        double minDistance = std::numeric_limits<double>::max();

        for (int i = 0; i <= 10; i++) {
            int point = min + qRound(i * step);
            double distance = std::abs(value - point);

            if (distance < minDistance) {
                minDistance = distance;
                closestPoint = point;
            }
        }
        if (closestPoint == minimum())closestPoint = minimum() + 1;
        // 直接设置到最近的点（跳过中间值）
        setSliderPosition(closestPoint);
        emit numChanged(closestPoint / 10);
    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        // 先调用基类处理
        QtMaterialSlider::mousePressEvent(event);
        m_isPress = true;
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        // 先调用基类处理（更新位置）
        QtMaterialSlider::mouseMoveEvent(event);
        if (!m_isPress)return;
        // 立即跳转到最近的十分之一点
        snapToPosition();
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        // 先调用基类处理
        QtMaterialSlider::mouseReleaseEvent(event);
        m_isPress = false;
        // 立即跳转到最近的十分之一点
        snapToPosition();
    }
    void showEvent(QShowEvent* event) override {
        QWidget::showEvent(event);
        setValue(maximum() / 2);
    }

signals:
    void numChanged(const int& num);

private:
    bool m_isPress = false;
};

class QLabel;
class QPushButton;
class QGraphicsDropShadowEffect;

class MYDIALOG_EXPORT SpeedDialog : public QWidget {
    Q_OBJECT

public:
    explicit SpeedDialog(QWidget *parent = nullptr);

    ~SpeedDialog() override;

private:
    void initUi();

protected:
    void paintEvent(QPaintEvent *ev) override;

    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void btnTextChanged(const QString &text);

private:
    std::unique_ptr<QGraphicsDropShadowEffect> m_effect; ///< 阴影效果
    QPushButton *m_lastBtn;
    QString m_preText           = "倍速";
    QString m_adjustmentText    = "";
    QString m_speedText         = "";
};


#endif //SPEEDDIALOG_H
