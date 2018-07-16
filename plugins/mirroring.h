/* 
 * File:   mirroring.h
 * Author: Fabricio
 *
 * Created on 12 de Dezembro de 2012, 17:00
 */

#ifndef MIRRORING_H
#define MIRRORING_H

#include <QObject>

#include <../OpenFile/plugininterface.h>

class mirroring: public QObject,
				 public AnalysisMethodInterface
{
	Q_OBJECT
	Q_INTERFACES(AnalysisMethodInterface)
	
public:
	QStringList methods() const;
	QString description() const;
	int numberofdata() const;
        int* elementsonch() const;
	QStringList name() const;
	QWidget* parameters() const;
	double*** analysis(QVector<double> X, QVector<double> Y);
private:
	int n_results;
        int* n_elements;
};
	
#endif