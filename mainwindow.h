#pragma once

#include "graphmodel.h"
#include "graphview.h"

// QtWidgets
#include <QMainWindow>
#include <QWidget>
#include <QShortcut>

#include <QHeaderView>
// QtGui
#include <QKeySequence>
// QtCore

#include <QtCore>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QOpenGLWidget;
QT_END_NAMESPACE

class MainWindow: public QMainWindow{
    Q_OBJECT
public:
    MainWindow(GraphView *pView, GraphModel* model, QWidget *parent= nullptr);

public slots:

private:
    Ui::MainWindow *ui;
    GraphView *graphView;
    GraphModel *graphModel;
    QMap<int_fast64_t, QVector<int_fast64_t>> color2nodes;
    QMap<int_fast64_t, QColor> colors;
    size_t color_index = 0;

    void checkColors(const QVector<int_fast64_t>& icolors);
public slots:
    void startAlgorithm();
    void changedEdges(int count);
    void changedNodes(int count);
    void changedColors(int value);
    void changeColor();
    void syncColors();
};
