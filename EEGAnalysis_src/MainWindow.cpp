/*
 * File:   MainWindow.cpp
 * Author: Fabricio
 *
 * Created on 28 de Fevereiro de 2013, 10:53
 */
//"comando make = make.exe" em Ferramentas
//Para isso, C:/msys/1.0/bin foi adicionado à variavel de ambiente PATH
//Possivel conflito ao tentar o comando make no terminal

#include "MainWindow.h"
#include <QDesktopWidget>
#include "readEDF.h"
#include "graphCurveConstruction.h"
#include "FIRfilter.h"
#include "writeMarkersText.h"
#include "convertEEGtoEDF.h"
#include "qwtCustomPlot.h"
#include "freqAnalysis.h"
#include "Signals.h"
#include "pluginInterface.h"
#include "methodResult.h"
#include "SignalFile.h"

#include "qwt_plot.h"
#include "qwt_plot_canvas.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_marker.h"
#include "qwt_symbol.h"

#include <QApplication>
#include <QThread>
#include <QFuture>
#include <QFutureWatcher>
#include <QMap>
#include <QThreadPool>
#include <QtConcurrentMap>
#include <QtConcurrentRun>
#include <QVector>
#include <QSignalMapper>
#include <QPluginLoader>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>

#include <QKeyEvent>

MainWindow::MainWindow() {
    widget.setupUi(this);
    
    signal = new Signals();
    
//    files = new fileQueue();
    
    read = new readEDF();
    
    QThreadPool::globalInstance()->setMaxThreadCount(6);
    /**
     * Defines if it is debugging or not. If it is, a series of messages will
     * apear on console indicating the functioning of the program.
     */
    isDebugging=1;
    if(isDebugging){
        printf("Debugging option is selected.\n");
        fflush(stdout);
    }
    
    //Finds and adds plugin methods to the software
    findPlugins();
    
    //Disables label and lineedit for filter order
    widget.label->setVisible(false);
    widget.lineEdit->setVisible(false);
    
    //Defines number of QwtPlots to be shown
    plotvector.resize(6);
    //filepath = new char[256];
    //Creates qwtCustomPlots
    for(int i=0; i<plotvector.count(); i++){
        qwtCustomPlot *plot = new qwtCustomPlot;
        plotvector[i]=plot;
        plotvector[i]->plot->enableAxis(plotvector[i]->plot->xBottom, false);
        plotvector[i]->plot->setAxisAutoScale(plotvector[i]->plot->yLeft, false);
        //Sets an index inside the object, useful for calling slots from
        //different qwt_plot
        plotvector[i]->setPlotIndex(i);
        
        //Connects unpickers to corresponding qwt_plot SLOT
        connect(plotvector[i], SIGNAL(unpicker_signal(const QPointF, int)),
                this, SLOT(unmarker(const QPointF, int)));
        //Connects zoomers to ZoomGraphs slot
//        connect(plotvector[i]->zoomer, SIGNAL(zoomed(const QRectF&)),
//                this, SLOT(ZoomGraphs(const QRectF&)));
        connect(plotvector[i]->zoomer, SIGNAL(zoomed(const QRectF&)),
                this, SLOT(ZoomGraphs(const QRectF&)));
        //Connects "point pickers" to corresponding qwt_plot SLOT
        connect(plotvector[i], SIGNAL(picker_signal( const QPointF, int)),
                this, SLOT(marker(const QPointF, int)));
        //Connects rect pickers signals to detail showing window slot
        connect(plotvector[i], SIGNAL(picker_rect_signal( const QPointF, const QPointF, int)),
                this, SLOT(showDetails(const QPointF, const QPointF, int)));
        
        //Add each QwtPlot to the corresponding layout on mainwindow
        widget.verticalLayout_1->addWidget(plotvector[i]);
    }
    //For the last QwtPlot, enables X axis and redefines height to similar size of the others
    plotvector[plotvector.count()-1]->plot->enableAxis(plotvector[plotvector.count()-1]->plot->xBottom, true);
    plotvector[plotvector.count()-1]->plot->setMinimumHeight(100);//cirep = 150
    
    //Connects reader to corresponding functions
    connect(read, SIGNAL(accepted()),
            this, SLOT(getFiles()));
    
    connect(widget.file_comboBox, SIGNAL(activated(int)),
            this, SLOT(ReadChannels()));
    
    //Connects actions to corresponding functions
    connect(widget.actionExit, SIGNAL(triggered()),
            this, SLOT(close()));
    connect(widget.actionOpen, SIGNAL(triggered()),
            this, SLOT(ReadEDF()));
    connect(widget.actionSave_Markers, SIGNAL(triggered()),
            this, SLOT(SaveMarkers()));
    connect(widget.action_EEG_to_EDF_conversion, SIGNAL(triggered()),
            this, SLOT(EEGtoEDF()));
    connect(widget.actionExport_to_ASCII, SIGNAL(triggered()),
            this, SLOT(toAscii()));
    connect(widget.actionNew_Window, SIGNAL(triggered()),
            this, SLOT(newWindow()));
    
    //Connects navigation buttons to corresponding navigation slots
    connect(widget.forwardButton, SIGNAL(pressed()),
            this, SLOT(Xforward()));
    connect(widget.backwardButton, SIGNAL(pressed()),
            this, SLOT(Xbackward()));
    connect(widget.jumpTo_lineEdit, SIGNAL(returnPressed()),
            this, SLOT(jumpToTime()));
    
    connect(widget.comboBox_amplitude, SIGNAL(activated(int)),
            this, SLOT(amplitudeRescale(int)));
    connect(widget.lineEdit_ampScale, SIGNAL(returnPressed()),
            this, SLOT(editAmplitudeScale()));
    
    connect(widget.f1_lineEdit, SIGNAL(returnPressed()),
            this, SLOT(FilterActivated()));
    connect(widget.f2_lineEdit, SIGNAL(returnPressed()),
            this, SLOT(FilterActivated()));
    connect(widget.filterComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(setCutOffFreqVisible(int)));
    connect(widget.filterComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(FilterActivated()));
    
    connect(widget.filterCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(FilterActivated()));
    
    //Connects zoomer checkbox to enable zoomer function
    connect(widget.zoomerButton, SIGNAL(pressed()),
            this, SLOT(enablesZoomer()));
    connect(widget.markerButton, SIGNAL(pressed()),
            this, SLOT(enablesPointPicker()));
//    connect(widget.rectmarkerButton, SIGNAL(pressed()),
//            this, SLOT(enablesRectPicker()));
    connect(widget.detailButton, SIGNAL(pressed()),
            this, SLOT(enablesRectPicker()));
    
    //Connects viewing mode selection
    connect(widget.actionView_marked, SIGNAL(triggered()),
            this, SLOT(accessMarkedOnly()));
    
    //Connects analysis signals and slots
    connect(widget.runMethodButton, SIGNAL(pressed()),
            this, SLOT(runAnalysis()));
//    QSignalMapper *runMapper = new QSignalMapper(this);
//    connect(runMapper, SIGNAL(mapped(QString)),
//            this, SLOT(runAnalysis(QString)));
//    runMapper->setMapping(widget.runMethodButton, "");
//    connect(widget.runMethodButton, SIGNAL(pressed()),
//            runMapper, SLOT(map()));
    
    connect(widget.methodSelect, SIGNAL(itemSelectionChanged()),
            this, SLOT(changeParametersPage()));
    
    //Map the button signals for the time scale, so only one slot needs to be
    //created
    QSignalMapper *timeMapper = new QSignalMapper(this);
    connect(timeMapper, SIGNAL(mapped(int)),
            this, SLOT(changeTimeScale(int)));
    timeMapper->setMapping(widget.timescaleButton1s, 1);
    timeMapper->setMapping(widget.timescaleButton10s, 10);
    connect(widget.timescaleButton1s, SIGNAL(pressed()),
            timeMapper, SLOT(map()));
    connect(widget.timescaleButton10s, SIGNAL(pressed()),
            timeMapper, SLOT(map()));
    
    connect(widget.lineEdit_timeScale, SIGNAL(returnPressed()),
            this, SLOT(editTimeScale()));
    
    //Map the button signals for the page changes, so only one slot needs to be
    //created
    QSignalMapper *mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(int)),
            this, SLOT(changePage(int)));
    mapper->setMapping(widget.pushButton, 0);
    mapper->setMapping(widget.pushButton_2, 1);
    mapper->setMapping(widget.pushButton_3, 2);
    connect(widget.pushButton, SIGNAL(pressed()),
            mapper, SLOT(map()));
    connect(widget.pushButton_2, SIGNAL(pressed()),
            mapper, SLOT(map()));
    connect(widget.pushButton_3, SIGNAL(pressed()),
            mapper, SLOT(map()));
}

MainWindow::~MainWindow() {
}
/*Reading Functions ************************************************************************************************/
/**
 * Handles open EDF file actions
 * Creates an instance of the readEDF form and uses it to select and emit back
 * the desired data
 */
void MainWindow::ReadEDF(){
    int fileselected = read->GetFilePath();
    
    if(fileselected == 1){
        read->show();
        disconnect(read, SIGNAL(EmitData(QVector <QVector <double> >, QVector< double >)),
                this, SLOT(GetData(QVector <QVector <double> >, QVector< double >)));
        connect(read, SIGNAL(EmitData(QVector <QVector <double> >, QVector< double >)),
                this, SLOT(GetData(QVector <QVector <double> >, QVector< double >)));
    }else{
        if(isDebugging){
            printf("File was not valid.\n");
            fflush(stdout);
        }
        return;
    }
    if(isDebugging){
        printf("File read.\n");
        fflush(stdout);
    }
}

void MainWindow::getFiles(){
    QVector< SignalFile*> temp_files = read->getFiles();
    
    int n = temp_files.count();
    
    for(int i=0; i<n; i++){
        files.append(temp_files[i]);
    }
    
    widget.file_comboBox->clear();
    
    for(int i=0; i<files.count(); i++)
        widget.file_comboBox->addItem(files[i]->getFileName());
    
    ReadChannels();
}

void MainWindow::ReadChannels(){
    //Clears previous signals from the memory
    if(signal->count()>0){
        for(int i=0; i<plotvector.count(); i++){
            plotvector[i]->plot->detachItems(QwtPlotItem::Rtti_PlotItem, false);
            plotvector[i]->plot->replot();
            plotvector[i]->label->setText("");
        }

        signal->clear();
    //    delete signal;

        signal = new Signals();

        from = 0;
        filepath.clear();
        hfo_markers.clear();
        widget.comboBox->clear();

        widget.channelSelect->clear();

        disconnect(widget.ref_comboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(ReadReference()));
        widget.ref_comboBox->clear();
        connect(widget.ref_comboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(ReadReference()));
    }
    int index = widget.file_comboBox->currentIndex();
    read->readChannels(files[index]);
}
/**
 * Stores data received from ReadEDF on main window and adds data labels to
 * combo box
 * @param data
 * @param labels
 */
void MainWindow::GetData(QVector<QVector<double> > data, QVector< double > l_reference){
    //New class test
    for(int i=0; i<data.count(); i++){
        QString label_temp = files[widget.file_comboBox->currentIndex()]->getSelectedChannelLabels().at(i).trimmed();
        signal->addData(data[i], label_temp);
        widget.comboBox->addItem(files[widget.file_comboBox->currentIndex()]->getSelectedChannelLabels().at(i).trimmed());
        widget.channelSelect->addItem(files[widget.file_comboBox->currentIndex()]->getSelectedChannelLabels().at(i).trimmed());
    }
    /*************************************************************************************************************************/
    hfo_markers.resize(data.count());
    //New class test
    signal->setAllToShow();
    
    widget.ref_comboBox->addItem("No reference");
    widget.ref_comboBox->addItems(files[widget.file_comboBox->currentIndex()]->getChannelLabels());
    //widget.ref_comboBox->setCurrentIndex(-1);
    
    disconnect(widget.ref_comboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(ReadReference()));
    connect(widget.ref_comboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(ReadReference()));
    
    //Filters received data
//    FIRfilter *filter = new FIRfilter();
//    
//    filter->threadlowpassFilter(signal);
//    
//    connect(filter, SIGNAL(EmitResult(QVector< QVector<double> >)),
//            this, SLOT(GetFilteredData(QVector< QVector<double> >)));
    
//    connect(widget.comboBox, SIGNAL(currentIndexChanged(int)),
//            this, SLOT(PlotGraphs(int)));
    
    disconnect(widget.comboBox, SIGNAL(activated(int)),
            this, SLOT(PlotGraphs(int)));
    connect(widget.comboBox, SIGNAL(activated(int)),
            this, SLOT(PlotGraphs(int)));
    
    if(isDebugging){
        printf("Got data. If valid, will call PlotGraphs(0).\n");
        fflush(stdout);
    }
    
    if(widget.comboBox->count()>0){
        PlotGraphs(0);
    }
}

void MainWindow::ReadReference(){
    int file_index = widget.file_comboBox->currentIndex();
    int index = widget.ref_comboBox->currentIndex();
    //int n = signal->getData(0).count();
    int n = files[file_index]->getNumberOfSamples();
    if(index-1>=0){
        readEDF *read = new readEDF;
        disconnect(read, SIGNAL(emitReference(QString, QVector< double >)),
                this, SLOT(setReference(QString, QVector< double >)));
        connect(read, SIGNAL(emitReference(QString, QVector< double >)),
                this, SLOT(setReference(QString, QVector< double >)));
        read->readReferenceOnly(index-1, files[file_index]->getPath(), n, files[file_index]->getStart());
    }else{
        QVector<double> temp;
        temp.clear();
        setReference("", temp);
    }
    if(isDebugging){
        printf("Reference defined\n");
        fflush(stdout);
    }
}

void MainWindow::setReference(QString label, QVector<double> l_reference){
    signal->setReference(l_reference, label);
    
    if(widget.comboBox->count()>0){
        PlotGraphs(0);
    }
}
/**
 * Unloads all signals, references, variables from files, etc
 */
//void MainWindow::unloadFile(){
//    for(int i=0; i<plotvector.count(); i++){
//        plotvector[i]->plot->detachItems(QwtPlotItem::Rtti_PlotItem, false);
//        plotvector[i]->plot->replot();
//        plotvector[i]->label->setText("");
//    }
//    
//    signal->clear();
////    delete signal;
//    
//    signal = new Signals();
//    
//    from = 0;
//    filepath.clear();
//    hfo_markers.clear();
//    widget.comboBox->clear();
//    
//    widget.channelSelect->clear();
//    
//    disconnect(widget.ref_comboBox, SIGNAL(currentIndexChanged(int)),
//            this, SLOT(ReadReference()));
//    widget.ref_comboBox->clear();
//    connect(widget.ref_comboBox, SIGNAL(currentIndexChanged(int)),
//            this, SLOT(ReadReference()));
//}

/*Plotting and navigating functions ********************************************************************************/
/**
 * Plots the selected data and next indexes on qwt_plots
 * @param index
 */
void MainWindow::PlotGraphs(int index){
    widget.comboBox->setCurrentIndex(index);
    
    //Gets one of the QRectF from qwt_plot
    QRectF rect = plotvector[0]->zoomer->zoomRect();
    
    //Tests if time scale is already defined
    int timeScale = (int)widget.lineEdit_timeScale->text().toDouble();
    
    if(timeScale>0)
        rect.setWidth(timeScale);
    else
        //Changes rect width to 10s (10s for 1kHz sampling frequency)
        rect.setWidth(10);
    
    //Zooms all the qwt_plots to the defined QRectF
//    ZoomGraphs(rect);
    ZoomGraphs(rect);
    
    if(isDebugging){
        printf("Graphs were plotted with ZoomGraphs(rect).\n");
        fflush(stdout);
    }
}

/**
 * Handles zooming for all qwt_plots
 * @param rect
 */
void MainWindow::ZoomGraphs(const QRectF& rect){
    if(widget.comboBox->count()==0){
        return;
    }
    
    if(isDebugging){
        printf("ZoomGraphs function started.... ");
        fflush(stdout);
    }
    
    int file_index = widget.file_comboBox->currentIndex();
    int index = widget.comboBox->currentIndex();
    
    GraphCurveConstruction *curvemaker = new GraphCurveConstruction;
    
    //Gets the position and length of the new selected data
    //Gets min and max x values for selected rectangle
    int sample_frequency = files[file_index]->getSampleFrequency();
//    printf("sample freq = %d\n", sample_frequency);
//    fflush(stdout);
    double x_min = (double) rect.bottomLeft().x()*sample_frequency;
    double x_max = (double) rect.bottomRight().x()*sample_frequency;
    
    //Corrects the bounds if selected rectangle would cause error due to
    //data ending or beggining - ALL SIGNALS MUST HAVE THE SAME LENGTH
    //int count = signal->getData(index).count();
    
    int count = files[file_index]->getNumberOfSamples();
    
    if(x_min<0){
        x_min = 0;
    }
    if(x_max>count){
        x_max=count;
    }
    if(x_max<0){
        return;
    }
    if(x_min>count){
        return;
    }
    int n = (int)((x_max)-(x_min));
    if(n>count){
        n = count;
    }

    for(int i=0; i<plotvector.count(); i++){
        plotvector[i]->plot->detachItems(QwtPlotItem::Rtti_PlotItem, false);
        plotvector[i]->plot->replot();
        
        disconnect(plotvector[i]->zoomer, SIGNAL(zoomed(const QRectF&)),
                this, SLOT(ZoomGraphs(const QRectF&)));

        plotvector[i]->zoomer->zoom(rect);

        connect(plotvector[i]->zoomer, SIGNAL(zoomed(const QRectF&)),
                this, SLOT(ZoomGraphs(const QRectF&)));
                
        plotvector[i]->label->setText("");
    }
    
    QVector< QFuture< QVector<double> > > futureArray_Filter;
    futureArray_Filter.resize(plotvector.count());
    
    QVector< QVector<double> > temp1;
    temp1.resize(plotvector.count());
    
    int filterOrder = 401;
    
    
    for(int i=0; i<plotvector.count(); i++){
        if(widget.comboBox->currentIndex() <= widget.comboBox->count()-1-i){
            //If filter is activated, filters the signal
            
            int temp_index = signal->getIndexToShow()[index+i];
            
            if(widget.filterCheckBox->isChecked() && widget.filterComboBox->currentIndex()>=0){
                double x_min1 = x_min;
                
                for(int j=0; j<(filterOrder-1)/2; j++){
                    if((x_min1 - 1)>=0){
                        x_min1 = x_min1 - 1;
                    }
                }
                
                double n1 = x_min-x_min1 + n;
                
                for(int j=0; j<(filterOrder-1)/2; j++){
                    //if((x_min1 + n1 + 1)<signal[markedIndexes[index+i]].count()){
                    //if((x_min1 + n1 + 1)<signal->getData(temp_index).count()){
                    if((x_min1 + n1 + 1)<files[file_index]->getNumberOfSamples()){
                        n1 = n1 + 1;
                    }
                }
                
                //Filters received data            
                temp1[i] = signal->getDataWithReference(temp_index, x_min1, n1);
                FIRfilter *filter = new FIRfilter();
                QVector<int> arguments;
                arguments.resize(5);
                arguments[0] = widget.f1_lineEdit->text().toInt();
                arguments[1] = widget.f2_lineEdit->text().toInt();
                arguments[2] = widget.filterComboBox->currentIndex();
                arguments[3] = (int)(x_min-x_min1);
                arguments[4] = (int)n;
                
                futureArray_Filter[i] = QtConcurrent::run(filter, &FIRfilter::simpleFilter, 
                        temp1[i], arguments, files[file_index]->getSampleFrequency());
                
            }else{
                //Declares temp vectors with the size of the zoomed rect
                temp1[i] = signal->getDataWithReference(temp_index, x_min, n);
            }
        }
    }
    
    QVector< QFuture< QwtPlotCurve* > > futureArray_Curve;
    QwtPlotCurve* curve;
    futureArray_Curve.resize(plotvector.count());
    
//    QVector< QwtPlotCurve* > curveArray;
//    curveArray.resize(plotvector.count());
            
    for(int i=0; i<plotvector.count(); i++){
        if(widget.comboBox->currentIndex() <= widget.comboBox->count()-1-i){
            if(widget.filterCheckBox->isChecked() && widget.filterComboBox->currentIndex()>=0){
                futureArray_Curve[i] = QtConcurrent::run(curvemaker, &GraphCurveConstruction::GraphCurve, futureArray_Filter[i].result(), (int)(double)plotvector[i]->plot->canvas()->size().width(),x_min, sample_frequency);
            }else{
                futureArray_Curve[i] = QtConcurrent::run(curvemaker, &GraphCurveConstruction::GraphCurve, temp1[i], (int)(double)plotvector[i]->plot->canvas()->size().width(),x_min, sample_frequency);
            }
        }
    }
    
    for(int i=0; i<plotvector.count(); i++){
        if(widget.comboBox->currentIndex() <= widget.comboBox->count()-1-i){
            futureArray_Curve[i].result()->attach(plotvector[i]->plot);

            if(widget.comboBox_amplitude->currentIndex()==0){
                plotvector[i]->plot->setAxisAutoScale(plotvector[i]->plot->yLeft, true);
                plotvector[i]->plot->replot();
                plotvector[i]->plot->setAxisAutoScale(plotvector[i]->plot->yLeft, false);
            }else{
                
//                plotvector[i]->zoomer->zoom(rect);
                
            }
            //Add markers for amplitude reference
            //Gets maximum Y vallue on respective qwt_plot
            double y_max =plotvector[i]->plot->axisInterval(plotvector[i]->plot->yLeft).maxValue();
            
            
            //Uses ScaleReference function to get a QwtPlotCurve and attaches it to
            //respective qwt_plot
//            curvemaker->ScaleReference(rect)->attach(plotvector[i]->plot);
            curvemaker->ScaleReference(y_max, x_min, rect, sample_frequency)->attach(plotvector[i]->plot);
            
            //Replots respective qwt_plot
            plotvector[i]->plot->replot();
            
            //Adds name of the channel to corresponding QLabel
            //plotvector[i]->label->setText(labels[markedIndexes[index+i]].trimmed()+" - "+reference_label);
            //plotvector[i]->label->setText(signal->getLabel(index+i)+" - "+signal->getReferenceLabel());
            plotvector[i]->label->setText(files[file_index]->getSelectedChannelLabels()[index+i].trimmed()+" - "+signal->getReferenceLabel());
        }
    }
    addMarkersToPlots();
    
//    if(isDebugging){
//        printf("Finished!\n");
//        fflush(stdout);
//    }
}

/**
 * Handles navigation backward through backward button for all qwt_plots
 */
void MainWindow::Xbackward(){
    //Gets min and max values for the current zoomed area
    QRectF rect = plotvector[0]->zoomer->zoomRect();
    double x_min = (double) rect.bottomLeft().x();
    double x_max = (double) rect.bottomRight().x();
    double width = rect.width();
    //Gets the size of current zoomed data
    double n = (double)((x_max)-(x_min));
    
    //Changes zoom rect on qwt_plots
    rect.setX(rect.x()-n/4);
    rect.setWidth(width);
    ZoomGraphs(rect);
}
/**
 * Handles navigation forward through forward button for all qwt_plots
 */
void MainWindow::Xforward(){
    //Gets min and max values for the current zoomed area
    QRectF rect = plotvector[0]->zoomer->zoomRect();
    double x_min = (double) rect.bottomLeft().x();
    double x_max = (double) rect.bottomRight().x();
    double width = rect.width();
    //Gets the size of current zoomed data
    double n = (double)((x_max)-(x_min));
    
    //Changes zoom rect on qwt_plots
    rect.setX(rect.x()+n/4);
    rect.setWidth(width);
    ZoomGraphs(rect);
}
/**
 * Handles direct time jumping
 */
void MainWindow::jumpToTime(){
    QRectF rect = plotvector[0]->zoomer->zoomRect();
    double width = rect.width();
    
    rect.setX(widget.jumpTo_lineEdit->text().toDouble());
    rect.setWidth(width);
    ZoomGraphs(rect);
}

/*Marker related functions *****************************************************************************************/
/**
 * Adds selected points to marker matrix and creates a qwt_plot_marker to
 * visually represent each of the markers through addMarkersToPlots()
 * @param pos
 */
void MainWindow::marker1(const QRectF &pos){
//    if(widget.comboBox->count()==0){
//        return;
//    }
//    
//    double X = pos.topLeft().x();
//    double Y = pos.topRight().x();
//    pointmarkers[widget.comboBox->currentIndex()].append(X);
//    pointmarkers[widget.comboBox->currentIndex()].append(Y);
//    addMarkersToPlots();
}
/**
 * Adds selected points to marker matrix and creates a qwt_plot_marker to
 * visually represent each of the markers through addMarkersToPlots()
 * @param pos
 * @param index
 */
void MainWindow::marker(const QPointF &pos, int index){
    if(widget.comboBox->currentIndex()>widget.comboBox->count()-1-index){
        return;
    }
    double X = pos.x();
    int temp_index = signal->getIndexToShow()[widget.comboBox->currentIndex()+index];
    if(widget.actionView_marked->isChecked()){
        hfo_markers[signal->getIndexToShow()[widget.comboBox->currentIndex()+index]].append(X);
    }
    else{
        signal->addMarker(temp_index, X);
    }
    addMarkersToPlots();
}
/**
 * Removes selected points from marker matrix and recalls ZoomGraphs(QRectF)
 * function so that only the currently existing markers are kept on the qwt_plots
 * @param pos
 * @param index
 */
void MainWindow::unmarker(const QPointF& pos, int index){
    if(widget.comboBox->currentIndex()>widget.comboBox->count()-1-index){
        return;
    }
//    int mappedIndex = markedIndexes[widget.comboBox->currentIndex()+index];
    int temp_index = signal->getIndexToShow()[widget.comboBox->currentIndex()+index];
    
    double X = pos.x();
    double check_region = (double)50/10000 * ((double)plotvector[0]->zoomer->zoomRect().width());

    signal->removeMarker(temp_index, X, check_region);
    
    QRectF rect = plotvector[0]->zoomer->zoomRect();
//    ZoomGraphs(rect);
    ZoomGraphs(rect);
}

/**
 * Creates a qwt_plot_marker to each selected point on markers matrix
 */
void MainWindow::addMarkersToPlots(){
    int index = widget.comboBox->currentIndex();
    for(int j=0; j<plotvector.count(); j++){
        if(widget.comboBox->currentIndex()<=widget.comboBox->count()-1-j){
            int temp_index = signal->getIndexToShow()[index+j];
            //Draws markers for pointmarkers vector
//            for(int i=0; i<pointmarkers[markedIndexes[index+j]].count(); i++){
            for(int i=0; i<signal->getMarkers(temp_index).count(); i++){
                QwtPlotMarker *marker_1 = new QwtPlotMarker();
                QwtSymbol *symb = new QwtSymbol();
                symb->setColor(Qt::red);
                symb->setStyle(QwtSymbol::VLine);
                symb->setSize(500);

                marker_1->setSymbol(symb);
//                double X = pointmarkers[markedIndexes[index+j]][i];
                double X = signal->getMarkers(temp_index)[i];
                marker_1->setXValue(X);
                marker_1->attach(plotvector[j]->plot);
                plotvector[j]->plot->replot();
            }
            //Draws markers for hfo_markers vector
            for(int i=0; i<hfo_markers[temp_index].count(); i++){
                QwtPlotMarker *marker_1 = new QwtPlotMarker();
                QwtSymbol *symb = new QwtSymbol();
                symb->setColor(Qt::green);
                symb->setStyle(QwtSymbol::VLine);
                symb->setSize(500);

                marker_1->setSymbol(symb);
                double X = hfo_markers[temp_index][i];
                marker_1->setXValue(X);
                marker_1->attach(plotvector[j]->plot);
                plotvector[j]->plot->replot();
            }
        }
    }
}

/**
 * Saves markers to a txt file
 */
void MainWindow::SaveMarkers(){
    writeMarkersText *writer = new writeMarkersText();
    QStringList labels;
    for(int i=0; i<widget.comboBox->count(); i++){
        labels<<(widget.comboBox->itemText(i));
    }
    
    QVector< QVector<double> > temp_point;
    //temp_point.resize(signal->count());
    temp_point.resize(files.count());
    for(int i=0; i<temp_point.count(); i++){
        temp_point[i]=(signal->getMarkers(i));
    }
    //Save file with pointmarkers vector
    writer->writeFile(temp_point, labels, signal->getReferenceLabel(), 0);
    //Save file with hfo_markers vector
//    writer->writeFile(hfo_markers, labels, reference_label, 1);
}

/**
 * Alters comboBox and displays only marked channels
 */
void MainWindow::accessMarkedOnly(){
    
//    markedIndexes.clear();
    
    for(int i=widget.comboBox->count()-1; i>=0; i--){
        widget.comboBox->removeItem(i);
    }
    
    if(widget.actionView_marked->isChecked()){
        signal->setMarkedToShow();
    }else{
        signal->setAllToShow();
    }
    
    for(int i=0; i<signal->getIndexToShow().count(); i++){
        widget.comboBox->addItem(signal->getLabel(signal->getIndexToShow()[i]));
    }
    
    QRectF rect = plotvector[0]->zoomer->zoomRect();
    ZoomGraphs(rect);
}

/*Filter related functions *****************************************************************************************/
/**
 * Changes the visibility of the line edits for the filter's cutoff frequencys
 * based on the chosen type
 * @param index
 */
void MainWindow::setCutOffFreqVisible(int index){
    if(index == 0 || index == 1){
        widget.f2_label->setVisible(false);
        widget.f2_lineEdit->setVisible(false);
    }else if(index == 2 || index == 3){
        widget.f2_label->setVisible(true);
        widget.f2_lineEdit->setVisible(true);
    }
}

/**
 * Filter signal using the parameters on the toolbox
 * @param isChecked
 */
void MainWindow::FilterActivated(){
    QRectF rect = plotvector[0]->zoomer->zoomRect();
//    ZoomGraphs(rect);
    ZoomGraphs(rect);
}

/*Rescale functions ************************************************************************************************/
/**
 * Changes the time scale depending on the chosen pre-set (1s or 10s)
 * @param index
 */
void MainWindow::changeTimeScale(int index){
    widget.lineEdit_timeScale->setText(QString::number(index));
    
    QRectF rect = plotvector[0]->zoomer->zoomRect();
    rect.setWidth(index);
    
    ZoomGraphs(rect);
}
/**
 * Changes the time scale depending on the chosen value (any > 0);
 */
void MainWindow::editTimeScale(){
    double scale = widget.lineEdit_timeScale->text().toDouble();
    widget.lineEdit_timeScale->setText(QString::number(scale));
    if(scale == 0)
        return;
    
    QRectF rect = plotvector[0]->zoomer->zoomRect();
    rect.setWidth(scale);
    
    ZoomGraphs(rect);
}

/**
 * Changes the amplityde scale of all plots based on chosen value
 * @param scale
 */
void MainWindow::amplitudeRescale(int scale){
//    scale = widget.comboBox_amplitude->currentIndex();
    QRectF rect = plotvector[0]->zoomer->zoomRect();
    
    rect.setHeight(1);
    rect.setY(1); 

    int scaleFactor;

    if(scale == 1)
        scaleFactor=1000;
    else if(scale == 2)
        scaleFactor=750;
    else if(scale == 3)
        scaleFactor=500;
    else if(scale == 4)
        scaleFactor=400;
    else if(scale == 5)
        scaleFactor=300;
    else if(scale == 6)
        scaleFactor=200;
    else if(scale == 7)
        scaleFactor=150;
    else if(scale == 8)
        scaleFactor=100;
    else if(scale == 9)
        scaleFactor=80;
    else if(scale == 10)
        scaleFactor=50;
    else if(scale == 11)
        scaleFactor=20;
    else if(scale == 12)
        scaleFactor=10;
    else if(scale == 13)
        scaleFactor=5;
    
    widget.lineEdit_ampScale->setText(QString::number((double)scaleFactor/4.0));
    
    rect.setHeight(scaleFactor);
    rect.setY(-scaleFactor/2);
    
    ZoomGraphs(rect);
}
/**
 * Changes the amplityde scale of all plots based on edited value
 */
void MainWindow::editAmplitudeScale(){
    QRectF rect = plotvector[0]->zoomer->zoomRect();
    rect.setHeight(1);
    rect.setY(1); 
    
    double scaleFactor = (widget.lineEdit_ampScale->text().toDouble());
    widget.lineEdit_ampScale->setText(QString::number(scaleFactor));
    
    rect.setHeight(scaleFactor*4);
    rect.setY(-scaleFactor/2*4);
    
    ZoomGraphs(rect);
}

/*Plugin functions *************************************************************************************************/
/**
 * Adds all the identified methods on .dll archives to the method repository of the software
 */
void MainWindow::findPlugins(){
    //Load static plugins
//    foreach(QObject *plugin, QPluginLoader::staticInstances()){
        //Adds a checkbox and page to the stackedwidget for analysis parameters
//        addMethods(plugin);
//    }
    //Declares the directory for plugin search
    QDir pluginsDir = QDir(qApp->applicationDirPath());

    const QString fileName;

    #if defined(Q_OS_WIN)
        if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
            pluginsDir.cdUp();
    #elif defined(Q_OS_MAC)
        if (pluginsDir.dirName() == "MacOS") {
            pluginsDir.cdUp();
            pluginsDir.cdUp();
            pluginsDir.cdUp();
        }
    #endif
    //Navigates to plugins directory - may change
    pluginsDir.cdUp();
    pluginsDir.cdUp();
    pluginsDir.cdUp();
    pluginsDir.cd("plugins");
    pluginsDir.cd("methods");

    //widget.log->append("Searching plugins in: " + pluginsDir.absolutePath());

    //Check each file. If it's a plugin, calls addMethods, and set absolute path
    //on methodsDir
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        
        if(isDebugging){
            printf("Checking: "+fileName.toAscii()+"... ");
            fflush(stdout);
        }
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        //widget.log->append("plugin located: "+fileName);
        QObject *plugin = loader.instance();
        if (plugin) {
            //If plugin is not null, adds a checkbox and page on stackedwidget
            //for analysis parameters
//             addMethods(plugin);
//             methodsDir.append(pluginsDir.absoluteFilePath(fileName));
            AnalysisMethodInterface *method = qobject_cast<AnalysisMethodInterface*>(plugin);
            methods.append( method );

            widget.methodParameters->addWidget(method->parameters());
            widget.methodParameters->setCurrentIndex(-1);
            widget.methodSelect->addItem(method->name().at(0));
            if(isDebugging){
                printf("plugin added: "+method->name().at(0).toAscii()+" ("+method->description().toAscii()+")"+"\n");
                fflush(stdout);
            }

         }else{
            if(isDebugging){
              printf("not a plugin\n");
              fflush(stdout);
            }
         }
    }

    //search for plugins parting from same directory as .exe:
    pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cd("plugins");
    pluginsDir.cd("methods");

    //Check each file. If it's a plugin, calls addMethods, and set absolute path
    //on methodsDir
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        printf("Checking: "+fileName.toAscii()+"... ");
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin) {
            //If plugin is not null, adds a checkbox and page on stackedwidget
            //for analysis parameters
   //             addMethods(plugin);
   //             methodsDir.append(pluginsDir.absoluteFilePath(fileName));
            AnalysisMethodInterface *method = qobject_cast<AnalysisMethodInterface*>(plugin);
            methods.append( method );

            widget.methodParameters->addWidget(method->parameters());
            widget.methodParameters->setCurrentIndex(-1);
            widget.methodSelect->addItem(method->name().at(0));
            if(isDebugging){
               printf("plugin added: "+method->name().at(0).toAscii()+" ("+method->description().toAscii()+")"+"\n");
               fflush(stdout);
            }

         }else{
            if(isDebugging){
               printf("not a plugin\n");
               fflush(stdout);
            }

         }
    }
}
/**
 * Changes the parameter page on stack widget to show current method parameters
 * @param index
 */
void MainWindow::changeParametersPage(){
    widget.methodParameters->setCurrentIndex(widget.methodSelect->currentRow());
}

/*Analysis functions ***********************************************************************************************/
void MainWindow::runAnalysis(){
    if(isDebugging){
        printf("Analysis started.\n");
        fflush(stdout);
    }
    if(widget.methodSelect->selectedItems().count()==0)
        return;
    if(widget.channelSelect->selectedItems().count()==0)
        return;
    
    //Gets number of channels to be potentially processed
//    int number_of_ch = widget.channelSelect->selectedItems().count();
    int number_of_ch = widget.channelSelect->count();
    
    //Gets index of method being executed
    int method_index;
    for(int i=0; i<widget.methodSelect->count(); i++){
        if(widget.methodSelect->item(i)->isSelected())
            method_index = i;
    }
    if(isDebugging){
        printf("Got selected method index.\n");
        fflush(stdout);
    }
    //Creates QFuture array to store thread processing results
    QVector< QFuture< methodResult* > > futureArray_Methods;
    futureArray_Methods.resize(number_of_ch);
//    writeMarkersText *writer = new writeMarkersText();
    //Launches threads
    QVector<methodResult*> result;
    result.resize(number_of_ch);
    
    methodResults.clear();
    methodResults.resize(number_of_ch);
    for(int i=0; i<number_of_ch; i++){
        methodResults[i] = new QFutureWatcher< methodResult* >();
    }
    
    if(isDebugging){
        printf("Declared all arrays to be used on processing (future, watcher, etc).\n");
        printf("number of selected ch = %d.\n", number_of_ch);
        fflush(stdout);
    }
    
    int file_index = widget.file_comboBox->currentIndex();
    
    //Get directory where the results are going to be saved
    QString savingPath = QFileDialog::getExistingDirectory(this, tr("Chose saving directory"), "D:/Bibliotecas/Dropbox/Mestrado/testes", QFileDialog::ShowDirsOnly);
    //Defining auxiliary counter
    int index = 0;
//    int threads_active = 0;
    for(int i=0; i<widget.channelSelect->count(); i++){
        if(widget.channelSelect->item(i)->isSelected()){
            if(isDebugging){
                printf("Starting %d selected ch.\n", i);
                fflush(stdout);
            }
            
            //Creates the path in which the file will be saved, including file name
            //Name follows pattern: file name (folder) + / + channel index + channel label + method name 
            QString fileName = files[file_index]->getFileName();
            QString prefix;
            QString channelName = prefix.sprintf("%02d", i+1)+"-"+files[file_index]->getSelectedChannelLabels()[i].trimmed()+"_"
                            +methods[method_index]->name()[0];
            
            if(widget.useReferenceBox->isChecked()){
                if(isDebugging){
                    printf("Launching thread\n");
                    fflush(stdout);
                }
                
                //Sequential
//                methods[method_index]->analysis(signal->getDataWithReference(i, 0,signal->getData(i).count()), signal->getDataWithReference(i, 0,signal->getData(i).count()));
                //Parallel
                if(widget.filterCheckBox->isChecked()){
                    FIRfilter *filter = new FIRfilter();
                    QVector<int> arguments;
                    arguments.resize(5);
                    arguments[0] = widget.f1_lineEdit->text().toInt();
                    arguments[1] = widget.f2_lineEdit->text().toInt();
                    arguments[2] = widget.filterComboBox->currentIndex();
                    arguments[3] = (int)(0-0);
                    //arguments[4] = (int)signal->getData(i).count();
                    arguments[4] = files[file_index]->getNumberOfSamples();
                    
                    futureArray_Methods[i]=QtConcurrent::run(methods[method_index], &AnalysisMethodInterface::analysis, 
                            filter->simpleFilter(signal->getDataWithReference(i, 0,files[file_index]->getNumberOfSamples()), arguments, files[file_index]->getSampleFrequency()), 
                            i, fileName, channelName, savingPath);
//                    watcher[i]->setFuture(futureArray_Methods[i]);
                    methodResults[i]->setFuture(futureArray_Methods[i]);
                }else{
                    futureArray_Methods[i]=QtConcurrent::run(methods[method_index], &AnalysisMethodInterface::analysis, 
                            signal->getDataWithReference(i, 0,files[file_index]->getNumberOfSamples()), 
                            i, fileName, channelName, savingPath);
//                    watcher[i]->setFuture(futureArray_Methods[i]);
                    methodResults[i]->setFuture(futureArray_Methods[i]);
                }
                
                if(isDebugging){
                    printf("Thread %d launched.\n", i);
                    fflush(stdout);
                }
            }else{
                if(isDebugging){
                    printf("Launching thread\n");
                    fflush(stdout);
                }
                if(widget.filterCheckBox->isChecked()){
                    FIRfilter *filter = new FIRfilter();
                    QVector<int> arguments;
                    arguments.resize(5);
                    arguments[0] = widget.f1_lineEdit->text().toInt();
                    arguments[1] = widget.f2_lineEdit->text().toInt();
                    arguments[2] = widget.filterComboBox->currentIndex();
                    arguments[3] = (int)(0-0);
                    //arguments[4] = (int)signal->getData(i).count();
                    arguments[4] = files[file_index]->getNumberOfSamples();
                    
//                    disconnect(methods[method_index], SIGNAL(EmitResult(methodResult*)),
//                            this, SLOT(writeResult(methodResult*)));
//                    connect(methods[method_index], SIGNAL(EmitResult(methodResult*)),
//                            this, SLOT(writeResult(methodResult*)));
                    
                    
                    futureArray_Methods[i]=QtConcurrent::run(methods[method_index], &AnalysisMethodInterface::analysis, 
                            filter->simpleFilter(signal->getDataWithReference(i, 0,files[file_index]->getNumberOfSamples()), arguments, files[file_index]->getSampleFrequency()), 
                            i, fileName, channelName, savingPath);
//                    watcher[i]->setFuture(futureArray_Methods[i]);
                    methodResults[i]->setFuture(futureArray_Methods[i]);
                }else{
                    futureArray_Methods[i]=QtConcurrent::run(methods[method_index], &AnalysisMethodInterface::analysis, 
                            signal->getData(i), 
                            i, fileName, channelName, savingPath);
//                    watcher[i]->setFuture(futureArray_Methods[i]);
                    methodResults[i]->setFuture(futureArray_Methods[i]);
                }

                if(isDebugging){
                    printf("Thread %d launched.\n", i);
                    fflush(stdout);
                }
            }
        }
    }
    //Connects threads to writeResults method
    QSignalMapper *threadMapper = new QSignalMapper(this);
    connect(threadMapper, SIGNAL(mapped(int)),
            this, SLOT(writeResults(int)));
    for(int i=0; i<number_of_ch; i++){
        threadMapper->setMapping(methodResults[i], i);
        disconnect(methodResults[i], SIGNAL(finished()),
                threadMapper, SLOT(map()));
        connect(methodResults[i], SIGNAL(finished()),
                threadMapper, SLOT(map()));
    }
    
    if(isDebugging){
        printf("All threads launched.\n");
        fflush(stdout);
    }
}

void MainWindow::writeResults(int resultIndex){
    methodResult* result = methodResults[resultIndex]->result();
    
    methodResults[resultIndex]->deleteLater();
      
    
    writeMarkersText *writer = new writeMarkersText();
//    
    writer->writeMethodResults(result, result->getSavingPath(), result->getDirName(), result->getResultName());
    if(isDebugging){
        printf("File created - %d\n", resultIndex);
        fflush(stdout);
    }
}
/**
 * This method handles the same analysis method running on various files
 */
//void MainWindow::runMoreFiles(){
//    if(signal->count()>0){
//        unloadFile();
//    }
//    QStringList fileNames;
//    fileNames = QFileDialog::getOpenFileNames(this, "Select files", "D:/Bibliotecas/Dropbox/Mestrado/CBFM 2013/Testes DFA/CHB-MIT Scalp EEG/chb01",
//            trUtf8("EDF Files (*.edf)"));
//    
//    QVector<QString> savingPath;
//    savingPath.resize(fileNames.count());
//    
//    int j=0;
//    for(int i=0; i<fileNames.count(); i++){
//        //Crop only file names
//        QFileInfo temp(fileNames[i]);
//        QString temp_str = temp.fileName();
//        temp_str.remove("_");
//        temp_str.remove(".edf");
////        savingPath[i]="D:/Bibliotecas/Dropbox/Mestrado/Dados/DFA_5k_seizuresonly/"+temp_str;
////        if(!QDir(savingPath[i]).exists())
////            QDir("D:/Bibliotecas/Dropbox/Mestrado/Dados/DFA_5k_seizuresonly").mkdir(temp_str);
//        
//        savingPath[i]="/home/fabricio/Dropbox/Mestrado/Dados/HFO_5k_seizuresonly_new/"+temp_str;
//        if(!QDir(savingPath[i]).exists())
//            QDir("/home/fabricio/Dropbox/Mestrado/Dados/HFO_5k_seizuresonly_new").mkdir(temp_str);
//        
//    }
//    
//    for(int i=0; i<fileNames.count(); i++){
//
////        ReadEDF(fileNames[i]);
//        read->hide();
//
//        for(int j=0; j<22; j++){
//            read->widget.chList->item(j)->setSelected(true);
//        }
//        
//        read->readChannels();
//        
//        QEventLoop loop;
//        
//        connect(this, SIGNAL(gotData()),
//                &loop, SLOT(quit()));
//        
//        loop.exec();
//        
//        widget.channelSelect->selectAll();
//        runAnalysis(savingPath[i]);
//        unloadFile();
//    }
//}

/*Other tools ******************************************************************************************************/
/**
 * Shows a new window (instance of freqAnalysis class), capable of detecting
 * peaks and estimating the frequency of the main oscilation
 * @param pos1
 * @param pos2
 * @param index
 */
void MainWindow::showDetails(const QPointF &pos1, const QPointF &pos2, int index){
    if(widget.comboBox->currentIndex()>widget.comboBox->count()-1-index){
        return;
    }
    
    freqAnalysis *freq_analysis = new freqAnalysis();
    
    freq_analysis->show();
    
    int file_index = widget.file_comboBox->currentIndex();
    
    //freq_analysis->set_sampleFrequency(signal->getSampleFrequency());
    freq_analysis->set_sampleFrequency(files[file_index]->getSampleFrequency());
    
//    double X = pos1.x();
    //int sample_frequency = signal->getSampleFrequency();
    int sample_frequency = files[file_index]->getSampleFrequency();
    int x_min = pos1.x()*sample_frequency;
    int x_max = pos2.x()*sample_frequency;
    
    //int count = signal->getData(index).count();
    int count = files[file_index]->getNumberOfSamples();
    
    int temp_index = signal->getIndexToShow()[widget.comboBox->currentIndex()+index];
    
    if(widget.filterCheckBox->isChecked() && widget.filterComboBox->currentIndex()>=0){
        if(x_min<0){
        x_min = 0;
        }
        if(x_max>count){
            x_max=count;
        }
        if(x_max<0){
            return;
        }
        if(x_min>count){
            return;
        }
        int n = (int)((x_max)-(x_min));
        if(n>count){
            n = count;
        }

        double x_min1 = x_min;

        int filterOrder = 401;

        for(int j=0; j<(filterOrder-1)/2; j++){
            if((x_min1 - 1)>=0){
                x_min1 = x_min1 - 1;
            }
        }

        double n1 = x_min-x_min1 + n;

        for(int j=0; j<(filterOrder-1)/2; j++){
            //if((x_min1 + n1 + 1)<signal->getData(temp_index).count()){
            if((x_min1 + n1 + 1)<files[file_index]->getNumberOfSamples()){
                n1 = n1 + 1;
            }
        }
        //Filters received data
    //    QVector<double> temp1;
    //    temp1 = signal[widget.comboBox->currentIndex() + index].mid(x_min1, n1);
        QVector<int> arguments;
        arguments.resize(5);
        arguments[0] = widget.f1_lineEdit->text().toInt();
        arguments[1] = widget.f2_lineEdit->text().toInt();
        arguments[2] = widget.filterComboBox->currentIndex();
        arguments[3] = (int)(x_min-x_min1);
        arguments[4] = (int)n;
        QVector<double> temp1 = signal->getDataWithReference(temp_index, x_min1, n1);
        FIRfilter *filter = new FIRfilter();
        freq_analysis->plotData(filter->simpleFilter(temp1, 
                //arguments, signal->getSampleFrequency()));
                arguments, files[file_index]->getSampleFrequency()));
    }else{
        QVector<double> temp1 = signal->getDataWithReference(temp_index, pos1.x()*sample_frequency, (pos2.x()-pos1.x())*sample_frequency);
        freq_analysis->plotData(temp1);
    }
}

/**
 * EEG to EDF conversion
 */
void MainWindow::EEGtoEDF(){
    convertEEGtoEDF *convert = new convertEEGtoEDF();
    convert->EEGtoEDF();
}

void MainWindow::toAscii(){
    QDir path = signal->getFilePath();
    path.cdUp();
    path.mkdir("ascii");
    path.cd("ascii");
    for(int i=0; i<signal->count(); i++){
        QFile file(path.absolutePath()+"/"+signal->getLabel(i));
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        QVector<double> temp_data = signal->getDataWithReference(i, 0,signal->getData(i).count());
        for(int j=0; j<temp_data.count(); j++){
            out << QString::number(temp_data[j]).toAscii() << "\n";
        }
    }
}

/**
 * This method creates a new window from inside this window
 */
void MainWindow::newWindow(){
    MainWindow *new_window = new MainWindow;
    new_window->showMaximized();
    
    //If there are signals loaded, pass the signals to the new window
    if(signal->count()>0){
        //Atributes same global variables
        new_window->signal = this->signal;
        new_window->filepath = this->filepath;
        new_window->from = this->from;
        new_window->methods = this->methods;

        QVector< QVector<double> > data;
        data.resize(widget.comboBox->count());
        QStringList labels;
        for(int i=0; i<widget.comboBox->count(); i++){
            labels<<widget.comboBox->itemText(i);
            data[i]=signal->getData(i);
        }
        QStringList ref_labels;
        for(int i=1; i<widget.ref_comboBox->count(); i++){
            ref_labels<<widget.ref_comboBox->itemText(i);
        }

        //new_window->GetData(data, labels, ref_labels, data[0]);
    }
}

/*Mouse listener selection *****************************************************************************************/
/**
 * Enables zoomers and disable pickers when zoom function is selected though
 * zoom button
 */
void MainWindow::enablesZoomer(){
    for(int i =0; i<plotvector.count(); i++){
        plotvector[i]->enableZoomer();
    }
}
/**
 * Enables pickers and disable zoomers when marker function is selected though
 * marker button
 */
void MainWindow::enablesPointPicker(){
    for(int i=0; i<plotvector.count(); i++){
        plotvector[i]->enablePickersPointSelection();
    }
}

void MainWindow::enablesRectPicker(){
    for(int i=0; i<plotvector.count(); i++){
        plotvector[i]->enablePickersRectSelection();
    }
}

/*General functions ************************************************************************************************/
/**
 * Changes the main window page
 * @param index
 */
void MainWindow::changePage(int index){
    widget.stackedWidget->setCurrentIndex(index);
}

/**
 * Handles key press events for plots navigation.
 * When left arrow key is pressed, calls Xbackward(), and when right key is
 * pressed, calls Xforward()
 * @param event
 */
void MainWindow::keyPressEvent(QKeyEvent* event){
    if(event->key() == Qt::Key_Left){
        Xbackward();
    }
    if(event->key() == Qt::Key_Right){
        Xforward();
    }
}