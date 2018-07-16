/* 
 * File:   dfa_plugin.h
 * Author: Fabricio
 *
 * Created on August 5, 2013, 11:19 AM
 */

#ifndef DFA_PLUGIN_H
#define	DFA_PLUGIN_H

#include <QtPlugin>
#include <QtGui>

#include "methodResult.h"
#include "../eeg_analysis/src/pluginInterface.h"

class dfa_plugin : public QObject,
                            public AnalysisMethodInterface{
    Q_OBJECT;
    Q_INTERFACES(AnalysisMethodInterface)
    
public:
    dfa_plugin();
    
    virtual QStringList name() const;
    
    virtual QString description() const;
    
    virtual QWidget* parameters() const;
    
    virtual methodResult* analysis(QVector<double> data, int channel_index, QString fileName, QString channelName, QString savingPath);
private:
    signals:
    void EmitResult(methodResult*);
//    QCheckBox *isTesting;
};

#endif	/* DFA_PLUGIN_H */

