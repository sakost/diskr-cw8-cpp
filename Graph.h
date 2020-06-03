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
    [[maybe_unused]] [[nodiscard]] QVector<QVector<int>> get_adjacency_matrix() const;
    [[maybe_unused]] [[nodiscard]] QVector<QVector<int>> get_incident_matrix() const;
    void operator=(const QVector<QVector<int>>& incident_matrix) {
        QVector::operator=(incident_matrix);
    }

    static Graph* from_incident_matrix(const QVector<QVector<int>>& incident_matrix);
    static Graph* from_adjacency_matrix(const QVector<QVector<int>>& adjacency_matrix);
};


#endif //DISKR_CW8_CPP_GRAPH_H
