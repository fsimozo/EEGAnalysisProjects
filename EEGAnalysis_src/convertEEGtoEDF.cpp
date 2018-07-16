/* 
 * File:   convertEEGtoEDF.cpp
 * Author: Fabricio
 * 
 * Created on 20 de Março de 2013, 13:50
 */

#include "convertEEGtoEDF.h"
#include <QProcess>
#include <QFileDialog>
#include <QFile>

convertEEGtoEDF::convertEEGtoEDF() {
}

convertEEGtoEDF::convertEEGtoEDF(const convertEEGtoEDF& orig) {
}

convertEEGtoEDF::~convertEEGtoEDF() {
}

/**
 * Uses edflib tool nk2edf.exe to convert .eeg files to .edf files.
 * Tries to convert to EDF+, and if the .log and .pnt files cannot be found, 
 * converts to edf instead.
 */
void convertEEGtoEDF::EEGtoEDF(){
    //Opens file dialog and gets path to EEG file
    QString s = QFileDialog::getOpenFileName(0, tr("Open File"),"/home",tr("EEG (*.eeg)\nAll files (*.*)"));
    
    //Checks if there are .LOG and .PNT files
    QString check = s;
    check.chop(4);
    QFile *logfile = new QFile(check+".log");
    QFile *pntfile = new QFile(check+".pnt");
    
    //If there are .LOG and .PNT files, creates edf+
    if(logfile->exists() && pntfile->exists()){
        //QString program = "C:/Users/Fabricio/Documents/NetBeansProjects/SpikeMarker/EEGtoEDF/nk2edf";
        QString program = "./EEGtoEDF/nk2edf";
        QStringList arguments;
        arguments << s;

        QProcess *convert = new QProcess(this);
        convert->start(program,arguments);
    }else{
        //Else, creates edf (by passing the argument "-no-annotations")
        QString program = "./EEGtoEDF/nk2edf";
        QStringList arguments;
        arguments << "-no-annotations" << s;

        QProcess *convert = new QProcess(this);
        convert->start(program,arguments);
    }
}
