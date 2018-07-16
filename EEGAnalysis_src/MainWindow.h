/* 
 * File:   MainWindow.h
 * Author: Fabricio
 *
 * Created on 28 de Fevereiro de 2013, 10:53
 */

#ifndef _MAINWINDOW_H
#define	_MAINWINDOW_H

#include "ui_MainWindow.h"
#include "readEDF.h"

#include "qwt_plot_zoomer.h"
#include "qwt_plot_picker.h"
#include "qwt_plot_marker.h"
#include "qwt_plot_curve.h"

#include "qwtCustomPlot.h"
#include "Signals.h"
#include "pluginInterface.h"
#include "SignalFile.h"

#include <QFutureWatcher>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();
    
public slots:
    //Reading Functions
    void ReadEDF();
    void getFiles();
    void ReadChannels();
    void GetData(QVector< QVector <double> > data, QVector< double > l_reference);
    void ReadReference();
    void setReference(QString reference_label, QVector< double > l_reference);
    //void unloadFile();
    
    //Plotting and navigating functions
    void PlotGraphs(int index);
    void ZoomGraphs(const QRectF& rect);
    void Xforward();
    void Xbackward();
    void jumpToTime();
    
    //Marker related functions
    void marker1(const QRectF &pos); //Changing
    void marker(const QPointF &pos, int index);
    void unmarker(const QPointF &pos, int index);
    void addMarkersToPlots();
    void SaveMarkers();
    void accessMarkedOnly();
    
    //Filter related functions
    void setCutOffFreqVisible(int index);
    void FilterActivated();
    
    //Rescale functions
    void changeTimeScale(int index);
    void editTimeScale();
    void amplitudeRescale(int scale);
    void editAmplitudeScale();
    
    //Plugin functions
    void findPlugins();    
    void changeParametersPage();
    
    //Analysis functions
    void runAnalysis();
    void writeResults(int resultIndex);
//    void runMoreFiles();
    
    //Other tools
    void showDetails(const QPointF &pos1, const QPointF &pos2, int index);
    void EEGtoEDF();
    void toAscii();
    void newWindow();
    
    //Mouse listener selection
    void enablesZoomer();
    void enablesPointPicker();
    void enablesRectPicker();
    
    //General functions
    void changePage(int index);
    void keyPressEvent(QKeyEvent* event);
signals:
    void gotData();

private:
    int isDebugging;
    
    Ui::MainWindow widget;
    QString filepath;
    int from;
    
    QVector<SignalFile*> files;
    Signals *signal;
    readEDF *read;
    
    QVector< AnalysisMethodInterface* > methods;
    
    QVector< QFutureWatcher<methodResult*> *> methodResults;
    
//    QVector< QVector <double> > signal;
//    QString reference_label;
//    QVector< double > reference;
//    QStringList labels;
//    QVector<int> markedIndexes;
//    QVector< QVector <double> > pointmarkers;
    QVector< QVector <double> > hfo_markers;
    QVector< qwtCustomPlot *> plotvector;
};

#endif	/* _MAINWINDOW_H */