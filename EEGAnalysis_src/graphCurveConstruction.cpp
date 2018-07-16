/* 
 * File:   GraphCurveConstruction.cpp
 * Author: Fabricio
 * 
 * Created on 1 de Março de 2013, 17:46
 */

#include "graphCurveConstruction.h"

GraphCurveConstruction::GraphCurveConstruction() {
}

GraphCurveConstruction::GraphCurveConstruction(const GraphCurveConstruction& orig) {
}

GraphCurveConstruction::~GraphCurveConstruction() {
}

/**
 * Constructs the curves to be plotted from input parameter "signal" to avoid
 * multiple points being plotted on a single pixel.
 * The construction takes into consideration the canvas size of the qwt_plot
 * ("canv_size") and the starting position of the zoom selection ("x_min")
 * @param signal
 * @param canv_size
 * @param x_min
 * @return 
 */
QwtPlotCurve* GraphCurveConstruction::GraphCurve(QVector<double> signal, int canv_size, double x_min, int sample_frequency){
    QwtPlotCurve *curve1 = new QwtPlotCurve("Curve");
    //Gets signal size
    int n = signal.count();
    
    if(n>signal.count()){
        n = signal.count();
    }
    
    QVector<double> temp_signal = signal;
    //If signal is too big, compresses the data to be plotted depending on canvas size
    if(n>600){
        //Gets canvas size
        
        int segment_size = ceil((double)n/(canv_size));
        int segment_number = 0;
        
        while(segment_number*segment_size < n){
            segment_number++;
        }
        if(segment_number*segment_size>n){
            segment_number--;
        }
        int remain = n - (segment_size * segment_number);
        QVector<QPointF> lines;
        double temp, min, max;
        for(int i = 0; i<segment_number; i++){
            temp = temp_signal[i*segment_size];
            min=temp;
            max=temp;
            for(int j = 1; j<segment_size; j++){
                temp = temp_signal[i*segment_size+j];
                if(temp<min){
                    min=temp;
                }else if(temp>max){
                    max=temp;
                }
            }
            lines << QPointF( (double)((x_min+ i*segment_size)/(double)sample_frequency), min );
            lines << QPointF( (double)((x_min+ i*segment_size)/(double)sample_frequency), max );
        }
        for(int i = 0; i<remain; i++){
            lines << QPointF((double)((x_min+segment_size*segment_number+i)/(double)sample_frequency), temp_signal[segment_size*segment_number+i]);
        }
        curve1->setSamples(lines);
    }else{
        QVector<double> temp_signalx;
        QVector<double> temp_signaly;
        temp_signalx.resize(n);
        temp_signaly.resize(n);
        for(int i = 0; i<n; i++){
            temp_signaly[i]=temp_signal[i];
            temp_signalx[i]=(double)((x_min+i)/(double)sample_frequency);
        }
        curve1->setSamples(temp_signalx, temp_signaly);
    }
    //emit EmitCurve(curve1);
    //curve1->setBrush(QBrush(Qt::red));
    curve1->setPen(QPen(QColor(100,100,100)));
    
    //Adds separate line to curve containing amplitude scale reference
//    QVector<QPointF> lines2;
//    lines2 << QPointF( (double)x_min, curve1->maxYValue() );
//    lines2 << QPointF( (double)x_min, curve1->maxYValue()-100 );
//    
//    curve1->setSamples(lines2);
    
    return(curve1);
}

/**
 * Returns a QwtPlotCurve as scale reference for the amplitude of the plotted
 * graphs. y_max and x_min are the reference vallues from qwt_plot to be used
 * on the definition of the position of this curve.
 * @param y_max
 * @param x_min
 * @return 
 */
QwtPlotCurve* GraphCurveConstruction::ScaleReference(double y_max, double x_min, QRectF rect, int sample_frequency){
    QwtPlotCurve *curve = new QwtPlotCurve();
    
//    double y_max = rect.y()+rect.height();
    y_max = rect.y()+rect.height();
//    double x_min = rect.x();
    
    double scale = y_max/4.0;
    double x_interval = rect.width()/(double)sample_frequency;
    double x_min1 = rect.x();
    double y1 = y_max-y_max/10;
    double y2 = y_max-(y_max/10+scale);
    
    QVector<QPointF> lines;
    lines << QPointF( (double)x_min1-x_interval, y1);
    lines << QPointF( (double)x_min1+x_interval, y1);
    lines << QPointF( (double)x_min1, y1);
    
    lines << QPointF( (double)x_min1, y2);
    lines << QPointF( (double)x_min1-x_interval, y2);
    lines << QPointF( (double)x_min1+x_interval, y2);
    
    curve->setSamples(lines);
    curve->setTitle("100 uV");
    
    return curve;
}