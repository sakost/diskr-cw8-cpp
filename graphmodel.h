//
// Created by sakost on 27.05.2020.
//

#ifndef DISKR_CW8_CPP_GRAPHMODEL_H
#define DISKR_CW8_CPP_GRAPHMODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include <QPair>
#include <QList>

#include "Graph.h"

class GraphModel: public QAbstractTableModel{
    Q_OBJECT
public:
    GraphModel();
    explicit GraphModel(Graph* newGraph);

    [[nodiscard]] QModelIndex parent(const QModelIndex &child) const override;

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] int columnCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool removeLastNode();
    bool removeLastEdge();
    bool addNode();
    bool addEdge();

protected:
    bool removeColumns(int column, int count, const QModelIndex& parent) override;
    bool removeRows(int row, int count, const QModelIndex& parent) override;
    bool insertRows(int row, int count, const QModelIndex& parent) override;
    bool insertColumns(int column, int count, const QModelIndex& parent) override;

private:
    Graph* graph;
public slots:
    void rebuildModel(int rows, int columns);
};

#endif //DISKR_CW8_CPP_GRAPHMODEL_H
