/*
NedAllocatorImpl - 自定义内存分配器接口
*/

#ifndef NEDALLOCATOR_IMPL_H_
#define NEDALLOCATOR_IMPL_H_

#include "common.h"

#if defined(__cplusplus)
extern "C" {
#endif

    /** @brief 分配指定大小的内存
     *
     *  分配 count 字节的内存，并返回指向分配内存的指针。
     *
     *  @param count 分配的内存大小（字节）
     *  @return 分配的内存地址，若失败则返回 nullptr
     */
    void* allocBytes(size_t count);

    /** @brief 释放内存
     *
     *  释放指定的内存块。
     *
     *  @param ptr 要释放的内存地址
     */
    void deallocBytes(void* ptr);

    /** @brief 分配对齐的内存
     *
     *  分配 count 字节的内存，并按指定的对齐方式对齐。
     *
     *  @param align 内存对齐字节数
     *  @param count 分配的内存大小（字节）
     *  @return 分配的内存地址，若失败则返回 nullptr
     */
    void* allocBytesAligned(size_t align, size_t count);

    /** @brief 释放对齐的内存
     *
     *  释放通过 allocBytesAligned 分配的内存。
     *
     *  @param align 对齐字节数
     *  @param ptr 要释放的内存地址
     */
    void deallocBytesAligned(size_t align, void* ptr);

    /** @brief 分配内存（替代 malloc）
     *
     *  分配指定大小的内存，功能类似于标准库的 malloc。
     *
     *  @param sz 分配的内存大小（字节）
     *  @return 分配的内存地址，若失败则返回 nullptr
     */
    void* MemoryMalloc(size_t sz);

    /** @brief 释放内存（替代 free）
     *
     *  释放通过 MemoryMalloc 或类似函数分配的内存。
     *
     *  @param ptr 要释放的内存地址
     */
    void MemoryFree(void* ptr);

    /** @brief 分配并清零内存
     *
     *  分配 num * size 字节的内存，并将内存初始化为零，功能类似于标准库的 calloc。
     *
     *  @param num 分配的元素数量
     *  @param size 每个元素的大小（字节）
     *  @return 分配的内存地址，若失败则返回 nullptr
     */
    void *Memorycalloc(size_t num, size_t size);

    /** @brief 重新分配内存
     *
     *  调整现有内存块的大小，功能类似于标准库的 realloc。
     *
     *  @param memblock 现有内存块的地址
     *  @param size 新的内存大小（字节）
     *  @return 重新分配的内存地址，若失败则返回 nullptr
     */
    void *Memoryrealloc(void* memblock, size_t size);

#if defined(__cplusplus)
}
#endif

#endif //NEDALLOCATOR_IMPL_H_