/* 
 * File:   qwtCustomPlot.cpp
 * Author: Fabricio
 * 
 * Created on 3 de Abril de 2013, 12:34
 */

#include "qwtCustomPlot.h"
#include "qwt_plot.h"
#include "qwt_plot_zoomer.h"
#include "qwt_plot_picker.h"
#include "qwt_picker_machine.h"
#include "qwt_plot.h"
#include "qwt_picker.h"
#include <QVBoxLayout>
#include <QLabel>

/**
 * Creates a Widget containing a qwt_plot, two pickers and a zoomer
 */
qwtCustomPlot::qwtCustomPlot() : QWidget() {
    //Creates an instance of QwtPlot
    plot = new QwtPlot();
    
    //Disables left Y axis
    plot->enableAxis(plot->yLeft, false);
    
    //Creates instances for the pickers and the zoomer on QwtPlot canvas
    zoomer = new QwtPlotZoomer(plot->canvas());
    picker = new QwtPlotPicker(plot->canvas());
    unpicker = new QwtPlotPicker(plot->canvas());
    
    label = new QLabel();
    
    //Sets parameters on picker and unpicker
    picker->setTrackerMode(QwtPicker::ActiveOnly);
    picker->setStateMachine(new QwtPickerDragPointMachine);
    picker->setRubberBandPen(QColor(Qt::blue));
    picker->setRubberBand(QwtPicker::VLineRubberBand);
    picker->setMousePattern(QwtPicker::MouseSelect1, Qt::LeftButton);
    
    unpicker->setTrackerMode(QwtPicker::ActiveOnly);
    unpicker->setStateMachine(new QwtPickerDragPointMachine);
    unpicker->setRubberBandPen(QColor(Qt::yellow));
    unpicker->setRubberBand(QwtPicker::VLineRubberBand);
    unpicker->setMousePattern(QwtPicker::MouseSelect1, Qt::RightButton);
    
    //Sets key patterns on zoomer and pickers so it doesn't change cursor when 
    //left and right keys are pressed
    zoomer->setMousePattern( QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern( QwtEventPattern::MouseSelect3,Qt::RightButton );
    
    zoomer->setKeyPattern(QwtEventPattern::KeyLeft, 0);
    zoomer->setKeyPattern(QwtEventPattern::KeyRight, 0);
    picker->setKeyPattern(QwtEventPattern::KeyLeft, 0);
    picker->setKeyPattern(QwtEventPattern::KeyRight, 0);
    unpicker->setKeyPattern(QwtEventPattern::KeyLeft, 0);
    unpicker->setKeyPattern(QwtEventPattern::KeyRight, 0);
    
    //Disables picker and unpicker - Only zoomer will work initially
    picker->setEnabled(false);
    unpicker->setEnabled(false);
    
    //Defines the layout of the widget, and adds QwtPlot to the layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(label);
    layout->addWidget(plot);
    
    //Connects unpicker signal with unpicker signal emitting slot
    connect(unpicker, SIGNAL(selected(const QPointF)),
            this, SLOT(emitUnpickerSignal(const QPointF)));
    //Connects picker signal with picker signal emitting slot
    connect(picker, SIGNAL(selected(const QPointF)),
            this, SLOT(emitPickerSignal(const QPointF)));
    connect(picker, SIGNAL(selected(const QRectF)),
            this, SLOT(emitPickerRectSignal(const QRectF)));
}

qwtCustomPlot::qwtCustomPlot(const qwtCustomPlot& orig) {
}

qwtCustomPlot::~qwtCustomPlot() {
}

void qwtCustomPlot::enablePickersPointSelection(){
    picker->setEnabled(true);
    unpicker->setEnabled(true);
    zoomer->setEnabled(false);
    
    picker->setStateMachine(new QwtPickerDragPointMachine);
    picker->setRubberBand(QwtPicker::VLineRubberBand);
    
//    unpicker->setStateMachine(new QwtPickerDragPointMachine);
//    unpicker->setRubberBand(QwtPicker::VLineRubberBand);
}
void qwtCustomPlot::enablePickersRectSelection(){
    picker->setEnabled(true);
    unpicker->setEnabled(true);
    zoomer->setEnabled(false);
    
    picker->setStateMachine(new QwtPickerDragRectMachine);
    picker->setRubberBand(QwtPicker::RectRubberBand);
    
//    unpicker->setStateMachine(new QwtPickerDragRectMachine);
//    unpicker->setRubberBand(QwtPicker::RectRubberBand);
}
void qwtCustomPlot::enableZoomer(){
    picker->setEnabled(false);
    unpicker->setEnabled(false);
    zoomer->setEnabled(true);
}

void qwtCustomPlot::setPlotIndex(int index){
    plot_index = index;
}

void qwtCustomPlot::emitUnpickerSignal(const QPointF &pos){
    emit unpicker_signal(pos, plot_index);
}

void qwtCustomPlot::emitPickerSignal(const QPointF &pos){
    emit picker_signal(pos, plot_index);
}

void qwtCustomPlot::emitPickerRectSignal(const QRectF &rect){
    
    QPointF x1 = rect.bottomLeft();
    QPointF x2 = rect.bottomRight();
    
    emit picker_rect_signal(x1, x2, plot_index);
}