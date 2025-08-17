/*
  NedAllocatorImpl - 自定义内存分配器实现
*/

#include "NedAllocatorImpl.h"

/** @brief 分配指定大小的内存
 *
 *  使用标准库 malloc 分配 count 字节的内存。
 *
 *  @param count 分配的内存大小（字节）
 *  @return 分配的内存地址，若失败则返回 nullptr
 */
void *allocBytes(size_t count) {
    void *ptr = nullptr;

    ptr = malloc(count);
    return ptr;
}

/** @brief 释放内存
 *
 *  使用标准库 free 释放指定的内存块。
 *
 *  @param ptr 要释放的内存地址
 */
void deallocBytes(void *ptr) {
    if (!ptr)
        return;

    free(ptr);
}

/** @brief 分配对齐的内存
 *
 *  根据指定的对齐方式分配 count 字节的内存。如果启用 nedalloc，则使用 nedmemalign 进行对齐分配。
 *
 *  @param align 内存对齐字节数
 *  @param count 分配的内存大小（字节）
 *  @return 分配的内存地址，若失败则返回 nullptr
 */
void *allocBytesAligned(size_t align, size_t count) {
    //const size_t SIMD_ALIGNMENT = 16;
    void *ptr = nullptr;

#if ENALBE_NEDALLOC == 1
    ptr = align ? nedalloc::nedmemalign(align, count)
               : nedalloc::nedmemalign(SIMD_ALIGNMENT, count);
#endif

    return ptr;
}

/** @brief 释放对齐的内存
 *
 *  使用标准库 free 释放通过 allocBytesAligned 分配的内存。
 *
 *  @param align 对齐字节数
 *  @param ptr 要释放的内存地址
 */
void deallocBytesAligned(size_t align, void *ptr) {
    if (!ptr)
        return;
    free(ptr);
}

/** @brief 分配内存（替代 malloc）
 *
 *  使用标准库 malloc 分配指定大小的内存。
 *
 *  @param size 分配的内存大小（字节）
 *  @return 分配的内存地址，若失败则返回 nullptr
 */
void *MemoryMalloc(size_t size) {
    void *ptr = nullptr;

    ptr = malloc(size);

    return ptr;
}

/** @brief 释放内存（替代 free）
 *
 *  使用标准库 free 释放指定的内存块。
 *
 *  @param ptr 要释放的内存地址
 */
void MemoryFree(void *ptr) {
    if (!ptr)
        return;

    free(ptr);
}

/** @brief 分配并清零内存
 *
 *  使用标准库 calloc 分配 num * size 字节的内存，并初始化为零。
 *
 *  @param num 分配的元素数量
 *  @param size 每个元素的大小（字节）
 *  @return 分配的内存地址，若失败则返回 nullptr
 */
void *Memorycalloc(size_t num, size_t size) {
    void *ptr = nullptr;

    ptr = calloc(num, size);
    return ptr;
}

/** @brief 重新分配内存
 *
 *  使用标准库 realloc 调整现有内存块的大小。
 *
 *  @param memblock 现有内存块的地址
 *  @param size 新的内存大小（字节）
 *  @return 重新分配的内存地址，若失败则返回 nullptr
 */
void *Memoryrealloc(void *memblock, size_t size) {
    if (!memblock)
        return nullptr;
    void *ptr = nullptr;

    ptr = realloc(memblock, size);
    return ptr;
}