#ifndef SQLITEDATAPROVIDER_H
#define SQLITEDATAPROVIDER_H

#include "common/recordset.h"

#include <QObject>
#include <QString>

class SqliteDataProvider : public QObject
{
    Q_OBJECT

public:
    /// 构造函数
    Q_INVOKABLE SqliteDataProvider(QObject *parent = nullptr);
    /// 析构函数
    ~SqliteDataProvider();

    /// 建立与数据库的连接
    Q_INVOKABLE bool connect(QString dbPath);
    /// 得到数据库文件路径
    Q_INVOKABLE inline QString getdbPath() { return m_dbPath; }
    /// 执行SQL语句
    Q_INVOKABLE const RecordSetList execSql(const QString& sql,QString connectionName="qt_sql_default_connection",bool longConnect=true);
    /// 执行sql的insert语句
    Q_INVOKABLE const RecordSetList execInsertSql(const QString& sql,QString connectionName="qt_sql_default_connection",bool longConnect=true);
    /// 执行多条语句
    Q_INVOKABLE const RecordSetList execSqls(const QStringList& sqls,QString connectionName="qt_sql_default_connection",bool longConnect=true);
    /// 执行事务
    Q_INVOKABLE const RecordSetList execTransaction(const QStringList& sqls,QString connectionName="qt_sql_default_connection",bool longConnect=true);    
    /// 关闭与数据库的连接
    Q_INVOKABLE void disconnect();

private:
    QString m_dbPath;              /**< 数据库路径 */
};

#endif // SQLITEDATAPROVIDER_H
