#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QShortcut>

#include "Algorithms.h"

#include <vector>


MainWindow::MainWindow(GraphView *pView, GraphModel* model, QWidget *parent)
            : QMainWindow(parent),
            ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Курсовая работа №8");
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setMaximumSize(700, 600);

    graphView = pView;
    graphModel = model;

    ui->tableView->setModel(graphModel);
    delete centralWidget()->layout()->replaceWidget(ui->graphOpenGLWidget, graphView);
    delete ui->graphOpenGLWidget;

    connect(ui->startButton, SIGNAL(clicked()), pView, SLOT(updateImage())); // todo replace with algorithm
    connect(graphModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int> &)),
            pView, SLOT(updateImage(const QModelIndex, const QModelIndex, const QVector<int>))); // reload image when data changed
    // spins for counting
    connect(ui->countEdges, SIGNAL(valueChanged(int)), this, SLOT(changedEdges(int)));
    connect(ui->countNodes, SIGNAL(valueChanged(int)), this, SLOT(changedNodes(int)));

    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(startAlgorithm()));

    auto s1 = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_R), this);
    s1->setEnabled(true);
    connect(s1, &QShortcut::activated, this, &MainWindow::startAlgorithm);

    emit graphView->updateImage();
}
void MainWindow::startAlgorithm() {
    ui->outputTextBrowser->clear();
    ui->outputTextBrowser->append("Starting algorithm of Mogoo");
    Graph *graph = Algorithms::hypergraph_to_usual(graphModel->graph);
    auto colors = Algorithms::Magoo(graph, ui->outputTextBrowser);
    for(auto &el: colors){
        qDebug() << el << "\n";
    }
    delete graph;
}

void MainWindow::changedEdges(int count) {
    emit graphModel->rebuildModel(count, -1);
}

void MainWindow::changedNodes(int count) {
    emit graphModel->rebuildModel(-1, count);
}
