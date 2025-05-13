/*
  NDBPool - 数据库连接池管理类
*/

#include "ndbpool.h"
#include "QsLog.h"

#include <QThread>

/** @brief 初始化单例类 NDBPool
 */
initialiseSingleton(NDBPool);

/** @brief 连接池映射表
 */
QMap<QString, NDBPool_p *> NDBPool::poolMap;

/** @brief 调试模式标志
 */
bool NDBPool::isDebug = false;

/** @brief 连接活跃时间记录
 */
QMap<QString, connectActive> NDBPool::connectionLastActiveTimeMap;

/** @brief 连接池互斥锁
 */
QMutex NDBPool::mLock;

/** @brief 释放所有连接池资源
 *
 *  释放所有数据库连接池对象。
 */
void NDBPool::release() {
    QMutexLocker locker(&mLock);
    foreach (QString hostName, poolMap.keys()) {
        if (NDBPool_p *tempDBPool = poolMap.value(hostName)) {
            delete tempDBPool;
            tempDBPool = nullptr;
        }
    }
}

/** @brief 测试数据库连接
 *
 *  测试指定参数是否能够成功连接数据库。
 *
 *  @param paramDB_Type 数据库类型
 *  @param paramHostName 主机名
 *  @param paramDatabaseName 数据库名
 *  @param paramUserName 用户名
 *  @param paramPassword 密码
 *  @param paramPort 端口号
 *  @return 连接成功返回 true，否则返回 false
 */
bool NDBPool::IsConnectioned(DB_Type paramDB_Type,
                             QString paramHostName,
                             QString paramDatabaseName,
                             QString paramUserName,
                             QString paramPassword,
                             int paramPort) {
    QSqlDatabase tempDBNew;

    switch (paramDB_Type) {
    case QMYSQL: {
        tempDBNew = QSqlDatabase::addDatabase("QMYSQL", QUuid::createUuid().toString());
        tempDBNew.setHostName(paramHostName);
        tempDBNew.setDatabaseName(paramDatabaseName);
        tempDBNew.setUserName(paramUserName);
        tempDBNew.setPassword(paramPassword);
        tempDBNew.setPort(paramPort);
        break;
    }
    case QODBC: {
        tempDBNew = QSqlDatabase::addDatabase("QODBC", QUuid::createUuid().toString());
        tempDBNew.setDatabaseName(QString("DRIVER={SQL SERVER};"
                                          "SERVER=%1,%5;"
                                          "DATABASE=%2;"
                                          "UID=%3;"
                                          "PWD=%4;")
                                      .arg(paramHostName)
                                      .arg(paramDatabaseName)
                                      .arg(paramUserName)
                                      .arg(paramPassword)
                                      .arg(paramPort));
        break;
    }
    case QSQLITE: {
        tempDBNew = QSqlDatabase::addDatabase("QSQLITE");
        tempDBNew.setDatabaseName(paramHostName);
        break;
    }
    default:
        break;
    }

    if (!tempDBNew.open()) {
        return false;
    }

    return true;
}

/** @brief 获取新的数据库连接
 *
 *  根据参数从连接池获取或创建新的数据库连接，支持长连接和短连接。
 *
 *  @param paramDB_Type 数据库类型
 *  @param paramHostName 主机名
 *  @param paramDatabaseName 数据库名
 *  @param paramUserName 用户名
 *  @param paramPassword 密码
 *  @param paramPort 端口号
 *  @param longConnect 是否为长连接
 *  @param longConnnectName 长连接名称
 *  @return 有效的数据库连接对象，若失败则返回空对象
 */
QSqlDatabase NDBPool::getNewConnection(DB_Type paramDB_Type,
                                      QString paramHostName,
                                      QString paramDatabaseName,
                                      QString paramUserName,
                                      QString paramPassword,
                                      int paramPort,
                                      bool longConnect,
                                      QString longConnnectName) {
    connectActive tempItem;
    tempItem.poolName = paramHostName + tr("_") + paramDatabaseName;
    tempItem.curthread = QThread::currentThreadId();

    if (poolMap.contains(tempItem.poolName)) {
        NDBPool_p *dbConnectPtr = NDBPool::getSingleton().poolMap.value(tempItem.poolName);
        if (dbConnectPtr->semaphore.tryAcquire(1, dbConnectPtr->getMaxWaitTime())) {
            mLock.lock();
            if (longConnect) {
                QSqlDatabase innerDB = dbConnectPtr->getNewConnection(longConnnectName);
                tempItem.lastActiveTime = QDateTime::currentMSecsSinceEpoch();
                tempItem.longConnect = true;
                connectionLastActiveTimeMap.insert(longConnnectName, tempItem);
                mLock.unlock();
                if (!innerDB.isOpen()) {
                    mLock.lock();
                    if (isDebug) {
                        QLOG_DEBUG() << "long connection:" << longConnnectName << "is dead link!";
                    }
                    dbConnectPtr->semaphore.release();
                    mLock.unlock();
                    return QSqlDatabase();
                } else {
                    mLock.lock();
                    if (!dbConnectPtr->usedConnectionName.contains(longConnnectName))
                        dbConnectPtr->usedConnectionName.push(longConnnectName);
                    dbConnectPtr->semaphore.release();
                    mLock.unlock();
                    return innerDB;
                }
            } else {
                QString tempConnectionName = dbConnectPtr->unusedConnectionName.length() > 0 ? dbConnectPtr->unusedConnectionName.pop() : QUuid::createUuid().toString();
                QSqlDatabase innerDB = dbConnectPtr->getNewConnection(tempConnectionName);
                tempItem.lastActiveTime = QDateTime::currentMSecsSinceEpoch();
                tempItem.longConnect = false;
                connectionLastActiveTimeMap.insert(tempConnectionName, tempItem);
                mLock.unlock();
                if (!innerDB.isOpen()) {
                    mLock.lock();
                    dbConnectPtr->semaphore.release();
                    if (isDebug) {
                        QLOG_DEBUG() << "short connection:" << tempConnectionName << "is dead link!";
                    }
                    mLock.unlock();
                    return QSqlDatabase();
                } else {
                    mLock.lock();
                    dbConnectPtr->usedConnectionName.push(tempConnectionName);
                    dbConnectPtr->semaphore.release();
                    mLock.unlock();
                    return innerDB;
                }
            }
        } else {
            if (isDebug) {
                QLOG_DEBUG() << "Time out to create connection.";
            }
            return QSqlDatabase();
        }
    } else {
        mLock.lock();
        auto *tempPool = new NDBPool_p();
        poolMap.insert(tempItem.poolName, tempPool);
        tempPool->setHostName(paramHostName);
        tempPool->setDatabaseName(paramDatabaseName);
        tempPool->setUsername(paramUserName);
        tempPool->setPassword(paramPassword);
        tempPool->setPort(paramPort);
        tempPool->setDBType(paramDB_Type);
        mLock.unlock();

        return getNewConnection(paramDB_Type, paramHostName, paramDatabaseName, paramUserName, paramPassword, paramPort, longConnect, longConnnectName);
    }
}

/** @brief 关闭数据库连接
 *
 *  关闭指定的数据库连接，并将其标记为未使用或从连接池移除。
 *
 *  @param connection 要关闭的数据库连接
 */
void NDBPool::closeConnection(const QSqlDatabase &connection) {
    QMutexLocker locker(&mLock);
    bool tempFlag = false;
    bool longConnenct = false;
    QString tempConnectionName = connection.connectionName();
    /*foreach (QString hostName, poolMap.keys()) {
        NDBPool_p * tempPool = poolMap.value(hostName);
        if(tempPool->usedConnectionName.contains(tempConnectionName)){
            // 长连接关闭连接
            if(connectionLastActiveTimeMap[tempConnectionName].longConnect){
                tempPool->closeConnection(tempConnectionName);
                connectionLastActiveTimeMap.remove(tempConnectionName);
                longConnenct = true;
            } else {
                tempPool->unusedConnectionName.push(tempConnectionName);
            }
            tempPool->usedConnectionName.removeOne(tempConnectionName);
            tempFlag = true;
            tempPool->semaphore.release();
        }
    }*/
    for (auto it = poolMap.begin(); it != poolMap.end(); ++it) {
        NDBPool_p *tempPool = it.value();
        if (tempPool->usedConnectionName.contains(tempConnectionName)) {
            if (connectionLastActiveTimeMap[tempConnectionName].longConnect) {
                tempPool->closeConnection(tempConnectionName);
                connectionLastActiveTimeMap.remove(tempConnectionName);
                longConnenct = true;
            } else {
                tempPool->unusedConnectionName.push(tempConnectionName);
            }
            tempPool->usedConnectionName.removeOne(tempConnectionName);
            tempFlag = true;
            tempPool->semaphore.release();
        }
    }

    if (isDebug && tempFlag) {
        if (longConnenct) {
            QLOG_DEBUG() << QString("long connect %1 is exist and closed").arg(tempConnectionName);
        } else {
            QLOG_DEBUG() << QString("short connect %1 is exist and closed").arg(tempConnectionName);
        }
    }
}

/** @brief 构造函数
 *
 *  初始化数据库连接池管理对象，设置调试模式和定时器以强制断开超时连接。
 *
 *  @param paramIsDebug 是否启用调试模式
 *  @param parent 父对象
 */
NDBPool::NDBPool(bool paramIsDebug, QObject *parent)
    : QObject(parent) {
    isDebug = paramIsDebug;
    checkForceBreakInterval = 1000;
    forceBreakTime = 120000;
    connect(&forceBreakTimer, SIGNAL(timeout()),
            this, SLOT(slots_forceBreak()),
            Qt::QueuedConnection);
    forceBreakTimer.setInterval(checkForceBreakInterval);
    forceBreakTimer.start();
}

/** @brief 析构函数
 *
 *  销毁数据库连接池管理对象，释放所有连接池资源。
 */
NDBPool::~NDBPool() {
    QMutexLocker locker(&mLock);
    for (auto it = poolMap.begin(); it != poolMap.end(); ++it) {
        if (const NDBPool_p *tempDBPool = it.value()) {
            delete tempDBPool;
        }
    }
    poolMap.clear();
}

/** @brief 强制断开超时连接
 *
 *  检查并关闭超时的数据库连接，将其标记为未使用或从连接池移除。
 */
void NDBPool::slots_forceBreak() {
    QMutexLocker tempLocker(&mLock);
    int use_count = 0;
    int unuse_count = 0;
    int long_connection = 0;
    foreach (QString connectionName, connectionLastActiveTimeMap.keys()) {
        if (connectionLastActiveTimeMap[connectionName].longConnect) {
            long_connection++;
            continue;
        }
        qint64 timeDiff = QDateTime::currentMSecsSinceEpoch() - connectionLastActiveTimeMap.value(connectionName).lastActiveTime;
        NDBPool_p *tempConnection = poolMap.value(connectionLastActiveTimeMap.value(connectionName).poolName);
        if (timeDiff >= forceBreakTime) {
            if (tempConnection->usedConnectionName.contains(connectionName)) {
                tempConnection->usedConnectionName.removeOne(connectionName);
                tempConnection->unusedConnectionName.push(connectionName);
            }
            if (tempConnection->unusedConnectionName.contains(connectionName)) {
                tempConnection->unusedConnectionName.removeOne(connectionName);
                tempConnection->closeConnection(connectionName,
                                               connectionLastActiveTimeMap[connectionName].curthread == QThread::currentThreadId() ? false : true);
            }
            connectionLastActiveTimeMap.remove(connectionName);
        }
        use_count = tempConnection->usedConnectionName.count();
        unuse_count = tempConnection->unusedConnectionName.count();
    }
    if (isDebug) {
        QLOG_DEBUG() << "using connect total:" << use_count;
        QLOG_DEBUG() << "using long connect total:" << long_connection;
        QLOG_DEBUG() << "no using connect total:" << unuse_count;
    }
}