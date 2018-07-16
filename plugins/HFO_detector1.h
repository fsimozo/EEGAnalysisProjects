/* 
 * File:   HFO_detector1.h
 * Author: Fabricio
 *
 * Created on 18 de Janeiro de 2013, 10:16
 */

#ifndef HFO_DETECTOR1_H
#define	HFO_DETECTOR1_H

#include <QObject>

#include <../OpenFile/plugininterface.h>

class HFO_detector1: public QObject,
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

#endif	/* HFO_DETECTOR1_H */

