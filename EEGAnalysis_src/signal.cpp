/* 
 * File:   signal.cpp
 * Author: Fabricio
 * 
 * Created on 21 de Junho de 2013, 14:54
 */

#include "signal.h"
#include <stdio.h>

signal::signal() {
}

signal::signal(const signal& orig) {
}

signal::~signal() {
}

/**
 * This function sets the data of a signal
 * @param data
 */
void signal::setData(QVector<double> data){
    this->data = data;
}
/**
 * This function sets the label of a signal
 * @param data_label
 */
void signal::setLabel(QString data_label){
    label = data_label;
}
/**
 * This function sets the index of this data on the original .edf file
 * @param index
 */
void signal::setFileIndex(int index){
    file_index = index;
}
/**
 * This function adds a marked position to markers array
 * @param position
 */
void signal::addMarker(double position){
    markers.append(position);
}
/**
 * This function removes a marked position from markers array
 * @param index
 */
void signal::removeMarker(int index){
    markers.remove(index);
}

/**
 * This function returns the data of a signal
 * @return 
 */
QVector< double > signal::getData(){
    return this->data;
}
/**
 * This function returns the label of a signal
 * @return 
 */
QString signal::getLabel(){
    return label;
}
/**
 * This function returns the index of a signal for it's original EDF file
 * @return 
 */
int signal::getFileIndex(){
    return file_index;
}
/**
 * This function returns an array containing all the markers made on a signal
 * @return 
 */
QVector< double > signal::getMarkers(){
    return markers;
}

void signal::clear(){
    for(int i=data.count()-1; i>=0; i--){
        data.remove(i);
    }
    data.clear();
    data.squeeze();
    
    for(int i=label.count()-1; i>=0; i--){
        label.remove(i,1);
    }
    label.clear();
    label.squeeze();
    
    for(int i=markers.count()-1; i>=0; i--){
        markers.remove(i);
    }
    markers.clear();
    markers.squeeze();
}