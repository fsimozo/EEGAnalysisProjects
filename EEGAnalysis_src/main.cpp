/*
 * File:   main.cpp
 * Author: Fabricio
 *
 * Created on 28 de Fevereiro de 2013, 10:53
 */

#include <QtGui/QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);

    // create and show your widgets here
    MainWindow main;
    main.showMaximized();
    
    return app.exec();
}
