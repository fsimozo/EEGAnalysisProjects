/* 
 * File:   signal.h
 * Author: Fabricio
 *
 * Created on 21 de Junho de 2013, 14:54
 */

#ifndef SIGNAL_H
#define	SIGNAL_H

#include <QObject>
#include <QVector>

class signal : public QObject{
    Q_OBJECT;
    
public:
    signal();
    signal(const signal& orig);
    virtual ~signal();
    
    void setData(QVector< double > data);
    void setLabel(QString data_label);
    void setFileIndex(int index);
    void addMarker(double position);
    void removeMarker(int index);
    
    QVector< double > getData();
    QString getLabel();
    int getFileIndex();
    QVector< double > getMarkers();
    
    void clear();
private:
    QVector< double > data;
    QString label;
    int file_index;
    QVector< double > markers;
};

#endif	/* SIGNAL_H */

