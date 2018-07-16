/* 
 * File:   methodResult.h
 * Author: Fabricio
 *
 * Created on 18 de Julho de 2013, 13:59
 */

#ifndef METHODRESULT_H
#define	METHODRESULT_H

#include <QObject>
#include <QVector>

#include "resultElement.h"

class methodResult : public QObject{
    Q_OBJECT;
public:
    methodResult();
    methodResult(const methodResult& orig);
    virtual ~methodResult();
    
    void addPoint(QString name, double value);
    void addSeries(QString name, QVector<double> X);
    void addGraph(QString name, QString X_label, QString Y_label, QVector<double> X, QVector<double> Y);
    void addTable(QString name, QVector<QString> horizontal, QVector<QString> vertical, QVector< QVector <double> > data);
    
    
    QString getPoints();
    QString getSeries();
    QString getGraphs();
    QString getTables();
    
public slots:
    void addPointToResult(QString name, double value);
    void addSeriesToResult(QString name, QVector<double> X);
    void addGraphToResult(QString name, QString X_label, QString Y_label, QVector<double> X, QVector<double> Y);
    void addTableToResult(QString name, QVector<QString> horizontal, QVector<QString> vertical, QVector< QVector <double> > data);
        
private:
    QVector< resultElement *> points;
    QVector< resultElement *> series;
    QVector< resultElement *> graphs;
    QVector< resultElement *> tables;
    
    QString result_points;
    QString result_series;
    QString result_graphs;
    QString result_tables;
};

#endif	/* METHODRESULT_H */

