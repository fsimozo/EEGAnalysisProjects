/* 
 * File:   readEDF.h
 * Author: Fabricio
 *
 * Created on 6 de Março de 2013, 10:12
 */

#ifndef _READEDF_H
#define	_READEDF_H

#include "ui_readEDF.h"
#include "edflib.h"

#include "SignalFile.h"

#include <QMutex>

class readEDF : public QDialog {
    Q_OBJECT
public:
    readEDF();
    virtual ~readEDF();
    
    Ui::readEDF widget;
public slots:
    int GetFilePath();
    int GetFilePath(QString path);
    void OpenSelectWindow(char* path);
    void readFile(int type);
    void showFileInfo();
    void addFile();
    void addAllFiles();
    void removeFile();
    void showAddedFileInfo();
    QVector<SignalFile*> getFiles();
    void readChannels(SignalFile* file);
    void readThread(int hdl, int j, int n, double* buf, int counter);
    void readReference(int hdl, int j, int n, double* buf, int counter);
    void readReferenceOnly(int index, QString filepath, int n, int from);
    void getData();
    void closeFile(int hdl);

signals:
    void EmitFilePath(char* path);
    void EmitFilePathToMain(char* path);
    void EmitSampleFrequency(int freq);
    void EmitFrom(int from);
    void EmitHandle(int hdl);
    void ThreadsFinished();
    void EmitData(QVector< QVector<double> > data, QVector< double > reference);
    void emitReference(QString label, QVector< double > reference);
//    void FileLoaded();
        
private:
    QVector<SignalFile*> files;
    
    struct edf_hdr_struct hdr;
    //int hdl;
    QVector< QVector<double> > data;
    QVector< int > isSelected;
    QStringList labels;
    QStringList allLabels;
    int reference_index;
    QVector< double > reference;
    int LimitVallue;
    int reference_done;
    QMutex mutex;
};

#endif	/* _READEDF_H */
