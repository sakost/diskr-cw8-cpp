//
// Created by sakost on 27.05.2020.
//

#include "graphmodel.h"

#include <exception>
#include <QDebug>
#include <QString>


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
    if(column < 0 || graph->empty() || column + count != (*graph)[0].size()){
        return false;
    }
    beginRemoveColumns(parent, column, column + count-1);
    for (int i = 0; i < count; ++i) {
        for (auto &edge: *graph){
            edge.pop_back();
        }
    }
    endRemoveColumns();
    emit dataChanged(this->createIndex(0, column), this->createIndex(graph->size()-1, column+count-1));
    return true;
}

bool GraphModel::removeRows(int row, int count, const QModelIndex &parent) {
    if(row < 0 || row + count != graph->size()){
        return false;
    }
    int col_removed;
    beginRemoveRows(parent, row, row+count-1);
    col_removed = graph->back().size() - 1;
    for (int i = 0; i < count; ++i) {
        graph->pop_back();
    }
    endRemoveRows();
    emit dataChanged(this->createIndex(row, 0), this->createIndex(row, col_removed));
    return true;
}


bool GraphModel::insertRows(int row, int count, const QModelIndex& parent) {
    if(row != graph->size() || count < 1){ // just append items
        return false;
    }
    if(row == graph->size()){ // append
        beginInsertRows(parent, row, row+count-1);
        int columns_count = DEFAULT_NODES;
        if (!graph->empty()) {
            columns_count = (*graph)[0].size();
        }
        for (int i = 0; i < count; ++i) {
            graph->push_back(QVector<int>(columns_count, 0));
        }
        endInsertRows();
        emit dataChanged(this->createIndex(row, 0), this->createIndex(row+count-1, (*graph)[0].size() - 1));
        return true;
    }
    return false; // can't be reached
}


bool GraphModel::insertColumns(int column, int count, const QModelIndex& parent) {
    if(!graph->empty() && column != (*graph)[0].size() || column < 1){ // just append items
        return false;
    }
    if(graph->empty()){
        if(!insertRow(graph->size())){ // append row
            return false;
        }
    }
    if(column == (*graph)[0].size()){ // append
        beginInsertColumns(parent, column, column+count-1);
        for (int i = 0; i < count; ++i) {
            for (auto &edge: *graph) {
                edge.push_back(0);
            }
        }
        endInsertColumns();
        emit dataChanged(this->createIndex(0, column), this->createIndex(graph->size()-1, column+count-1));
        return true;
    }
    return false; // can't be reached
}


void GraphModel::rebuildModel(int rows, int columns) {
    if(rows == -1){
        rows = graph->size();
    }
    if(columns == -1){
        columns = graph->empty() ? DEFAULT_NODES : (*graph)[0].size();
    }
    if(rows > graph->size()){
        insertRows(rowCount(QModelIndex()), rows - rowCount(QModelIndex()), QModelIndex());
    } else if(rows < graph->size()){
        removeRows(rows, rowCount(QModelIndex()) - rows, QModelIndex());
    }
    if(rowCount(QModelIndex()) == 0){
        return;
    }
    if(columnCount(QModelIndex()) < columns){
        insertColumns(columnCount(QModelIndex()), columns - columnCount(QModelIndex()), QModelIndex());
    } else if((*graph)[0].size() > columns){
        removeColumns(columns, columnCount(QModelIndex()) - columns, QModelIndex());
    }
}
