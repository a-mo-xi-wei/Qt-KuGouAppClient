/**
 * @file ElaMessageBar.h
 * @brief 定义 ElaMessageBar 类，提供消息通知栏功能
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAMESSAGEBAR_H
#define ELAMESSAGEBAR_H

#include <QWidget>

#include "Def.h"

class ElaMessageBarPrivate;

/**
 * @class ElaMessageBar
 * @brief 消息通知栏类，支持多种消息类型和位置策略
 */
class ELA_EXPORT ElaMessageBar : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaMessageBar)

public:
    /**
     * @brief 显示成功消息
     * @param policy 位置策略
     * @param title 消息标题
     * @param text 消息内容
     * @param displayMsec 显示时长（毫秒）
     * @param parent 父控件指针，默认为 nullptr
     */
    static void success(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent = nullptr);

    /**
     * @brief 显示警告消息
     * @param policy 位置策略
     * @param title 消息标题
     * @param text 消息内容
     * @param displayMsec 显示时长（毫秒）
     * @param parent 父控件指针，默认为 nullptr
     */
    static void warning(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent = nullptr);

    /**
     * @brief 显示信息消息
     * @param policy 位置策略
     * @param title 消息标题
     * @param text 消息内容
     * @param displayMsec 显示时长（毫秒）
     * @param parent 父控件指针，默认为 nullptr
     */
    static void information(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent = nullptr);

    /**
     * @brief 显示错误消息
     * @param policy 位置策略
     * @param title 消息标题
     * @param text 消息内容
     * @param displayMsec 显示时长（毫秒）
     * @param parent 父控件指针，默认为 nullptr
     */
    static void error(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent = nullptr);

protected:
    /**
     * @brief 绘制事件，绘制消息栏外观
     * @param event 绘图事件
     */
    void paintEvent(QPaintEvent* event) override;

    /**
     * @brief 事件过滤器，处理父控件事件
     * @param watched 监控对象
     * @param event 事件对象
     * @return 是否处理事件
     */
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    friend class ElaMessageBarManager;

    /**
     * @brief 构造函数，初始化消息栏
     * @param policy 位置策略
     * @param messageMode 消息类型
     * @param title 消息标题
     * @param text 消息内容
     * @param displayMsec 显示时长（毫秒）
     * @param parent 父控件指针，默认为 nullptr
     */
    explicit ElaMessageBar(ElaMessageBarType::PositionPolicy policy, ElaMessageBarType::MessageMode messageMode, QString& title, QString& text, int displayMsec, QWidget* parent = nullptr);

    /**
     * @brief 析构函数，释放消息栏资源
     */
    ~ElaMessageBar();
};

#endif // ELAMESSAGEBAR_H