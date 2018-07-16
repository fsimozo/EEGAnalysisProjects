/* 
 * File:   FIRfilter.cpp
 * Author: Fabricio
 * 
 * Created on 14 de Março de 2013, 10:05
 */

#include "FIRfilter.h"

#include <math.h>

#include <QVector>
#include <QThread>

#include <QFuture>
#include <QtConcurrentRun>

FIRfilter::FIRfilter() {
    //locker = new QMutex();
    connect(this, SIGNAL(ThreadsFinished()),
            this, SLOT(getResult()));
}

FIRfilter::FIRfilter(const FIRfilter& orig) {
}

FIRfilter::~FIRfilter() {
}

/**
 * Creates a FIR filter and uses convolution to filter input
 * parameter "data"
 * @param data
 * @param F1
 * @param F2
 * @param type
 */
//QVector<double> FIRfilter::simpleFilter(QVector<double> data, double F1, double F2, int type, int x_ini, int n){
QVector<double> FIRfilter::simpleFilter(QVector<double> data, QVector<int> arguments, int freq_aq){
    int F1 = arguments[0];
    int F2 = arguments[1];
    int type = arguments[2];
    int x_ini = arguments[3];
    int n = arguments[4];
    
//    printf("\nn = %d", n);
//    printf("\nx_ini = %d", x_ini);
//    printf("\ndata count = %d", data.count());
    
    //Creates FIR low pass filter
    //Filter Parameters
    int N = 401;
    double alpha = 0.54;
    
    QVector<double> filter;
    filter = hammingWindowFIR(N, alpha, F1, F2, type, freq_aq);
    
    QVector<double> filtered;
    filtered.resize(n);
    
//    for(int i = (N-1)/2+x_ini; i<(n+(N-1)/2+x_ini); i++){
//        filtered[i-((N-1)/2 + x_ini)]=0;
//        for(int j = 0; j<filter.count(); j++){
//            if((i-j)>=0 && (i-j)<data.count()){
//                filtered[i-((N-1)/2 + x_ini)]+=filter[j]*data[i-j];
//            }
//        }
//    }
    
    for(int i = x_ini; i<x_ini+n; i++){
        filtered[i - x_ini] = 0;
        for(int j=0; j<filter.count(); j++){
            if((i-(N-1)/2+j)>=0 && ((i-(N-1)/2+j)<data.count())){
                filtered[i - x_ini] += filter[j] * data[i-(N-1)/2+j];
            }
        }
    }
//    filtered = filtered.mid(x_ini, n);
    return filtered;
}

void FIRfilter::threadFilter(QVector<QVector<double> > data, double F1, double F2, int type, int freq_aq){
    //Creates FIR low pass filter
    //Filter Parameters
    int N = 401;
    double alpha = 0.54;
    
    QVector<double> filter;
    filter = hammingWindowFIR(N, alpha, F1, F2, type, freq_aq);
    
    //Get number of data channels
    N = data.count();
    result.resize(N);
    verifier = 0;
    
    int Ncores = QThread::idealThreadCount()-2;
    
    //Checks if the number of data is bigger than the number of cores
    if(N>Ncores){
//    if(2<1){
        //Calculates the number of data to be processed in each 
        //Number of threads created = number of processor cores
        QVector<int> Data_for_thread;
        Data_for_thread.resize(Ncores);
        int core_counter=0;
        for(int i = 0; i<Ncores; i++){
            Data_for_thread[i]=0;
        }
        for(int i = 0; i<N; i++){
            Data_for_thread[core_counter]++;
            core_counter++;
            if(core_counter>(Ncores-1)){
                core_counter=0;
            }
        }
        
//        int Ch_per_thread = floor(N/(Ncores-1));
//        int Ch_extra_thread = N - Ch_per_thread*(Ncores-1);
        for(int i = 0; i<(Ncores); i++){
            //Creates threads for Ncores
            int counter = 0;
            for(int j = 0; j<i; j++){
                counter += Data_for_thread[j];
            }
            QVector< QVector<double> > temp = data.mid(counter, Data_for_thread[i]);
            QFuture<void> future = QtConcurrent::run(this, &FIRfilter::convolutionfilter, temp, filter, counter);
        }
    }else{
        //Creates one thread for each of data channels
        for(int i = 0; i<N; i++){
            QVector< QVector<double> > temp = data.mid(i, 1);
            QFuture<void> future = QtConcurrent::run(this, &FIRfilter::convolutionfilter, temp, filter, i);
        }
    }
    //Test with 1 thread
//    lowpassfilter(data, filter, 0);
}

/**
 * Do a convolution between input parameters data and filter.
 * This is done in multi-threading, so 'counter' is needed to indicate the starting
 * position of the data matrix each thread will start on
 * @param data
 * @param filter
 * @param counter
 */
void FIRfilter::convolutionfilter(QVector< QVector<double> > data, QVector<double> filter, int counter){
    for(int ch = 0; ch < data.count(); ch++){
        int N = filter.count();
        //Calculates convolution
        QVector<double> filtered;
        filtered.resize(data[ch].count());

        for(int i = N; i<(data[ch].count()); i++){
            filtered[i-N]=0;
            for(int j = 0; j<filter.count(); j++){
                if((i-j)>=0){
                    filtered[i-N]+=filter[j]*data[ch][i-j];
                }
            }
        }
        //Adds filteres signal to result matrix
        result[counter+ch] = filtered;
    }
    //Each Thread adds 1 to verifier
    printf("verifier = %d\n", verifier);
    //When verifier equals the number of data channels, result is ready to be passed
    //to the MainWindow
    if(verifier==result.count()){
        emit ThreadsFinished();
    }
}

/**
 * Once all threads are finished filtering the input data, the result is emitted
 * through a SIGNAL to the main window.
 */
void FIRfilter::getResult(){
    emit EmitResult(result);
}

QVector<double> FIRfilter::hammingWindowFIR(int N, double alpha, int F1, int F2, int type, int freq_aq){
    
    //Filter Parameters
        int Tau = (N-1)/2;
        double pi = 3.14159265;
        double wp1 = ((double)F1*2*pi)/freq_aq;
        double wp2 = ((double)F2*2*pi)/freq_aq;
        QVector<double> filter;
        filter.resize(N);
        
    if(type == 0){
        //Creates FIR low pass filter
        for(int i=0; i<N; i++){
            if (i == Tau)
                filter[i] = wp1/(pi)*(alpha-(1-alpha)*cos(2*pi*i/(N-1)));
            else
                filter[i] = sin((i - Tau)*wp1)/(pi*(i-Tau))*(alpha-(1-alpha)*cos(2*pi*i/(N-1)));
        }
    }else if(type == 1){
        //Creates FIR high pass filter
        for(int i=0; i<N; i++){
            if (i == Tau)
                filter[i] = (1-(wp1/pi))*(alpha-(1-alpha)*cos(2*pi*i/(N-1)));
            else
                filter[i] = ((sin((i - Tau)*pi)-sin((i - Tau)*wp1))/(pi*(i-Tau)))*(alpha-(1-alpha)*cos(2*pi*i/(N-1)));
        }
    }else if(type == 2){
        //Creates FIR band pass filter
        for(int i=0; i<N; i++){
            if (i == Tau)
                filter[i] = ((wp2-wp1)/pi)*(alpha-(1-alpha)*cos(2*pi*i/(N-1)));
            else
                filter[i] = ((sin((i - Tau)*wp2)-sin((i - Tau)*wp1))/(pi*(i-Tau)))*(alpha-(1-alpha)*cos(2*pi*i/(N-1)));
        }
    }else if(type == 3){
        //Creates FIR band stop filter
        for(int i=0; i<N; i++){
            if (i == Tau)
                filter[i] = (1-(wp2-wp1)/pi);
            else
                filter[i] = ((sin((i - Tau)*wp1)-sin((i - Tau)*wp2)+sin((i - Tau)*pi))/(pi*(i-Tau)))*(alpha-(1-alpha)*cos(2*pi*i/(N-1)));
        }
    }
    
    return filter;
}