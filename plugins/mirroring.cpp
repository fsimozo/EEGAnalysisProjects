/* 
 * File:   mirroring.cpp
 * Author: Fabricio
 * 
 * Created on 12 de Dezembro de 2012, 17:00
 */

#include <QtGui>

#include <math.h>
#include <stdlib.h>

#include "mirroring.h"

QCheckBox *check1 = new QCheckBox("x axis");
QCheckBox *check2 = new QCheckBox("y axis");
//QStringList n_labels = new QStringList;

QStringList mirroring::methods() const{
	return QStringList() <<tr("axis mirroring");
}

QString mirroring::description() const{
	QString description = "Mirrors the signal over the y axis - horizontal flip";
	return description;
}

int mirroring::numberofdata() const{
	int number = n_results;
	return number;
}

int* mirroring::elementsonch() const{
    int* number = n_elements;
    return number;
}

QStringList mirroring::name() const{
	QStringList name;
	name.append("Horizontal mirroring");
	name.append("Vertical mirroring");
	return name;
}

QWidget* mirroring::parameters() const{
	QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(check1);
    layout->addWidget(check2);
    page->setLayout(layout);
	return page;
}

double*** mirroring::analysis(QVector<double> X, QVector<double> Y){
	double*** result;// = new QVector<>;
	result = new double**[2];
	//Horizontal and Vertical mirroring
	result[0] = new double*[2];
	result[1] = new double*[2];
	//X axis - index 0
	result[0][0] = new double[X.count()];
	result[1][0] = new double[X.count()];
	//Y axis - index 1
	result[0][1] = new double[Y.count()];
	result[1][1] = new double[X.count()];
	//xmirroring:
	n_results=0;
	if(check1->isChecked()){
		for(int i = 0; i<X.count(); i++){
			result[n_results][0][i] = X[i];
			result[n_results][1][Y.count()-(1+i)] = Y[i];
		}
		n_results++;
		//n_labels<<tr("vertical mirroring");
	}
	//ymirroring:
	if(check2->isChecked()){
		for(int i = 0; i<X.count(); i++){
			result[n_results][0][i] = X[i];
			result[n_results][1][i] = -Y[i];
		}
		n_results++;
		//n_labels<<tr("horizontal mirroring");
	}
        //Sets number of elements in each result
        n_elements = new int[n_results];
        for(int i = 0; i<n_results; i++){
            n_elements[i]=X.count();
        }
	//Sets number of results being returned to main application
	//n_results=2;
	//Sets labels for each returning result
	
	return result;
}

 Q_EXPORT_PLUGIN2(method_mirror, mirroring)