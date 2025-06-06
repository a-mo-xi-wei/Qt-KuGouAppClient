#ifndef ELASUGGESTMODEL_H
#define ELASUGGESTMODEL_H

#include <QAbstractListModel>

#include "Def.h"
class ElaSuggestion;

class ElaSuggestModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit ElaSuggestModel(QObject *parent = nullptr);

    ~ElaSuggestModel() override;

    int rowCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const override;

    void setSearchSuggestion(QVector<ElaSuggestion *> suggestionVector);

    void clearSearchNode();

    ElaSuggestion *getSearchSuggestion(int row);

private:
    QVector<ElaSuggestion *> _suggestionVector; //符合搜索的节点
};

#endif // ELASUGGESTMODEL_H
