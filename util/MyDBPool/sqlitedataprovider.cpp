#include "sqlitedataprovider.h"
#include "ndbpool.h"
#include "QsLog.h"
#include <QSqlError>
#include <QSqlRecord>

//initialiseSingleton(SqliteDataProvider);

/**
 * @brief SqliteDataProvider::SqliteDataProvider 构造函数
 * @param parent
 */
SqliteDataProvider::SqliteDataProvider(QObject *parent)
    : QObject(parent)
{
}

/**
 * @brief SqliteDataProvider::~SqliteDataProvider 析构函数
 */
SqliteDataProvider::~SqliteDataProvider()
{
    disconnect();
}

/**
 * 建立与数据库的连接
 *
 * @param dbPath 要连接的数据库的路径
 */
bool SqliteDataProvider::connect(const QString &dbPath)
{
    m_dbPath = dbPath;
    return true;
}

/**
 * @brief execTransaction 执行事务
 * @param sqls
 * @param connectionName
 * @param longConnect
 * @return
 */
RecordSetList SqliteDataProvider::execTransaction(const QStringList& sqls,QString connectionName,bool longConnect)
{
    RecordSetList pRecordSetList;

    QSqlDatabase tempDB = NDBPool::getNewConnection(QSQLITE,m_dbPath,"","","",1433,longConnect,connectionName);
    //QLOG_INFO() << " connection name:" << tempDB.connectionName() << "is vaild:" << tempDB.isOpen() << "\n";

    if(!tempDB.isOpen())
    {
        QLOG_INFO() << " connection name:" << tempDB.connectionName() << "is valid:" << tempDB.isOpen() << "\n";
        return pRecordSetList;
    }

    QSqlQuery queryresult(tempDB);

    bool isOk = true;

    if(tempDB.transaction())
    {
        for(int i=0;i<sqls.size();i++)
        {
            if(!queryresult.exec(sqls[i]))
            {
                isOk = false;
                break;
            }
        }

        if(tempDB.commit())
        {
            if(isOk)
            {
                try {
                    do
                    {
                        RecordSet pRecordSet;
                        QSqlRecord precord = queryresult.record();

                        Row fieldNames;
                        for(int i=0;i<precord.count();i++)
                        {
                            //qDebug()<<"query:"<<precord.fieldName(i);
                            fieldNames.push_back(precord.fieldName(i));
                        }

                        pRecordSet.setColumnHeaders(fieldNames);

                        while(queryresult.next())
                        {
                            Row fieldDatas;
                            for(int i=0;i<precord.count();i++)
                            {
                                fieldDatas.push_back(queryresult.value(i).toString());
                            }

                            pRecordSet.add(fieldDatas);
                        }

                        if(!pRecordSet.isEmpty())
                            pRecordSetList.add(pRecordSet);
                    }
                    while(queryresult.nextResult());
                } catch (...) {
                    QLOG_ERROR()<<"query error:"<<queryresult.lastError().text();
                }
            }
        }
        else
        {
            tempDB.rollback();
        }
    }

    return pRecordSetList;
}

/**
 * 执行SQL语句
 *
 * @param sql 要执行的SQL语句
 *
 * @return 如果成功获取到数据返回这个数据记录，否则抛出异常
 */
RecordSetList SqliteDataProvider::execSql(const QString& sql,
                                                const QString &connectionName,
                                                bool longConnect)
{
    RecordSetList pRecordSetList;

    const QSqlDatabase tempDB = NDBPool::getNewConnection(QSQLITE,m_dbPath,"","","",1433,longConnect,connectionName);

    if(!tempDB.isOpen())
    {
        qDebug() << " connection name:" << tempDB.connectionName() << "is vaild:" << tempDB.isOpen() << "\n";
        QLOG_INFO() << " connection name:" << tempDB.connectionName() << "is vaild:" << tempDB.isOpen() << "\n";
        return pRecordSetList;
    }

    QSqlQuery query_result(tempDB);

    if(query_result.exec(sql))
    {
        try {
            do
            {
                RecordSet pRecordSet;
                QSqlRecord precord = query_result.record();

                Row fieldNames;
                for(int i=0;i<precord.count();i++)
                {
                    //qDebug()<<"query:"<<precord.fieldName(i);
                    fieldNames.push_back(precord.fieldName(i));
                }

                pRecordSet.setColumnHeaders(fieldNames);

                while(query_result.next())
                {
                    Row fieldDatas;
                    for(int i=0;i<precord.count();i++)
                    {
                        fieldDatas.push_back(query_result.value(i).toString());
                    }

                    pRecordSet.add(fieldDatas);
                }

                if(!pRecordSet.isEmpty())
                    pRecordSetList.add(pRecordSet);
            }
            while(query_result.nextResult());
        } catch (...) {
            QLOG_ERROR()<<"query error:"<<query_result.lastError().text();
        }
    }

    //NDBPool::closeConnection(tempDB);

    return pRecordSetList;
}

/**
 * @brief SqliteDataProvider::execSqls 执行多条语句
 * @param sqls 要执行的SQL语句
 *
 * @return 如果成功获取到数据返回这个数据记录，否则抛出异常
 */
RecordSetList SqliteDataProvider::execSqls(const QStringList& sqls,
                                                 QString connectionName,
                                                 bool longConnect)
{
    RecordSetList pRecordSetList;

    const QSqlDatabase tempDB = NDBPool::getNewConnection(QSQLITE,m_dbPath,"","","",1433,longConnect,connectionName);
    //QLOG_INFO() << " connection name:" << tempDB.connectionName() << "is vaild:" << tempDB.isOpen() << "\n";

    if(!tempDB.isOpen())
    {
        QLOG_INFO() << " connection name:" << tempDB.connectionName() << "is vaild:" << tempDB.isOpen() << "\n";
        return pRecordSetList;
    }

    QSqlQuery queryresult(tempDB);

    bool isOk = true;

    for(int i=0;i<sqls.size();i++)
    {
        if(!queryresult.exec(sqls[i]))
        {
            isOk = false;
            break;
        }
    }

    if(isOk)
    {
        try {
            do
            {
                RecordSet pRecordSet;
                QSqlRecord precord = queryresult.record();

                Row fieldNames;
                for(int i=0;i<precord.count();i++)
                {
                    //qDebug()<<"query:"<<precord.fieldName(i);
                    fieldNames.push_back(precord.fieldName(i));
                }

                pRecordSet.setColumnHeaders(fieldNames);

                while(queryresult.next())
                {
                    Row fieldDatas;
                    for(int i=0;i<precord.count();i++)
                    {
                        fieldDatas.push_back(queryresult.value(i).toString());
                    }

                    pRecordSet.add(fieldDatas);
                }

                if(!pRecordSet.isEmpty())
                    pRecordSetList.add(pRecordSet);
            }
            while(queryresult.nextResult());
        } catch (...) {
            QLOG_ERROR()<<"query error:"<<queryresult.lastError().text();
        }
    }

    //NDBPool::closeConnection(tempDB);

    return pRecordSetList;
}

/**
 * @brief SqliteDataProvider::execInsertSql 执行sql的insert语句
 * @param sql 要执行的SQL语句
 *
 * @return 如果成功获取到数据返回这个数据记录，否则抛出异常
 */
RecordSetList SqliteDataProvider::execInsertSql(const QString& sql,
                                                  const QString &connectionName,
                                                  bool longConnect)
{
    RecordSetList pRecordSetList;

    const QSqlDatabase tempDB = NDBPool::getNewConnection(QSQLITE,m_dbPath,"","","",1433,longConnect,connectionName);


    if(!tempDB.isOpen())
    {
        qDebug() << " connection name:" << tempDB.connectionName() << "is vaild:" << tempDB.isOpen() << "\n";
        QLOG_INFO() << " connection name:" << tempDB.connectionName() << "is vaild:" << tempDB.isOpen() << "\n";
        return pRecordSetList;
    }

    QSqlQuery queryresult(tempDB);

    if(queryresult.exec(sql) &&
            queryresult.exec("select last_insert_rowid()"))
    {
        try {
            do
            {
                RecordSet pRecordSet;
                QSqlRecord precord = queryresult.record();

                Row fieldNames;
                for(int i=0;i<precord.count();i++)
                {
                    //qDebug()<<"query:"<<precord.fieldName(i);
                    fieldNames.push_back(precord.fieldName(i));
                }

                pRecordSet.setColumnHeaders(fieldNames);

                while(queryresult.next())
                {
                    Row fieldDatas;
                    for(int i=0;i<precord.count();i++)
                    {
                        fieldDatas.push_back(queryresult.value(i).toString());
                    }

                    pRecordSet.add(fieldDatas);
                }

                if(!pRecordSet.isEmpty())
                    pRecordSetList.add(pRecordSet);
            }
            while(queryresult.nextResult());
        } catch (...) {
            QLOG_ERROR()<<"query error:"<<queryresult.lastError().text();
        }
    }

    //NDBPool::closeConnection(tempDB);

    return pRecordSetList;
}

RecordSetList SqliteDataProvider::execDeleteSql(const QString &sql, const QString &connectionName, bool longConnect) {
    RecordSetList pRecordSetList;

    const QSqlDatabase tempDB = NDBPool::getNewConnection(QSQLITE, m_dbPath, "", "", "", 1433, longConnect, connectionName);

    if (!tempDB.isOpen())
    {
        QLOG_INFO() << " connection name:" << tempDB.connectionName() << "is valid:" << tempDB.isOpen();
        return pRecordSetList;
    }

    QSqlQuery queryresult(tempDB);

    if (queryresult.exec(sql))
    {
        int affected = queryresult.numRowsAffected();

        try {
            // 构建一个包含影响行数的结果
            RecordSet pRecordSet;
            pRecordSet.setColumnHeaders({ "rowsAffected" });

            Row fieldDatas;
            fieldDatas.push_back(QString::number(affected));

            pRecordSet.add(fieldDatas);
            pRecordSetList.add(pRecordSet);
        } catch (...) {
            QLOG_ERROR() << "delete error:" << queryresult.lastError().text();
        }
    }
    else
    {
        QLOG_ERROR() << "delete exec error:" << queryresult.lastError().text();
    }

    return pRecordSetList;
}

/**
 * 关闭与数据库的连接
 */
void SqliteDataProvider::disconnect()
{
    //NDBPool::release();
}

