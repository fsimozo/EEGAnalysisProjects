/* 
 * File:   fileQueue.cpp
 * Author: fabricio
 * 
 * Created on April 8, 2014, 3:18 PM
 */

#include "SignalFile.h"

#include <stdio.h>

#include <QVector>
#include <QStringList>
#include <QDir>

SignalFile::SignalFile(QString path, QStringList channelLabels, QStringList selectedChannelLabels, QVector<int> channelIndexes, int type) {
    this->path=path;
    this->channelLabels = channelLabels;
    this->selectedChannelLabels = selectedChannelLabels;
    this->channelIndexes = channelIndexes;
    this->type = type;
}

SignalFile::SignalFile(const SignalFile& orig) {
}

SignalFile::~SignalFile() {
}

void SignalFile::setStart(int startSample){
    this->startSample = startSample;
}

void SignalFile::setEnd(int endSample){
    this->endSample = endSample;
}

void SignalFile::setSampleFrequency(int sampleFrequency){
    this->sampleFrequency = sampleFrequency;
}

QString SignalFile::getPath(){
    return this->path;
}

QString SignalFile::getFileName(){
    QDir dir = this->path;
    
    return dir.dirName();
}

QStringList SignalFile::getChannelLabels(){
    return this->channelLabels;
}

QStringList SignalFile::getSelectedChannelLabels(){
    return this->selectedChannelLabels;
}

QVector< int > SignalFile::getChannelIndexes(){
    return this->channelIndexes;
}

int SignalFile::getStart(){
    return this->startSample;
}

int SignalFile::getEnd(){
    return this->endSample;
}

int SignalFile::getSampleFrequency(){
    return this->sampleFrequency;
}

int SignalFile::getType(){
    return this->type;
}

int SignalFile::getNumberOfSamples(){
    return (this->endSample - this->startSample);
}

int SignalFile::count(){
    return this->path.count();
}