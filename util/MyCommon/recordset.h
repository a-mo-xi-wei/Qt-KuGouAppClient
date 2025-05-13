/*
  RecordSet - 用于存储 SQL 查询结果的类
*/

#ifndef RECORD_SET_H_INCLUDE
#define RECORD_SET_H_INCLUDE

#include <QVector>
#include <QString>

/** @typedef Row
 *  @brief 用于存储 RecordSet 中单行数据的类型
 *
 *  定义为 QString 类型的 QVector，用于表示一行数据。
 */
typedef QVector<QString> Row;

/** @class RecordSet
 *  @brief 用于存储和操作 SQL 查询结果的类
 *
 *  该类提供存储和管理 SQL 查询结果的功能，支持添加行、设置列头、查询数据等操作。
 */
class RecordSet {
public:
    /** @brief 构造函数
     *
     *  初始化 RecordSet 对象。
     *
     *  @throw 无异常抛出
     */
    RecordSet() throw();

    /** @brief 析构函数
     *
     *  销毁 RecordSet 对象，释放资源。
     *
     *  @throw 无异常抛出
     */
    ~RecordSet() throw();

    /** @brief 清除所有记录
     *
     *  清空当前存储的列头和行数据。
     */
    void clear();

    /** @brief 检查记录是否为空
     *
     *  @return 如果当前记录为空则返回 true，否则返回 false
     */
    bool isEmpty() const;

    /** @brief 获取行数
     *
     *  @return 当前记录的行数
     */
    unsigned int rows() const;

    /** @brief 获取列数
     *
     *  @return 当前记录的列数
     */
    unsigned int cols() const;

    /** @brief 获取指定列的字段名称
     *
     *  @param col 列索引
     *  @return 指定列的字段名称
     */
    QString getField(const int& col);

    /** @brief 设置列头
     *
     *  设置记录的列头数据。如果已存在列头，则记录警告并返回。
     *
     *  @param headers 列头数据
     */
    void setColumnHeaders(const Row& headers);

    /** @brief 添加一行数据
     *
     *  将一行数据添加到记录中。要求行数据的列数与列头一致。
     *
     *  @param row 要添加的行数据
     */
    void add(const Row& row);

    /** @brief 获取指定行列的数据
     *
     *  获取指定行和列的数据。如果索引越界，则记录警告并抛出异常。
     *
     *  @param row 行索引
     *  @param col 列索引
     *  @return 指定位置的数据
     */
    const QString& operator()(const unsigned int row, const unsigned int col) const;

    /** @brief 获取指定行和字段名称的数据
     *
     *  获取指定行和字段名称的数据。如果行索引越界或字段名称不存在，则记录警告并抛出异常。
     *
     *  @param row 行索引
     *  @param name 字段名称
     *  @return 指定位置的数据
     */
    const QString& operator()(const unsigned int row, const QString& name) const;

private:
    /** @brief 存储列头数据
     */
    Row mHeaders;

    /** @typedef Rows
     *  @brief 用于存储多行数据的类型
     *
     *  定义为 Row 类型的 QVector，用于表示多行数据。
     */
    typedef QVector<Row> Rows;

    /** @brief 存储行数据
     */
    Rows mRows;
};

/** @class RecordSetList
 *  @brief 用于存储一组 RecordSet 的类
 *
 *  该类提供管理多个 RecordSet 的功能，支持添加、清除和访问记录集。
 */
class RecordSetList {
public:
    /** @brief 构造函数
     *
     *  初始化 RecordSetList 对象。
     *
     *  @throw 无异常抛出
     */
    RecordSetList() throw();

    /** @brief 析构函数
     *
     *  销毁 RecordSetList 对象，释放资源。
     *
     *  @throw 无异常抛出
     */
    ~RecordSetList() throw();

    /** @brief 清除所有记录
     *
     *  清空当前存储的所有记录集。
     */
    void clear();

    /** @brief 检查记录是否为空
     *
     *  @return 如果当前记录集为空则返回 true，否则返回 false
     */
    bool isEmpty() const;

    /** @brief 获取记录集数量
     *
     *  @return 当前存储的记录集数量
     */
    int Count() const { return static_cast<int>(mRecordSets.size()); }

    /** @brief 添加一个记录集
     *
     *  将一个 RecordSet 添加到记录集列表中。
     *
     *  @param rowset 要添加的记录集
     */
    void add(const RecordSet& rowset);

    /** @brief 获取指定索引的记录集
     *
     *  获取指定索引的记录集。如果索引越界，则记录警告并抛出异常。
     *
     *  @param row 记录集索引
     *  @return 指定索引的记录集
     */
    const RecordSet& operator()(const unsigned int& row) const;

private:
    /** @typedef RecordSets
     *  @brief 用于存储多个 RecordSet 的类型
     *
     *  定义为 RecordSet 类型的 QVector。
     */
    typedef QVector<RecordSet> RecordSets;

    /** @brief 存储记录集列表
     */
    RecordSets mRecordSets;
};

#endif // RECORD_SET_H_INCLUDE