/**
* @file ElaSuggestModel.cpp
 * @brief 实现 ElaSuggestModel 类，建议项模型
 * @author [Liniyous]
 * @date 2025-06-25
 * @version 1.0
 */

#include "ElaSuggestModel.h"

#include <utility>

/**
 * @brief 构造函数
 * @param parent 父对象指针，默认为 nullptr
 */
ElaSuggestModel::ElaSuggestModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

/**
 * @brief 析构函数
 */
ElaSuggestModel::~ElaSuggestModel() = default;

/**
 * @brief 获取行数
 * @param parent 父索引
 * @return 建议项数量
 */
int ElaSuggestModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(_suggestionVector.count()); ///< 返回建议项数量
}

/**
 * @brief 获取数据
 * @param index 模型索引
 * @param role 数据角色
 * @return 数据（当前未使用）
 */
QVariant ElaSuggestModel::data(const QModelIndex &index, int role) const
{
    return {};                                        ///< 当前未使用
}

/**
 * @brief 设置搜索建议项
 * @param suggestionVector 建议项列表
 * @note 更新模型数据
 */
void ElaSuggestModel::setSearchSuggestion(QVector<QSharedPointer<ElaSuggestion>> suggestionVector)
{
    beginResetModel();                                        ///< 开始重置模型
    _suggestionVector = std::move(suggestionVector);         ///< 更新建议项列表
    endResetModel();                                          ///< 结束重置模型
}

/**
 * @brief 清除搜索节点
 * @note 清空建议项列表
 */
void ElaSuggestModel::clearSearchNode()
{
    this->_suggestionVector.clear();                          ///< 清空列表
}

/**
 * @brief 获取搜索建议项
 * @param row 行号
 * @return 建议项指针
 */
ElaSuggestion *ElaSuggestModel::getSearchSuggestion(int row)
{
    if (row >= 0 && row < _suggestionVector.size())
        return _suggestionVector[row].get();
    return nullptr;
}