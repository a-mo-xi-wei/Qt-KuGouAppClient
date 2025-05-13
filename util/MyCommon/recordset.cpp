/*
  RecordSet - 用于存储 SQL 查询结果的类实现
*/

#include "recordset.h"
#include "QsLog.h"

/** @brief 构造函数
 *
 *  初始化 RecordSet 对象。
 *
 *  @throw 无异常抛出
 */
RecordSet::RecordSet() throw() {
}

/** @brief 析构函数
 *
 *  销毁 RecordSet 对象，释放资源。
 *
 *  @throw 无异常抛出
 */
RecordSet::~RecordSet() noexcept {
}

/** @brief 清除所有记录
 *
 *  清空当前存储的列头和行数据。
 */
void RecordSet::clear() {
    mHeaders.clear();
    mRows.clear();
}

/** @brief 检查记录是否为空
 *
 *  @return 如果当前记录为空则返回 true，否则返回 false
 */
bool RecordSet::isEmpty() const {
    return mRows.empty();
}

/** @brief 获取行数
 *
 *  @return 当前记录的行数
 */
unsigned int RecordSet::rows() const {
    return static_cast<unsigned int>(mRows.size());
}

/** @brief 获取列数
 *
 *  @return 当前记录的列数
 */
unsigned int RecordSet::cols() const {
    return static_cast<unsigned int>(mHeaders.size());
}

/** @brief 获取指定列的字段名称
 *
 *  @param col 列索引
 *  @return 指定列的字段名称
 */
QString RecordSet::getField(const int &col) {
    return mHeaders[col];
}

/** @brief 设置列头
 *
 *  设置记录的列头数据。如果已存在列头，则记录警告并返回。
 *
 *  @param headers 列头数据
 */
void RecordSet::setColumnHeaders(const Row& headers) {
    if (mHeaders.size() > 0) {
        QLOG_WARN() << "list hearder is exist.";
        return;
    }

    mHeaders = headers;
}

/** @brief 添加一行数据
 *
 *  将一行数据添加到记录中。要求行数据的列数与列头一致。
 *
 *  @param row 要添加的行数据
 */
void RecordSet::add(const Row& row) {
    const unsigned int nCols = (unsigned int)mHeaders.size();

    if (nCols == 0) {
        QLOG_WARN() << "data not exist.";
        return;
    }

    if (row.size() != nCols) {
        QLOG_WARN() << "rows:" << (unsigned int)row.size() << " "
                    << "real rows:" << nCols;
        return;
    }

    mRows.push_back(row);
}

/** @brief 获取指定行列的数据
 *
 *  获取指定行和列的数据。如果索引越界，则记录警告并抛出异常。
 *
 *  @param row 行索引
 *  @param col 列索引
 *  @return 指定位置的数据
 */
const QString& RecordSet::operator()(const unsigned int row, const unsigned int col) const {
    if ((row >= (quint32)mRows.size() || (col >= (quint32)mHeaders.size()))) {
        QLOG_WARN() << "(" << row << "," << col << ") Out of range;"
                    << "max row:" << (unsigned int)mRows.size()
                    << ",max list:" << (unsigned int)mHeaders.size();
    }

    return mRows[row][col];
}

/** @brief 获取指定行和字段名称的数据
 *
 *  获取指定行和字段名称的数据。如果行索引越界或字段名称不存在，则记录警告并抛出异常。
 *
 *  @param row 行索引
 *  @param name 字段名称
 *  @return 指定位置的数据
 */
const QString& RecordSet::operator()(const unsigned int row, const QString& name) const {
    if (row >= mRows.size()) {
        QLOG_WARN() << "row num" << row << "out of range;"
                    << "current max row:" << (unsigned int)mRows.size();
    }

    Row::const_iterator it = std::find(mHeaders.begin(), mHeaders.end(), name);

    if (it == mHeaders.end()) {
        QLOG_WARN() << "name:" << name << " data is not exist.";
    }

    // 找到字段名称对应的索引
    const unsigned int nCols = (unsigned int)mHeaders.size();
    unsigned int i;
    for (i = 0; i < nCols; ++i) {
        if (mHeaders[i] == name)
            break;
    }

    return mRows[row][i];
}

/** @brief 构造函数
 *
 *  初始化 RecordSetList 对象。
 *
 *  @throw 无异常抛出
 */
RecordSetList::RecordSetList() throw() {
}

/** @brief 析构函数
 *
 *  销毁 RecordSetList 对象，释放资源。
 *
 *  @throw 无异常抛出
 */
RecordSetList::~RecordSetList() throw() {
}

/** @brief 清除所有记录
 *
 *  清空当前存储的所有记录集。
 */
void RecordSetList::clear() {
    mRecordSets.clear();
}

/** @brief 检查记录是否为空
 *
 *  @return 如果当前记录集为空则返回 true，否则返回 false
 */
bool RecordSetList::isEmpty() const {
    return static_cast<int>(mRecordSets.size()) == 0;
}

/** @brief 添加一个记录集
 *
 *  将一个 RecordSet 添加到记录集列表中。
 *
 *  @param rowset 要添加的记录集
 */
void RecordSetList::add(const RecordSet& rowset) {
    mRecordSets.push_back(rowset);
}

/** @brief 获取指定索引的记录集
 *
 *  获取指定索引的记录集。如果索引越界，则记录警告并抛出异常。
 *
 *  @param row 记录集索引
 *  @return 指定索引的记录集
 */
const RecordSet& RecordSetList::operator()(const unsigned int& row) const {
    if (row >= mRecordSets.size() || row < 0) {
        QLOG_WARN() << "(" << row << ") is out of range;";
    }

    return mRecordSets[row];
}