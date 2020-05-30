//
// Created by sakost on 30.05.2020.
//

#ifndef DISKR_CW8_CPP_GRAPH_H
#define DISKR_CW8_CPP_GRAPH_H

#include <QVector>
#include <QObject>

class Graph: public QVector<QVector<int> >{
//    Q_OBJECT
public:
    [[maybe_unused]] QVector<QVector<int>> get_adjacency_matrix();
};


#endif //DISKR_CW8_CPP_GRAPH_H
