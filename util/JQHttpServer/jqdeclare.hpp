/**
 * @file jqdeclare.hpp
 * @brief JQLibrary 的声明文件，包含宏定义和导出声明。
 *
 * 此文件是 JQLibrary 的一部分，提供了属性声明宏、运行时保护宏等实用工具。
 *
 * @author Jason
 * @contact 188080501@qq.com
 * @copyright GNU Lesser General Public License
 */

#ifndef JQLIBRARY_INCLUDE_JQDECLARE_HPP_
#define JQLIBRARY_INCLUDE_JQDECLARE_HPP_

// 宏定义
/**
 * @def JQPROPERTYDECLARE(Type, name, setName, ...)
 * @brief 声明一个私有成员变量及其 getter 和 setter 方法。
 *
 * @param Type 成员变量的类型。
 * @param name 成员变量的名称。
 * @param setName setter 方法的名称。
 * @param ... 成员变量的初始化参数。
 */
#define JQPROPERTYDECLARE( Type, name, setName, ... )                  \
private:                                                               \
    Type name##_ __VA_ARGS__;                                          \
                                                                       \
public:                                                                \
    inline const Type &name() const { return name##_; }                \
    inline void        setName( const Type &name ) { name##_ = name; } \
                                                                       \
private:

/**
 * @def JQPROPERTYDECLAREWITHSLOT(Type, name, setName, ...)
 * @brief 声明一个私有成员变量及其 getter 和 setter 方法（作为槽函数）。
 *
 * @param Type 成员变量的类型。
 * @param name 成员变量的名称。
 * @param setName setter 方法的名称。
 * @param ... 成员变量的初始化参数。
 */
#define JQPROPERTYDECLAREWITHSLOT( Type, name, setName, ... ) \
private:                                                      \
    Type name##_ __VA_ARGS__;                                 \
public Q_SLOTS:                                               \
    Type name() const { return name##_; }                     \
    void setName( const Type &name ) { name##_ = name; }      \
                                                              \
private:

/**
 * @def JQPTRPROPERTYDECLARE(Type, name, setName, ...)
 * @brief 声明一个私有指针成员变量及其 getter 和 setter 方法。
 *
 * @param Type 成员变量的类型。
 * @param name 成员变量的名称。
 * @param setName setter 方法的名称。
 * @param ... 成员变量的初始化参数。
 */
#define JQPTRPROPERTYDECLARE( Type, name, setName, ... ) \
private:                                                 \
    Type *name##_ __VA_ARGS__;                           \
                                                         \
public:                                                  \
    inline const Type *name() const { return name##_; }  \
    inline void        setName( const Type &name )       \
    {                                                    \
        if ( name##_ )                                   \
        {                                                \
            delete name##_;                              \
        }                                                \
        name##_ = new Type( name );                      \
    }                                                    \
                                                         \
private:

/**
 * @def JQ_READ_AND_SET_PROPERTY(Type, name, setName)
 * @brief 声明一个公共的 getter 和 setter 方法。
 *
 * @param Type 成员变量的类型。
 * @param name 成员变量的名称。
 * @param setName setter 方法的名称。
 */
#define JQ_READ_AND_SET_PROPERTY( Type, name, setName )                \
public:                                                                \
    inline const Type &name() const { return name##_; }                \
    inline void        setName( const Type &name ) { name##_ = name; } \
                                                                       \
private:

/**
 * @def JQ_STATIC_READ_AND_SET_PROPERTY(Type, name, setName)
 * @brief 声明一个静态的 getter 和 setter 方法。
 *
 * @param Type 成员变量的类型。
 * @param name 成员变量的名称。
 * @param setName setter 方法的名称。
 */
#define JQ_STATIC_READ_AND_SET_PROPERTY( Type, name, setName )                \
public:                                                                       \
    static inline const Type &name() { return name##_; }                      \
    static inline void        setName( const Type &name ) { name##_ = name; } \
                                                                              \
private:

/**
 * @def JQ_STATIC_SET_PROPERTY(Type, name, setName)
 * @brief 声明一个静态的 setter 方法。
 *
 * @param Type 成员变量的类型。
 * @param name 成员变量的名称。
 * @param setName setter 方法的名称。
 */
#define JQ_STATIC_SET_PROPERTY( Type, name, setName )                  \
public:                                                                \
    static inline void setName( const Type &name ) { name##_ = name; } \
                                                                       \
private:

/**
 * @def RUNONOUTRANGEHELPER2(x, y)
 * @brief 辅助宏，用于连接标识符。
 */
#define RUNONOUTRANGEHELPER2( x, y ) x##y

/**
 * @def RUNONOUTRANGEHELPER(x, y)
 * @brief 辅助宏，用于连接标识符。
 */
#define RUNONOUTRANGEHELPER( x, y )  RUNONOUTRANGEHELPER2( x, y )

/**
 * @def RUNONOUTRANGE(...)
 * @brief 在超出范围时运行回调。
 *
 * @param ... 回调函数。
 */
#define RUNONOUTRANGE( ... )                                                                    \
    auto                  RUNONOUTRANGEHELPER( runOnOutRangeCallback, __LINE__ ) = __VA_ARGS__; \
    QSharedPointer< int > RUNONOUTRANGEHELPER( runOnOutRange, __LINE__ )(                       \
        new int,                                                                                \
        [ RUNONOUTRANGEHELPER( runOnOutRangeCallback, __LINE__ ) ]( int *data ) {               \
            RUNONOUTRANGEHELPER( runOnOutRangeCallback, __LINE__ )                              \
            ();                                                                                 \
            delete data;                                                                        \
        } );                                                                                    \
    if ( RUNONOUTRANGEHELPER( runOnOutRange, __LINE__ ).data() == nullptr )                     \
    {                                                                                           \
        exit( -1 );                                                                             \
    }

/**
 * @def RUNONOUTRANGETIMER(message)
 * @brief 在超出范围时运行计时器并记录消息。
 *
 * @param message 要记录的消息。
 */
#define RUNONOUTRANGETIMER( message )                                                            \
    const auto &&runOnOutRangeTimerTime = QDateTime::currentMSecsSinceEpoch();                   \
    RUNONOUTRANGE( [ = ]() {                                                                     \
        qDebug() << message << ( QDateTime::currentMSecsSinceEpoch() - runOnOutRangeTimerTime ); \
    } )

/**
 * @def JQCONST(property)
 * @brief 返回属性的常量引用。
 *
 * @param property 属性。
 */
#define JQCONST( property ) static_cast< const decltype( property ) >( property )

/**
 * @def JQTICKCOUNTERMESSAGE(message)
 * @brief 记录消息和滴答计数。
 *
 * @param message 要记录的消息。
 */
#define JQTICKCOUNTERMESSAGE( message )                     \
    {                                                       \
        static JQTickCounter tickCounter;                   \
        tickCounter.tick();                                 \
        qDebug() << message << tickCounter.tickPerSecond(); \
    }

/**
 * @def JQBUILDDATETIMESTRING
 * @brief 获取构建日期和时间字符串。
 */
#define JQBUILDDATETIMESTRING                                                                             \
    ( QDateTime(                                                                                          \
          QLocale( QLocale::English ).toDate( QString( __DATE__ ).replace( "  ", " 0" ), "MMM dd yyyy" ), \
          QTime::fromString( __TIME__, "hh:mm:ss" ) )                                                     \
          .toString( "yyyy-MM-dd hh:mm:ss" )                                                              \
          .toLatin1()                                                                                     \
          .data() )

/**
 * @def JQONLYONCE
 * @brief 确保代码块只执行一次。
 */
#define JQONLYONCE                    \
    if ( []() {                       \
             static auto flag = true; \
             if ( flag )              \
             {                        \
                 flag = false;        \
                 return true;         \
             }                        \
             return false;            \
         }() )

/**
 * @def JQSKIPFIRST
 * @brief 跳过第一次执行。
 */
#define JQSKIPFIRST                   \
    if ( []() {                       \
             static auto flag = true; \
             if ( flag )              \
             {                        \
                 flag = false;        \
                 return false;        \
             }                        \
             return true;             \
         }() )

/**
 * @def JQINTERVAL(timeInterval)
 * @brief 确保代码块在指定时间间隔内执行。
 *
 * @param timeInterval 时间间隔（毫秒）。
 */
#define JQINTERVAL( timeInterval )                                            \
    if ( []() {                                                               \
             static qint64 lastTime    = 0;                                   \
             const auto && currentTime = QDateTime::currentMSecsSinceEpoch(); \
             if ( qAbs( currentTime - lastTime ) > timeInterval )             \
             {                                                                \
                 lastTime = currentTime;                                      \
                 return true;                                                 \
             }                                                                \
             return false;                                                    \
         }() )

// 导出
#ifdef JQLIBRARY_EXPORT_ENABLE
#    ifdef JQLIBRARY_EXPORT_MODE
#        define JQLIBRARY_EXPORT Q_DECL_EXPORT
#    else
#        define JQLIBRARY_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define JQLIBRARY_EXPORT
#endif

#endif    // JQLIBRARY_INCLUDE_JQDECLARE_HPP_