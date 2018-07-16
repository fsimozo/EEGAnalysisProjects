/* 
 * File:   simulatedHfoGenerator.h
 * Author: Fabricio
 *
 * Created on August 5, 2013, 11:43 AM
 */

#ifndef SIMULATEDHFOGENERATOR_H
#define	SIMULATEDHFOGENERATOR_H

#include <QObject>
#include <QVector>
//#include "fftw3.h"

class simulatedHfoGenerator : public QObject {
    Q_OBJECT;
    
public:
    simulatedHfoGenerator();
    simulatedHfoGenerator(const simulatedHfoGenerator& orig);
    virtual ~simulatedHfoGenerator();
    
public slots:
    //Signal generator
    QVector<double> generateSignal(int size, int number_of_oscilations, int noise_amplitude);
    
    //Noise generators
    QVector<double> whiteNoise(int size);
    QVector<double> gaussianWhiteNoise(int size);
//    QVector< QVector<double> > pinkNoise(int size, double alpha, int n_noises);
    QVector<double> brownNoise(int size);
    
private:
    int seed_multiplier;
};

#endif	/* SIMULATEDHFOGENERATOR_H */

