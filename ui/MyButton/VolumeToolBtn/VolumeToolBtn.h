/**
 * @file VolumeToolBtn.h
 * @brief 定义 VolumeToolBtn 类，提供音量调节按钮功能
 * @author [WeiWang]
 * @date 2025-05-15
 * @version 1.0
 */

#ifndef VOLUMETOOLBTN_H
#define VOLUMETOOLBTN_H

#include "./SliderWidget/SliderWidget.h"
#include <QToolButton>
#include <QLabel>

/** @brief 动态库导出宏，定义库的导出/导入行为 */
#if defined(MYBUTTON_LIBRARY)
#define MYBUTTON_EXPORT Q_DECL_EXPORT
#else
#define MYBUTTON_EXPORT Q_DECL_IMPORT
#endif

class QTimer;
class QVBoxLayout;

/**
 * @class VolumeToolBtn
 * @brief 音量调节按钮类，继承自 QToolButton，支持音量滑块和静音功能
 */
class MYBUTTON_EXPORT VolumeToolBtn : public QToolButton {
    Q_OBJECT

public:
    /**
     * @brief 构造函数，初始化音量调节按钮
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit VolumeToolBtn(QWidget *parent = nullptr);

    /**
     * @brief 初始化音量调节控件
     */
    void initVolumeWidget();

    /**
     * @brief 初始化按钮界面
     */
    void initUi();

    /**
     * @brief 检查鼠标位置，控制音量控件显示
     */
    void checkMousePosition() const;

    /**
     * @brief 获取音量控件位置
     */
    void getVolumeWidgetPosition();

    /**
     * @brief 获取当前音量值
     * @return 音量值
     */
    int getVolumeValue() const;

    /**
     * @brief 更新按钮图标
     * @param isHovered 是否悬浮，默认为 false
     */
    void updateIcon(bool isHovered = false);

    /**
     * @brief 设置音量值
     * @param value 音量值
     */
    void setVolume(const int& value) const;

protected:
    /**
     * @brief 鼠标进入事件，显示音量控件
     * @param event 进入事件
     */
    void enterEvent(QEnterEvent *event) override;

    /**
     * @brief 鼠标离开事件，延迟隐藏音量控件
     * @param event 事件
     */
    void leaveEvent(QEvent *event) override;

    /**
     * @brief 控件显示事件，更新音量控件位置
     * @param event 显示事件
     */
    void showEvent(QShowEvent *event) override;

    /**
     * @brief 事件过滤器，处理主窗口事件
     * @param watched 监听对象
     * @param event 事件
     * @return 是否处理事件
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

public slots:
    /**
     * @brief 处理静音状态变化
     * @param flag 是否静音
     */
    void onNoVolume(bool flag);

signals:
    /**
     * @brief 音量值变化信号
     * @param value 新音量值
     */
    void volumeChange(int value);

private:
    bool                    m_isNoVolume;             ///< 是否静音
    std::unique_ptr<MyWidget>       m_volumeWidget;   ///< 音量调节控件
    std::unique_ptr<QLabel>         m_volumeLab;      ///< 音量值标签
    std::unique_ptr<SliderWidget>   m_volumeSlider;   ///< 音量滑块
    QVBoxLayout*            m_vBoxLayout;             ///< 音量控件布局
    QPoint                  m_volumePosition;         ///< 音量控件位置
    QWidget*                m_volumeParent{};         ///< 音量控件父窗口
    QTimer*                 m_leaveTimer{};           ///< 鼠标离开延迟定时器
    QTimer*                 m_positionCheckTimer{};   ///< 鼠标位置检查定时器
};

#endif // VOLUMETOOLBTN_H