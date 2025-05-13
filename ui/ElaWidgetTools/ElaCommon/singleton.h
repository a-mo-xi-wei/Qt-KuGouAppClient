#ifndef SINGLETON_H
#define SINGLETON_H

#include <QMutex>

/**
 * @class   Singleton
 * @brief   模板单例类
 * @tparam  T  单例类的类型
 *
 * 提供线程安全的单例模式实现，确保全局唯一实例。
 */
template <typename T>
class Singleton
{
public:
    /**
     * @brief   获取单例实例
     * @return  指向单例对象的指针
     *
     * 线程安全地创建并返回单例实例。
     */
    static T* getInstance();

private:
    /**
     * @brief   禁用拷贝构造函数
     *
     * 防止单例类被拷贝。
     */
    Q_DISABLE_COPY(Singleton)
};

/**
 * @brief   获取单例实例的实现
 * @tparam  T  单例类的类型
 * @return  指向单例对象的指针
 *
 * 使用双检锁机制确保线程安全地创建单例实例。
 */
template <typename T>
T* Singleton<T>::getInstance()
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    static T* instance = nullptr;
    if (instance == nullptr)
    {
        instance = new T();
    }
    return instance;
}

/**
 * @def     Q_SINGLETON_CREATE
 * @brief   创建单例类的宏
 * @param   Class  单例类名
 *
 * 为指定类生成单例模式的实现，包括友元类和 getInstance 方法。
 */
#define Q_SINGLETON_CREATE(Class)               \
private:                                        \
    friend class Singleton<Class>;              \
                                                \
public:                                         \
    static Class* getInstance()                 \
    {                                           \
        return Singleton<Class>::getInstance(); \
    }

/**
 * @def     Q_SINGLETON_CREATE_H
 * @brief   创建单例类头文件声明
 * @param   Class  单例类名
 *
 * 为单例类生成静态实例指针和 getInstance 方法的声明。
 */
#define Q_SINGLETON_CREATE_H(Class) \
private:                            \
    static Class* _instance;        \
                                    \
public:                             \
    static Class* getInstance();

/**
 * @def     Q_SINGLETON_CREATE_CPP
 * @brief   创建单例类实现
 * @param   Class  单例类名
 *
 * 为单例类实现线程安全的 getInstance 方法。
 */
#define Q_SINGLETON_CREATE_CPP(Class)  \
Class* Class::_instance = nullptr; \
Class* Class::getInstance()        \
{                                  \
    static QMutex mutex;           \
    QMutexLocker locker(&mutex);   \
    if (_instance == nullptr)      \
    {                              \
        _instance = new Class();   \
    }                              \
    return _instance;              \
}

#endif // SINGLETON_H