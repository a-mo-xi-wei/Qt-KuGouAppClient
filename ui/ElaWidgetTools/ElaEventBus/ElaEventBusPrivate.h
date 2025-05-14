/**
* @file ElaEventBusPrivate.h
 * @brief 定义 ElaEventPrivate 和 ElaEventBusPrivate 类，管理事件和事件总线的私有实现
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#ifndef ELAEVENTBUSPRIVATE_H
#define ELAEVENTBUSPRIVATE_H

#include <QMap>

#include "Def.h"
#include "stdafx.h"

class ElaEvent;

/**
 * @class ElaEventPrivate
 * @brief 事件私有实现类，存储事件名称、函数名称和连接类型
 */
class ElaEventPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaEvent)
    Q_PROPERTY_CREATE_D(QString, EventName)
    Q_PROPERTY_CREATE_D(QString, FunctionName)
    Q_PROPERTY_CREATE_D(Qt::ConnectionType, ConnectionType)

public:
    /**
     * @brief 构造函数，初始化事件私有对象
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaEventPrivate(QObject* parent = nullptr);

    /**
     * @brief 析构函数，释放事件私有资源
     */
    ~ElaEventPrivate();
};

class ElaEventBus;

/**
 * @class ElaEventBusPrivate
 * @brief 事件总线私有实现类，管理事件映射和注册
 */
class ElaEventBusPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaEventBus)

public:
    /**
     * @brief 构造函数，初始化事件总线私有对象
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaEventBusPrivate(QObject* parent = nullptr);

    /**
     * @brief 析构函数，释放事件总线私有资源
     */
    ~ElaEventBusPrivate();

    /**
     * @brief 注册事件
     * @param event 事件对象指针
     * @return 事件总线操作结果 (ElaEventBusType::EventBusReturnType)
     */
    ElaEventBusType::EventBusReturnType registerEvent(ElaEvent* event);

    /**
     * @brief 注销事件
     * @param event 事件对象指针
     */
    void unRegisterEvent(ElaEvent* event);

private:
    /**
     * @brief 事件映射，存储事件名称到事件对象列表的映射
     */
    QMap<QString, QList<ElaEvent*>> _eventMap;
};

#endif // ELAEVENTBUSPRIVATE_H