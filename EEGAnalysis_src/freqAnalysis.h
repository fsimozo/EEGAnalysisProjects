 /* 
 * File:   freqAnalysis.h
 * Author: Fabricio
 *
 * Created on 2 de Maio de 2013, 15:29
 */

#ifndef _FREQANALYSIS_H
#define	_FREQANALYSIS_H

#include "ui_freqAnalysis.h"

#include "qwtCustomPlot.h"

class freqAnalysis : public QDialog {
    Q_OBJECT
public:
    freqAnalysis();
    virtual ~freqAnalysis();
    
public slots:
    void plotData(QVector<double> data);
    void freqEstimative();
    QVector<double> mavg(QVector<double> data, int w);
    QVector<int> findPeaks(QVector<double> data);
    
    void set_sampleFrequency(int freq);
        
private:
    Ui::freqAnalysis widget;
    
    int samp_freq;
    qwtCustomPlot* plot;
    QVector<double> data_;
};

#endif	/* _FREQANALYSIS_H */
