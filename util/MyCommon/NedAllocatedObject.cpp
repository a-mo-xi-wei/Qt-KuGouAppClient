/*
NedAllocatedObject - 自定义内存分配对象类实现
*/

#include "NedAllocatedObject.h"
#include "NedAllocatorImpl.h"

/** @brief 重载 new 运算符
 *
 *  调用 allocBytes 函数分配指定大小的内存。
 *
 *  @param sz 分配的内存大小（字节）
 *  @return 分配的内存地址
 */
void *NedAllocatedObject::operator new(size_t sz) {
    return allocBytes(sz);
}

/** @brief 重载 placement new 运算符
 *
 *  返回指定的内存地址，不分配新内存。
 *
 *  @param sz 分配的内存大小（字节）
 *  @param ptr 指定内存地址
 *  @return 指定内存地址
 */
void *NedAllocatedObject::operator new(size_t sz, void *ptr) {
    return ptr;
}

/** @brief 重载 new[] 运算符
 *
 *  调用 allocBytes 函数为对象数组分配指定大小的内存。
 *
 *  @param sz 分配的内存大小（字节）
 *  @return 分配的内存地址
 */
void *NedAllocatedObject::operator new[](size_t sz) {
    return allocBytes(sz);
}

/** @brief 重载 delete 运算符
 *
 *  调用 deallocBytes 函数释放通过 new 运算符分配的内存。
 *
 *  @param ptr 要释放的内存地址
 */
void NedAllocatedObject::operator delete(void *ptr) {
    deallocBytes(ptr);
}

/** @brief 重载 placement delete 运算符
 *
 *  调用 deallocBytes 函数释放通过 placement new 分配的内存。
 *
 *  @param ptr 要释放的内存地址
 *  @param ptr2 占位参数，与 placement new 对应
 */
void NedAllocatedObject::operator delete(void *ptr, void *) {
    deallocBytes(ptr);
}

/** @brief 重载 delete[] 运算符
 *
 *  调用 deallocBytes 函数释放通过 new[] 运算符分配的内存。
 *
 *  @param ptr 要释放的内存地址
 */
void NedAllocatedObject::operator delete[](void *ptr) {
    deallocBytes(ptr);
}