/*
NedAllocatedObject - 自定义内存分配对象类
*/

#ifndef _NEDALLOCATED_OBJECT_H_
#define _NEDALLOCATED_OBJECT_H_

#include "common.h"

/** @class NedAllocatedObject
 *  @brief 提供自定义内存分配和释放功能的基类
 *
 *  该类通过重载 new 和 delete 运算符，支持自定义内存分配策略，通常与 NedAllocatorImpl 配合使用。
 */
class NedAllocatedObject {
public:
    /** @brief 构造函数
     *
     *  初始化 NedAllocatedObject 对象。
     */
    explicit NedAllocatedObject() {
    }

    /** @brief 虚析构函数
     *
     *  确保派生类的正确析构。
     */
    virtual ~NedAllocatedObject() {
    }

    /** @brief 重载 new 运算符
     *
     *  为对象分配指定大小的内存。
     *
     *  @param sz 分配的内存大小（字节）
     *  @return 分配的内存地址
     */
    void *operator new(size_t sz);

    /** @brief 重载 placement new 运算符
     *
     *  在指定地址上构造对象，不分配新内存。
     *
     *  @param sz 分配的内存大小（字节）
     *  @param ptr 指定内存地址
     *  @return 指定内存地址
     */
    void *operator new(size_t sz, void *ptr);

    /** @brief 重载 new[] 运算符
     *
     *  为对象数组分配指定大小的内存。
     *
     *  @param sz 分配的内存大小（字节）
     *  @return 分配的内存地址
     */
    void *operator new[](size_t sz);

    /** @brief 重载 delete 运算符
     *
     *  释放通过 new 运算符分配的内存。
     *
     *  @param ptr 要释放的内存地址
     */
    void operator delete(void *ptr);

    /** @brief 重载 placement delete 运算符
     *
     *  释放通过 placement new 分配的内存。
     *
     *  @param ptr 要释放的内存地址
     *  @param ptr2 占位参数，与 placement new 对应
     */
    void operator delete(void *ptr, void *);

    /** @brief 重载 delete[] 运算符
     *
     *  释放通过 new[] 运算符分配的内存。
     *
     *  @param ptr 要释放的内存地址
     */
    void operator delete[](void *ptr);
};

#endif //_NEDALLOCATED_OBJECT_H_