//
// Created by sakost on 01.06.2020.
//

#include "Algorithms.h"
#include <cmath>

QVector<QSet<uint_fast64_t> > Algorithms::Magoo(const Graph *graph, QTextBrowser *output) {
    // does not work todo fix
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
    for(size_t i = 1;  i < result.size(); i++){
        QVector< QSet< uint_fast64_t > > sset(begin(result), begin(result)+i);
        for (size_t j = i; j < result.size(); ++j) {
            if(!std::includes(begin(result[j]), end(result[j]),
                    begin(result[i]), end(result[i]))){
                sset.push_back(result[j]);
            }
        }
        result = sset;
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

QVector<int_fast64_t> Algorithms::get_colors(const Graph *graph, Algorithms::Algorithm method) {
    int nodes_count = (*graph)[0].size();
    QVector<int_fast64_t> result(nodes_count, -1);
    switch (method) {
        case AlgoMagoo:
            return result;
        case AlgoDefault:
            QVector<int_fast64_t> colors(nodes_count);
            std::iota(colors.begin(), colors.end(), 0);

            auto adj_matrix = graph->get_adjacency_matrix();
            // building adjacency lists
            QVector<QVector<int_fast64_t>> adj_lists(adj_matrix.size());
            for (int i = 0; i < adj_matrix.size(); ++i) {
                for (int j = 0; j < adj_matrix[i].size(); ++j) {
                    if(adj_matrix[i][j])
                        adj_lists[i].push_back(j);
                }
            }

            for (const auto & color : colors) {
                bool found = false;
                size_t cur;
                for (int i = 0; i < adj_lists.size(); ++i) { // iterate over all nodes
                    if(result[i] == -1){ // node was not colored
                        cur = i;
                        found = true;
                        break;
                    }
                }
                if(!found){ // all nodes are colored
                    break;
                }
                result[cur] = color;
                for (int i = 0; i < adj_lists.size(); ++i) {
                    if(i == cur || result[i] != -1){
                        continue;
                    }
                    bool have_same = false;
                    for (int j = 0; j < adj_lists[i].size(); ++j) {
                        if(result[adj_lists[i][j]] == color){
                            have_same = true;
                            break;
                        }
                    }
                    if(!have_same && result[i] == -1){
                        result[i] = color;
                    }
                }
            }
    }
    return result;
}



