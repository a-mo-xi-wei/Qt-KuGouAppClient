#include "recordset.h"
#include "../MyQsLog/QsLog.h"

/** 
 * 初始的构造函数
 */
RecordSet::RecordSet()
	throw()
{

}

/** 
 * 析构函数
 */
RecordSet::~RecordSet()
	throw()
{

}

/** 
 * 清除所有的记录
 */
void RecordSet::clear()
{
	mHeaders.clear();
	mRows.clear();
}

/** 
 * 检测当前记录是否为空
 *
 * @return 如果当前记录为空的话返回真，否则返回假
 */
bool RecordSet::isEmpty() const
{
	return (mRows.size() == 0);
}

/** 
 * 得到当前记录有多少行
 *
 * @return 返回当前记录有多少行数据
 */
unsigned int RecordSet::rows() const
{
	return (unsigned int)mRows.size();
}

/** 
 * 得到当前记录有多少列
 *
 * @return 返回当前记录有多少列数据
 */
unsigned int RecordSet::cols() const
{
	return (unsigned int)mHeaders.size();
}

/** 
 * 设置当前记录的列表头
 *
 * @param headers 要设置的列表头
 */
void RecordSet::setColumnHeaders(const Row& headers)
{
	if(mHeaders.size() > 0)
	{
        QLOG_WARN()<<"list hearder is exist.";
        return;
	}

	mHeaders = headers;
}

/** 
 * 添加一行新的数据
 *
 * @param row 要添加的一行数据
 */
void RecordSet::add(const Row& row)
{
	const unsigned int nCols = (unsigned int)mHeaders.size();

	if(nCols == 0)
	{
        QLOG_WARN()<<"data not exist.";

        return;
	}

	if(row.size() != nCols)
	{
        QLOG_WARN() << "rows:" << (unsigned int)row.size() << " "
            << "real rows:" << nCols;

        return;
	}

	mRows.push_back(row);
}

/** 
* 得到指定行和指定列的数据
*
* @param row,col 要取得的数据的行号和列号
*
* @return 如果这个数据存在返回这个数据的字符串表示，否则抛出异常
*/
const QString& RecordSet::operator()(const unsigned int row,
										 const unsigned int col) const
{
    if((row >= (quint32)mRows.size() || (col >= (quint32)mHeaders.size())))
	{
        QLOG_WARN() << "(" << row << "," << col << ") Out of range;"
            << "max row:" << (unsigned int)mRows.size()
            << ",max list:" << (unsigned int)mHeaders.size();
	}

	return mRows[row][col];
}

/** 
 * 得到指定行指定名称的数据
 *
 * @param row 要取得的数据的行号
 * @param name 要取得的数据的名称
 *
 * @return 如果这个数据存在返回这个数据的字符串表示，否则抛出异常
 */
const QString& RecordSet::operator()(const unsigned int row,
                              const QString& name) const
{
	if(row >= mRows.size())
	{
        QLOG_WARN() << "row num" << row << "out of range;"
            << "current max row:" << (unsigned int)mRows.size();
	}

    Row::const_iterator it = std::find(mHeaders.begin(), mHeaders.end(), name);

	if(it == mHeaders.end())
	{
        QLOG_WARN() << "name:" << name << " data is not exist.";
	}

	// 找到这个数据的索引号
	const unsigned int nCols = (unsigned int)mHeaders.size();
	unsigned int i;
	for(i=0;i<nCols;++i)
	{
		if(mHeaders[i] == name)
			break;
	}

	return mRows[row][i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

/** 
 * 初始的构造函数
 */
RecordSetList::RecordSetList()
	throw()
{

}

/** 
 * 析构函数
 */
RecordSetList::~RecordSetList()
	throw()
{

}

/** 
 * 清除所有的记录
 */
void RecordSetList::clear()
{
	mRecordSets.clear();
}

/** 
 * 检测当前记录是否为空
 *
 * @return 如果当前记录为空的话返回真，否则返回假
 */
bool RecordSetList::isEmpty() const
{
	return ((int)mRecordSets.size() == 0);
}

/** 
 * 添加一行新的数据
 *
 * @param row 要添加的一行数据
 */
void RecordSetList::add(const RecordSet& row)
{
	mRecordSets.push_back(row);
}

/** 
* 得到指定行和指定列的数据
*
* @param row,col 要取得的数据的行号和列号
*
* @return 如果这个数据存在返回这个数据的字符串表示，否则抛出异常
*/
const RecordSet& RecordSetList::operator()(const unsigned int& row) const
{
	if(row >= mRecordSets.size() || row < 0)
	{
        QLOG_WARN() << "(" << row << ") is out of range;";
	}

	return mRecordSets[row];
}
