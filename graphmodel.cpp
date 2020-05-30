//
// Created by sakost on 27.05.2020.
//

#include "graphmodel.h"

#include <exception>
#include <QDebug>
#include <QString>

#define DEFAULT_NODES 5
#define DEFAULT_EDGES 3

GraphModel::GraphModel(): GraphModel(new Graph){}


GraphModel::GraphModel(Graph* newGraph): QAbstractTableModel()
{
    graph = newGraph;

    for (int i = 0; i < DEFAULT_EDGES; ++i) {
        (*graph).push_back(QVector<int>(DEFAULT_NODES));
    }
}



int GraphModel::rowCount(const QModelIndex &parent) const {
    return graph->size();
}

int GraphModel::columnCount(const QModelIndex &parent) const {
    if(graph->empty()) return 0;
    return (*graph)[0].size();
}

QVariant GraphModel::data(const QModelIndex &index, int role) const {
    if(role == Qt::DisplayRole) {
        if (!checkIndex(index))return QVariant();
        if(index.row() >= graph->size())return QVariant();
        if(graph->empty()) return QVariant();
        if(index.column() >= (*graph)[0].size()) {
            return QVariant();
        }
        return (*graph)[index.row()][index.column()];
    }
    return QVariant();
}


Qt::ItemFlags GraphModel::flags(const QModelIndex &index) const {
    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool GraphModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(!checkIndex(index)){
        return false;
    }
    if (role == Qt::EditRole){
        int i = index.row(), j = index.column();

        bool ok;
        int val = value.toInt(&ok);
        if(!ok){
            return false;
        }
        if(val < 0){
            return false;
        }
        (*graph)[i][j] = val > 0;
        emit dataChanged(this->createIndex(i, j, (*graph)[i][j]),
                this->createIndex(i, j, (*graph)[i][j]), {role});
        return true;
    }
    return false;
}

QModelIndex GraphModel::parent(const QModelIndex &child) const {
    return QModelIndex();
}


bool GraphModel::removeColumns(int column, int count, const QModelIndex &parent) {
    if(column < 0 || count != 1 || graph->empty() || column != (*graph)[0].size() - 1){
        return false;
    }
    beginRemoveColumns(parent, column, column);
    for (auto &edge: *graph){
        edge.pop_back();
    }
    endRemoveColumns();
    emit dataChanged(this->createIndex(0, column), this->createIndex(graph->size()-1, column));
    return true;
}

bool GraphModel::removeRows(int row, int count, const QModelIndex &parent) {
    if(row < 0 || count != 1 || row != graph->size() - 1){
        return false;
    }
    int col_removed;
    beginRemoveRows(parent, row, row);
    col_removed = graph->back().size() - 1;
    graph->pop_back();
    endRemoveRows();
    emit dataChanged(this->createIndex(row, 0), this->createIndex(row, col_removed));
    return true;
}

bool GraphModel::addNode() {
    if(graph->empty())
        return insertColumn(0);
    return insertColumn((*graph)[0].size());
}

bool GraphModel::insertRows(int row, int count, const QModelIndex& parent) {
    if(row != graph->size() || count != 1){ // just append one item
        return false;
    }
    if(row == graph->size()){ // append
        beginInsertRows(parent, row, row);
        int columns_count = DEFAULT_NODES;
        if(!graph->empty()){
            columns_count = (*graph)[0].size();
        }
        graph->push_back(QVector<int>(columns_count, 0));
        endInsertRows();
        emit dataChanged(this->createIndex(row, 0), this->createIndex(row, graph->size()-1));
        return true;
    }
    return false; // can't be reached
}

bool GraphModel::addEdge() {
    return insertRow(graph->size());
}

bool GraphModel::insertColumns(int column, int count, const QModelIndex& parent) {
    if(!graph->empty() && (column != (*graph)[0].size() || count != 1)){ // just append one item
        return false;
    }
    if(graph->empty()){
        if(!insertRow(graph->size())){ // append row
            return false;
        }
    }
    if(column == (*graph)[0].size()){ // append
        beginInsertColumns(parent, column, column);
        for(auto &edge: *graph){
            edge.push_back(0);
        }
        endInsertColumns();
        emit dataChanged(this->createIndex(0, column), this->createIndex((*graph)[0].size()-1, column));
        return true;
    }
    return false; // can't be reached
}

bool GraphModel::removeLastNode() {
    if(graph->empty()){
        return false;
    }
    return removeColumn((*graph)[0].size()-1);
}

bool GraphModel::removeLastEdge() {
    if(graph->empty()){
        return false;
    }
    return removeRow(graph->size()-1);
}
