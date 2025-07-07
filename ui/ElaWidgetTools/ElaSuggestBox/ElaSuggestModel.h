/**
* @file ElaSuggestModel.h
 * @brief 定义 ElaSuggestModel 类，建议项模型
 * @author [Liniyous]
 * @date 2025-06-25
 * @version 1.0
 */

#ifndef ELASUGGESTMODEL_H
#define ELASUGGESTMODEL_H

#include <QAbstractListModel>

#include "Def.h"

class ElaSuggestion;

/**
 * @class ElaSuggestModel
 * @brief 建议项模型，管理搜索建议项列表
 */
class ElaSuggestModel : public QAbstractListModel
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针，默认为 nullptr
     */
    explicit ElaSuggestModel(QObject *parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~ElaSuggestModel() override;

    /**
     * @brief 获取行数
     * @param parent 父索引
     * @return 建议项数量
     */
    int rowCount(const QModelIndex &parent) const;

    /**
     * @brief 获取数据
     * @param index 模型索引
     * @param role 数据角色
     * @return 数据（当前未使用）
     */
    QVariant data(const QModelIndex &index, int role) const override;

    /**
     * @brief 设置搜索建议项
     * @param suggestionVector 建议项列表
     * @note 更新模型数据
     */
    void setSearchSuggestion(QVector<QSharedPointer<ElaSuggestion>> suggestionVector);

    /**
     * @brief 清除搜索节点
     * @note 清空建议项列表
     */
    void clearSearchNode();

    /**
     * @brief 获取搜索建议项
     * @param row 行号
     * @return 建议项指针
     */
    ElaSuggestion *getSearchSuggestion(int row);

private:
    QVector<QSharedPointer<ElaSuggestion>> _suggestionVector;               ///< 符合搜索的建议项列表
};

#endif // ELASUGGESTMODEL_H