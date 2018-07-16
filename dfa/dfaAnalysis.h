/* 
 * File:   dfaAnalysis.h
 * Author: Fabricio
 *
 * Created on August 5, 2013, 3:23 PM
 */

#ifndef DFAANALYSIS_H
#define	DFAANALYSIS_H

#include <QObject>
#include <QVector>

class dfaAnalysis : public QObject {
    Q_OBJECT;
    
public:
    dfaAnalysis();
    dfaAnalysis(const dfaAnalysis& orig);
    virtual ~dfaAnalysis();
    
public slots:
    QVector<double> doAnalysis(QVector<double> signal);
    QVector<double> linearfit(QVector<double> Y, int offset, int n);
    QVector<double> linearfit(QVector<double> X, QVector<double> Y);
    QVector<double> findAlphas(QVector<double> X, QVector<double> Y);
    
private:

};

#endif	/* DFAANALYSIS_H */

