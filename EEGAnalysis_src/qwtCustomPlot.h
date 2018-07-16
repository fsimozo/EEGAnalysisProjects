/* 
 * File:   qwtCustomPlot.h
 * Author: Fabricio
 *
 * Created on 3 de Abril de 2013, 12:34
 */

#ifndef QWTCUSTOMPLOT_H
#define	QWTCUSTOMPLOT_H

#include <QWidget>
#include <QLabel>

#include "qwt_plot.h"
#include "qwt_plot_zoomer.h"
#include "qwt_plot_picker.h"

class qwtCustomPlot : public QWidget {
    Q_OBJECT
public:
    qwtCustomPlot();
    qwtCustomPlot(const qwtCustomPlot& orig);
    virtual ~qwtCustomPlot();
    void setPlotIndex(int index);
public slots:
    void enablePickersPointSelection();
    void enablePickersRectSelection();
    void enableZoomer();
    void emitUnpickerSignal(const QPointF &pos);
    void emitPickerSignal(const QPointF &pos);
    void emitPickerRectSignal(const QRectF &rect);
signals:
    void unpicker_signal(const QPointF &pos, int index);
    void picker_signal(const QPointF &pos, int index);
    void picker_rect_signal(const QPointF &pos1, const QPointF &pos2, int index);
private:
    int plot_index;

    
public:
    QwtPlot *plot;
    QwtPlotZoomer *zoomer;
    QwtPlotPicker *picker;
    QwtPlotPicker *unpicker;
    QLabel *label;
};

#endif	/* QWTCUSTOMPLOT_H */

