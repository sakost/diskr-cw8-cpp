#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::addNode() {
    graphModel->addNode();
}

void MainWindow::removeLastNode() {
    graphModel->removeLastNode();
}



MainWindow::MainWindow(GraphView *pView, GraphModel* model, QWidget *parent)
            : QMainWindow(parent),
            ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Курсовая работа №8");
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    graphView = pView;
    graphModel = model;

    ui->tableView->setModel(graphModel);
    delete centralWidget()->layout()->replaceWidget(ui->graphOpenGLWidget, graphView);
    delete ui->graphOpenGLWidget;

    connect(ui->startButton, SIGNAL(clicked()), pView, SLOT(updateImage())); // todo replace with algorithm
    connect(graphModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int> &)),
            pView, SLOT(updateImage(const QModelIndex, const QModelIndex, const QVector<int>))); // reload image when data changed



    emit graphView->updateImage();
}
