#ifndef RECORD_SET_H_INCLUDE
#define RECORD_SET_H_INCLUDE

#include <QVector>
#include <QString>

/** 
 * 用于存储RecordSet中的数据类型
 */
typedef QVector<QString> Row;

/** 
 * RecordSet 用于存储通过SQL语句查询的结果
 */
class RecordSet
{
public:
	/// 初始的构造函数
    RecordSet()
		throw();
	/// 析构函数
	~RecordSet()
		throw();

	/// 清除所有的记录
    void clear();
	/// 检测当前记录是否为空
    bool isEmpty() const;
	/// 得到当前记录有多少行
    unsigned int rows() const;
	/// 得到当前记录有多少列
    unsigned int cols() const;
	/// 得到指定行的字段名称
	QString getField(const int& col);
	/// 设置当前记录的列表头
    void setColumnHeaders(const Row& headers);
	/// 添加一行新的数据
    void add(const Row& row);
	/// 得到指定行和指定列的数据
    const QString& operator()(const unsigned int row,
		                          const unsigned int col) const;
	/// 得到指定行指定名称的数据
    const QString& operator()(const unsigned int row,
                                  const QString& name) const;
private:
	Row mHeaders;              /**< 用于存储当前记录的数据名称 */
    typedef QVector<Row> Rows;
	Rows mRows;                /**< 用于存储读取的数据 */
};

/** 
 * 用于保存一组结果集
 */
class RecordSetList
{
public:
	/// 初始的构造函数
	RecordSetList()
		throw();
	/// 析构函数
	~RecordSetList()
		throw();

	/// 清除所有的记录
	void clear();
	/// 检测当前记录是否为空
	bool isEmpty() const;
	/// 得到记录个数
	int Count() const { return static_cast<int>(mRecordSets.size()); }

	/// 添加一行新的数据
	void add(const RecordSet& rowset);
	/// 得到指定行和指定列的数据
	const RecordSet& operator()(const unsigned int& row) const;

private:
    typedef QVector<RecordSet> RecordSets;
	RecordSets mRecordSets;
};

#endif
