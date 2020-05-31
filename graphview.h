//
// Created by sakost on 28.05.2020.
//

#ifndef DISKR_CW8_CPP_GRAPHVIEW_H
#define DISKR_CW8_CPP_GRAPHVIEW_H

#include <QOpenGLWidget>
#include <QTemporaryFile>
#include <QPixmap>
#include <QPixmapCache>
#include <QModelIndex>

#include "Graph.h"

class GraphView: public QOpenGLWidget{
    Q_OBJECT
public:
    explicit GraphView(QWidget *parent= nullptr);
    explicit GraphView(Graph* newGraph, QWidget* parent = nullptr);
    ~GraphView() override;
    [[nodiscard]] QByteArray toGraphviz();

protected:
    void paintEvent(QPaintEvent *event) override ;

private:
    QTemporaryFile *dotFile;
    bool needReloadImage = true;
    QPixmap imageBuffer;
    Graph *graph;
    void updateCache();

    const int max_image_width=700, max_image_height = 600;
public slots:
    [[maybe_unused]] void updateImage(QModelIndex topLeft, QModelIndex bottomRight, const QVector<int>& roles = QVector<int>());
    void updateImage();
};

#endif //DISKR_CW8_CPP_GRAPHVIEW_H
