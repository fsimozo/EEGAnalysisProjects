/* 
 * File:   writeMarkersText.h
 * Author: Fabricio
 *
 * Created on 18 de Março de 2013, 16:46
 */

#ifndef WRITEMARKERSTEXT_H
#define	WRITEMARKERSTEXT_H

#include "methodResult.h"

#include <QObject>
#include <QVector>
#include <QStringList>
#include <QDir>

class writeMarkersText : public QObject {
    Q_OBJECT
public:
    writeMarkersText();
    writeMarkersText(const writeMarkersText& orig);
    virtual ~writeMarkersText();
    
public slots:
    void writeFile(QVector< QVector<double> > data, QStringList labels, QString reference_label, int type);
    void writeMethodResults(methodResult *result, QDir path, QString ch_label, QString method_name, int channel_index);
    
private:

};

#endif	/* WRITEMARKERSTEXT_H */

