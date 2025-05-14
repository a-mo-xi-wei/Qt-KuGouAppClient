/**
* @file ElaPushButtonPrivate.cpp
 * @brief 实现 ElaPushButtonPrivate 类，管理按钮的私有实现
 * @author [Liniyous]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaPushButtonPrivate.h"
#include "ElaPushButton.h"

/**
 * @brief 构造函数，初始化按钮私有对象
 * @param parent 父对象指针，默认为 nullptr
 */
ElaPushButtonPrivate::ElaPushButtonPrivate(QObject* parent)
    : QObject{parent}
{
}

/**
 * @brief 析构函数，释放按钮私有资源
 */
ElaPushButtonPrivate::~ElaPushButtonPrivate()
{
}