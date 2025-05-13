/*
  NDBPool_p - 数据库连接池私有实现
*/

#include "ndbpool_p.h"
#include "QsLog.h"

#include <QSqlError>

/** @brief 数据库连接池的互斥锁
 */
QMutex NDBPool_p::mutex;

/** @brief 构造函数
 *
 *  初始化数据库连接池，设置默认最大连接数、最大等待时间和测试 SQL 语句。
 *
 *  @param parent 父对象
 *  @param paramDebug 是否启用调试模式
 */
NDBPool_p::NDBPool_p(QObject *parent, bool paramDebug)
    : QObject(parent), m_DB_Type(TYPE_NULL) {
    setMaxConnectionCount(50);
    setMaxWaitTime(10);
    semaphore.release(getMaxConnectionCount());
    isDebug = paramDebug;
    testSql = "select 0;";
}

/** @brief 析构函数
 *
 *  销毁数据库连接池，关闭所有已使用和未使用的数据库连接。
 */
NDBPool_p::~NDBPool_p() {
    foreach (QString connectionName, usedConnectionName) {
        getNewConnection(connectionName).close();
        if (isDebug) {
            QLOG_DEBUG() << "close connected connectionName:" << connectionName;
        }
        if (QSqlDatabase::contains(connectionName)) {
            closeConnection(connectionName);
        }
    }
    foreach (QString connectionName, unusedConnectionName) {
        if (isDebug) {
            QLOG_DEBUG() << "close unconnected short connectionName:" << connectionName;
        }
        if (QSqlDatabase::contains(connectionName)) {
            closeConnection(connectionName);
        }
    }
}

/** @brief 获取新的数据库连接
 *
 *  根据连接名称获取或创建新的数据库连接，并验证连接有效性。
 *
 *  @param paramConnectionName 连接名称
 *  @return 有效的数据库连接对象，若失败则返回空对象
 */
QSqlDatabase NDBPool_p::getNewConnection(const QString &paramConnectionName) {
    QMutexLocker tempLock(&mutex);
    if (QSqlDatabase::contains(paramConnectionName)) {
        QSqlDatabase tempDB = QSqlDatabase::database(paramConnectionName);
        if (testSql.length() != 0) {
            if (!tempDB.isOpen()) {
                tempDB.open();
            }
            if (!tempDB.isOpenError()) {
                QSqlQuery tempQuery(tempDB);
                tempQuery.exec(testSql);
                if (tempQuery.lastError().type() == QSqlError::NoError) {
                    return tempDB;
                } else {
                    QLOG_ERROR() << paramConnectionName << ":" << tempDB.lastError().text() << ";" << tempQuery.lastError().type();
                }
            } else {
                if (isDebug) {
                    QLOG_DEBUG() << "Open old database error-Create a new connection:" << tempDB.lastError().text();
                }
                return {};
            }
        }
    }

    QSqlDatabase tempDBNew;

    switch (m_DB_Type) {
    case QMYSQL: {
        tempDBNew = QSqlDatabase::addDatabase("QMYSQL", paramConnectionName);
        tempDBNew.setHostName(hostName);
        tempDBNew.setDatabaseName(databaseName);
        tempDBNew.setUserName(username);
        tempDBNew.setPassword(password);
        tempDBNew.setPort(port);
        break;
    }
    case QODBC: {
        tempDBNew = QSqlDatabase::addDatabase("QODBC", paramConnectionName);
        tempDBNew.setDatabaseName(QString("DRIVER={SQL SERVER};"
                                          "SERVER=%1,%5;"
                                          "DATABASE=%2;"
                                          "UID=%3;"
                                          "PWD=%4;")
                                      .arg(hostName)
                                      .arg(databaseName)
                                      .arg(username)
                                      .arg(password)
                                      .arg(port));
        break;
    }
    case QSQLITE: {
        if (QSqlDatabase::contains(paramConnectionName.isEmpty() ? "qt_sql_default_connection" : paramConnectionName)) {
            tempDBNew = QSqlDatabase::database(paramConnectionName.isEmpty() ? "qt_sql_default_connection" : paramConnectionName);
        } else {
            tempDBNew = QSqlDatabase::addDatabase("QSQLITE",
                                                 paramConnectionName.isEmpty() ? "qt_sql_default_connection" : paramConnectionName);
            tempDBNew.setDatabaseName(hostName);
        }
        break;
    }
    default:
        break;
    }

    if (!tempDBNew.isOpen()) {
        tempDBNew.open();
    }
    if (!tempDBNew.isOpenError()) {
        QSqlQuery tempQuery(tempDBNew);
        tempQuery.exec(testSql);
        if (tempQuery.lastError().type() == QSqlError::NoError) {
            return tempDBNew;
        } else {
            QLOG_ERROR() << paramConnectionName << ":" << tempDBNew.lastError().text() << ";" << tempQuery.lastError().type();
        }
    } else {
        if (isDebug) {
            QLOG_DEBUG() << "Open old database error-Create a new connection:" << tempDBNew.lastError().text();
        }
        return QSqlDatabase();
    }

    return tempDBNew;
}

/** @brief 关闭数据库连接
 *
 *  关闭指定的数据库连接并从连接池中移除。
 *
 *  @param paramConnectionName 连接名称
 *  @param isdirectdelete 是否直接删除连接
 *  @return 关闭成功返回 true，否则返回 false
 */
bool NDBPool_p::closeConnection(const QString &paramConnectionName, bool isdirectdelete) {
    QMutexLocker tempLock(&mutex);
    if (QSqlDatabase::contains(paramConnectionName)) {
        if (!isdirectdelete) {
            QSqlDatabase tempDBNew = QSqlDatabase::database(paramConnectionName);
            if (tempDBNew.isOpen()) {
                tempDBNew.close();
            }
        }

        QSqlDatabase::removeDatabase(paramConnectionName);
        return true;
    }
    if (isDebug) {
        QLOG_DEBUG() << "The database connection does not exist:" << paramConnectionName;
    }
    return false;
}

/** @brief 获取主机名
 *
 *  @return 当前设置的主机名
 */
QString NDBPool_p::getHostName() const {
    return hostName;
}

/** @brief 获取数据库类型
 *
 *  @return 当前设置的数据库类型
 */
DB_Type NDBPool_p::getDBType() const {
    return m_DB_Type;
}

/** @brief 设置数据库类型
 *
 *  @param ptype 数据库类型
 */
void NDBPool_p::setDBType(DB_Type ptype) {
    m_DB_Type = ptype;
}

/** @brief 设置主机名
 *
 *  @param value 主机名
 */
void NDBPool_p::setHostName(const QString &value) {
    hostName = value;
}

/** @brief 获取数据库名
 *
 *  @return 当前设置的数据库名
 */
QString NDBPool_p::getDatabaseName() const {
    return databaseName;
}

/** @brief 设置数据库名
 *
 *  @param value 数据库名
 */
void NDBPool_p::setDatabaseName(const QString &value) {
    databaseName = value;
}

/** @brief 获取用户名
 *
 *  @return 当前设置的用户名
 */
QString NDBPool_p::getUsername() const {
    return username;
}

/** @brief 设置用户名
 *
 *  @param value 用户名
 */
void NDBPool_p::setUsername(const QString &value) {
    username = value;
}

/** @brief 获取密码
 *
 *  @return 当前设置的密码
 */
QString NDBPool_p::getPassword() const {
    return password;
}

/** @brief 设置密码
 *
 *  @param value 密码
 */
void NDBPool_p::setPassword(const QString &value) {
    password = value;
}

/** @brief 获取端口号
 *
 *  @return 当前设置的端口号
 */
int NDBPool_p::getPort() const {
    return port;
}

/** @brief 设置端口号
 *
 *  @param value 端口号
 */
void NDBPool_p::setPort(int value) {
    port = value;
}

/** @brief 获取最大连接数
 *
 *  @return 当前设置的最大连接数
 */
int NDBPool_p::getMaxConnectionCount() const {
    return maxConnectionCount;
}

/** @brief 设置最大连接数
 *
 *  @param value 最大连接数
 */
void NDBPool_p::setMaxConnectionCount(int value) {
    maxConnectionCount = value;
}

/** @brief 获取最大等待时间
 *
 *  @return 当前设置的最大等待时间（毫秒）
 */
int NDBPool_p::getMaxWaitTime() const {
    return maxWaitTime;
}

/** @brief 设置最大等待时间
 *
 *  @param value 最大等待时间（毫秒）
 */
void NDBPool_p::setMaxWaitTime(int value) {
    maxWaitTime = value;
}