/* 
 * File:   Signals.cpp
 * Author: Fabricio
 * 
 * Created on 21 de Junho de 2013, 16:08
 */

#include "Signals.h"
#include "signal.h"
#include <stdio.h>

Signals::Signals() {
}

Signals::Signals(const Signals& orig) {
}

Signals::~Signals() {
}

void Signals::setFilePath(QString path){
    filepath = path;
}
void Signals::setDataOffset(int from){
    offset = from;
}
void Signals::setSampleFrequency(int freq){
    sample_frequency = freq;
}

/**
 * This function sets the reference for this group of signals
 * @param signal
 */
void Signals::setReference(QVector<double> signal, QString label){
    reference.setData(signal);
    reference.setLabel(label);
}
/**
 * This function adds a signal to the class
 * @param signal
 * @param label
 */
void Signals::addData(QVector<double> i_signal, QString label){
    int n_previousData = data.count();
    
    QVector< signal *> temp = data;
    
    data.resize(n_previousData+1);
    data[n_previousData] = new signal();
    for(int i=0; i<n_previousData; i++){
        data[i]->setData(temp[i]->getData());
        data[i]->setLabel(temp[i]->getLabel());
    }
    
    data[n_previousData]->setData(i_signal);
    data[n_previousData]->setLabel(label);
}
/**
 * This function removes a signal from the class
 * @param index
 */
void Signals::removeData(int index){
    for(int i = index; i<data.count()-1; i++){
//        data[i]=data[i+1];
    }
    data.resize(data.count()-1);
}
/**
 * This function adds a marker on a signal from the group
 * @param index
 * @param position
 */
void Signals::addMarker(int index, double position){
    data[index]->addMarker(position);
}
/**
 * This function removes a marker on a signal from the group
 * @param index
 * @param marker_index
 */
void Signals::removeMarker(int index, double position, double check_region){
    for(int i = 0; i<data[index]->getMarkers().count(); i++){
        if((data[index]->getMarkers()[i]>position-check_region) && (data[index]->getMarkers()[i]<position+check_region)){
            data[index]->removeMarker(i);
        }
    }
    if(!data[index]->getMarkers().isEmpty()){
        for(int i = 0; i<data[index]->getMarkers().count(); i++){
            if((data[index]->getMarkers()[i]>position-check_region) && (data[index]->getMarkers()[i]<position+check_region)){
                data[index]->removeMarker(i);
            }
        }
    }
}
/**
 * This function sets only the marked signals to be shown
 */
void Signals::setMarkedToShow(){
    indexToShow.clear();
    for(int i=0; i<data.count(); i++)
        if(data[i]->getMarkers().count()>0)
            indexToShow.append(i);
}
/**
 * This function sets all the signals to be shown
 */
void Signals::setAllToShow(){
    indexToShow.clear();
    for(int i=0; i<data.count(); i++)
        indexToShow.append(i);
    
}

/**
 * this function returns the number of signals on this group
 * @return 
 */
int Signals::count(){
    return data.count();
}
/**
 * This function returns a raw signal from this group
 * @param index
 * @return 
 */
QVector<double> Signals::getData(int index){
    return data[index]->getData();
}
/**
 * This function returns a referenced signal from the group, if reference is 
 * valid
 * @param index
 * @return 
 */
QVector<double> Signals::getDataWithReference(int index, int x_i, int n){
    QVector<double> temp = data[index]->getData().mid(x_i, n);
    if(reference.getData().count()>0){
        QVector<double> ref = reference.getData();
        for(int i=0; i<temp.count(); i++)
            temp[i]=temp[i]-ref[x_i+i];
    }
    return temp;
}
/**
 * This function returns the label of a signal from this group
 * @param index
 * @return 
 */
QString Signals::getLabel(int index){
    return data[index]->getLabel();
}
/**
 * This function returns the label of the reference signal for this group
 * @return 
 */
QString Signals::getReferenceLabel(){
    return reference.getLabel();
}
/**
 * This function returns a list of the indexes to be shown on main window
 * @return 
 */
QVector<int> Signals::getIndexToShow(){
    return indexToShow;
}
/**
 * This function returns the markers for a signal on this group
 * @param index
 * @return 
 */
QVector<double> Signals::getMarkers(int index){
    return data[index]->getMarkers();
}

/**
 * Returns file path for this signal group
 * @return 
 */
QString Signals::getFilePath(){
    return filepath;
}
/**
 * Returns sample frequency for this signal group
 * @return 
 */
int Signals::getSampleFrequency(){
    return sample_frequency;
}

void Signals::clear(){
    for(int i=filepath.count()-1; i>=0; i--){
        filepath.remove(i, 1);
    }
    filepath.clear();
    filepath.squeeze();
    
    for(int i=indexToShow.count()-1; i>=0; i--){
        indexToShow.remove(i);
    }
    indexToShow.clear();
    indexToShow.squeeze();
    
//    qDeleteAll(data);
    for(int i=data.count()-1; i>=0; i--){
        data[i]->clear();
        data.remove(i);
    }
    data.clear();
    data.squeeze();
//    data.resize(0);
//    data.clear();
    reference.clear();
}