/* 
 * File:   HFO_detector.cpp
 * Author: Fabricio
 * 
 * Created on 13 de Dezembro de 2012, 12:35
 */

#include "HFO_detector.h"

#include <QtGui>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>


QStringList HFO_detector::methods() const{
	return QStringList() <<tr("HFO Detection");
}

QString HFO_detector::description() const{
	QString description = "Uses an algorithm involving HFO power to detect HFO";
	return description;
}

int HFO_detector::numberofdata() const{
	int number = n_results;
	return number;
}

int* HFO_detector::elementsonch() const{
    int* number = n_elements;
    return number;
}

QStringList HFO_detector::name() const{
	QStringList name;
	name.append("HFO Detection");
//        name.append("Number of segments with HFO");
        name.append("FT of 1st segment");
        name.append("FT of 2nd segment");
        name.append("FT of 3rd segment");
        name.append("FT of 4th segment");
        name.append("FT of 5th segment");
        name.append("FT of 6th segment");
        name.append("FT of 7th segment");
        name.append("FT of 8th segment");
        name.append("FT of 9th segment");
        name.append("FT of 10th segment");
	return name;
}

QWidget* HFO_detector::parameters() const{
	QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    page->setLayout(layout);
	return page;
}

double*** HFO_detector::analysis(QVector<double> X, QVector<double> Y){
    double*** result;// = new QVector<>;
    //Returns 2 signals:
    //1 - segments with HFO detected
    result = new double**[11];
    //X and Y axis
    result[0] = new double*[2];
    //X axis - index 0
    result[0][0] = new double[X.count()];
    //Y axis - index 1
    result[0][1] = new double[X.count()];
//    //X and Y axis
//    result[1] = new double*[2];
//    //X axis - index 0
//    result[1][0] = new double[1];
//    //Y axis - index 1
//    result[1][1] = new double[1];
    for(int i = 0; i<11; i++){
        //X and Y axis
        result[i] = new double*[2];
        //X axis - index 0
        result[i][0] = new double[X.count()];
        //Y axis - index 1
        result[i][1] = new double[X.count()];
    }
    //Sets number of results this function is returning
    n_results=11;
//    n_results=2;
    //Sets number of elements in each result this function is returning
    n_elements = new int[11];
    n_elements[0]=Y.count();
    n_elements[1]=500;
    n_elements[2]=500;
    n_elements[3]=500;
    n_elements[4]=500;
    n_elements[5]=500;
    n_elements[6]=500;
    n_elements[7]=500;
    n_elements[8]=500;
    n_elements[9]=500;
    n_elements[10]=500;
    

    //Step 1: Transform 1 second segments in zero mean segments
    double mean;
    int n_segments = Y.count()/1000; //1KHz
    int length = 1000;
    for(int i = 0; i<n_segments; i++){
        mean = 0;
        for(int j = 0; j<length; j++){
            mean += Y[i*length+j]/1000;
        }
        for(int j = 0; j<length; j++){
            Y[i*length+j] = Y[i*length+j]-mean;
        }
    }
    
    //Step 2: Fourier Transform for each 1 second segment
    //Calculates autocorrelation function
    QVector<double> rxx;

    rxx.resize(Y.count());
    for(int i = 0; i<rxx.count(); i++){
        rxx[i]=Y[i];
        //rxx[i]=0;
        //for(int j = 0; j<rxx.count()-i; j++){
        //    rxx[i]+=Y[j]*Y[i+j];
        //}
    }
    
    //Destro
    QVector< QVector < double > > FT;
    QVector<double> DFT;
    QVector<double> real;
    QVector<double> imag;
    double g;
    double h;
    double pi = 3.141592;

    FT.resize(n_segments);
    for(int i = 0; i<n_segments; i++){
        DFT.resize(500);
        real.resize(length);
        imag.resize(length);
        
        //Calculates DFT
        double T = 0.001; //Sample frequency
        double inc = 0.001;
        
        for(int freq = 0; freq < 500; freq+=10){
            //Real component
            for(int t = 0; t<length; t++){
                real[t]=rxx[i*length+t]*cos(2*pi*freq*(i*length+t)*T);
            }
            g = (real[0]*inc/2);
            for(int t = 1; t<length-1; t++){
                g += (real[t]+real[t+1])*inc;
            }
            g += real[length-1]*inc/2;

            //Imaginary component
            for(int t = 0; t<length; t++){
                imag[t]=rxx[i*length+t]*sin(2*pi*freq*(i*length+t)*T);
            }
            h = (imag[0]*inc/2);
            for(int t = 1; t<length-1; t++){
                h += (imag[t]+imag[t+1])*inc;
            }
            h += imag[length-1]*inc/2;

            DFT[freq] = sqrt(g*g + h*h);
        }
        //Stores power spectrum on FT
        FT[i] = DFT;
    }

    //For each segment, repeats step 3 and 4
    QVector <bool> isHFO;
    for(int i = 0; i<isHFO.count(); i++){
        isHFO[i]=false;
    }
    isHFO.resize(n_segments);

    int number_of_hfo=0;
    
    for (int i = 0; i<n_segments; i++){
        //Step 3: Calculates total power and HFO power of the signal
        double Tp = 0;
        double HFOp = 0;
        for(int freq = 0; freq < FT[i].count(); freq++){
            Tp += FT[i][freq]*FT[i][freq];
            if(freq >= 100 && freq <= 500){
                HFOp += FT[i][freq]*FT[i][freq];
            }
        }
        //printf("\nsegment: %d \nHFOp = %f", i, HFOp);
        //printf("\nTp = %f", Tp);
        
        //Step 4: Compares HFOp with threshold to check if there are HFOs on
        //segment
        if(HFOp >= 0.1*Tp){
            isHFO[i] = true;
            number_of_hfo++;
        }
    }
    
    //Sets values on results:
    for(int j = 0; j<n_segments; j++){
        for(int i = 0; i<1000; i++){
            result[0][0][1000*j+i]=i;
            if(isHFO[j]){
                result[0][1][1000*j+i] = Y[1000*j+i];
            }else{
                result[0][1][1000*j+i] = 0;
            }
        }
    }
    result[1][0][0]=0;
    result[1][1][0]=number_of_hfo;
    for(int segment = 0; segment < 10; segment++){
        for(int i = 0; i<500; i++){
            result[segment+1][0][i] = i;
            result[segment+1][1][i] = FT[segment][i];
        }
    }
    return result;
}

 Q_EXPORT_PLUGIN2(method_hfo, HFO_detector)