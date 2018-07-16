/* 
 * File:   GraphCurveConstruction.h
 * Author: Fabricio
 *
 * Created on 1 de Março de 2013, 17:46
 */

#ifndef GRAPHCURVECONSTRUCTION_H
#define	GRAPHCURVECONSTRUCTION_H

#include "qwt_plot_curve.h"

class GraphCurveConstruction {
public:
    GraphCurveConstruction();
    GraphCurveConstruction(const GraphCurveConstruction& orig);
    virtual ~GraphCurveConstruction();
public slots:
    QwtPlotCurve* GraphCurve(QVector<double> signal, int canv_size, double x_min, int sample_frequency);
    QwtPlotCurve* ScaleReference(double y_max, double x_min, QRectF rect, int sample_frequency);
    
    signals:
    void EmitCurve(QwtPlotCurve *curve);
    
private:

};

#endif	/* GRAPHCURVECONSTRUCTION_H */

