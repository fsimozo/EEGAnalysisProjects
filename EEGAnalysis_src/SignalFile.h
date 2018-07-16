/* 
 * File:   fileQueue.h
 * Author: fabricio
 *
 * Created on April 8, 2014, 3:18 PM
 */

#ifndef SignalFile_H
#define	SignalFile_H

#include <QObject>
#include <QVector>
#include <QStringList>

class SignalFile : public QObject {
    Q_OBJECT;
public:
    SignalFile(QString path, QStringList channelLabels, QStringList selectedChannelLabels, QVector<int> channelIndexes, int type);
    SignalFile(const SignalFile& orig);
    virtual ~SignalFile();
    
    void setStart(int startSample);
    void setEnd(int endSample);
    void setSampleFrequency(int sampleFrequency);
    
    QString getPath();
    QString getFileName();
    QStringList getChannelLabels();
    QStringList getSelectedChannelLabels();
    QVector< int > getChannelIndexes();
    int getStart();
    int getEnd();
    int getSampleFrequency();
    int getType();
    int getNumberOfSamples();
    int count();
    
private:
    QString path;
    QStringList channelLabels;
    QStringList selectedChannelLabels;
    QVector<int> channelIndexes;
    int type, startSample, endSample, sampleFrequency;
};

#endif	/* FILEQUEUE_H */

