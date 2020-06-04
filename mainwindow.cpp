#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QShortcut>

#include <vector>
#include <set>
#include <string>
#include <sstream>


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
    connect(ui->addButton, &QPushButton::clicked, graphModel, &GraphModel::addNode);
    connect(ui->deleteButton, &QPushButton::clicked, graphModel, &GraphModel::deleteNode);

    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(startAlgorithm()));

    auto s1 = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_R), this);
    s1->setEnabled(true);
    connect(s1, &QShortcut::activated, this, &MainWindow::startAlgorithm);

    emit graphView->updateImage();
}

long double fact(int N)
{
    int sum=0, plus=1;
    while (N > 1) {
        sum += plus;
        plus++;
        N--;
    }
    return sum;
}

//перебор комбинаций
bool NextSet(std::vector<int> &a, int n, int m)
{
    int k = m;
    for (int i = k - 1; i >= 0; --i)
        if (a[i] < n - k + i + 1)
        {
            ++a[i];
            for (int j = i + 1; j < k; ++j)
                a[j] = a[j - 1] + 1;
            return true;
        }
    return false;
}

void MainWindow::startAlgorithm() {
    ui->outputTextBrowser->clear();
    ui->outputTextBrowser->append("Starting algorithm");

    Graph *graph = graphModel->graph;
    int n,m, graph1, graph2;
    n = m = graph->size();
    std::vector<std::vector<int>> a(n, std::vector<int>(m));
    std::vector<std::pair<int, int>> pairs;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            a[i][j] = (*graph)[i][j];
        }
    }

    int count, kol;
    count = 0;
    kol = 0;
    //создание пар несмежных вершин
    for (int i = 0; i < n; i++) {
        for (; count < m; count++) {
            if (i != count && a[i][count] == 0) {
                graph1 = i + 1;
                graph2 = count + 1;
                pairs.push_back(std::make_pair(graph1, graph2));
            }
        }
        kol++;
        count = kol;
    }

    int point;
    point = pairs.size();


    int t = n, s, run = 1, k;
    std::set<int> set1;
    std::set<int> set2;
    std::vector<int> lan(t), res(t);
    s = t - 1;
    std::vector<std::vector<int>> result(run, std::vector<int>(t));
    for (int i = 0; i < t; i++)
        lan[i] = i + 1;
    for (int i = 0; i < t; i++) {
        result[0][i] = lan[i];
    }

    res = lan;
    k = s;

    lan.pop_back();
    res.pop_back();
    if (t > s) {
        for (; k >= 2; k--) {
            result.push_back(lan);
            while (NextSet(lan, t, s)) {
                result.push_back(lan);
            }
            s--;
            res.pop_back();
            lan = res;
        }
    }

    std::vector<std::vector<int>> result1;

    int final = 0, Ron;
    for (int i = 0; i < result.size(); i++) {
        for (int j = 0; j < result[i].size() - 1; j++) {
            for (int crow = 0; crow < pairs.size(); crow++) {
                for (int koron = j + 1; koron < result[i].size(); koron++) {
                    if (result[i][j] == pairs[crow].first) {
                        if (result[i][koron] == pairs[crow].second) {
                            final++;
                            break;
                        }
                    }
                }
            }

        }

        Ron = fact(result[i].size());
        if (final == Ron) {
            result1.push_back(result[i]);
        }
        final = 0;
    }
    std::vector<std::vector<int>> done;

    for (int i = result1.size() - 1; i >= 0; i--) {
        bool flag = true;
        for (int kroll = i - 1; kroll >= 0; kroll--) {
            if (includes(result1[kroll].begin(), result1[kroll].end(), result1[i].begin(), result1[i].end()) ||
                i == 0) {
                flag = false;
                break;
            }
        }
        if (flag)
            done.push_back(result1[i]);
    }
    for (int i = 0; i < done.size(); ++i) {
        std::stringstream ss;
        for (int j = 0; j < done[i].size(); ++j) {
            ss << done[i][j] << ' ';
        }
        std::string temp;
        std::getline(ss, temp);
        ui->outputTextBrowser->append("{ " + QString(temp.c_str()) + " }");
        ui->outputTextBrowser->append("Done!");
    }
}

