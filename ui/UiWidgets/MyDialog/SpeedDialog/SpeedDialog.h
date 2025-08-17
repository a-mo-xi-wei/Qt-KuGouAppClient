/**
 * @file SpeedDialog.h
 * @brief 定义 SpeedDialog 和 SnapSlider 类，用于倍速和调音控制弹窗
 * @author WeiWang
 * @date 2025-05-27
 * @version 1.0
 */

#ifndef SPEEDDIALOG_H
#define SPEEDDIALOG_H

/**
 * @def UIWIDGETS_EXPORT
 * @brief 动态库导出宏，定义库的导出/导入行为
 */
#if defined(UIWIDGETS_LIBRARY)
#define UIWIDGETS_EXPORT Q_DECL_EXPORT
#else
#define UIWIDGETS_EXPORT Q_DECL_IMPORT
#endif

#include <QMouseEvent>

#include "QtMaterialSlider/qtmaterialslider.h"
#include "SpeedDialogState.h"

class DynamicBackgroundInterface;
class ElaToggleSwitch;
class QLabel;
class QPushButton;
class QGraphicsDropShadowEffect;

/**
 * @class SnapSlider
 * @brief 继承 QtMaterialSlider，支持吸附到十分之一位置的滑块
 */
class SnapSlider : public QtMaterialSlider
{
    Q_OBJECT

public:
    using QtMaterialSlider::QtMaterialSlider;
    /**
     * @brief 构造函数
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit SnapSlider(QWidget *parent)
        : QtMaterialSlider(parent)
    {
        setCursor(Qt::OpenHandCursor); // 默认悬停光标
        setMouseTracking(true);
    }

    /**
     * @brief 吸附到最近的十分之一位置
     * @note 计算最近点并触发 numChanged 信号
     */
    void snapToPosition()
    {
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
        if (closestPoint == minimum())
            closestPoint = minimum() + 1;
        // 直接设置到最近的点（跳过中间值）
        setSliderPosition(closestPoint);
        emit numChanged(closestPoint / 10);
    }

protected:
    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     * @note 标记按下状态并调用基类处理
     */
    void mousePressEvent(QMouseEvent *event) override
    {
        // 先调用基类处理
        QtMaterialSlider::mousePressEvent(event);
        if (event->button() == Qt::LeftButton) {
            setCursor(Qt::ClosedHandCursor); // 按下时变为闭合手型
        }
        m_isPress = true;
    }

    /**
     * @brief 鼠标移动事件
     * @param event 鼠标事件
     * @note 更新位置并吸附到十分之一点
     */
    void mouseMoveEvent(QMouseEvent *event) override
    {
        // 先调用基类处理（更新位置）
        QtMaterialSlider::mouseMoveEvent(event);
        if (event->buttons() & Qt::LeftButton) {
            // 拖拽时保持闭合手型
            setCursor(Qt::ClosedHandCursor);
        }
        if (!m_isPress)
            return;
        // 立即跳转到最近的十分之一点
        snapToPosition();
    }

    /**
     * @brief 鼠标释放事件
     * @param event 鼠标事件
     * @note 清除按下状态并吸附到十分之一点
     */
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        // 先调用基类处理
        QtMaterialSlider::mouseReleaseEvent(event);
        setCursor(Qt::OpenHandCursor); // 释放时恢复打开手型
        m_isPress = false;
        // 立即跳转到最近的十分之一点
        snapToPosition();
    }

    /**
     * @brief 显示事件
     * @param event 显示事件
     * @note 设置滑块初始值为中间位置
     */
    void showEvent(QShowEvent *event) override
    {
        QWidget::showEvent(event);
        setValue(maximum() / 2);
        // 立即跳转到最近的十分之一点
        snapToPosition();
    }

signals:
    /**
     * @brief 滑块值变化信号
     * @param num 吸附后的值（十分之一）
     */
    void numChanged(const int &num);

private:
    bool m_isPress = false; ///< 鼠标是否按下
};

/**
 * @class SpeedDialog
 * @brief 倍速和调音控制弹窗，支持 DJ 模式
 */
class UIWIDGETS_EXPORT SpeedDialog : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit SpeedDialog(QWidget *parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~SpeedDialog() override;

    /**
     * @brief 设置弹窗状态
     * @param state 状态对象
     */
    void setState(const SpeedDialogState &state);

    /**
     * @brief 获取弹窗状态
     * @return 状态对象
     */
    SpeedDialogState getState() const;

private:
    /**
     * @brief 初始化界面
     * @note 设置布局、控件和信号连接
     */
    void initUi();

protected:
    /**
     * @brief 绘制事件
     * @param ev 绘制事件
     * @note 绘制圆角背景和小三角形底部
     */
    void paintEvent(QPaintEvent *ev) override;

    /**
     * @brief 事件过滤
     * @param obj 对象
     * @param event 事件
     * @return 是否处理事件
     * @note 处理点击弹窗外关闭
     */
    bool eventFilter(QObject *obj, QEvent *event) override;

    /**
     * @brief 关闭事件
     * @param event 关闭事件
     * @note 触发 aboutToClose 信号
     */
    void closeEvent(QCloseEvent *event) override;

signals:
    /**
     * @brief 按钮文本变化信号
     * @param text 新的文本
     */
    void btnTextChanged(const QString &text);

    /**
     * @brief 即将关闭信号
     */
    void aboutToClose();

    // void adjustmentChanged(const int& adjustment); ///< 实现不来，我不会

    void speedChanged(const float &speed);

private:
    std::unique_ptr<QGraphicsDropShadowEffect> m_effect; ///< 阴影效果
    QPushButton *m_lastBtn;                              ///< 最后选中的 DJ 按钮
    QPushButton *m_btn1 = nullptr;                       ///< DJ 按钮 - 劲爆
    QPushButton *m_btn2 = nullptr;                       ///< DJ 按钮 - 社会摇
    QPushButton *m_btn3 = nullptr;                       ///< DJ 按钮 - 慢摇
    QPushButton *m_btn4 = nullptr;                       ///< DJ 按钮 - 抖腿
    ElaToggleSwitch *m_switchBtn = nullptr;              ///< DJ 模式开关
    SnapSlider *m_adjustmentSlider = nullptr;            ///< 升降调滑块
    SnapSlider *m_speedSlider = nullptr;                 ///< 倍速滑块
    QString m_preText = "倍速";                            ///< 前缀文本
    QString m_adjustmentText = "";                       ///< 升降调文本
    QString m_speedText = "";                            ///< 倍速文本
    DynamicBackgroundInterface *dm_bg;                   ///< 动态背景
};

#endif //SPEEDDIALOG_H