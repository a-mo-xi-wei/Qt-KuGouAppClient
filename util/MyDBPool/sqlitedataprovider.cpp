#include "../../../includes/database/ndbpool/sqlitedataprovider.h"
#include "../../../includes/database/ndbpool/ndbpool.h"
#include "../../../includes/QsLog/QsLog.h"

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
SqliteDataProvider::~SqliteDataProvider(void)
{
    disconnect();
}

/**
 * 建立与数据库的连接
 *
 * @param dbPath 要连接的数据库的路径
 */
bool SqliteDataProvider::connect(QString dbPath)
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
const RecordSetList SqliteDataProvider::execTransaction(const QStringList& sqls,QString connectionName,bool longConnect)
{
    RecordSetList pRecordSetList;

    QSqlDatabase tempDB = NDBPool::getNewConnection(QSQLITE,m_dbPath,"","","",1433,longConnect,connectionName);
    //QLOG_INFO() << " connection name:" << tempDB.connectionName() << "is vaild:" << tempDB.isOpen() << "\n";

    if(!tempDB.isOpen())
    {
        QLOG_INFO() << " connection name:" << tempDB.connectionName() << "is vaild:" << tempDB.isOpen() << "\n";
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
 * @brief SqliteDataProvider::execSqls 执行多条语句
 * @param sqls 要执行的SQL语句
 *
 * @return 如果成功获取到数据返回这个数据记录，否则抛出异常
 */
const RecordSetList SqliteDataProvider::execSqls(const QStringList& sqls,
                                                 QString connectionName,
                                                 bool longConnect)
{
    RecordSetList pRecordSetList;

    QSqlDatabase tempDB = NDBPool::getNewConnection(QSQLITE,m_dbPath,"","","",1433,longConnect,connectionName);
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
const RecordSetList SqliteDataProvider::execInsertSql(const QString& sql,
                                                      QString connectionName,
                                                      bool longConnect)
{
    RecordSetList pRecordSetList;

    QSqlDatabase tempDB = NDBPool::getNewConnection(QSQLITE,m_dbPath,"","","",1433,longConnect,connectionName);
    //QLOG_INFO() << " connection name:" << tempDB.connectionName() << "is vaild:" << tempDB.isOpen() << "\n";

    if(!tempDB.isOpen())
    {
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

/**
 * 执行SQL语句
 *
 * @param sql 要执行的SQL语句
 *
 * @return 如果成功获取到数据返回这个数据记录，否则抛出异常
 */
const RecordSetList SqliteDataProvider::execSql(const QString& sql,
                                                QString connectionName,
                                                bool longConnect)
{
    RecordSetList pRecordSetList;

    QSqlDatabase tempDB = NDBPool::getNewConnection(QSQLITE,m_dbPath,"","","",1433,longConnect,connectionName);
   // QLOG_INFO() << " connection name:" << tempDB.connectionName() << "is vaild:" << tempDB.isOpen() << "\n";

    if(!tempDB.isOpen())
    {
        QLOG_INFO() << " connection name:" << tempDB.connectionName() << "is vaild:" << tempDB.isOpen() << "\n";
        return pRecordSetList;
    }

    QSqlQuery queryresult(tempDB);

    if(queryresult.exec(sql))
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
 * 关闭与数据库的连接
 */
void SqliteDataProvider::disconnect(void)
{
    //NDBPool::release();
}

