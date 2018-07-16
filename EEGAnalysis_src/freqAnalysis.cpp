/*
 * File:   freqAnalysis.cpp
 * Author: Fabricio
 *
 * Created on 2 de Maio de 2013, 15:29
 */

#include "freqAnalysis.h"

#include "qwtCustomPlot.h"

#include "qwt_plot_curve.h"
#include "qwt_plot_marker.h"
#include "qwt_symbol.h"

#include <QVector>

freqAnalysis::freqAnalysis() {
    widget.setupUi(this);
    
    plot = new qwtCustomPlot();
    widget.verticalLayout->addWidget(plot);
    
    plot->plot->enableAxis(plot->plot->xBottom, false);
    
    widget.lineEdit->setText("0.6");
    
    connect(widget.estimateButton, SIGNAL(pressed()),
            this, SLOT(freqEstimative()));
}

freqAnalysis::~freqAnalysis() {
}

void freqAnalysis::plotData(QVector<double> data){
    data_ = data;
    
    QwtPlotCurve* curve = new QwtPlotCurve("curve");
    QVector<double> X;
    X.resize(data.count());
    for(int i = 0; i<X.count(); i++){
        X[i]=i;
    }
    
    curve->setSamples(X, data);
    curve->attach(plot->plot);
    
    plot->plot->replot();
    plot->zoomer->setZoomBase();
}

void freqAnalysis::freqEstimative(){
    plot->plot->detachItems(QwtPlotItem::Rtti_PlotItem, false);
    plotData(data_);
    
    QVector<int> peaks;
    
    peaks = findPeaks(mavg(data_, 3));
    
    if(peaks.count()>1){
        widget.lineEdit_3->setText(QString::number(peaks.count()-1));
        widget.lineEdit_4->setText(QString::number((double)peaks[peaks.count()-1]/samp_freq-(double)peaks[0]/samp_freq));
        double freq = (double)(peaks.count()-1)/(((double)peaks[peaks.count()-1]/samp_freq-(double)peaks[0]/samp_freq));
        
        widget.lineEdit_2->setText(QString::number(freq, 'f', 1));
    }
    
    for(int i=0; i<peaks.count(); i++){
        QwtPlotMarker *marker_1 = new QwtPlotMarker();
        QwtSymbol *symb = new QwtSymbol();
        symb->setColor(Qt::red);
        symb->setStyle(QwtSymbol::VLine);
        symb->setSize(500);

        marker_1->setSymbol(symb);
        double X = peaks[i];
        marker_1->setXValue(X);
        marker_1->attach(plot->plot);
        plot->plot->replot();
    }
}

QVector<double> freqAnalysis::mavg(QVector<double> data, int w){
    QVector<double> result;
    result.resize(data.count());
    
    int w2 = w/2;
    
    for(int i=0; i<data.count(); i++){
        result[i]=data[i];
        for(int j=0; j<w2; j++){
            if((i-j)<0)
                result[i]+=2*data[i+j];
            
            else if((i+j)>=data.count())
                result[i]+=2*data[i-j];
            
            else 
                result[i]+=data[i-j]+data[i+j];
            
            result[i]=result[i]/w2;
        }
    }
    
    return result;
}

QVector<int> freqAnalysis::findPeaks(QVector<double> data){
    QVector<int> indexes;
    int size = data.count();
    
    double max = data[0];
    for(int i=1; i<size; i++){
        if(data[i]>max){
            max = data[i];
        }
    }
    
    double threshold = widget.lineEdit->text().toDouble()*max;
    
    bool isAbove = false;
    
    int localMax = 0;
    
    for(int i=0; i<size; i++){
        if(!isAbove){
            if(data[i]>threshold){
                localMax = i;
//                indexes.append(i);
                isAbove = true;
            }
        }else{
            if(data[i]>data[localMax]){
                localMax = i;
            }
            if(data[i]<threshold){
                isAbove = false;
                indexes.append(localMax);
            }
        }
    }
    
    return indexes;
}

void freqAnalysis::set_sampleFrequency(int freq){
    samp_freq = freq;
}