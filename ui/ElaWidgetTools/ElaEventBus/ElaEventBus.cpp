/**
 * @file ElaEventBus.cpp
 * @brief 实现 ElaEvent 和 ElaEventBus 类，管理事件注册、发布和查询
 * @author [Your Name]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaEventBus.h"
#include "ElaEventBusPrivate.h"

#include <QVariant>

/**
 * @brief 定义 ElaEventBus 单例实例
 */
Q_SINGLETON_CREATE_CPP(ElaEventBus)

/**
 * @brief 定义 ElaEvent 的 EventName 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaEvent, QString, EventName)

/**
 * @brief 定义 ElaEvent 的 FunctionName 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaEvent, QString, FunctionName)

/**
 * @brief 定义 ElaEvent 的 ConnectionType 属性
 */
Q_PROPERTY_CREATE_Q_CPP(ElaEvent, Qt::ConnectionType, ConnectionType)

/**
 * @brief 构造函数，初始化事件对象
 * @param parent 父对象指针，默认为 nullptr
 */
ElaEvent::ElaEvent(QObject* parent)
    : QObject{parent}, d_ptr(new ElaEventPrivate())
{
    Q_D(ElaEvent);
    d->q_ptr = this;
    d->_pConnectionType = Qt::AutoConnection;
    d->_pFunctionName = "";
    d->_pEventName = "";
}

/**
 * @brief 构造函数，初始化事件对象并设置事件名称和函数名称
 * @param eventName 事件名称
 * @param functionName 函数名称
 * @param parent 父对象指针，默认为 nullptr
 */
ElaEvent::ElaEvent(QString eventName, QString functionName, QObject* parent)
    : QObject{parent}, d_ptr(new ElaEventPrivate())
{
    Q_D(ElaEvent);
    d->q_ptr = this;
    d->_pConnectionType = Qt::AutoConnection;
    d->_pEventName = eventName;
    d->_pFunctionName = functionName;
}

/**
 * @brief 注册并初始化事件
 * @return 事件总线操作结果 (ElaEventBusType::EventBusReturnType)
 */
ElaEventBusType::EventBusReturnType ElaEvent::registerAndInit()
{
    return ElaEventBus::getInstance()->d_ptr->registerEvent(this);
}

/**
 * @brief 析构函数，释放事件对象资源并注销事件
 */
ElaEvent::~ElaEvent()
{
    ElaEventBus::getInstance()->d_ptr->unRegisterEvent(this);
}

/**
 * @brief 构造函数，初始化事件总线对象
 * @param parent 父对象指针，默认为 nullptr
 */
ElaEventBus::ElaEventBus(QObject* parent)
    : QObject{parent}, d_ptr(new ElaEventBusPrivate())
{
    Q_D(ElaEventBus);
    d->q_ptr = this;
}

/**
 * @brief 析构函数，释放事件总线资源
 */
ElaEventBus::~ElaEventBus()
{
}

/**
 * @brief 发布事件
 * @param eventName 事件名称
 * @param data 事件携带的数据，默认为空
 * @return 事件总线操作结果 (ElaEventBusType::EventBusReturnType)
 */
ElaEventBusType::EventBusReturnType ElaEventBus::post(const QString& eventName, const QVariantMap& data)
{
    Q_D(ElaEventBus);
    if (eventName.isEmpty())
    {
        return ElaEventBusType::EventBusReturnType::EventNameInvalid;
    }
    if (d->_eventMap.contains(eventName))
    {
        QList<ElaEvent*> eventList = d->_eventMap.value(eventName);
        foreach (auto event, eventList)
        {
            if (event->parent())
            {
                QMetaObject::invokeMethod(event->parent(), event->getFunctionName().toLocal8Bit().constData(), event->getConnectionType(), Q_ARG(QVariantMap, data));
            }
        }
    }
    return ElaEventBusType::EventBusReturnType::Success;
}

/**
 * @brief 获取已注册的事件名称列表
 * @return 事件名称列表
 */
QStringList ElaEventBus::getRegisteredEventsName() const
{
    Q_D(const ElaEventBus);
    if (d->_eventMap.count() == 0)
    {
        return QStringList();
    }
    QStringList eventsNameList = d->_eventMap.keys();
    return eventsNameList;
}