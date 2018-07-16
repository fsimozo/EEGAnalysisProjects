/* 
 * File:   writeMarkersText.cpp
 * Author: Fabricio
 * 
 * Created on 18 de Março de 2013, 16:46
 */

#include "writeMarkersText.h"

#include <stdio.h>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDate>
#include <QTime>

writeMarkersText::writeMarkersText() {
}

writeMarkersText::writeMarkersText(const writeMarkersText& orig) {
}

writeMarkersText::~writeMarkersText() {
}

/**
 * Write a .txt file containing the labels of the openned data channels on MainWindow
 * and the data points corresponding to all the makers created for each channel.
 * @param data
 * @param labels
 */
void writeMarkersText::writeFile(QVector<QVector<double> > data, QStringList labels, QString reference_label, int type){
    //Opens dialog for saving
    QString s;
    if(type == 0)
        s = QFileDialog::getSaveFileName(0, tr("Save File"),"/markers",tr("TXT (*.txt)\nAll files (*.*)"));
    else if(type == 1)
        s = QFileDialog::getSaveFileName(0, tr("Save File"),"/markers_1",tr("TXT (*.txt)\nAll files (*.*)"));
    
    QFile file(s);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
//    out << "Markers selected from SpikeMarker software\n";
//    out << "Each row contains all markers for the corresponding channel\n";
//    out << "No vallues means no markers were made\n\n";
//    out << "Reference electrode: " << reference_label.toAscii() << "\n";
    
    int max = data[0].count();
    
    labels.replaceInStrings(QRegExp("^\\t*"),"");
    
    for(int i=0; i<labels.count(); i++){
        out << labels[i] << "\t\t";
        if(data[i].count()>max){
            max = data[i].count();
        }
    }
    out << "\n";
    
    for(int i=0; i<max; i++){
        for(int j=0; j<data.count(); j++){
            if(data[j].count()>i){
                out << QString::number(data[j][i]) << "\t\t";
            }else{
                out << "\t\t";
            }
        }
        out << "\n";
    }
    
    //closes file
    file.close(); 
}

void writeMarkersText::writeMethodResults(methodResult* result, QString path, QString folderName, QString fileName){
    QString date = QDate::currentDate().toString("dd.MM.yyyy");
    QString time = QTime::currentTime().toString("hh.mm.ss");

    QString output;
    
    QDir filePath = path;
    
//    folderName = folderName.replace(".edf", "", 0);
    
    if(!QDir(path+"/"+folderName).exists()){
        filePath.mkdir(folderName);
    }
    filePath.cd(folderName);
    
    //Save point information
    if((output=result->getPoints()).count()>0){
        QFile point_file(filePath.absolutePath()+"/"+fileName+"p.txt");
        point_file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&point_file);
        out << output.toAscii();

        point_file.close();
    }
    output.clear();
    
    //Save series information
    if((output=result->getSeries()).count()>0){
        QFile series_file(filePath.absolutePath()+"/"+fileName+"s.txt");
        series_file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream s_out(&series_file);
        
        s_out << output.toAscii();
        
        series_file.close();
    }
    output.clear();
    
    //Save graph information
    if((output=result->getGraphs()).count()>0){
        QFile graph_file(filePath.absolutePath()+"/"+fileName+"g.txt");
        graph_file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream g_out(&graph_file);
        
//        output.replace(".", ",", Qt::CaseSensitive);
        g_out << output.toAscii();
        graph_file.close();
    }
    output.clear();
    //Save table information
    if((output=result->getTables()).count()>0){
        QFile table_file(filePath.absolutePath()+"/"+fileName+"t.txt");
        table_file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream t_out(&table_file);
        t_out << output.toAscii();

        table_file.close();
    }
}