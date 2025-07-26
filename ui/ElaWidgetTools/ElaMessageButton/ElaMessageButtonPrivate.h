/**
* @file ElaMessageButtonPrivate.h
 * @brief 定义 ElaMessageButtonPrivate 类，管理消息按钮的私有实现
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAMESSAGEBUTTONPRIVATE_H
#define ELAMESSAGEBUTTONPRIVATE_H

#include "Def.h"

class ElaMessageButton;

/**
 * @class ElaMessageButtonPrivate
 * @brief 消息按钮私有实现类，管理按钮状态和主题
 */
class ElaMessageButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaMessageButton)
    Q_PROPERTY_CREATE_D(int,                              BorderRadius)
    Q_PROPERTY_CREATE_D(QString,                         BarTitle)
    Q_PROPERTY_CREATE_D(QString,                         BarText)
    Q_PROPERTY_CREATE_D(int,                             DisplayMsec)
    Q_PROPERTY_CREATE_D(QWidget*,                        MessageTargetWidget)
    Q_PROPERTY_CREATE_D(ElaMessageBarType::MessageMode,  MessageMode)
    Q_PROPERTY_CREATE_D(ElaMessageBarType::PositionPolicy, PositionPolicy)

public:
    /**
     * @brief 构造函数，初始化消息按钮私有对象
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaMessageButtonPrivate(QObject* parent = nullptr);

    /**
     * @brief 析构函数，释放消息按钮私有资源
     */
    ~ElaMessageButtonPrivate() override;

private:
    /**
     * @brief 左鼠标按下状态
     */
    bool                     _isLeftButtonPress{false};

    /**
     * @brief 阴影边框宽度
     */
    int                      _shadowBorderWidth{3};

    /**
     * @brief 笔刷边框宽度
     */
    int                      _penBorderWidth{1};

    /**
     * @brief 当前主题模式
     */
    ElaThemeType::ThemeMode  _themeMode;
};

#endif // ELAMESSAGEBUTTONPRIVATE_H