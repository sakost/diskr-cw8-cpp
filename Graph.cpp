//
// Created by sakost on 30.05.2020.
//

#include "Graph.h"

QVector<QVector<int>> Graph::get_adjacency_matrix() const {
    QVector<QVector<int>> result;
    if (this->empty()){
        return result;
    }

    int adj_size = (*this)[0].size(); // size of adjacency matrix

    for (int i = 0; i < adj_size; ++i) {
        result.push_back(QVector<int>(adj_size, 0)); // fill the matrix by vectors
    }

    for(auto &edge: *this){
        for (int fs = 0; fs < edge.size(); fs++) {
            if(edge[fs] == 0){
                continue;
            }
            for(int sc = 0; sc < edge.size(); sc++){
                if(edge[sc] == 0){
                    continue;
                }
                result[fs][sc] = 1;
                result[sc][fs] = 1;
            }
        }
    }
    return result;
}

QVector<QVector<int>> Graph::get_incident_matrix() const{
    return QVector< QVector < int > > (*this);
}

Graph *Graph::from_incident_matrix(const QVector<QVector<int>> &incident_matrix) {
    auto graph = new Graph;
    (*graph) = incident_matrix;
    return graph;
}

Graph *Graph::from_adjacency_matrix(const QVector<QVector<int>> &adjacency_matrix) {
    using std::begin;
    using std::end;

    if(adjacency_matrix.empty()){
        return nullptr;
    }
    if(adjacency_matrix.size() != adjacency_matrix[0].size()){
        return nullptr;
    }
    auto graph = new Graph;
    for (int i = 0; i < adjacency_matrix.size(); ++i) {
        for (int j = i+1; j < adjacency_matrix[i].size(); ++j) {
            if(adjacency_matrix[i][j] != 0) {
                QVector<int> edge(adjacency_matrix.size());
//                std::iota(begin(edge), end(edge), 0);
                edge[i] = 1;
                edge[j] = 1;
                graph->push_back(edge);
            }
        }
    }
    if(graph->empty()){
        graph->push_back(QVector<int> (adjacency_matrix.size()));
    }

    return graph;
}

