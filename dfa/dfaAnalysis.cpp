/* 
 * File:   dfaAnalysis.cpp
 * Author: Fabricio
 * 
 * Created on August 5, 2013, 3:23 PM
 */

#include "dfaAnalysis.h"
#include "writeMarkersText.h"
#include "methodResult.h"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QElapsedTimer>

#include <QVector>
#include <math.h>

dfaAnalysis::dfaAnalysis() {
}

dfaAnalysis::dfaAnalysis(const dfaAnalysis& orig) {
}

dfaAnalysis::~dfaAnalysis() {
}

QVector<double> dfaAnalysis::doAnalysis(QVector<double> signal){
    int isDebugging = 0;
    
//    QElapsedTimer *timer = new QElapsedTimer();
//    int time;
    
//    timer->start();
    
    long n = signal.count();
    
    int N_iterations;
    long min = 0L, max = 0L;
    
    min = 4;
    max = n/4;
    
    double ratio = pow(2.0, 1.0/8.0);
    
    N_iterations = log10(max/(double)min) / log10(ratio) + 1.5;
    
    if(isDebugging){
        printf("Number of iterations calculated.\n");
        fflush(stdout);
    }
    
    int L;
    long temp;
    QVector<long> interval;
    interval.resize(N_iterations);
    double avg=0;
    
    interval[0] = 4;
    
//    for(int i=1, j=1; j<N_iterations && interval[i-1]<max; i++){
//        if(temp = min*pow(ratio, i)+0.5 > interval[j-1]){
//            interval[j]=temp;
//            printf("i=%d; interval = %d\n",i, interval[j++]);
//        }
//    }
    for(int i=1, j=1; j<N_iterations; i++){
        temp = min*pow(ratio, i)+0.5;
        if(temp > interval[j-1]){
            interval[j++] = temp;
        }
    }
    if(interval[N_iterations-1]>max)
        N_iterations--;
    
    if(isDebugging){
        printf("Boxes sizes calculated\n");
        fflush(stdout);
    }
    
    QVector<double> integ;
    integ.resize(n);
    
    QVector<double> RMSD;
    RMSD.resize(N_iterations);
    QVector<double> interv;
    interv.resize(N_iterations);
    
    //Averages input signal
    for(int i=0; i<n; i++){
        avg+=(signal[i]);
    }
    avg = avg/n;
    if(isDebugging){
        
        printf("Average calculated\n");
        fflush(stdout);
    }
    
    //Calculates integrated signal
    integ[0] = signal[0]-avg;
    for(int i=1; i<n; i++){
        integ[i]+=integ[i-1]+ signal[i]-avg;
    }
    
    if(isDebugging){
        printf("Integ. signal calculated\n");
        fflush(stdout);
    }
    
    //Linear fitting of segments
    //Also, calculates RMSD (root mean square deviation)
    
    for(int index=0; index<N_iterations; index++){
        RMSD[index]=0;
        L = (int)floor(n/interval[index]);
        
//        QVector<double> linear;
//        linear.resize(L*interval[index]);
        
        QVector<double> temp;
        temp.resize(interval[index]);
        QVector<double> temp_x;
        temp_x.resize(interval[index]);
        
        QVector<double> coef;
        coef.resize(2);
        
//        timer->restart();
        for(int l=0; l<L; l++){
            temp = integ.mid(l*interval[index], interval[index]);
            for(int i=0; i<interval[index]; i++){
                temp_x[i] = l*interval[index]+i;
            }
//            coef = linearfit(temp, l*interval[index], interval[index]);
            coef = linearfit(temp_x, temp);
            for(int i=0; i<interval[index]; i++){
                int ii = l*interval[index]+i;
                RMSD[index] += (pow((integ[ii]-(coef[0]+coef[1]*ii)),2))/(L*interval[index]);
            }
        }
        
        RMSD[index] = sqrt(RMSD[index]);
        interv[index] = interval[index];
    }
    
    if(isDebugging){
        printf("RMSD calculated\n");
        fflush(stdout);
    }
    
//    calculates log
    QVector<double> log_interv = interv;
    QVector<double> log_RMSD = RMSD;
    for(int i=0; i<interv.count(); i++){
        log_interv[i] = log10(interv[i]);
        log_RMSD[i] = log10(RMSD[i]);
//    
//        printf("%f; %f\n", log_interv[i], log_RMSD[i]);
    }
//    methodResult *result = new methodResult();
//    result->addGraph("DFA", "log_interv", "log_RMSD", log_interv, log_RMSD);
//    writeMarkersText *writer = new writeMarkersText();
//    
//    QDir s_path = QString("C:/Users/Fabricio/Desktop/Teste");
//    
//    writer->writeMethodResults(result, s_path, "label", "DFA", ch_index);
//    time = timer->elapsed();
//    printf("%d\n", time);
    //Calculates coefficient
    QVector<double> coef;
    coef.resize(2);
    
//    coef = linearfit(log_interv, log_RMSD);
    coef = findAlphas(log_interv, log_RMSD);
    
    if(isDebugging){
        printf("Coef. calculated\n");
        fflush(stdout);
    }
    
    return coef;
}

QVector<double> dfaAnalysis::linearfit(QVector<double> Y, int offset, int n){
    QVector<double> coef;
    coef.resize(2);
    
    //declares the coefficients a and b
    //and the neessary parameters to calculate them
    double sum_x, sum_x2, sum_y, sum_xy, a, b;
    sum_x=sum_x2=sum_y=sum_xy=0;

    //calculates the parameters
    for(int i = 0; i<n; i++){
        sum_x   +=offset+i;
        sum_x2  +=pow((offset+i),2);
        sum_y   +=Y[i];
        sum_xy  +=(offset+i)*Y[i];
    }
    //calculates the coeficients a and b
    a = 1/(n*sum_x2-pow(sum_x,2))*(sum_y*sum_x2-sum_x*sum_xy);
    b = 1/(n*sum_x2-pow(sum_x,2))*(n*sum_xy-sum_x*sum_y);
    
    coef[0]=a;
    coef[1]=b;
    
    return coef;
}
QVector<double> dfaAnalysis::linearfit(QVector<double> X, QVector<double> Y){
    QVector<double> coef;
    coef.resize(2);
    
    //declares the coefficients a and b
    //and the neessary parameters to calculate them
    int n = Y.count();
    double sum_x, sum_x2, sum_y, sum_xy, a, b;
    sum_x=sum_x2=sum_y=sum_xy=0;

    //calculates the parameters
    for(int i = 0; i<n; i++){
        sum_x   +=X[i];
        sum_x2  +=pow(X[i],2);
        sum_y   +=Y[i];
        sum_xy  +=X[i]*Y[i];
    }
    //calculates the coeficients a and b
    a = 1/(n*sum_x2-pow(sum_x,2))*(sum_y*sum_x2-sum_x*sum_xy);
    b = 1/(n*sum_x2-pow(sum_x,2))*(n*sum_xy-sum_x*sum_y);

    coef[0]=a;
    coef[1]=b;

    return coef;
}
QVector<double> dfaAnalysis::findAlphas(QVector<double> X, QVector<double> Y){
    double div=1.75;
    QVector<double> coefs1;
    coefs1.resize(2);
    QVector<double> coefs2;
    coefs2.resize(2);
    QVector<double> result;
    result.resize(2);
    
    double min_RMSD = 100000;
    double RMSD;
    double divisor = 0;
    
//    for(int i=0; i<20; i++){
        RMSD = 0;
        int j;
        for(j=0; X[j]<div; j++){
            
        }
        coefs1 = linearfit(X.mid(0, j-1), Y.mid(0, j-1));
        for(int k=0; k<j; k++){
            RMSD+=pow(Y[k]-(coefs1[0]+coefs1[1]*k),2);
        }
        coefs2 = linearfit(X.mid(j, X.count()-1), Y.mid(j, Y.count()-1));
        for(int k=j; k<Y.count()-1; k++){
            RMSD+=pow(Y[k]-(coefs2[0]+coefs2[1]*k),2);
        }
        RMSD = RMSD/Y.count();
        
//        printf("RMSD = %f, min_RMSD = %f\n", RMSD, min_RMSD);
        
        if(RMSD<min_RMSD){
            min_RMSD = RMSD;
            result[0] = coefs1[1];
            result[1] = coefs2[1];
            divisor = div;
        }
//        div = div+0.05;
//    }
//    printf("min alpha = %f\n", divisor);
//    fflush(stdout);
    
    return result;
}