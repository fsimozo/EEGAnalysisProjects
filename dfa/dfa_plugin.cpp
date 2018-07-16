/* 
 * File:   dfa_plugin.cpp
 * Author: Fabricio
 * 
 * Created on August 5, 2013, 11:19 AM
 */

#include "dfa_plugin.h"
#include "simulatedHfoGenerator.h"
#include "dfaAnalysis.h"

#include "writeMarkersText.h"
#include "methodResult.h"
#include "FIRfilter.h"

#include <QStringList>
#include <QtGui>
#include <QElapsedTimer>
#include <QFileDialog>

#include <stdio.h>
QCheckBox *isTesting = new QCheckBox("Test");
QRadioButton *whiteButton = new QRadioButton("White noise test");
QRadioButton *brownButton = new QRadioButton("Brown noise test");
QRadioButton *pinkButton = new QRadioButton("Pink noise test");
QRadioButton *fileButton = new QRadioButton("Open data from file");
QLabel *n_points_label = new QLabel("Size of the simulated signal");
QLineEdit *n_points = new QLineEdit("");
QLabel *n_repeat_label = new QLabel("Number of signals generated");
QLineEdit *n_repeat = new QLineEdit("");
QRadioButton *temporalSplit = new QRadioButton("Analyze through time");
QLabel *n_segments_label = new QLabel("size of the segments analyzed");
QLineEdit *size_segments = new QLineEdit("");
QCheckBox *isFiltering = new QCheckBox("Filter for High Frequency analysis");

dfa_plugin::dfa_plugin(){
    
}

QStringList dfa_plugin::name() const {
    QStringList name;
    return name << "DFA";
}

QString dfa_plugin::description() const {
    QString description = "This method estimates the Hurst coefficient of local "
            "windows arround HFOs using the detrendred fluctiation analysis";
    return description;
}

QWidget* dfa_plugin::parameters() const {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(isTesting);
    layout->addWidget(whiteButton);
    layout->addWidget(brownButton);
    layout->addWidget(pinkButton);
    layout->addWidget(fileButton);
    
    QHBoxLayout *layout_n_points = new QHBoxLayout();
    layout_n_points->addWidget(n_points_label);
    layout_n_points->addWidget(n_points);
    layout->addLayout(layout_n_points);
    
    QHBoxLayout *layout_n_repeat = new QHBoxLayout();
    layout_n_repeat->addWidget(n_repeat_label);
    layout_n_repeat->addWidget(n_repeat);
    layout->addLayout(layout_n_repeat);
    
    QHBoxLayout *layout_temporalAnalysis = new QHBoxLayout();
    layout->addWidget(temporalSplit);
    layout_temporalAnalysis->addWidget(n_segments_label);
    layout_temporalAnalysis->addWidget(size_segments);
    layout->addLayout(layout_temporalAnalysis);
    
    layout->addWidget(isFiltering);
    
    page->setLayout(layout);
    return page;
}

methodResult* dfa_plugin::analysis(QVector<double> data, int channel_index, QString fileName, QString channelName, QString savingPath){
    int isDebugging = 0;
    
    methodResult *result = new methodResult();
    
    dfaAnalysis *dfa = new dfaAnalysis();
    
    //Takes variables defined by parameters
    int size = n_points->text().toInt();
    if (size==0)
        size = 100000;
    
    int iterations = n_repeat->text().toInt();
    if (iterations == 0)
        iterations = 1;
    
    QVector< QVector<double> > coef;
    coef.resize(2);
    
    QVector<double> signal; 
    
//    If is testing
    if(isTesting->isChecked()){
        
        QVector< QVector<double> > noises;
        noises.resize(iterations);
        
        simulatedHfoGenerator *generator1 = new simulatedHfoGenerator();
        
        if(whiteButton->isChecked()){
            for(int i=0; i<iterations; i++){
                noises[i] = generator1->whiteNoise(size);
            }
        } else if(brownButton->isChecked()){
            for(int i=0; i<iterations; i++){
                noises[i] = generator1->brownNoise(size);
            }
        } else if(pinkButton->isChecked()){
//            noises = generator1->pinkNoise(size, 0.5, iterations);
        }
        
        coef[0].resize(iterations);
        coef[1].resize(iterations);
        
        QVector<double> elapsed_time;
        elapsed_time.resize(iterations);
        
        for(int i=0; i<iterations; i++){
            QVector<double> temp;
            temp.resize(2);
            
            temp = dfa->doAnalysis(noises[i]);
            coef[0][i] = temp[0];
            coef[1][i] = temp[1];
        }
        
//        result->addGraph("time", "elapsed", "time", elapsed_time, elapsed_time);
        
   } else {
        if(fileButton->isChecked()){
            //Testing from file
            FILE *noise;
            noise = fopen("D:/Bibliotecas/Dropbox/Mestrado/testes/Testes DFA C++/reference/rr_interval.txt", "r");
        
            char line[10];
            unsigned char temp;
            signal.resize(106084);
            int i=0;
            while(fgets(line, 15, noise) != NULL){
                sscanf (line, "%ld", &temp);
                signal[i++] = temp;
            }
            
//            for(int i=0; i<106084; i++){
//                sscanf (line, "%ld", &temp);
//                signal[i] = temp;
//            }
        } else{
            signal = data;
            size = signal.count();
        }
        
        //Filters signal if filtering is checked
        if(isFiltering->isChecked()){
//            QVector<double> filtered_signal;
//            FIRfilter *filter = new FIRfilter();
////            filtered_signal.resize(signal.count());
//            QVector<int> arguments;
//            arguments.resize(5);
//            arguments[0] = 80;             //cuttof freq 1
//            arguments[1] = 128;            //cuttof freq 2 (not used for high pass)
//            arguments[2] = 2;              //filter type (2=high pass)
//            arguments[3] = 0;              //initial position of filtering
//            arguments[4] = signal.count(); //size of signal to be filtered
//            signal = filter->simpleFilter(signal,arguments, 256);
            int sig_freq = 256;
            double pi = atan(1)*4;
            int N = 401;
            int Tau = (N-1)/2;
            
            QVector<double> filter;
            filter.resize(N);
            
            
            
            double wp1 = (80*2*pi)/sig_freq; //1000 = sample freq.
            double wp2= (128*2*pi)/sig_freq;

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
            QVector<double> temp;
            temp.resize(signal.count());
            for(int i = N; i<data.count(); i++){
                temp[i-N]=0;
                for(int j = 0; j<filter.count(); j++){
                    if((i-j)>=0){
                        temp[i-N]+=filter[j]*signal[i-j];
                    }
                }
            }
//            signal.clear();
            signal = temp;
        }
        
        coef[0].resize(1);
        coef[1].resize(1);
        
        if(temporalSplit->isChecked()){
            int segment_size = size_segments->text().toInt();
            if(segment_size == 0)
                segment_size = 5000;
            
            int n_segments = floor(size/segment_size);
            
            coef[0].resize(n_segments);
            coef[1].resize(n_segments);
            
            for(int i=0; i<n_segments; i++){
                QVector<double> temp;
                temp.resize(2);
                
                temp = dfa->doAnalysis(signal.mid(segment_size*i, segment_size));
                coef[0][i] = temp[0];
                coef[1][i] = temp[1];
            }
            
//            if(size%segment_size){
//                int rest = size - n_segments*segment_size;
//                coef.resize(n_segments+1);
//                coef[n_segments] = dfa->doAnalysis(signal.mid(segment_size*n_segments, rest));
//            }
        }
        else{
            QVector<double> temp;
            temp.resize(2);
            
            temp = dfa->doAnalysis(signal);
            coef[0][0] = temp[0];
            coef[1][0] = temp[1];
        }
   }
//    result->addSeries("dfa coef.", coef[0]);
//    result->addSeries("dfa coef2", coef[1]);
    result->addGraph("DFA coef", "alpha1", "alpha2", coef[0], coef[1]);
    
    //Writing results
//    
//    QDir path = saving_path;
//    
//    writeMarkersText *writer = new writeMarkersText();
//    
//    printf("channel index = %d\n", channel_index);
//    fflush(stdout);
//    writer->writeMethodResults(result, path, "", "DFA_Analysis", channel_index);
//    
//    if(isDebugging){
//        printf("Files created\n");
//        fflush(stdout);
//    }
    
    result->setResultName(channelName, fileName, savingPath);
    
    emit EmitResult(result);
    
    return result;
}

Q_EXPORT_PLUGIN2(AnalysisMethodInterface, dfa_plugin)