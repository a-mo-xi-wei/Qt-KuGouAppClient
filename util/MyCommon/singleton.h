/*
Singleton - 单例模式模板类
*/

#ifndef SINGLETON_H
#define SINGLETON_H

#include <cassert>

/** @def initialiseSingleton
 *  @brief 初始化单例类的静态成员
 *
 *  用于初始化指定类型的单例静态指针。
 *
 *  @param type 单例类类型
 */
#define initialiseSingleton(type) \
template <> type * Singleton < type > :: mSingleton = 0

/** @def initialiseTemplateSingleton
 *  @brief 初始化模板单例类的静态成员
 *
 *  用于初始化模板类型的单例静态指针。
 *
 *  @param temp 模板类
 *  @param type 模板参数类型
 */
#define initialiseTemplateSingleton(temp, type) \
template <> temp< type > * Singleton < temp< type > > :: mSingleton = 0

/** @def createFileSingleton
 *  @brief 创建文件级单例对象
 *
 *  初始化并定义文件级单例对象。
 *
 *  @param type 单例类类型
 */
#define createFileSingleton(type) \
initialiseSingleton(type); \
type the##type

/** @class Singleton
 *  @brief 单例模式模板类
 *
 *  提供单例模式的实现，确保类只有一个实例。
 *
 *  @tparam type 单例类类型
 */
template <class type>
class Singleton {
public:
    /** @brief 构造函数
     *
     *  初始化单例对象，确保只有一个实例。
     */
    Singleton() {
        assert(this->mSingleton == nullptr);
        mSingleton = static_cast<type*>(this);
    }

    /** @brief 虚析构函数
     *
     *  销毁单例对象，清除静态指针。
     */
    virtual ~Singleton() {
        mSingleton = 0;
    }

    /** @brief 获取单例实例
     *
     *  返回单例对象的引用。
     *
     *  @return 单例对象的引用
     */
    static type& getSingleton() {
        assert(mSingleton);
        return *mSingleton;
    }

    /** @brief 获取单例指针
     *
     *  返回单例对象的指针。
     *
     *  @return 单例对象的指针
     */
    static type* getSingletonPtr() {
        return mSingleton;
    }

protected:
    /** @brief 单例静态指针
     */
    static type* mSingleton;
};

#endif // SINGLETON_H