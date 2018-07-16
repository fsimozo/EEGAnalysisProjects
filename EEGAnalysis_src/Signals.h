/* 
 * File:   Signals.h
 * Author: Fabricio
 *
 * Created on 21 de Junho de 2013, 16:08
 */

#ifndef SIGNALS_H
#define	SIGNALS_H

#include <QObject>
#include <QVector>
#include "signal.h"

class Signals : public QObject {
    Q_OBJECT;
public:
    Signals();
    Signals(const Signals& orig);
    virtual ~Signals();
    
    void setFilePath(QString path);
    void setDataOffset(int from);
    void setSampleFrequency(int freq);
    void setReference(QVector< double > signal, QString label);
    
    void addData(QVector< double > signal, QString label);
    void removeData(int index);
    
    void addMarker(int index, double position);
    void removeMarker(int index, double position, double check_region);
    
    void setMarkedToShow();
    void setAllToShow();
    
    int count();
    QVector< double > getData(int index);
    QVector< double > getDataWithReference(int index, int x_i, int n);
    QString getLabel(int index);
    QString getReferenceLabel();
    QVector< int > getIndexToShow();
    QVector< double > getMarkers(int index);
    QString getFilePath();
    int getSampleFrequency();
    
    void clear();
    
private:
    QString filepath;
    int offset;
    int sample_frequency;
    QVector<int> indexToShow;
    QVector< signal *> data;
    signal reference;
};

#endif	/* SIGNALS_H */