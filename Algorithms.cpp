//
// Created by sakost on 01.06.2020.
//

#include "Algorithms.h"
#include <cmath>

QVector<QSet<uint_fast64_t> > Algorithms::Magoo(const Graph *graph, QTextBrowser *output) {
    if(graph->empty()){
        output->append("Graph is empty. Exiting...");
        return QVector< QSet< uint_fast64_t > >();
    }
    const uint_fast64_t nodes_count = (*graph)[0].size();
    const uint_fast64_t edges_count = graph->size();
    using std::begin;
    using std::end;

    if(std::accumulate(begin((*graph)[0]), end((*graph)[0]), 0) == 0){ // one edge just stub
        output->append("Graph has no edges");
        output->append("Make all nodes of one color");
        auto tmp = QVector<uint_fast64_t>(nodes_count);
        std::iota(begin(tmp), end(tmp), 0);
        output->append("Done!");
        return QVector<QSet<uint_fast64_t>>(1, QSet<uint_fast64_t>(begin(tmp), end(tmp)));
    }

    // number of iterations is in case of usual graph is 2 in power of n
    size_t iters = std::pow(2ll, edges_count); // todo make for hyperedges
    output->append(QString("%1 iterations is going to be done...").arg(iters));
    if(iters > 15e9){
        output->append("Too many iterations going to be done. Exiting...");
        return QVector< QSet< uint_fast64_t > >();
    }

    output->append("building the list of edges");
    QVector< QPair< uint_fast64_t, uint_fast64_t > > edges(edges_count);
    for(size_t i = 0; i < edges_count; i++){
        bool fs = true, fin=false;
        for(size_t j = 0; j < nodes_count; j++){
            if((*graph)[i][j]){
                if(fin){
                    throw std::exception();
                }
                if(fs){
                    edges[i].first = j;
                    fs = false;
                }else{
                    edges[i].second = j;
                    fin = true;
                }
            }
        }
    }

    output->append("processing all subsets...");
    QVector<QSet<uint_fast64_t>> result;
    result.reserve(iters);
    for (uint_fast64_t i = 0; i < iters; ++i) {
        QSet< uint_fast64_t > tmp_result;
        uint_fast64_t parse = i;
        for (int j = 0; j < edges_count; ++j) {
            tmp_result.insert(parse&1 ? edges[j].second : edges[j].first);
            parse >>= 1;
        }
        result.append(std::move(tmp_result));
    }

    output->append("sorting...");

    std::stable_sort(begin(result), end(result), [](const QSet< uint_fast64_t >& lhs, const QSet< uint_fast64_t >& rhs){
        return lhs.size() < rhs.size();
    });

    output->append("filtering...");
    size_t i = 0;
    while(i < result.size()){
        QVector< QSet< uint_fast64_t > > sset(begin(result), begin(result)+i);
        for (size_t j = i+1; j < result.size(); ++j) {
            if(!std::includes(begin(result[j]), end(result[j]),
                    begin(result[i]), end(result[i]))){
                sset.push_back(result[j]);
            }
        }
        result = sset;
        i++;
    }

    output->append("reversing sets...");

    // filling universal set
    QSet< uint_fast64_t > U;
    for (uint_fast64_t j = 0; j < nodes_count; ++j) {
        U.insert(j);
    }
    
    for (auto & j : result) {
        QVector< uint_fast64_t > tmp(nodes_count, -1);
        std::set_difference(begin(U), end(U), begin(j), end(j), begin(tmp));
        auto rit = std::find_if(tmp.rbegin(), tmp.rend(), [](int v){
            return v != -1;
        });
        tmp.erase(rit.base(), tmp.end());
        j = QSet<uint_fast64_t >(begin(tmp), end(tmp));
    }

    output->append("Done!");
    return result;
}

Graph *Algorithms::hypergraph_to_usual(const Graph *graph) {
    return Graph::from_adjacency_matrix(graph->get_adjacency_matrix());
}

