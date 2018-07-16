/* 
 * File:   pluginInterface.h
 * Author: Fabricio
 *
 * Created on 18 de Julho de 2013, 12:12
 */

#ifndef PLUGININTERFACE_H
#define	PLUGININTERFACE_H

#include <QtPlugin>
#include "methodResult.h"

class QString;
class QStringList;

class AnalysisMethodInterface
{
public:
    virtual ~AnalysisMethodInterface() {}
    
    virtual QStringList name() const = 0;
    
    virtual QString description() const = 0;
    
    virtual QWidget* parameters() const = 0;
    
    virtual methodResult* analysis(QVector<double> data, int channel_index, QString fileName, QString channelName, QString savingPath) = 0;
private:
    signals:
    void EmitResult(methodResult*);
};

Q_DECLARE_INTERFACE(AnalysisMethodInterface,
                     "SpikeMarker.AnalysisMethodInterface/0.02")

#endif	/* PLUGININTERFACE_H */

