//
// Created by sakost on 01.06.2020.
//

#ifndef DISKR_CW8_CPP_ALGORITHMS_H
#define DISKR_CW8_CPP_ALGORITHMS_H

#include <QVector>
#include <QSet>
#include <QTextBrowser>
#include "Graph.h"

class Algorithms {
public:
    enum Algorithm{
        AlgoMagoo,
        AlgoDefault,
    };
    static QVector< QSet<uint_fast64_t> > Magoo(const Graph *graph, QTextBrowser* output);
    static Graph* hypergraph_to_usual(const Graph *graph);
    static QVector<int_fast64_t> get_colors(const Graph* graph, Algorithm method=AlgoDefault);
};


#endif //DISKR_CW8_CPP_ALGORITHMS_H
