/* 
 * File:   FIR_filtering.cpp
 * Author: Fabricio
 * 
 * Created on 16 de Janeiro de 2013, 13:59
 */

#include "FIR_filtering.h"

#include <QtGui>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

QRadioButton *type1 = new QRadioButton("Low-pass");
QRadioButton *type2 = new QRadioButton("High-pass");
QRadioButton *type3 = new QRadioButton("Band-pass");
QRadioButton *type4 = new QRadioButton("Band-stop");
QRadioButton *window1 = new QRadioButton("Hann");
QRadioButton *window2 = new QRadioButton("Hamming");
QRadioButton *window3 = new QRadioButton("Blackman");

QLineEdit *cutfreq1 = new QLineEdit("Cuttoff frequency 1");
QLineEdit *cutfreq2 = new QLineEdit("Cuttoff frequency 2");

QLineEdit *filterlength = new QLineEdit("Filter length");
QLineEdit *alpha1 = new QLineEdit("Hamming or Blackman's window alpha");

QStringList FIR_filtering::methods() const{
    return QStringList() <<tr("FIR filtering");
}

QString FIR_filtering::description() const{
    QString description = "Uses parameters defined by the user to design a FIR filter with the windowing method, and then aply the filter on selected signals";
    return description;
}

int FIR_filtering::numberofdata() const{
    int number = n_results;
    return number;
}

int* FIR_filtering::elementsonch() const{
    int* number = n_elements;
    return number;
}

QStringList FIR_filtering::name() const{
    QStringList name;
    name.append("Filtered");
    return name;
}

QWidget* FIR_filtering::parameters() const{
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    
    window1->setAutoExclusive(false);
    window2->setAutoExclusive(false);
    window3->setAutoExclusive(false);
    
    layout->addWidget(type1);
    layout->addWidget(type2);
    layout->addWidget(type3);
    layout->addWidget(type4);
    layout->addWidget(window1);
    layout->addWidget(window2);
    layout->addWidget(window3);
    layout->addWidget(cutfreq1);
    layout->addWidget(cutfreq2);
    layout->addWidget(filterlength);
    layout->addWidget(alpha1);
    page->setLayout(layout);
    return page;
}

double*** FIR_filtering::analysis(QVector<double> X, QVector<double> Y){

    int N = filterlength->text().toInt();
    int Tau = (N-1)/2;
    double alpha = alpha1->text().toDouble();
    double F1 = cutfreq1->text().toDouble();
    double F2 = cutfreq2->text().toDouble();
    double pi = 3.14159265;
    double wp1 = (F1*2*pi)/1000;
    double wp2= (F2*2*pi)/1000;
    
    QVector<double> filter;
    filter.resize(N);
    
    //Calculates the filter impulse response
    //Desired impulse response
    //Low-pass
    if(type1->isChecked()){
        for(int i=0; i<N; i++){
            if (i == Tau)
                filter[i] = wp1/(pi);
            else
                filter[i] = sin((i - Tau)*wp1)/(pi*(i-Tau));
        }
    }
    //High-pass
    if(type2->isChecked()){
        for(int i=0; i<N; i++){
            if (i == Tau)
                filter[i] = 1-(wp1/pi);
            else
                filter[i] = (sin((i - Tau)*pi)-sin((i - Tau)*wp1))/(pi*(i-Tau));
        }
    }
    //Band-pass
    if(type3->isChecked()){
        for(int i=0; i<N; i++){
            if (i == Tau)
                filter[i] = (wp2-wp1)/pi;
            else
                filter[i] = (sin((i - Tau)*wp2)-sin((i - Tau)*wp1))/(pi*(i-Tau));
        }
    }
    //Band-stop
    if(type4->isChecked()){
        for(int i=0; i<N; i++){
            if (i == Tau)
                filter[i] = 1-(wp2-wp1)/pi;
            else
                filter[i] = (sin((i - Tau)*wp1)-sin((i - Tau)*wp2)+sin((i - Tau)*pi))/(pi*(i-Tau));
        }
    }
    
    //Multiply desired by the selected window
    //Generalized Hamming window
    if(window1->isChecked()){
        for(int i=0; i<N; i++){
            filter[i]=filter[i]*(alpha-(1-alpha)*cos(2*pi*i/(N-1)));
        }
    }
    //Hann window
    if(window2->isChecked()){
        for(int i=0; i<N; i++){
            filter[i]=filter[i]*0.5*(1-cos(2*pi*i/(N-1)));
        }
    }
    //Blackman window
    if(window3->isChecked()){
        for(int i=0; i<N; i++){
            filter[i]=filter[i]*((1-alpha)/2-1/2*cos(2*pi*i/(N-1))+alpha/2*cos(4*pi*i/(N-1)));
        }
    }
    
    double*** result;// = new QVector<>;
    result = new double**[1];
    for(int i = 0; i<1; i++){
        //X and Y axis
        result[i] = new double*[2];
        //X axis - index 0
        result[i][0] = new double[X.count()];
        //Y axis - index 1
        result[i][1] = new double[X.count()];
    }
    
    n_results = 0;
    
    //Calculates convolution
    //Calculates autocorrelation function
    QVector<double> filtered;
    filtered.resize(Y.count());

    for(int i = 0; i<Y.count(); i++){
        filtered[i]=0;
        for(int j = 0; j<filter.count(); j++){
            if((i-j)>=0){
                filtered[i]+=filter[j]*Y[i-j];
            }
        }
    }
    //Sets result as filtered signal
    for(int i = 0; i<X.count(); i++){
        result[n_results][0][i]=i;
        result[n_results][1][i]=filtered[i];
    }
    n_results++;
    
    //Sets number of elements in each ch returning
    n_elements = new int[n_results];
    for(int i = 0; i<n_results; i++){
        n_elements[i] = X.count();
    }
    
    return result;
}

 Q_EXPORT_PLUGIN2(method_fir, FIR_filtering)