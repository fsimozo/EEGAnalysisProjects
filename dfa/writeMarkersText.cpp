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

void writeMarkersText::writeMethodResults(methodResult* result, QDir path, QString ch_label, QString method_name, int channel_index){
    QString date = QDate::currentDate().toString("dd.MM.yyyy");
    QString time = QTime::currentTime().toString("hh.mm.ss");

    QVector< QString > file_names;
    file_names.resize(22);
    file_names[0]="01-FP1-F7";
    file_names[1]="02-F7-T7";
    file_names[2]="03-T7-P7";
    file_names[3]="04-P7-O1";
    file_names[4]="05-FP1-F3";
    file_names[5]="06-F3-C3";
    file_names[6]="07-C3-P3";
    file_names[7]="08-P3-O1";
    file_names[8]="09-FP2-F4";
    file_names[9]="10-F4-C4";
    file_names[10]="11-C4-P4";
    file_names[11]="12-P4-O2";
    file_names[12]="13-FP2-F8";
    file_names[13]="14-F8-T8";
    file_names[14]="15-T8-P8";
    file_names[15]="16-P8-O2";
    file_names[16]="17-FZ-CZ";
    file_names[17]="18-CZ-PZ";
    file_names[18]="19-P7-T7";
    file_names[19]="20-T7-FT9";
    file_names[20]="21-FT9-FT10";
    file_names[21]="22-FT10-T8";
    
    QString output;
    //Save point information
//    for(int i=0; i<50; i++){
//        if(!(QFile(path.absolutePath()+"/"+"DFA analysis"+"/"+file_names[i]+".txt").exists())){
////                printf((path.absolutePath()+"/"+"DFA analysis"+"/graphs"+"_"+QString::number(i)+".txt").toAscii()+"\n");
//                ch_label = file_names[i];
//                break;
//        }
//    }
    
    ch_label = file_names[channel_index];
    
//    path.mkdir(method_name+"."+ch_label);
//    path.cd(method_name+"."+ch_label);
//    if(!QDir("D:/Bibliotecas/Dropbox/Mestrado/CBFM 2013/Testes DFA/CHB-MIT Scalp EEG/chb01/DFA analysis").exists()){
//        path.mkdir("DFA analysis");
//    }
//    path.cd("DFA analysis");
    
//    printf("dir done\n");
    
    if((output=result->getPoints()).count()>0){
        QFile point_file(path.absolutePath()+"/points"+"_"+ch_label+".txt");
        point_file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&point_file);
        out << "This file contains all the 'points' variables calculated by "<<method_name<<" method.\n";
        out << "channel: "<< ch_label << "; date: " << date << "; time: " << time << "\n";
        out << output.toAscii();

        point_file.close();
    }
//    printf("points done\n");
    output.clear();
    
//    Save series information
    if((output=result->getSeries()).count()>0){
        QFile series_file(path.absolutePath()+"/"+ch_label+".txt");
        series_file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream s_out(&series_file);
        
//        output.replace(".", ",", Qt::CaseSensitive);
        
        s_out << output.toAscii();
        
        series_file.close();
    }
    output.clear();
    
    //Save graph information
    if((output=result->getGraphs()).count()>0){
        QFile graph_file(path.absolutePath()+"/"+ch_label+".txt");
        graph_file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream g_out(&graph_file);
        
//        output.replace(".", ",", Qt::CaseSensitive);
        
        g_out << output.toAscii();

        graph_file.close();
    }
//    printf("graphs done\n");
    output.clear();
    
    //Save table information
    if((output=result->getTables()).count()>0){
        QFile table_file(path.absolutePath()+"/tables.txt");
        table_file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream t_out(&table_file);
        t_out << "This file contains all the tables calculated by "<<method_name<<" method.\n";
        t_out << "channel: "<< ch_label << "; date: " << date << "; time: " << time << "\n";
        t_out << output.toAscii();

        table_file.close();
    }
//    printf("tables done\n");
}