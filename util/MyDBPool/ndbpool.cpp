#include "ndbpool.h"
#include "QsLog.h"

#include <QThread>

initialiseSingleton(NDBPool);

QMap<QString, NDBPool_p *> NDBPool::poolMap;
bool NDBPool::isDebug = false;
QMap<QString, connectActive> NDBPool::connectionLastActiveTimeMap;
QMutex NDBPool::mLock;

void NDBPool::release()
{
    QMutexLocker locker(&(NDBPool::mLock));
    foreach (QString hostName, poolMap.keys()) {
        if(NDBPool_p * tempDBPool = poolMap.value(hostName)){
            delete tempDBPool;
            tempDBPool = nullptr;
        }
    }
}

bool NDBPool::IsConnectioned(DB_Type paramDB_Type,
                                 QString paramHostName,
                                 QString paramDatabaseName,
                                 QString paramUserName,
                                 QString paramPassword,
                                 int paramPort)
{
    QSqlDatabase tempDBNew;

    switch (paramDB_Type) {
    case QMYSQL:
    {
        tempDBNew = QSqlDatabase::addDatabase("QMYSQL", QUuid::createUuid().toString());
        tempDBNew.setHostName(paramHostName);
        tempDBNew.setDatabaseName(paramDatabaseName);
        tempDBNew.setUserName(paramUserName);
        tempDBNew.setPassword(paramPassword);
        tempDBNew.setPort(paramPort);
    }
        break;
    case QODBC:
    {
        tempDBNew = QSqlDatabase::addDatabase("QODBC", QUuid::createUuid().toString());
        tempDBNew.setDatabaseName(QString("DRIVER={SQL SERVER};"
                                          "SERVER=%1,%5;"
                                          "DATABASE=%2;"
                                          "UID=%3;"
                                          "PWD=%4;"
                                          )
                                  .arg(paramHostName)
                                  .arg(paramDatabaseName)
                                  .arg(paramUserName)
                                  .arg(paramPassword)
                                  .arg(paramPort));
    }
        break;
    case QSQLITE:
    {
        tempDBNew = QSqlDatabase::addDatabase("QSQLITE");
        tempDBNew.setDatabaseName(paramHostName);
    }
        break;
    default:
        break;
    }

    if(!tempDBNew.open()){
        return false;
    }

    return true;
}

QSqlDatabase NDBPool::getNewConnection(DB_Type paramDB_Type,QString paramHostName, QString paramDatabaseName, QString paramUserName, QString paramPassword,
                                       int paramPort, bool longConnect, QString longConnnectName)
{
    connectActive tempItem;
    tempItem.poolName = paramHostName+tr("_")+paramDatabaseName;
    tempItem.curthread = QThread::currentThreadId();

    // 选择连接池
    if(poolMap.contains(tempItem.poolName)){
        NDBPool_p * dbConnectPtr = NDBPool::getSingleton().poolMap.value(tempItem.poolName);
        // 尝试获取一个连接
        if(dbConnectPtr->semaphore.tryAcquire(1, dbConnectPtr->getMaxWaitTime())){
            NDBPool::mLock.lock();
            // 依据长短连接从连接池中获取
            if(longConnect){
                // 长连接
                QSqlDatabase innerDB = dbConnectPtr->getNewConnection(longConnnectName);
                tempItem.lastActiveTime = QDateTime::currentMSecsSinceEpoch();
                tempItem.longConnect = true;
                connectionLastActiveTimeMap.insert(longConnnectName, tempItem);
                NDBPool::mLock.unlock();
                if(!innerDB.isOpen()){
                    NDBPool::mLock.lock();
                    if(isDebug){
                        QLOG_DEBUG()<<"long connection:"<<longConnnectName<<"is dead link!";
                    }
                    dbConnectPtr->semaphore.release();
                    NDBPool::mLock.unlock();
                    return QSqlDatabase();
                } else {
                    NDBPool::mLock.lock();
                    if(!dbConnectPtr->usedConnectionName.contains(longConnnectName))
                        dbConnectPtr->usedConnectionName.push(longConnnectName);
                    dbConnectPtr->semaphore.release();
                    NDBPool::mLock.unlock();
                    return innerDB;
                }
            } else {
                // 短连接
                QString tempConnectionName = dbConnectPtr->unusedConnectionName.length()>0 ? dbConnectPtr->unusedConnectionName.pop() : QUuid::createUuid().toString();
                QSqlDatabase innerDB = dbConnectPtr->getNewConnection(tempConnectionName);
                tempItem.lastActiveTime = QDateTime::currentMSecsSinceEpoch();
                tempItem.longConnect = false;
                connectionLastActiveTimeMap.insert(tempConnectionName, tempItem);
                NDBPool::mLock.unlock();
                if(!innerDB.isOpen()){
                    NDBPool::mLock.lock();
                    dbConnectPtr->semaphore.release();
                    if(isDebug){
                        QLOG_DEBUG()<<"short connection:"<<tempConnectionName<<"is dead link!";
                    }
                    NDBPool::mLock.unlock();
                    return QSqlDatabase();
                } else {
                    NDBPool::mLock.lock();
                    dbConnectPtr->usedConnectionName.push(tempConnectionName);
                    dbConnectPtr->semaphore.release();
                    NDBPool::mLock.unlock();
                    return innerDB;
                }
            }
        } else {
            if(isDebug){
                QLOG_DEBUG() << "Time out to create connection.";
            }
            return QSqlDatabase();
        }
    } else {
        // 链接池不存在则创建并使用连接池
        NDBPool::mLock.lock();
        NDBPool_p * tempPool = new NDBPool_p();
        poolMap.insert(tempItem.poolName, tempPool);
        tempPool->setHostName(paramHostName);
        tempPool->setDatabaseName(paramDatabaseName);
        tempPool->setUsername(paramUserName);
        tempPool->setPassword(paramPassword);
        tempPool->setPort(paramPort);
        tempPool->setDBType(paramDB_Type);
        NDBPool::mLock.unlock();

        return getNewConnection(paramDB_Type,paramHostName,paramDatabaseName,paramUserName,paramPassword,paramPort,longConnect,longConnnectName);
    }
}

void NDBPool::closeConnection(const QSqlDatabase &connection)
{
    QMutexLocker locker(&(NDBPool::mLock));
    bool tempFlag = false;
    bool longConnenct = false;
    QString tempConnectionName = connection.connectionName();
    foreach (QString hostName, poolMap.keys()) {
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
    }
    if(isDebug && tempFlag){
        if(longConnenct){
            QLOG_DEBUG()<<QString("long connect %1 is exist and closed").arg(tempConnectionName);
        } else {
            QLOG_DEBUG()<<QString("short connect %1 is exist and closed").arg(tempConnectionName);
        }
    }
}

NDBPool::NDBPool(bool paramIsDebug, QObject *parent):QObject(parent)
{
    // 调试模式
    isDebug = paramIsDebug;
    // 强制断开连接检查频率
    checkForceBreakInterval = 1000;
    // 强制断开连接的时间差
    forceBreakTime = 120000;
    // 强制断开连接
    connect(&forceBreakTimer, SIGNAL(timeout()),
            this,SLOT(slots_forceBreak()),
            Qt::QueuedConnection);
    forceBreakTimer.setInterval(checkForceBreakInterval);
    forceBreakTimer.start();
}

NDBPool::~NDBPool()
{
    QMutexLocker locker(&(NDBPool::mLock));
    foreach (QString hostName, poolMap.keys()) {
        NDBPool_p * tempDBPool = poolMap.value(hostName);
        if(tempDBPool){
            delete tempDBPool;
            tempDBPool = nullptr;
        }
    }
}

void NDBPool::slots_forceBreak()
{
    QMutexLocker tempLocker(&NDBPool::mLock);
    int use_count = 0;
    int unuse_count = 0;
    int long_connection = 0;
    foreach (QString connectionName, connectionLastActiveTimeMap.keys()) {
        if(connectionLastActiveTimeMap[connectionName].longConnect){
            long_connection++;
            continue;
        }
        qint64 timeDiff = QDateTime::currentMSecsSinceEpoch() - connectionLastActiveTimeMap.value(connectionName).lastActiveTime;
        NDBPool_p * tempConnection = poolMap.value(connectionLastActiveTimeMap.value(connectionName).poolName);
        if(timeDiff >= forceBreakTime){
            if(tempConnection->usedConnectionName.contains(connectionName)){
                tempConnection->usedConnectionName.removeOne(connectionName);
                tempConnection->unusedConnectionName.push(connectionName);
            }
            if(tempConnection->unusedConnectionName.contains(connectionName)){
                tempConnection->unusedConnectionName.removeOne(connectionName);
                tempConnection->closeConnection(connectionName,
                                                connectionLastActiveTimeMap[connectionName].curthread == QThread::currentThreadId() ? false : true);
            }
            connectionLastActiveTimeMap.remove(connectionName);
        }
        use_count = tempConnection->usedConnectionName.count();
        unuse_count = tempConnection->unusedConnectionName.count();
    }
    if(isDebug){
        QLOG_DEBUG()<<"using connect total:"<<use_count;
        QLOG_DEBUG()<<"using long connect total:"<<long_connection;
        QLOG_DEBUG()<<"no using connect total:"<<unuse_count;
    }
}
