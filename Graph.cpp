//
// Created by sakost on 30.05.2020.
//

#include "Graph.h"

QVector<QVector<int>> Graph::get_adjacency_matrix() {
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

