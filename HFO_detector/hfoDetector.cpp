/* 
 * File:   hfoDetector.cpp
 * Author: Fabricio
 * 
 * Created on August 19, 2013, 11:54 AM
 */

#include "hfoDetector.h"

#include <stdio.h>

//#include "../eeg_analysis/src/methodResult.h"
#include "methodResult.h"
#include "FIRfilter.h"
#include "writeMarkersText.h"


QCheckBox *isTesting = new QCheckBox("Test with simulated signal");

hfoDetector::hfoDetector() {
}

QStringList hfoDetector::name() const {
    QStringList name;
    return name << "HFO Detector";
}

QString hfoDetector::description() const {
    QString description = "This method estimates the Hurst coefficient of local "
            "windows arround HFOs using the detrendred fluctiation analysis";
    return description;
}

QWidget* hfoDetector::parameters() const {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(isTesting);
    page->setLayout(layout);
    return page;
}

methodResult* hfoDetector::analysis(QVector<double> X, QVector<double> Y, int channel_index, QString saving_path){
    methodResult *result = new methodResult();
    
    double sig_freq = 256;
    
//    if(isTesting->isChecked()){
//        int size = Y.count();
//        Y.clear();
//        Y.squeeze();
//        
//        Y.resize(size);
//        
//        double pi = atan(1)*4;
//        
//        double samp_freq = 1000;
//        
//        for(int i = 0; i<Y.count(); i++){
//            Y[i]=100*sin(2*pi*10*(1.0/samp_freq)*i)+100*sin(2*pi*20*(1.0/samp_freq)*i) 
//                    + 100*sin(2*pi*30*(1.0/samp_freq)*i)+100*sin(2*pi*40*(1.0/samp_freq)*i);
//            
//            int freq = 80;
//
//            if(i>=10000 && i<=10050){
//                Y[i]+=10*sin(2*pi*freq*0.001*i);
//            }else if(i>=15000 && i<=15050){
//                Y[i]+=10*sin(2*pi*(freq+2)*0.001*i);
//            }else if(i>=20000 && i<=20050){
//                Y[i]+=10*sin(2*pi*(freq+4)*0.001*i);
//            }else if(i>=25000 && i<=25050){
//                Y[i]+=10*sin(2*pi*(freq+6)*0.001*i);
//            }else if(i>=30000 && i<=30050){
//                Y[i]+=10*sin(2*pi*(freq+8)*0.001*i);
//            }else if(i>=35000 && i<=35050){
//                Y[i]+=10*sin(2*pi*(freq+10)*0.001*i);
//            }else if(i>=40000 && i<=40050){
//                Y[i]+=10*sin(2*pi*(freq+12)*0.001*i);
//            }else if(i>=45000 && i<=45050){
//                Y[i]+=10*sin(2*pi*(freq+14)*0.001*i);
//            }else if(i>=50000 && i<=50050){
//                Y[i]+=10*sin(2*pi*(freq+16)*0.001*i);
//            }else if(i>=55000 && i<=55050){
//                Y[i]+=10*sin(2*pi*(freq+18)*0.001*i);
//            }else if(i>=60000 && i<=60050){
//                Y[i]+=10*sin(2*pi*(freq+20)*0.001*i);
//            }else if(i>=65000 && i<=65050){
//                Y[i]+=10*sin(2*pi*(freq+22)*0.001*i);
//            }else if(i>=70000 && i<=70050){
//                Y[i]+=10*sin(2*pi*(freq+24)*0.001*i);
//            }else if(i>=75000 && i<=75050){
//                Y[i]+=10*sin(2*pi*(freq+26)*0.001*i);
//            }else if(i>=80000 && i<=80050){
//                Y[i]+=10*sin(2*pi*(freq+28)*0.001*i);
//            }else if(i>=85000 && i<=85050){
//                Y[i]+=10*sin(2*pi*(freq+30)*0.001*i);
//            }else if(i>=90000 && i<=90050){
//                Y[i]+=10*sin(2*pi*(freq+32)*0.001*i);
//            }else if(i>=95000 && i<=95050){
//                Y[i]+=10*sin(2*pi*(freq+34)*0.001*i);
//            }else if(i>=100000 && i<=100050){
//                Y[i]+=10*sin(2*pi*(freq+36)*0.001*i);
//            }else if(i>=105000 && i<=105050){
//                Y[i]+=10*sin(2*pi*(freq+38)*0.001*i);
//            }else if(i>=110000 && i<=110050){
//                Y[i]+=10*sin(2*pi*(freq+40)*0.001*i);
//            }
//        }
//    }
    
    //Step 1: Apply the necessary filters to the singal
//    FIRfilter *filter = new FIRfilter();
//    //Broad - Band signal
//    QVector<int> arguments;
//    arguments.resize(5);
//    arguments[0] = 80;             //cuttof freq 1
//    arguments[1] = 300;            //cuttof freq 2 (not used for high pass)
//    arguments[2] = 2;              //filter type (1=high pass)
//    arguments[3] = 0;              //initial position of filtering
//    arguments[4] = Y.count();      //size of signal to be filtered
//    
//    Y = filter->simpleFilter(Y, arguments, sig_freq);
    int N = 401;
    int Tau = (N-1)/2;

    QVector<double> filter;
    filter.resize(N);

    double pi = atan(1)*4;
    
    double wp1 = (80*2*pi)/sig_freq; //1000 = sample freq.
    double wp2= (300*2*pi)/sig_freq;
    
    //Band-pass
    for(int i=0; i<N; i++){
        if (i == Tau)
            filter[i] = (wp2-wp1)/pi;
        else
            filter[i] = (sin((i - Tau)*wp2)-sin((i - Tau)*wp1))/(pi*(i-Tau));
    }
    //Generalized Hamming window
    double alpha = 0.54;
    for(int i=0; i<N; i++){
        filter[i]=filter[i]*(alpha-(1-alpha)*cos(2*pi*i/(N-1)));
    }
    //filters signal
    QVector<double> filtered;
    filtered.resize(Y.count());

    for(int i = N; i<Y.count(); i++){
        filtered[i-N]=0;
        for(int j = 0; j<filter.count(); j++){
            if((i-j)>=0){
                filtered[i-N]+=filter[j]*Y[i-j];
            }
        }
    }
    
    //Filters signal in 5 different frequency bands
    int n_freqbands = 1;
    
    QVector< QVector<double> > freq_bands;
    freq_bands.resize(n_freqbands);
    
    QVector< QVector <double> > dk;
    dk.resize(n_freqbands);
    
    double F1_vec[] = {
//        80,
//        122,
//        182,
//        242   
        80
    };
    double F2_vec[] = {
//        118,
//        178,
//        238,
//        298
        128
    };
    
    
    for(int k=0; k<n_freqbands; k++){
//        QVector<int> arguments;
//        arguments.resize(5);
//        arguments[0] = F1_vec[i];      //cuttof freq 1
//        arguments[1] = F2_vec[i];      //cuttof freq 2 (not used for high pass)
//        arguments[2] = 2;              //filter type (1=high pass)
//        arguments[3] = 0;              //initial position of filtering
//        arguments[4] = Y.count();      //size of signal to be filtered
//        
//        freq_bands[i]=filter->simpleFilter(Y, arguments, sig_freq);
//        int N = 401;
//        int Tau = (N-1)/2;
        
        freq_bands[k].resize(Y.count());
        
        wp1 = (F1_vec[k]*2*pi)/sig_freq; //1000 = sample freq.
        wp2= (F2_vec[k]*2*pi)/sig_freq;
        
        //Band-pass
        for(int i=0; i<N; i++){
            if (i == Tau)
                filter[i] = (wp2-wp1)/pi;
            else
                filter[i] = (sin((i - Tau)*wp2)-sin((i - Tau)*wp1))/(pi*(i-Tau));
        }
        //Generalized Hamming window
        double alpha = 0.54;
        for(int i=0; i<N; i++){
            filter[i]=filter[i]*(alpha-(1-alpha)*cos(2*pi*i/(N-1)));
        }
        
        for(int i = N; i<Y.count(); i++){
            freq_bands[k][i]=0;
            for(int j = 0; j<filter.count(); j++){
                if((i-j)>=0){
                    freq_bands[k][i-N]+=filter[j]*Y[i-j];
                }
            }
        }
    }
    
    QVector<double> Bk_r;
    QVector<double> RMS_r;
    
    for(int i=0; i<n_freqbands; i++){
        QVector<double> RMS;
        RMS.resize(freq_bands[i].count());
        //Step 2: For each frequency band, compute the RMS
        int Nk = (int)sig_freq/((F2_vec[i]-F1_vec[i])/2);
        //double Nk = 1;
        for(int k = 0; k<freq_bands[i].count(); k++){
            double sum = 0;
            for(int j = -2*Nk;j<=2*Nk;j++){
            //for(int j = 0; j<1; j++){
                if((k-j)>=0 && (k-j)<freq_bands[i].count()){
                        sum += pow(freq_bands[i][k-j],2);
                }
            }
            RMS[k] = sqrt(1/(4*(double)(Nk) +1)*sum);
        }
        
        //Step 3: Compute the moving background level
        double Nb = 30*sig_freq;//30: defined on paper, 1000: sample freq.
        double c = 2.5;
        
        QVector<double> Bk,Z_k;
        Bk.resize(Y.count());
        Z_k.resize(Y.count());
        
        Bk[0]=3*RMS[0]/c;
//        Bk[0]=1;
        
        for(int j = 1; j<RMS.count(); j++){
            if((j-2*Nk-Nb)>=0){
                Bk[j]=Bk[j-1]+1/Nb*(Z_k[j-2*Nk]-Z_k[j-2*Nk-Nb]);
            }else if((j-2*Nk)>=0){
                Bk[j]=Bk[j-1]+1/Nb*(Z_k[j-2*Nk]);
            }else{
                Bk[j]=Bk[j-1];
                //Bk[i]=RMS[i]/Nb;
            }
            if(RMS[j]<=c*Bk[j]){
                Z_k[j]=RMS[j];
            }else{
                Z_k[j]=c*Bk[j];
            }
        }
        Bk_r.resize(Bk.count());
        RMS_r.resize(RMS.count());
        
        //Step 4: Compares filtered_bands with background
        for(int j = 0; j<RMS.count(); j++){
            dk[i].resize(RMS.count());
            Bk_r[j] = c*Bk[j];
            RMS_r[j] = RMS[j];
            
            if(RMS[j]>=c*Bk[j]){
                dk[i][j]=1;
            }else{
                dk[i][j]=0;
            }
        }
        
    }
    
    //Constructs final results
    QVector<int> n_detections;
    n_detections.resize(n_freqbands);
    
    QVector< QVector <double> > detections;
    detections.resize(n_freqbands);
    
    int M=0;
    
    for(int i = 0; i<n_freqbands; i++){
        int counter = 0;
        detections[i].resize(Y.count());
        n_detections[i]=0;
        for(int j = 0; j<dk[i].count();j++){
            if(dk[i][j]==1){
                counter++;
            }else{
                if(counter>=(50)){
                    for(int k = (j-counter-M); k<(j+M); k++){
                        if(k>=0 && k<freq_bands[i].count()){
//                                result[i][k] = freq_bands[i][k];
                                detections[i][k] = dk[i][k];
                        }
                    }
                    n_detections[i]++;
                }
                counter = 0;
            }
        }
    }
    
    //Convert detections to detection_t_5k window
    int wind_size = 5000;
    int n_segments = Y.count()/wind_size;
    QVector<double> detections_t;
    detections_t.resize(n_segments);
    
    for(int i=0; i<n_segments; i++){
        detections_t[i]=0;
        for(int j=0; j<wind_size; j++){
            detections_t[i]+=detections[0][i*wind_size+j];
        }
        detections_t[i]=detections_t[i]/(double)wind_size;
    }
    
    //Adds detections to results
    for(int i=0; i<n_freqbands; i++){
//        result->addPoint("detections_"+QString::number(i+1), n_detections[i]);
//        result->addGraph("Detections"+QString::number(i+1), "detections", "detections",detections[i], detections[i]);
//        result->addSeries("Detections "+QString::number(i), detections[i]);
        result->addSeries("Detections "+QString::number(i), detections_t);
        result->addGraph("Compare vectors", "RMS", "c*Bk", RMS_r, Bk_r);
    }
    //
//    result->addGraph("dfa coef.", "iteration", "Coef.", x, coef);
//    result->addGraph("dfa filtered coef.", "iteration", "f_Coef.", x, f_coef);
//    if(isTesting->isChecked()){
//        result->addGraph("dfa no hfo coef.", "iteration", "n_Coef.", x, n_coef);
//    }
    
    //Writing results
    QDir path = saving_path;
    
    writeMarkersText *writer = new writeMarkersText();
    writer->writeMethodResults(result, path, "", "DFA_Analysis", channel_index);
    return result;
}

Q_EXPORT_PLUGIN2(AnalysisMethodInterface, hfoDetector)