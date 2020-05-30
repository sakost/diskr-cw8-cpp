#include <iostream>

#include <QApplication>
#include <QScreen>
#include <QLayout>

#include "mainwindow.h"
#include "graphmodel.h"
#include "graphview.h"

const QSize window_size = {1000, 800};

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    auto graph = new Graph;
    auto w = new MainWindow(new GraphView(graph), new GraphModel(graph));

    QScreen* screen = w->screen();
    QSize screen_size = screen->size();


    w->setGeometry(
            screen_size.width()/2-window_size.width()/2,
            screen_size.height()/2-window_size.height()/2,
            window_size.width(),
            window_size.height()
            );

    w->show();
    w->activateWindow();
    w->raise();
    return QApplication::exec();
}
