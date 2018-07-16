/* 
 * File:   resultElement.h
 * Author: Fabricio
 *
 * Created on 18 de Julho de 2013, 15:19
 */

#ifndef RESULTELEMENT_H
#define	RESULTELEMENT_H

#include <QObject>
#include <QVector>

class resultElement : public QObject{
    Q_OBJECT;
public:
    resultElement();
    resultElement(const resultElement& orig);
    virtual ~resultElement();
    
    void setType(QString type);
    
    void setPoint(QString name, double value);
    void setSeries(QString name, QVector<double> X);
    void setGraph(QString name, QString X_label, QString Y_label, QVector<double> X, QVector<double> Y);
    void setTable(QString name, QVector<QString> horizontal, QVector<QString> vertical, QVector< QVector <double> > data);
    
    void getResultElement();
    QString getResult();
    
    signals:
    void emitPoint(QString name, double value);
    void emitSeries(QString name, QVector<double> X);
    void emitGraph(QString name, QString X_label, QString Y_label, QVector<double> X, QVector<double> Y);
    void emitTable(QString name, QVector<QString> horizontal, QVector<QString> vertical, QVector< QVector <double> > data);
private:
    QString type;
    
    QString point_name;
    double point_value;
    
    QString result;
    
    QString series_name;
    QVector<double> series_Xdata;
    
    QString graph_name;
    QString graph_Xlabel;
    QString graph_Ylabel;
    QVector< double > graph_Xdata;
    QVector< double > graph_Ydata;
    
    QString table_name;
    QVector< QVector < double > > table_data;
    QVector< QString > table_horizontal;
    QVector< QString > table_vertical;
};

#endif	/* RESULTELEMENT_H */