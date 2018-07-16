/* 
 * File:   FIRfilter.h
 * Author: Fabricio
 *
 * Created on 14 de Março de 2013, 10:05
 */

#ifndef FIRFILTER_H
#define	FIRFILTER_H

#include <QObject>
#include <QVector>
#include <QMutex>

class FIRfilter : public QObject {
    Q_OBJECT
    
public:
    FIRfilter();
    FIRfilter(const FIRfilter& orig);
    virtual ~FIRfilter();
public slots:
//    QVector<double> simpleFilter(QVector<double> data, double F1, double F2, int type, int x_ini, int n);
    QVector<double> simpleFilter(QVector<double> data, QVector<int> arguments, int freq_aq);
    void threadFilter(QVector< QVector<double> > data, double F1, double F2, int type, int freq_aq);
    void convolutionfilter(QVector< QVector<double> > data, QVector<double> filter, int counter);
    void getResult();
    QVector<double> hammingWindowFIR(int N, double alpha, int F1, int F2, int type, int freq_aq);
    signals:
    void ThreadsFinished();
    void EmitResult(QVector< QVector<double> > result);
private:
    QVector< QVector <double> > result;
    int verifier;
    QMutex locker;
};

#endif	/* FIRFILTER_H */

