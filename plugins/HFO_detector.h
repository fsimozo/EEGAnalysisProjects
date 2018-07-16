/* 
 * File:   HFO_detector.h
 * Author: Fabricio
 *
 * Created on 13 de Dezembro de 2012, 12:35
 */

#ifndef HFO_DETECTOR_H
#define	HFO_DETECTOR_H

#include <QObject>

#include <../OpenFile/plugininterface.h>

class HFO_detector: public QObject,
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

#endif	/* HFO_DETECTOR_H */