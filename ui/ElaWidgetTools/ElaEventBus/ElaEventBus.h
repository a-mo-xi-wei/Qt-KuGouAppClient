/**
 * @file ElaEventBus.h
 * @brief 定义 ElaEvent 和 ElaEventBus 类，提供事件总线功能
 * @author [Your Name]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAEVENTBUS_H
#define ELAEVENTBUS_H

#include <QObject>
#include <QVariantMap>

#include "Def.h"
#include "singleton.h"
#include "stdafx.h"

class ElaEventPrivate;

/**
 * @class ElaEvent
 * @brief 事件类，用于定义和管理事件名称、函数名称和连接类型
 */
class ELA_EXPORT ElaEvent : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(ElaEvent)
    Q_PROPERTY_CREATE_Q_H(QString, EventName)
    Q_PROPERTY_CREATE_Q_H(QString, FunctionName)
    Q_PROPERTY_CREATE_Q_H(Qt::ConnectionType, ConnectionType)

public:
    /**
     * @brief 构造函数，初始化事件对象
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaEvent(QObject* parent = nullptr);

    /**
     * @brief 构造函数，初始化事件对象并设置事件名称和函数名称
     * @param eventName 事件名称
     * @param functionName 函数名称
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaEvent(QString eventName, QString functionName, QObject* parent = nullptr);

    /**
     * @brief 析构函数，释放事件对象资源
     */
    ~ElaEvent();

    /**
     * @brief 注册并初始化事件
     * @return 事件总线操作结果 (ElaEventBusType::EventBusReturnType)
     */
    ElaEventBusType::EventBusReturnType registerAndInit();
};

class ElaEventBusPrivate;

/**
 * @class ElaEventBus
 * @brief 事件总线类，管理事件注册、发布和查询
 * @note 该类为单例模式，通过 ElaEventBus::getInstance() 访问实例
 */
class ELA_EXPORT ElaEventBus : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(ElaEventBus)
    Q_SINGLETON_CREATE_H(ElaEventBus)

private:
    /**
     * @brief 构造函数，初始化事件总线对象
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaEventBus(QObject* parent = nullptr);

    /**
     * @brief 析构函数，释放事件总线资源
     */
    ~ElaEventBus();

public:
    /**
     * @brief 发布事件
     * @param eventName 事件名称
     * @param data 事件携带的数据，默认为空
     * @return 事件总线操作结果 (ElaEventBusType::EventBusReturnType)
     */
    ElaEventBusType::EventBusReturnType post(const QString& eventName, const QVariantMap& data = {});

    /**
     * @brief 获取已注册的事件名称列表
     * @return 事件名称列表
     */
    QStringList getRegisteredEventsName() const;

private:
    friend class ElaEvent;
};

#endif // ELAEVENTBUS_H