//
// Created by sakost on 28.05.2020.
//

#include "graphview.h"
#include <exception>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QProcess>
#include <QByteArray>

GraphView::GraphView(QWidget *parent):GraphView(new Graph, parent){}

GraphView::GraphView(Graph *newGraph, QWidget *parent)
        : QOpenGLWidget(parent)
{
    setAutoFillBackground(false);
    dotFile = new QTemporaryFile();
    if(!dotFile->open()){
        throw std::exception();
    }
    qDebug() << "temporary file:" << dotFile->fileName();
    graph = newGraph;

    setMinimumSize(100, 100);
}

void GraphView::paintEvent(QPaintEvent *event) {
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), Qt::white);
    if(needReloadImage){
        updateCache();
        needReloadImage = false;
    }
    painter.drawPixmap(imageBuffer.rect(), imageBuffer);
    painter.end();
}


QByteArray GraphView::toGraphviz() {
    QString connections, result = "graph G {\n"
//                        "\trank=same;\n"
//                        "\trankdir=LR;\n"
                        "\toverlap=false; splines=false;\n"
                        "\tsubgraph cluster_0 {\n"
                        "\t\tstyle=filled;\n"
                        "\t\tcolor=lightgrey;\n"
                        "\t\tnode [style=filled,color=white];\n"
                        "%1"
                        "\t\tlabel = \"nodes\";\n"
                        "\t}\n"
                        "\n"
                        "\tsubgraph cluster_1 {\n"
                        "\t\tnode [style=filled];\n"
                        "%2"
                        "\t\tlabel = \"edges\";\n"
                        "\t\tcolor=blue\n"
                        "\t}\n"
                        "%3"
                        "}";
    QStringList nodes, edges;

    if(!graph->empty()){
        for (int i = 0; i < (*graph)[0].size(); ++i) {
            nodes += "node" + QString::number(i+1);
        }
    }

    QString snodes = "\t\t" + nodes.join(" -- ") + "[style=invis];\n";

    for (int i = 0; i < graph->size(); ++i) {
        edges += "edge" + QString::number(i+1);
    }

    QString sedges = "\t\t" + edges.join(" -- ") + "[style=invis];\n";


    for (qsizetype i = 0; i < graph->size(); ++i) {
        for (qsizetype j = 0; j < (*graph)[i].size(); ++j) {
            if((*graph)[i][j] > 0) {
                connections += "\tedge" + QString::number(i+1) + " -- node" + QString::number(j+1) + "[constraint=false];\n";
            }
        }
    }
    return result.arg(snodes, sedges, connections).toUtf8();
}

void GraphView::updateCache() {
    dotFile->resize(0);
    dotFile->write(toGraphviz());
    dotFile->flush();
    QProcess process;
    process.start("dot", {"-Tpng", dotFile->fileName()});
    process.waitForFinished();
    auto outBytes = process.readAllStandardOutput();
    auto image = QImage::fromData(outBytes, "png");
    imageBuffer = QPixmap::fromImage(image);
    if(imageBuffer.width() > max_image_width){
        imageBuffer = imageBuffer.scaledToWidth(max_image_width);
    }
    setMinimumSize(imageBuffer.size());
}

[[maybe_unused]] void GraphView::updateImage(const QModelIndex topLeft, const QModelIndex bottomRight, const QVector<int>& roles) {
    Q_UNUSED(roles);
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);

    updateImage();
}

void GraphView::updateImage() {
    updateCache();
    emit update();
}

GraphView::~GraphView(){
    delete dotFile;

}
