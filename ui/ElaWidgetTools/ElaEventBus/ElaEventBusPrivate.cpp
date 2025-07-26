/**
 * @file ElaEventBusPrivate.cpp
 * @brief 实现 ElaEventPrivate 和 ElaEventBusPrivate 类，管理事件和事件总线的私有实现
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaEventBusPrivate.h"
#include "ElaEventBus.h"

/**
 * @brief 构造函数，初始化事件私有对象
 * @param parent 父对象指针，默认为 nullptr
 */
ElaEventPrivate::ElaEventPrivate(QObject* parent)
    : QObject{parent}
{
}

/**
 * @brief 析构函数，释放事件私有资源
 */
ElaEventPrivate::~ElaEventPrivate() = default;

/**
 * @brief 构造函数，初始化事件总线私有对象
 * @param parent 父对象指针，默认为 nullptr
 */
ElaEventBusPrivate::ElaEventBusPrivate(QObject* parent)
    : QObject{parent}
{
}

/**
 * @brief 析构函数，释放事件总线私有资源
 */
ElaEventBusPrivate::~ElaEventBusPrivate()
{
}

/**
 * @brief 注册事件
 * @param event 事件对象指针
 * @return 事件总线操作结果 (ElaEventBusType::EventBusReturnType)
 */
ElaEventBusType::EventBusReturnType ElaEventBusPrivate::registerEvent(ElaEvent* event)
{
    if (!event)
    {
        return ElaEventBusType::EventBusReturnType::EventInvalid;
    }
    if (event->getEventName().isEmpty())
    {
        return ElaEventBusType::EventBusReturnType::EventNameInvalid;
    }
    if (_eventMap.contains(event->getEventName()))
    {
        QList<ElaEvent*> eventList = _eventMap.value(event->getEventName());
        if (eventList.contains(event))
        {
            return ElaEventBusType::EventBusReturnType::EventInvalid;
        }
        eventList.append(event);
        _eventMap[event->getEventName()] = eventList;
    }
    else
    {
        QList<ElaEvent*> eventList;
        eventList.append(event);
        _eventMap.insert(event->getEventName(), eventList);
    }
    return ElaEventBusType::EventBusReturnType::Success;
}

/**
 * @brief 注销事件
 * @param event 事件对象指针
 */
void ElaEventBusPrivate::unRegisterEvent(ElaEvent* event)
{
    if (!event)
    {
        return;
    }
    if (event->getEventName().isEmpty())
    {
        return;
    }
    if (_eventMap.contains(event->getEventName()))
    {
        if (_eventMap[event->getEventName()].count() == 1)
        {
            _eventMap.remove(event->getEventName());
        }
        else
        {
            QList<ElaEvent*> eventList = _eventMap.value(event->getEventName());
            eventList.removeOne(event);
            _eventMap[event->getEventName()] = eventList;
        }
    }
}