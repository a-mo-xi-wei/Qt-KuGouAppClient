/**
 * @file ElaMessageBarPrivate.h
 * @brief 定义 ElaMessageBarPrivate 类和 ElaMessageBarManager 类，管理消息通知栏的私有实现
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAMESSAGEBARPRIVATE_H
#define ELAMESSAGEBARPRIVATE_H

#include <QMap>
#include <QObject>
#include <QVariantMap>

#include "Def.h"
#include "singleton.h"

enum WorkStatus
{
    /**
     * @brief 空闲状态
     */
    Idle = 0x0000,

    /**
     * @brief 创建动画状态
     */
    CreateAnimation = 0x0001,

    /**
     * @brief 其他事件动画状态
     */
    OtherEventAnimation = 0x0002
};

class ElaMessageBar;

/**
 * @class ElaMessageBarManager
 * @brief 消息通知栏管理类，处理消息栏事件和活动状态
 */
class ElaMessageBarManager : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(ElaMessageBarManager)

private:
    /**
     * @brief 构造函数，初始化消息栏管理器
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaMessageBarManager(QObject* parent = nullptr);

    /**
     * @brief 析构函数，释放消息栏管理器资源
     */
    ~ElaMessageBarManager();

public:
    /**
     * @brief 请求消息栏事件
     * @param messageBar 消息栏对象
     */
    void requestMessageBarEvent(ElaMessageBar* messageBar);

    /**
     * @brief 发布消息栏创建事件
     * @param messageBar 消息栏对象
     */
    void postMessageBarCreateEvent(ElaMessageBar* messageBar);

    /**
     * @brief 发布消息栏终止事件
     * @param messageBar 消息栏对象
     */
    void postMessageBarEndEvent(ElaMessageBar* messageBar);

    /**
     * @brief 强制发布消息栏终止事件
     * @param messageBar 消息栏对象
     */
    void forcePostMessageBarEndEvent(ElaMessageBar* messageBar);

    /**
     * @brief 获取消息栏事件数量
     * @param messageBar 消息栏对象
     * @return 事件数量
     */
    int getMessageBarEventCount(ElaMessageBar* messageBar);

    /**
     * @brief 更新活动消息栏映射
     * @param messageBar 消息栏对象
     * @param isActive 是否激活
     */
    void updateActiveMap(ElaMessageBar* messageBar, bool isActive);

private:
    /**
     * @brief 消息栏事件映射
     */
    QMap<ElaMessageBar*, QList<QVariantMap>>  _messageBarEventMap;
};

class ElaIconButton;
class QPainter;

/**
 * @class ElaMessageBarPrivate
 * @brief 消息通知栏私有实现类，管理动画、绘制和位置逻辑
 */
class ElaMessageBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaMessageBar)
    Q_PROPERTY_CREATE(qreal, Opacity)

public:
    /**
     * @brief 构造函数，初始化消息栏私有对象
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaMessageBarPrivate(QObject* parent = nullptr);

    /**
     * @brief 析构函数，释放消息栏私有资源
     */
    ~ElaMessageBarPrivate();

    /**
     * @brief 尝试请求消息栏事件
     */
    void tryToRequestMessageBarEvent();

    /**
     * @brief 获取当前工作模式
     * @return 当前工作状态
     */
    WorkStatus getWorkMode() const;

    /**
     * @brief 处理其他消息栏终止事件
     * @param eventData 事件数据
     */
    Q_INVOKABLE void onOtherMessageBarEnd(QVariantMap eventData);

    /**
     * @brief 终止消息栏
     * @param eventData 事件数据
     */
    Q_INVOKABLE void messageBarEnd(QVariantMap eventData);

    /**
     * @brief 关闭按钮点击事件处理
     */
    Q_SLOT void onCloseButtonClicked();

private:
    friend class ElaMessageBarManager;

    /**
     * @brief 当前主题模式
     */
    ElaThemeType::ThemeMode           _themeMode;

    /**
     * @brief 边框圆角半径
     */
    int                              _borderRadius{6};

    /**
     * @brief 消息标题
     */
    QString                          _title{""};

    /**
     * @brief 消息内容
     */
    QString                          _text{""};

    /**
     * @brief 位置策略
     */
    ElaMessageBarType::PositionPolicy _policy;

    /**
     * @brief 消息类型
     */
    ElaMessageBarType::MessageMode   _messageMode;

    /**
     * @brief 创建时间
     */
    qreal                            _createTime{0};

    /**
     * @brief 左边框到图标中心的距离
     */
    int                              _leftPadding{20};

    /**
     * @brief 图标中心到标题左侧的距离
     */
    int                              _titleLeftSpacing{30};

    /**
     * @brief 标题右侧到正文左侧的距离
     */
    int                              _textLeftSpacing{15};

    /**
     * @brief 关闭按钮左右总边距
     */
    int                              _closeButtonLeftRightMargin{20};

    /**
     * @brief 关闭按钮宽度
     */
    int                              _closeButtonWidth{30};

    /**
     * @brief 消息栏水平边距
     */
    int                              _messageBarHorizontalMargin{20};

    /**
     * @brief 消息栏底部垂直边距
     */
    int                              _messageBarVerticalBottomMargin{20};

    /**
     * @brief 消息栏顶部垂直边距
     */
    int                              _messageBarVerticalTopMargin{50};

    /**
     * @brief 消息栏间距
     */
    int                              _messageBarSpacing{15};

    /**
     * @brief 阴影边框宽度
     */
    int                              _shadowBorderWidth{6};

    /**
     * @brief 创建动画是否完成
     */
    bool                             _isMessageBarCreateAnimationFinished{false};

    /**
     * @brief 是否准备终止
     */
    bool                             _isReadyToEnd{false};

    /**
     * @brief 是否正常显示
     */
    bool                             _isNormalDisplay{false};

    /**
     * @brief 事件动画是否开始
     */
    bool                             _isMessageBarEventAnimationStart{false};

    /**
     * @brief 关闭按钮
     */
    ElaIconButton*                   _closeButton{nullptr};

    /**
     * @brief 创建消息栏
     * @param displayMsec 显示时长（毫秒）
     */
    Q_INVOKABLE void _messageBarCreate(int displayMsec);

    /**
     * @brief 计算初始位置
     * @param startX 起始X坐标
     * @param startY 起始Y坐标
     * @param endX 结束X坐标
     * @param endY 结束Y坐标
     */
    void _calculateInitialPos(int& startX, int& startY, int& endX, int& endY);

    /**
     * @brief 获取其他消息栏的总高度和次序信息
     * @param isJudgeCreateOrder 是否判断创建顺序
     * @return 包含总高度和次序的列表
     */
    QList<int> _getOtherMessageBarTotalData(bool isJudgeCreateOrder = false);

    /**
     * @brief 计算目标Y坐标
     * @return 目标Y坐标
     */
    qreal _calculateTargetPosY();

    /**
     * @brief 判断消息栏创建顺序
     * @param otherMessageBar 其他消息栏对象
     * @return 是否其他消息栏先创建
     */
    bool _judgeCreateOrder(ElaMessageBar* otherMessageBar);

    /**
     * @brief 绘制成功消息
     * @param painter 绘图对象
     */
    void _drawSuccess(QPainter* painter);

    /**
     * @brief 绘制警告消息
     * @param painter 绘图对象
     */
    void _drawWarning(QPainter* painter);

    /**
     * @brief 绘制信息消息
     * @param painter 绘图对象
     */
    void _drawInformation(QPainter* painter);

    /**
     * @brief 绘制错误消息
     * @param painter 绘图对象
     */
    void _drawError(QPainter* painter);
};

#endif // ELAMESSAGEBARPRIVATE_H