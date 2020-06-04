//
// Created by sakost on 27.05.2020.
//

#include "graphmodel.h"

#include <exception>
#include <QDebug>
#include <QString>

#define DEFAULT_NODES 5

GraphModel::GraphModel(): GraphModel(new Graph){}


GraphModel::GraphModel(Graph* newGraph): QAbstractTableModel()
{
    graph = newGraph;

    for (int i = 0; i < DEFAULT_NODES; ++i) {
        (*graph).push_back(QVector<int>(DEFAULT_NODES));
    }
}


int GraphModel::rowCount(const QModelIndex &parent) const {
    return graph->size();
}

int GraphModel::columnCount(const QModelIndex &parent) const {
    return graph->size();
}

QVariant GraphModel::data(const QModelIndex &index, int role) const {
    if(role == Qt::DisplayRole) {
        if (!checkIndex(index))return QVariant();
        if(index.row() >= graph->size())return QVariant();
        if(graph->empty()) return QVariant();
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
        if(i == j){
            return false;
        }
        (*graph)[i][j] = (*graph)[j][i] = val > 0;
        int u = std::min(i, j), v = std::max(i, j);
        emit dataChanged(this->createIndex(u, u),this->createIndex(v, v), {role});
        return true;
    }
    return false;
}

QModelIndex GraphModel::parent(const QModelIndex &child) const {
    return QModelIndex();
}

void GraphModel::addNode() {
    beginResetModel();
    for(auto &node: *graph){
        node.push_back(0);
    }
    graph->push_back(QVector<int>(graph->size() + 1));
    endResetModel();
    emit dataChanged(createIndex(0, 0), createIndex(graph->size()-1, graph->size()-1));
}

void GraphModel::deleteNode() {
    beginResetModel();
    if(graph->empty()){
        return;
    }
    graph->pop_back();

    for(auto &node: *graph){
        node.pop_back();
    }
    endResetModel();
    emit dataChanged(createIndex(0, 0), createIndex(graph->size()-1, graph->size()-1));
}

