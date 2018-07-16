/* 
 * File:   FIR_filtering.h
 * Author: Fabricio
 *
 * Created on 16 de Janeiro de 2013, 13:59
 */

#ifndef FIR_FILTERING_H
#define	FIR_FILTERING_H

#include <QObject>

#include <../OpenFile/plugininterface.h>

class FIR_filtering: public QObject,
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

#endif	/* FIR_FILTERING_H */

