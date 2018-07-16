/*
 * File:   readEDF.cpp
 * Author: Fabricio
 *
 * Created on 6 de Março de 2013, 10:12
 */

#include "readEDF.h"

#include "edflib.h"
#include "graphCurveConstruction.h"
#include "SignalFile.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QFuture>
#include <QtConcurrentRun>
#include <QSignalMapper>
#include <QMessageBox>
#include <QStringList>

readEDF::readEDF() {
    //GetFilePath();
    widget.setupUi(this);
    
    reference_done = 0;
    
//    files = new fileQueue();
    
    connect(widget.buttonBox, SIGNAL(rejected()),
            this, SLOT(close()));
    
    QSignalMapper *readMapper = new QSignalMapper(this);
    connect(readMapper, SIGNAL(mapped(int)),
            this, SLOT(readFile(int)));
    readMapper->setMapping(widget.readFileButton, 0);
    connect(widget.readFileButton, SIGNAL(pressed()),
            readMapper, SLOT(map()));
    
    connect(widget.filesLoaded, SIGNAL(currentRowChanged(int)),
            this, SLOT(showFileInfo()));
    connect(widget.filesAdded, SIGNAL(itemPressed(QListWidgetItem*)),
            this, SLOT(showAddedFileInfo()));
    connect(widget.addButton, SIGNAL(pressed()),
            this, SLOT(addFile()));
    connect(widget.addAllFilesButton, SIGNAL(pressed()),
            this, SLOT(addAllFiles()));
    connect(widget.removeButton, SIGNAL(pressed()),
            this, SLOT(removeFile()));
    
    connect(this, SIGNAL(ThreadsFinished()),
            this, SLOT(getData()));
}

readEDF::~readEDF() {
}
//Gets path to EDF file and emits signal with it to the filler
//Returns 0 if file not selected, 1 if succeded
int readEDF::GetFilePath(){
//    QString s;
//    char filepath[256];
//    //Opens the file chooser window and copies the path to a string
//    s = QFileDialog::getOpenFileName(this, tr("Open File"),"/home",tr("EDF (*.edf)\nAll files (*.*)"));
//    strcpy(filepath, s.toAscii().constData());
//    
    int fileselected;
//    QFile *file = new QFile(s);
//    if(file->exists()){
//        fileselected=1;
//        disconnect(this, SIGNAL(EmitFilePath(char*)),
//                this, SLOT(OpenSelectWindow(char*)));
//        connect(this, SIGNAL(EmitFilePath(char*)),
//                this, SLOT(OpenSelectWindow(char*)));
//        emit EmitFilePath(filepath);
//        emit EmitFilePathToMain(filepath);
//    }else{
//        fileselected=0;
//    }
//    return fileselected;
    return 1;
}

int readEDF::GetFilePath(QString path){ 
    char filepath[256];
    
    strcpy(filepath, path.toAscii().constData());
    
    int fileselected;
    QFile *file = new QFile(path);
    if(file->exists()){
        fileselected=1;
        disconnect(this, SIGNAL(EmitFilePath(char*)),
                this, SLOT(OpenSelectWindow(char*)));
        connect(this, SIGNAL(EmitFilePath(char*)),
                this, SLOT(OpenSelectWindow(char*)));
        emit EmitFilePath(filepath);
        emit EmitFilePathToMain(filepath);
    }else{
        fileselected=0;
    }
    return fileselected;
}

//Gets path from signal and fills all text labels with EDF file properties
void readEDF::OpenSelectWindow(char* path){
    QFileInfo *fileInfo = new QFileInfo(path);
    
    QStringList chlabels;
    QStringList chfreq;
    
    widget.chList->clear();
    
    if(fileInfo->suffix()=="edf"){
        edfopen_file_readonly(path, &hdr, 0);

        //Shows some header information
        //widget.ch ->append("Channels: "+ QString::number(hdr.edfsignals));
        widget.nameLabel->setText(QString::fromAscii(hdr.patient));
        if(widget.nameLabel->text()==""){
            widget.nameLabel->setText(QString::fromAscii(hdr.patient_name));
        }
        widget.recordingLabel->setText(QString::fromAscii(hdr.recording));
        widget.dateLabel->setText(QString::number(hdr.startdate_day) + "/" + QString::number(hdr.startdate_month)
                                    + "/" + QString::number(hdr.startdate_year));
        widget.timeLabel->setText(QString::number(hdr.starttime_hour) + ":" + QString::number(hdr.starttime_minute)
                                    + ":" + QString::number(hdr.starttime_second) + ":" + QString::number((double)hdr.starttime_subsecond/10000));
        widget.filedurationLabel->setText(QString::number((double)hdr.file_duration/EDFLIB_TIME_DIMENSION) + " seconds");
        widget.nOfSamplesLabel->setText(QString::number((double)hdr.signalparam[0].smp_in_file));
        widget.freqLabel->setText(QString(QString::number(((double)hdr.signalparam[0].smp_in_datarecord / (double)hdr.datarecord_duration) * EDFLIB_TIME_DIMENSION)));

        widget.lineFrom->setText(QString::number(0));
        widget.lineTo->setText(QString::number((int)hdr.signalparam[0].smp_in_file));

        //Gets the label and frequency (Hz) for each channel
        //Adds data labels to chList
        for(int i =0; i<hdr.edfsignals; i++){
            chlabels+=hdr.signalparam[i].label;
            chfreq+=QString::number(((double)hdr.signalparam[i].smp_in_datarecord / (double)hdr.datarecord_duration) * EDFLIB_TIME_DIMENSION);
        }

        //Emits sample frequency to main
        int freq = ((double)hdr.signalparam[0].smp_in_datarecord / (double)hdr.datarecord_duration) * EDFLIB_TIME_DIMENSION;
        emit(EmitSampleFrequency(freq));

    //    //Closes EDF
        edfclose_file(hdr.handle);
        
    }else
    if(fileInfo->suffix()=="txt"){
        QFile file(path);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::information(this, "Error", "Failed to read file.");
        }

        QTextStream in(&file);
        in.setAutoDetectUnicode(true);
        //reads first line
        in.readLine();
        //reads channel names
        while(!in.atEnd()){
            QString line = in.readLine();
            if(!line.trimmed().compare(""))
                break;
            
            line.remove(":");
            line.remove("uV");
            chlabels.append(line);
        }
        
        //Clears labels
        widget.nameLabel->clear();
        widget.recordingLabel->clear();
        widget.dateLabel->clear();
        widget.timeLabel->clear();
        widget.filedurationLabel->clear();
        widget.nOfSamplesLabel->clear();
        widget.freqLabel->clear();
        
        widget.lineFrom->setText(QString::number(0));
        widget.lineTo->clear();
        
        //set labels found on file
        QString sampleFreq = in.readLine();
        QString something = in.readLine();
        QString numberOfSamples = in.readLine();
        
        sampleFreq.remove("sr:");
        sampleFreq.remove(".0");
        something.remove("x:");
        numberOfSamples.remove("len:");
        
        widget.freqLabel->setText(sampleFreq);
        widget.nOfSamplesLabel->setText(numberOfSamples);
        widget.lineTo->setText(numberOfSamples);
        file.close();
    }else{
        QMessageBox::information(this, "Error", "File format not supported.");
    }
    
    widget.chList->addItems(chlabels);
}

void readEDF::readFile(int type){
    QStringList s;
    //Opens the file chooser window and copies the path to a string
    s = QFileDialog::getOpenFileNames(this, tr("Open File"),"/home/fabricio/Documentos/Sinais EEG/Physionet_CHB-MIT",tr("EDF (*.edf)\nAll files (*.*)"));
    
    //Checks if all files are ok
    for(int i=0; i<s.count(); i++){
        QFile *file = new QFile(s[i]);
        if(!file->exists()){
            QMessageBox::information(this, "Error", "One or more files are invalid.");
            return;
        }
    }
    
    widget.filesLoaded->addItems(s);
}

void readEDF::showFileInfo(){
    QString path = widget.filesLoaded->currentItem()->text();
    char filepath[256];
    strcpy(filepath, path.toAscii().constData());
    
    OpenSelectWindow(filepath);
}

void readEDF::addFile(){
    QString filepath = widget.filesLoaded->currentItem()->text();
    
    int chCount = widget.chList->selectedItems().count();
    SignalFile* newFile;
    if(chCount>0){
        widget.filesAdded->addItem(filepath);
        QStringList channels;
        QStringList selectedChannels;
        QVector< int > indexes;
        
        indexes.resize(chCount);
        
        int j = 0;
        for(int i=0; i<widget.chList->count(); i++){
            channels.append(widget.chList->item(i)->text());
            if(widget.chList->item(i)->isSelected()){
                selectedChannels.append(widget.chList->item(i)->text());
                indexes[j++] = i;
            }
        }
        QFileInfo *info = new QFileInfo(filepath);
        if(info->suffix()=="edf"){
            newFile = new SignalFile(filepath, channels, selectedChannels, indexes, 0);
        }else if(info->suffix()=="txt"){
            newFile = new SignalFile(filepath, channels, selectedChannels, indexes, 1);
        }
        newFile->setStart(widget.lineFrom->text().toInt());
        newFile->setEnd(widget.lineTo->text().toInt());
        newFile->setSampleFrequency(widget.freqLabel->text().toInt());
        
        files.append(newFile);
    }
}

void readEDF::addAllFiles(){
    //Gets number of files loaded
    int N = widget.filesLoaded->count();
    
    //Gets indexes of selected channels
    QVector<int> indexes;
    for(int i=0; i<widget.chList->count(); i++)
        if(widget.chList->item(i)->isSelected())
            indexes.append(i);
    
    for(int i=0; i < N; i++){
        widget.filesLoaded->setCurrentRow(i);
        
        QString filepath = widget.filesLoaded->currentItem()->text();
    
        SignalFile* newFile;
        widget.filesAdded->addItem(filepath);
        QStringList channels;
        QStringList selectedChannels;

        for(int i=0; i<indexes.count(); i++)
            widget.chList->item(indexes[i])->setSelected(true);

        int j = 0;
        for(int i=0; i<widget.chList->count(); i++){
            channels.append(widget.chList->item(i)->text());
            if(widget.chList->item(i)->isSelected()){
                selectedChannels.append(widget.chList->item(i)->text());
                indexes[j++] = i;
            }
        }
        QFileInfo *info = new QFileInfo(filepath);
        if(info->suffix()=="edf"){
            newFile = new SignalFile(filepath, channels, selectedChannels, indexes, 0);
        }else if(info->suffix()=="txt"){
            newFile = new SignalFile(filepath, channels, selectedChannels, indexes, 1);
        }
        newFile->setStart(widget.lineFrom->text().toInt());
        newFile->setEnd(widget.lineTo->text().toInt());
        newFile->setSampleFrequency(widget.freqLabel->text().toInt());

        files.append(newFile);
    }
}

void readEDF::removeFile(){
    //checks if there is a file added selected
    if(widget.filesAdded->selectedItems().count()<1)
        return;
    
    disconnect(widget.filesAdded, SIGNAL(currentRowChanged(int)),
            this, SLOT(showAddedFileInfo()));
    
    int index = widget.filesAdded->currentRow();
    files.remove(index);
    widget.filesAdded->takeItem(index);
    
    connect(widget.filesAdded, SIGNAL(currentRowChanged(int)),
            this, SLOT(showAddedFileInfo()));
    
    showAddedFileInfo();
}

void readEDF::showAddedFileInfo(){
    if(widget.filesAdded->count()<1)
        return;
    
    int index = widget.filesAdded->currentRow();
    
    QStringList temp_list = files[index]->getSelectedChannelLabels();
    
    widget.channelsForFile->clear();
    widget.channelsForFile->addItems(temp_list);
}

QVector<SignalFile*> readEDF::getFiles(){
    return this->files;
}

void readEDF::readChannels(SignalFile* file){
    //Calculates length of each data from the imput parameters
    int n = file->getEnd() - file->getStart();
    
    //Finds number of channels selected
    int N = file->getChannelIndexes().count();
    fflush(stdout);
    //creates matrix to receive read data
    //QVector< QVector <double> > data;
    data.resize(N);
    for(int i = 0; i<N; i++){
        data[i].resize(n);
    }
    isSelected.resize(N);
    for(int i=0; i<N; i++){
        isSelected[i]=0;
    }
    reference.resize(n);
    LimitVallue = 0;
    
    char filepath[256];
    strcpy(filepath, file->getPath().toAscii().constData());
    
    if(file->getType()==0){
        //Guarantees there is no other file opened
        edfclose_file(hdr.handle);
        edfopen_file_readonly(filepath, &hdr, 0);
        //Defines the handle (type of file) for this file
        int hdl = hdr.handle;

        double *buf;
        buf = (double *)malloc(sizeof(double[n]));
        //Gets which indexes are going to be read
        QVector<int> indexesToRead = file->getChannelIndexes();

        //Reads selected signals
        int counter = 0;
        for(int j = 0; j < N; j++){
            edfseek(hdl, j, file->getStart(), 0);

            n = file->getEnd() - file->getStart();

            n = edfread_physical_samples(hdl, indexesToRead[j], n, buf);
            for(int i=0; i<n; i++){
                data[counter][i]=-buf[i];
            }
            counter++;
        }
        emit ThreadsFinished();
        disconnect(this, SIGNAL(EmitHandle(int)),
                this, SLOT(closeFile(int)));
        connect(this, SIGNAL(EmitHandle(int)),
                this, SLOT(closeFile(int)));
        emit EmitHandle(hdl);
    }else if(file->getType()==1){
        //QMessageBox::information(this, "Aew", "Chegou ate aqui!");
        //Reads till the end of the header and then skip line
        QFile rFile(filepath);

        if (!rFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::information(this, "Error", "Failed to read file.");
        }

        QTextStream in(&rFile);
        in.setAutoDetectUnicode(true);
        //reads first line
        in.readLine();
        //reads channel names
        while(!in.atEnd()){
            QString line = in.readLine();
            if(!line.trimmed().compare("#"))
                break;
        }
        in.readLine();
        QVector<int> indexes = file->getChannelIndexes();
        //reads data from file
        for(int i=0; i<n; i++){
            QString line = in.readLine();
            if(!line.trimmed().compare(""))
                break;
            
            QStringList fileData = line.split(" ");
            for(int j=0; j<N; j++){
                data[j][i] = fileData[indexes[j]].toDouble();
            }
        }
        rFile.close();
        emit ThreadsFinished();
    }
}

void readEDF::readThread(int hdl, int j, int n, double* buf, int counter){
    mutex.lock();
    edfseek(hdl, j, widget.lineFrom->text().toInt(), 0);
    n=edfread_physical_samples(hdl, j, n, buf);
    mutex.unlock();
    for(int i=0; i<n; i++){
        data[counter][i]=-buf[i];
    }
    data.at(0);
    
    mutex.lock();
    LimitVallue++;
    mutex.unlock();
    
//    if(LimitVallue==data.size() && reference_done == 1){
    if(LimitVallue==data.size()){
        emit ThreadsFinished();
        
        disconnect(this, SIGNAL(EmitHandle(int)),
                this, SLOT(closeFile(int)));
        connect(this, SIGNAL(EmitHandle(int)),
                this, SLOT(closeFile(int)));
        emit EmitHandle(hdl);
    }
}

void readEDF::readReference(int hdl, int j, int n, double* buf, int counter){
    mutex.lock();
    edfseek(hdl, j, widget.lineFrom->text().toInt(), 0);
    n=edfread_physical_samples(hdl, j, n, buf);
    mutex.unlock();
    for(int i=0; i<n; i++){
        reference[i]=-buf[i];
    }
    reference_done = 1;
}

void readEDF::readReferenceOnly(int index, QString filepath, int n, int from){
    char path[256];
    strcpy(path, filepath.toAscii().constData());
    
    edfclose_file(hdr.handle);
    edfopen_file_readonly(path, &hdr, 0);
    
    fflush(stdout);
    
    int hdl = hdr.handle;
    double *buf;
    buf = (double *)malloc(sizeof(double[n]));
    
    edfseek(hdl, index, from, 0);
    n = edfread_physical_samples(hdl, index, n, buf);
    QVector< double > reference;
    reference.resize(n);
    for(int i=0; i<n; i++){
        reference[i]=-buf[i];
    }
    
    emit emitReference(QString::fromAscii(hdr.signalparam[index].label), reference);
    closeFile(hdl);
}

void readEDF::getData(){
    
    emit EmitData(data, reference);
}

void readEDF::closeFile(int hdl){
    edfclose_file(hdl);
    
    //int hdl;
    
//    for(int i=labels.count()-1; i>=0; i--){
////        for(int j=labels.at(i).count()-1; j>=0; j--){
////            labels.at(i).remove(j,1);
////        }
////        labels.at(i).clear();
////        labels.at(i).squeeze();
//        labels.removeAt(i);
//    }
//    for(int i=allLabels.count()-1; i>=0; i--){
////        for(int j=allLabels.at(i).count()-1; j>=0; j--){
////            allLabels.at(i).remove(j,1);
////        }
////        allLabels.at(i).clear();
////        allLabels.at(i).squeeze();
//        allLabels.removeAt(i);
//    }
        
}