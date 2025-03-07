#include "ndbpool_p.h"
#include "QsLog.h"

#include <QSqlError>

QMutex NDBPool_p::mutex;

NDBPool_p::NDBPool_p(QObject *parent, bool paramDebug):QObject(parent),m_DB_Type(TYPE_NULL)
{
    // 读取配置文件获取并设置数据库的基本信息
    setMaxConnectionCount(50);
    setMaxWaitTime(10);
    // 获取连接数量的互斥量
    semaphore.release(getMaxConnectionCount());
    // 设置程序的运行级别
    isDebug = paramDebug;
    // 测试数据连接通畅性的SQL语句
    testSql = "select 0;";
}

NDBPool_p::~NDBPool_p()
{
    // 销毁所有正在连接的数据库连接
    foreach (QString connectionName, usedConnectionName) {
        getNewConnection(connectionName).close();
        if(isDebug){
            QLOG_DEBUG()<<"close connected connectionName:" << connectionName;
        }
        if(QSqlDatabase::contains(connectionName)){
            closeConnection(connectionName);
        }
    }
    // 销毁所有处于等待状态的短数据库连接
    foreach (QString connectionName, unusedConnectionName) {
        if(isDebug){
            QLOG_DEBUG()<<"close unconnected short connectionName:" << connectionName;
        }
        if(QSqlDatabase::contains(connectionName)){
            closeConnection(connectionName);
        }
    }
}

QSqlDatabase NDBPool_p::getNewConnection(const QString &paramConnectionName)
{
    QMutexLocker tempLock(&mutex);
    if(QSqlDatabase::contains(paramConnectionName)){
        QSqlDatabase tempDB = QSqlDatabase::database(paramConnectionName);
        if(testSql.length()!=0){
            if(!tempDB.isOpen()){
                tempDB.open();
            }
            if(!tempDB.isOpenError()){
                QSqlQuery tempQuery = tempDB.exec(testSql);
                if((tempQuery.lastError().type() == QSqlError::NoError)){
                    return tempDB;
                }
                else
                {
                    QLOG_ERROR()<<paramConnectionName<<":"<<tempDB.lastError().text()<<";"<<tempQuery.lastError().type();
                }
            } else {
                if(isDebug){
                    QLOG_DEBUG() << "Open old datatabase error-Create a new connection:" << tempDB.lastError().text();
                }
                return QSqlDatabase();
            }
        }
    }

    QSqlDatabase tempDBNew;

    switch (m_DB_Type) {
    case QMYSQL:
    {
        tempDBNew = QSqlDatabase::addDatabase("QMYSQL", paramConnectionName);
        tempDBNew.setHostName(hostName);
        tempDBNew.setDatabaseName(databaseName);
        tempDBNew.setUserName(username);
        tempDBNew.setPassword(password);
        tempDBNew.setPort(port);
    }
        break;
    case QODBC:
    {
        tempDBNew = QSqlDatabase::addDatabase("QODBC", paramConnectionName);
        tempDBNew.setDatabaseName(QString("DRIVER={SQL SERVER};"
                                          "SERVER=%1,%5;"
                                          "DATABASE=%2;"
                                          "UID=%3;"
                                          "PWD=%4;"
                                          )
                                  .arg(hostName)
                                  .arg(databaseName)
                                  .arg(username)
                                  .arg(password)
                                  .arg(port));
    }
        break;
    case QSQLITE:
    {
        if(QSqlDatabase::contains(paramConnectionName.isEmpty() ? "qt_sql_default_connection" : paramConnectionName))
        {
            tempDBNew = QSqlDatabase::database(paramConnectionName.isEmpty() ? "qt_sql_default_connection" : paramConnectionName);
        }
        else
        {
          tempDBNew = QSqlDatabase::addDatabase("QSQLITE",
                                                paramConnectionName.isEmpty() ? "qt_sql_default_connection" : paramConnectionName);
          tempDBNew.setDatabaseName(hostName);
        }
    }
        break;    
    default:
        break;
    }

    if(!tempDBNew.isOpen()){
        tempDBNew.open();
    }
    if(!tempDBNew.isOpenError()){
        QSqlQuery tempQuery = tempDBNew.exec(testSql);
        if((tempQuery.lastError().type() == QSqlError::NoError)){
            return tempDBNew;
        }
        else
        {
            QLOG_ERROR()<<paramConnectionName<<":"<<tempDBNew.lastError().text()<<";"<<tempQuery.lastError().type();
        }
    } else {
        if(isDebug){
            QLOG_DEBUG() << "Open old datatabase error-Create a new connection:" << tempDBNew.lastError().text();
        }
        return QSqlDatabase();
    }

    return tempDBNew;
}

bool NDBPool_p::closeConnection(const QString &paramConnectionName,bool isdirectdelete)
{
    QMutexLocker tempLock(&mutex);
    if(QSqlDatabase::contains(paramConnectionName))
    {
        if(!isdirectdelete)
        {
            QSqlDatabase tempDBNew = QSqlDatabase::database(paramConnectionName);
            if(tempDBNew.isOpen()){
                tempDBNew.close();
            }
        }

        QSqlDatabase::removeDatabase(paramConnectionName);
        return true;
    }
    if(isDebug){
        QLOG_DEBUG()<<"The database connection does not exist:"<< paramConnectionName;
    }
    return false;
}

QString NDBPool_p::getHostName() const
{
    return hostName;
}

DB_Type NDBPool_p::getDBType() const
{
    return m_DB_Type;
}

void NDBPool_p::setDBType(DB_Type ptype)
{
    m_DB_Type = ptype;
}

void NDBPool_p::setHostName(const QString &value)
{
    hostName = value;
}

QString NDBPool_p::getDatabaseName() const
{
    return databaseName;
}

void NDBPool_p::setDatabaseName(const QString &value)
{
    databaseName = value;
}

QString NDBPool_p::getUsername() const
{
    return username;
}

void NDBPool_p::setUsername(const QString &value)
{
    username = value;
}

QString NDBPool_p::getPassword() const
{
    return password;
}

void NDBPool_p::setPassword(const QString &value)
{
    password = value;
}

int NDBPool_p::getPort() const
{
    return port;
}

void NDBPool_p::setPort(int value)
{
    port = value;
}

int NDBPool_p::getMaxConnectionCount() const
{
    return maxConnectionCount;
}

void NDBPool_p::setMaxConnectionCount(int value)
{
    maxConnectionCount = value;
}

int NDBPool_p::getMaxWaitTime() const
{
    return maxWaitTime;
}

void NDBPool_p::setMaxWaitTime(int value)
{
    maxWaitTime = value;
}
