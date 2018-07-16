/* 
 * File:   FIR_test.h
 * Author: Fabricio
 *
 * Created on 7 de Janeiro de 2013, 12:52
 */

#ifndef FIR_TEST_H
#define	FIR_TEST_H

#include <QObject>

#include <../OpenFile/plugininterface.h>

class FIR_test: public QObject,
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

#endif	/* FIR_TEST_H */

