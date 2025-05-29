/**
* @file ElaToggleSwitch.h
 * @brief 定义 ElaToggleSwitch 类，开关控件
 * @author [Liniyous]
 * @date 2024-12-15
 * @version 1.0
 */

#ifndef ELATOGGLESWITCH_H
#define ELATOGGLESWITCH_H

#include <QWidget>

#include "stdafx.h"

class ElaToggleSwitchPrivate;

/**
 * @class ElaToggleSwitch
 * @brief 开关控件，支持滑动动画和主题切换
 */
class ELA_EXPORT ElaToggleSwitch : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(ElaToggleSwitch); ///< 声明私有类指针

public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaToggleSwitch(QWidget *parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~ElaToggleSwitch() override;

    /**
     * @brief 设置开关状态
     * @param isToggled 是否开启
     */
    void setIsToggled(bool isToggled);

    /**
     * @brief 获取开关状态
     * @return 当前开关状态
     */
    bool getIsToggled() const;

    Q_SIGNALS:
        /**
         * @brief 开关状态变化信号
         * @param checked 是否开启
         */
        Q_SIGNAL void toggled(bool checked);

protected:
    /**
     * @brief 事件处理
     * @param event 事件
     * @return 是否处理事件
     * @note 处理鼠标进入和离开事件
     */
    bool event(QEvent *event) override;

    /**
     * @brief 鼠标按下事件
     * @param event 鼠标事件
     * @note 记录按下位置并启动半径动画
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标释放事件
     * @param event 鼠标事件
     * @note 根据位置启动位置动画
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标移动事件
     * @param event 鼠标事件
     * @note 更新圆心位置
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief 绘制事件
     * @param event 绘制事件
     * @note 绘制背景和滑块
     */
    void paintEvent(QPaintEvent *event) override;
};

#endif // ELATOGGLESWITCH_H