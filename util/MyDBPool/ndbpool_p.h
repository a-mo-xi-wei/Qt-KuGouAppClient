#ifndef NDBPOOL_P_H
#define NDBPOOL_P_H

#include "../../common/common.h"

#include <QObject>
#include <QSql>
#include <QSqlDatabase>
#include <QString>
#include <QMutex>
#include <QSemaphore>
#include <QStack>
#include <QUuid>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QTimer>

class NDBPool_p: public QObject
{
public:
    NDBPool_p(QObject *parent=0, bool paramDebug=true);
    ~NDBPool_p();

public:
    /**
     * @brief getNewConnection 获取一个数据库连接(名字存在则复用，不存在则创建)
     * @param paramConnectionName 连接名字
     * @return 数据库连接实例
     */
    QSqlDatabase getNewConnection(const QString &paramConnectionName);

    /**
     * @brief closeConnection 关闭数据库连接
     * @param paramConnectionName 连接名字
     * @return
     */
    bool closeConnection(const QString &paramConnectionName,bool isdirectdelete=false);

    /**
     * @brief getHostName 数据主机地址的属性获取
     * @return
     */
    QString getHostName() const;

    /**
     * @brief getDBType 获取数据库连接类型
     * @return
     */
    DB_Type getDBType() const;

    /**
     * @brief setDBType 设置数据库连接类型
     * @return
     */
    void setDBType(DB_Type ptype);

    /**
     * @brief setHostName 数据主机地址的设置
     * @param value 数据主机地址
     */
    void setHostName(const QString &value);

    /**
     * @brief getDatabaseName 数据库名字的属性获取
     * @return
     */
    QString getDatabaseName() const;

    /**
     * @brief setDatabaseName 数据库名字的属性设置
     * @param value 数据库的名字
     */
    void setDatabaseName(const QString &value);

    /**
     * @brief getUsername 登录用户名的属性获取
     * @return
     */
    QString getUsername() const;

    /**
     * @brief setUsername 登录用户名的属性设置
     * @param value 用户名字
     */
    void setUsername(const QString &value);

    /**
     * @brief getPassword 登录密码的属性获取
     * @return
     */
    QString getPassword() const;

    /**
     * @brief setPassword 登录密码的属性的设置
     * @param value 登录密码
     */
    void setPassword(const QString &value);

    /**
     * @brief getPort 端口号的属性的获取
     * @return
     */
    int getPort() const;

    /**
     * @brief setPort 端口号的属性的设置
     * @param value 端口号
     */
    void setPort(int value);

    /**
     * @brief getMaxConnectionCount 最大连接数的属性的获取
     * @return
     */
    int getMaxConnectionCount() const;

    /**
     * @brief setMaxConnectionCount 最大连接数的属性的设置
     * @param value 最大连接数
     */
    void setMaxConnectionCount(int value);

    /**
     * @brief getMaxWaitTime 最大连接等待时间的属性的获取
     * @return
     */
    int getMaxWaitTime() const;

    /**
     * @brief setMaxWaitTime 最大连接等待时间的属性的设置
     * @param value 最大连接等待时间
     */
    void setMaxWaitTime(int value);

public:
    /**
     * @brief usedConnectionName 处于链接中的连接名
     */
    QStack<QString> usedConnectionName;

    /**
     * @brief unuseConnectionName 处于连接状态但是未被使用的短连接名
     */
    QStack<QString> unusedConnectionName;

    /**
     * @brief mutex 互斥锁
     */
    static QMutex mutex;

    /**
     * @brief semaphore 互斥计数
     */
    QSemaphore semaphore;

private:

    /**
     * @brief hostName 数据库主机地址
     */
    QString hostName;

    /**
     * @brief databaseName 数据库名字
     */
    QString databaseName;

    /**
     * @brief username 登录用户名
     */
    QString username;

    /**
     * @brief password 登录密码
     */
    QString password;

    /**
     * @brief m_DB_Type 数据库类型
     */
    DB_Type m_DB_Type;

    /**
     * @brief port 数据库端口
     */
    int port;

    /**
     * @brief maxConnectionCount 最大连接数
     */
    int maxConnectionCount;

    /**
     * @brief maxWaitTime 最大连接等待时间
     */
    int maxWaitTime;

    /**
     * @brief isDebug 程序运行级别
     */
    bool isDebug;

    /**
     * @brief testSql 测试连接通畅的语句
     */
    QString testSql;
};

#endif // NDBPOOL_P_H
