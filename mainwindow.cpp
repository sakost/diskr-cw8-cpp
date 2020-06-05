#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QShortcut>
#include <QColorDialog>

#include "Algorithms.h"

#include <vector>
#include <sstream>

QColor randomColor(){
    return QColor(
            QRandomGenerator64::global()->generate64() % 236 + 20,
            QRandomGenerator64::global()->generate64() % 236 + 20,
            QRandomGenerator64::global()->generate64() % 236 + 20);
}

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

    for (int i = 0; i < DEFAULT_NODES; ++i) {
        graphView->colors->push_back(randomColor());
    }
    syncColors();

    ui->tableView->setModel(graphModel);
    delete centralWidget()->layout()->replaceWidget(ui->graphOpenGLWidget, graphView);
    delete ui->graphOpenGLWidget;

    connect(graphModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int> &)),
            this, SLOT(syncColors())); // reload image when data changed
    // spins for counting
    connect(ui->countEdges, SIGNAL(valueChanged(int)), this, SLOT(changedEdges(int)));
    connect(ui->countNodes, SIGNAL(valueChanged(int)), this, SLOT(changedNodes(int)));

    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(startAlgorithm()));

    // spins for colour
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(changedColors(int)));

    // change color
    connect(ui->toolButton, &QToolButton::clicked, this, &MainWindow::changeColor);

    auto s1 = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_R), this);
    s1->setEnabled(true);
    connect(s1, &QShortcut::activated, this, &MainWindow::startAlgorithm);
}
void MainWindow::startAlgorithm() {
    ui->outputTextBrowser->clear();
    ui->outputTextBrowser->append("Starting algorithm");
    Graph *graph = Algorithms::hypergraph_to_usual(graphModel->graph);
    auto iicolors = Algorithms::get_colors(graph);
    delete graph;
    graphView->started = false;
    checkColors(iicolors);

    std::stringstream ss;

    for(auto &col: color2nodes.keys()){
        ss << "color: " << col << " {";
        for (int i = 0; i < color2nodes[col].size(); ++i) {
            if(graphView->colors->size() > i) {
                if(!colors.contains(col)){
                    continue;
                }
                ss << color2nodes[col][i]+1 << ' ';
            }
        }
        ss << " }" <<std::endl;
    }
    ss << "Done!";
    std::string text = ss.str();
    ui->outputTextBrowser->append(QString(text.c_str()));
}

void MainWindow::changedEdges(int count) {
    graphModel->rebuildModel(count, -1);
}

void MainWindow::changedNodes(int count) {
    graphModel->rebuildModel(-1, count);
}

void MainWindow::checkColors(const QVector<int_fast64_t> &icolors) {
    color2nodes.clear();
    for (int i = 0; i < icolors.size(); ++i) {
        if(!color2nodes.count(icolors[i])) {
            color2nodes[icolors[i]] = QVector<int_fast64_t>();
        }
        color2nodes[icolors[i]].push_back(i);
    }
    syncColors();
}

void MainWindow::changedColors(int value) {
    if(!colors.contains(value)){
        colors[value] = randomColor();
    }
    QPixmap pix(100, 100);
    pix.fill(colors[value]);
    ui->toolButton->setIcon(QIcon(pix));
    color_index = value;
}

void MainWindow::changeColor() {
    auto col = QColorDialog::getColor(colors[color_index], this, QString("Color for %1 nodes group").arg(color_index+1));
    if(!col.isValid()){
        return;
    }
    colors[color_index] = col;
    syncColors();
}

void MainWindow::syncColors() {
    graphView->colors->clear();

    int_fast64_t nodes = graphModel->columnCount(QModelIndex());

    graphView->colors->resize(nodes);

    for(auto &col: color2nodes.keys()){
        for (int i = 0; i < color2nodes[col].size(); ++i) {
            if(graphView->colors->size() > i) {
                if(!colors.contains(col)){
                    colors[col] = randomColor();
                }
                (*graphView->colors)[color2nodes[col][i]] = colors[col];
            }
        }
    }

    this->changedColors(color_index);
    graphView->updateImage();
}
