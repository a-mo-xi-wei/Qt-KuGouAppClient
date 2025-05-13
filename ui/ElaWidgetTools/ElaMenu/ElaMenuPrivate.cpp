/**
* @file ElaMenuPrivate.cpp
 * @brief 实现 ElaMenuPrivate 类，管理菜单的私有实现
 * @author [Your Name]
 * @date 2025-05-13
 * @version 1.0
 */

#include "ElaMenuPrivate.h"
#include "ElaMenu.h"

/**
 * @brief 构造函数，初始化菜单私有对象
 * @param parent 父对象指针，默认为 nullptr
 */
ElaMenuPrivate::ElaMenuPrivate(QObject* parent)
    : QObject{parent}
{
}

/**
 * @brief 析构函数，释放菜单私有资源
 */
ElaMenuPrivate::~ElaMenuPrivate()
{
}