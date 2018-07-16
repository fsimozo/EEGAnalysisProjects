/* 
 * File:   resultElement.cpp
 * Author: Fabricio
 * 
 * Created on 18 de Julho de 2013, 15:19
 */

#include "resultElement.h"
#include <stdio.h>

resultElement::resultElement() {
}

resultElement::resultElement(const resultElement& orig) {
}

resultElement::~resultElement() {
}
/**
 * Sets the type of this element: "point", "graph" or "table"
 * @param type
 */
void resultElement::setType(QString type){
    if(type!="point" && type!="graph" && type!="table" && type!="series")
        return;
    
    this->type = type;
    
}

/**
 * Sets the name and value of the point element. Only works if type is "point"
 * @param name
 * @param value
 */
void resultElement::setPoint(QString name, double value){
    if(type!="point")
        return;
    
    this->point_name = name;
    this->point_value = value;
}
/**
 * Sets the name, labels and values of the series element. Only works if type is "series"
 * @param name
 * @param X
 */
void resultElement::setSeries(QString name, QVector<double> X){
    if(type!="series")
        return;
    
    this->series_name = name;
    this->series_Xdata = X;
}
/**
 * Sets the name, labels and values of the graph element. Only works if type is "graph"
 * @param name
 * @param X_label
 * @param Y_label
 * @param X
 * @param Y
 */
void resultElement::setGraph(QString name, QString X_label, QString Y_label, QVector<double> X, QVector<double> Y){
    if(type!="graph")
        return;
    
    this->graph_name = name;
    this->graph_Xlabel = X_label;
    this->graph_Ylabel = Y_label;
    this->graph_Xdata = X;
    this->graph_Ydata = Y;
}
/**
 * Sets the name, colum and row labels and matrix data of the table element. Only works if type is "table"
 * @param name
 * @param horizontal
 * @param vertical
 * @param data
 */
void resultElement::setTable(QString name, QVector<QString> horizontal, QVector<QString> vertical, QVector<QVector<double> > data){
    if(type!="table")
        return;
    
    this->table_name = name;
    this->table_horizontal = horizontal;
    this->table_vertical = vertical;
    this->table_data = data;
}


/**
 * Emits all the data corresponding to this element's type.
 * Is connected to a slot on methodResult class
 */
void resultElement::getResultElement(){
    if(type=="point"){
        emit emitPoint(point_name, point_value);
    }
    if(type=="series"){
        emit emitSeries(series_name, series_Xdata);
    }
    if(type=="graph"){
        emit emitGraph(graph_name, graph_Xlabel, graph_Ylabel, graph_Xdata, graph_Ydata);
    }
    if(type=="table"){
        emit emitTable(table_name, table_horizontal, table_vertical, table_data);
    }
}