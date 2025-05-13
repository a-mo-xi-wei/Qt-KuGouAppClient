#ifndef STDAFX_H
#define STDAFX_H

#include <QtCore/qglobal.h>

/**
 * @def     ELA_EXPORT
 * @brief   定义动态库导出/导入宏
 *
 * 根据是否定义 ELAWIDGETTOOLS_LIBRARY，决定是导出还是导入符号。
 */
#ifdef ELAWIDGETTOOLS_LIBRARY
#define ELA_EXPORT Q_DECL_EXPORT
#else
#define ELA_EXPORT Q_DECL_IMPORT
#endif

/**
 * @def     Q_PROPERTY_CREATE
 * @brief   创建普通属性的宏
 * @param   TYPE  属性类型
 * @param   M     属性名称
 *
 * 自动生成属性声明、信号、setter 和 getter 方法，以及私有成员变量。
 */
#define Q_PROPERTY_CREATE(TYPE, M)                          \
    Q_PROPERTY(TYPE p##M MEMBER _p##M NOTIFY p##M##Changed) \
public:                                                     \
    Q_SIGNAL void p##M##Changed();                          \
    void set##M(TYPE M)                                     \
    {                                                       \
        _p##M = M;                                          \
        Q_EMIT p##M##Changed();                             \
    }                                                       \
    TYPE get##M() const                                     \
    {                                                       \
        return _p##M;                                       \
    }                                                       \
                                                            \
private:                                                    \
    TYPE _p##M;

/**
 * @def     Q_PROPERTY_CREATE_Q_H
 * @brief   创建 Q_D/Q_Q 模式属性的头文件声明
 * @param   TYPE  属性类型
 * @param   M     属性名称
 *
 * 为 Q_D/Q_Q 模式生成属性声明、信号、setter 和 getter 的声明。
 */
#define Q_PROPERTY_CREATE_Q_H(TYPE, M)                                  \
    Q_PROPERTY(TYPE p##M READ get##M WRITE set##M NOTIFY p##M##Changed) \
public:                                                                 \
    Q_SIGNAL void p##M##Changed();                                      \
    void set##M(TYPE M);                                                \
    TYPE get##M() const;

/**
 * @def     Q_PRIVATE_CREATE_Q_H
 * @brief   创建 Q_D/Q_Q 模式私有变量的头文件声明
 * @param   TYPE  变量类型
 * @param   M     变量名称
 *
 * 为 Q_D/Q_Q 模式生成私有变量的 setter 和 getter 声明。
 */
#define Q_PRIVATE_CREATE_Q_H(TYPE, M) \
public:                               \
    void set##M(TYPE M);              \
    TYPE get##M() const;

/**
 * @def     Q_PROPERTY_CREATE_Q_CPP
 * @brief   创建 Q_D/Q_Q 模式属性的实现
 * @param   CLASS  类名
 * @param   TYPE   属性类型
 * @param   M      属性名称
 *
 * 为 Q_D/Q_Q 模式实现属性的 setter 和 getter 方法。
 */
#define Q_PROPERTY_CREATE_Q_CPP(CLASS, TYPE, M) \
    void CLASS::set##M(TYPE M)                  \
    {                                           \
        Q_D(CLASS);                             \
        d->_p##M = M;                           \
        Q_EMIT p##M##Changed();                 \
    }                                           \
    TYPE CLASS::get##M() const                  \
    {                                           \
        return d_ptr->_p##M;                    \
    }

/**
 * @def     Q_PRIVATE_CREATE_Q_CPP
 * @brief   创建 Q_D/Q_Q 模式私有变量的实现
 * @param   CLASS  类名
 * @param   TYPE   变量类型
 * @param   M      变量名称
 *
 * 为 Q_D/Q_Q 模式实现私有变量的 setter 和 getter 方法。
 */
#define Q_PRIVATE_CREATE_Q_CPP(CLASS, TYPE, M) \
    void CLASS::set##M(TYPE M)                 \
    {                                          \
        Q_D(CLASS);                            \
        d->_p##M = M;                          \
    }                                          \
    TYPE CLASS::get##M() const                 \
    {                                          \
        return d_ptr->_p##M;                   \
    }

/**
 * @def     Q_PROPERTY_CREATE_D
 * @brief   创建私有成员变量
 * @param   TYPE  变量类型
 * @param   M     变量名称
 *
 * 为私有成员变量生成声明。
 */
#define Q_PROPERTY_CREATE_D(TYPE, M) \
private:                             \
    TYPE _p##M;

/**
 * @def     Q_PRIVATE_CREATE_D
 * @brief   创建私有成员变量（无额外方法）
 * @param   TYPE  变量类型
 * @param   M     变量名称
 *
 * 为私有成员变量生成声明，无 setter 和 getter。
 */
#define Q_PRIVATE_CREATE_D(TYPE, M) \
private:                            \
    TYPE _p##M;

/**
 * @def     Q_PRIVATE_CREATE
 * @brief   创建私有变量及其 setter 和 getter
 * @param   TYPE  变量类型
 * @param   M     变量名称
 *
 * 生成私有变量及其 setter 和 getter 方法。
 */
#define Q_PRIVATE_CREATE(TYPE, M) \
public:                           \
    void set##M(TYPE M)           \
    {                             \
        _p##M = M;                \
    }                             \
    TYPE get##M() const           \
    {                             \
        return _p##M;             \
    }                             \
                                  \
private:                          \
    TYPE _p##M;

/**
 * @def     Q_Q_CREATE
 * @brief   创建 Q_Q 模式的类声明
 * @param   CLASS  类名
 *
 * 为 Q_Q 模式生成构造函数、禁用拷贝和私有指针声明。
 */
#define Q_Q_CREATE(CLASS)                                        \
protected:                                                       \
    explicit CLASS(CLASS##Private& dd, CLASS* parent = nullptr); \
    QScopedPointer<CLASS##Private> d_ptr;                        \
                                                                 \
private:                                                         \
    Q_DISABLE_COPY(CLASS)                                        \
    Q_DECLARE_PRIVATE(CLASS);

/**
 * @def     Q_D_CREATE
 * @brief   创建 Q_D 模式的私有类声明
 * @param   CLASS  类名
 *
 * 为 Q_D 模式生成私有类指针和声明。
 */
#define Q_D_CREATE(CLASS) \
protected:                \
    CLASS* q_ptr;         \
                          \
private:                  \
    Q_DECLARE_PUBLIC(CLASS);

#endif // STDAFX_H