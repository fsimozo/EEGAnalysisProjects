/* 
 * File:   methodResult.cpp
 * Author: Fabricio
 * 
 * Created on 18 de Julho de 2013, 13:59
 */

#include "methodResult.h"
#include "resultElement.h"

#include <stdio.h>
#include <QVector>

methodResult::methodResult(){
//    points = new resultElement();
//    graphs = new resultElement();
//    tables = new resultElement();
    
    points.clear();
    series.clear();
    graphs.clear();
    tables.clear();
}

methodResult::methodResult(const methodResult& orig) {
}

methodResult::~methodResult() {
}

/**
 * Adds a point to points QVector
 * @param name
 * @param value
 */
void methodResult::addPoint(QString name, double value){
    resultElement *temp_element = new resultElement;
    
    temp_element->setType("point");
    temp_element->setPoint(name, value);
    points.append(temp_element);
    
    connect(points[points.count()-1], SIGNAL(emitPoint(QString, double)),
                this, SLOT(addPointToResult(QString, double)));
}
/**
 * Adds a series to series QVector
 * @param name
 * @param X
 */
void methodResult::addSeries(QString name, QVector<double> X){
    resultElement *temp_element = new resultElement;
    
    temp_element->setType("series");
    temp_element->setSeries(name, X);
    series.append(temp_element);
    
    connect(series[series.count()-1], SIGNAL(emitSeries(QString, QVector<double>)),
            this, SLOT(addSeriesToResult(QString, QVector<double>)));
}
/**
 * Adds a graph to graphs QVector
 * @param name
 * @param X_label
 * @param Y_label
 * @param X
 * @param Y
 */
void methodResult::addGraph(QString name, QString X_label, QString Y_label, QVector<double> X, QVector<double> Y){
    resultElement *temp_element = new resultElement;
    QString type_arg = "graph";
    temp_element->setType(type_arg);
    temp_element->setGraph(name, X_label, Y_label, X, Y);
    graphs.append(temp_element);
    connect(graphs[graphs.count()-1], SIGNAL(emitGraph(QString, QString, QString, QVector<double>, QVector<double>)),
            this, SLOT(addGraphToResult(QString, QString, QString, QVector<double>, QVector<double>)));
}
/**
 * Adds a table to tables QVector
 * @param name
 * @param horizontal
 * @param vertical
 * @param data
 */
void methodResult::addTable(QString name, QVector<QString> horizontal, QVector<QString> vertical, QVector<QVector<double> > data){
    resultElement *temp_element = new resultElement;
    temp_element->setType("table");
    temp_element->setTable(name, horizontal, vertical, data);
    tables.append(temp_element);
    
    connect(tables[tables.count()-1], SIGNAL(emitTable(QString, QVector<QString>, QVector<QString>, QVector<QVector<double>>)),
            this, SLOT(addTableToResult(QString, QVector<QString>, QVector<QString>, QVector<QVector<double>>)));
}

void methodResult::setResultName(QString resultName, QString dirName, QString savingPath){
    this->resultName = resultName;
    this->dirName = dirName;
    this->savingPath = savingPath;
}

/**
 * For each element on points array, connects the signal with the slot and
 * runs the signal emitter function
 * @return 
 */
QString methodResult::getPoints(){
    result_points.clear();
    for(int i=0; i<points.count(); i++){
        points[i]->getResultElement();
    }
    
    return result_points;
}
/**
 * For each element on series array, connects the signal with the slot and
 * runs the signal emitter function
 * @return 
 */
QString methodResult::getSeries(){
    result_series.clear();
    for(int i=0; i<series.count(); i++){
        series[i]->getResultElement();
    }
    
    return result_series;
}
/**
 * For each element on graphs array, connects the signal with the slot and
 * runs the signal emitter function
 * @return 
 */
QString methodResult::getGraphs(){
    result_graphs.clear();
    for(int i=0; i<graphs.count(); i++){
//        graphs[i]->getResultElement();
        return graphs[i]->getResult();
    }
    
//    return result_graphs;
}
/**
 * For each element on tables array, connects the signal with the slot and
 * runs the signal emitter function
 * @return 
 */
QString methodResult::getTables(){
    result_tables.clear();
    for(int i=0; i<tables.count(); i++){
        tables[i]->getResultElement();
    }
    
    return result_tables;
}

QString methodResult::getResultName(){
    return this->resultName;
}
QString methodResult::getDirName(){
    return this->dirName;
}
QString methodResult::getSavingPath(){
    return this->savingPath;
}

//SLOTS
/**
 * Receives the signal from an element of the points array and adds the names
 * and values to point result QString
 * @param name
 * @param value
 */
void methodResult::addPointToResult(QString name, double value){
    result_points.append(name+" = "+QString::number(value)+"\n");
}
/**
 * Receives the signal from an element of the series array and adds the names
 * and values to series result QString
 * @param name
 * @param X
 */
void methodResult::addSeriesToResult(QString name, QVector<double> X){
//    result_series.append(name+"\n");
    for(int i=0; i<X.count(); i++){
        result_series.append(QString::number(X[i]).trimmed()+"\n");
    }
}
/**
 * Receives the signal from an element of the graphs array and adds the names,
 * labels and values to graph result QString
 * @param name
 * @param X_label
 * @param Y_label
 * @param X
 * @param Y
 */
void methodResult::addGraphToResult(QString name, QString X_label, QString Y_label, QVector<double> X, QVector<double> Y){
//    result_graphs.append(name+"\n");
//    result_graphs.append(X_label.trimmed()+"\t"+Y_label.trimmed()+"\n");
    for(int i=0; i<X.count();i++)
        result_graphs.append(QString::number(X[i]).trimmed()+"\t"+QString::number(Y[i]).trimmed()+"\n");
    result_graphs.append("\n\n");
}
/**
 * Receives the signal from an element of the tables array and adds the names,
 * column and row labels and matrix data to table result QString
 * @param name
 * @param horizontal
 * @param vertical
 * @param data
 */
void methodResult::addTableToResult(QString name, QVector<QString> horizontal, QVector<QString> vertical, QVector<QVector<double> > data){
    result_tables.append(name+"\n");
    result_tables.append(" \t");
    for(int i=0; i<horizontal.count(); i++){
        result_tables.append(horizontal[i]+"\t");
    }
    result_tables.append("\n");
    for(int i=0; i<vertical.count();i++){
        result_tables.append(vertical[i]+"\t");
        for(int j=0; j<horizontal.count();j++){
            result_tables.append(QString::number(data[i][j])+"\t");
        }
        result_tables.append("\n");
    }
    result_tables.append("\n\n");
}