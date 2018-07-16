/* 
 * File:   convertEEGtoEDF.h
 * Author: Fabricio
 *
 * Created on 20 de Março de 2013, 13:50
 */

#ifndef CONVERTEEGTOEDF_H
#define	CONVERTEEGTOEDF_H

#include <QObject>

class convertEEGtoEDF : public QObject{
    Q_OBJECT
public:
    convertEEGtoEDF();
    convertEEGtoEDF(const convertEEGtoEDF& orig);
    virtual ~convertEEGtoEDF();
public slots:
    void EEGtoEDF();
private:

};

#endif	/* CONVERTEEGTOEDF_H */

