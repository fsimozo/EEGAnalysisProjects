/* 
 * File:   hfoDetector.h
 * Author: Fabricio
 *
 * Created on August 19, 2013, 11:54 AM
 */

#ifndef HFODETECTOR_H
#define	HFODETECTOR_H

#include <QtPlugin>
#include <QtGui>

#include "methodResult.h"
#include "../eeg_analysis/src/pluginInterface.h"

class hfoDetector : public QObject,
                            public AnalysisMethodInterface{
    Q_OBJECT;
    Q_INTERFACES(AnalysisMethodInterface)
    
public:
    hfoDetector();
    
    virtual QStringList name() const;
    
    virtual QString description() const;
    
    virtual QWidget* parameters() const;
    
    virtual methodResult* analysis(QVector<double> X, QVector<double> Y, int channel_index, QString saving_path);
private:
//    QCheckBox *isTesting;
};

#endif	/* HFODETECTOR_H */

