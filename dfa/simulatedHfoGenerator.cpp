/* 
 * File:   simulatedHfoGenerator.cpp
 * Author: Fabricio
 * 
 * Created on August 5, 2013, 11:43 AM
 */

#include "simulatedHfoGenerator.h"

#include <stdio.h>
#include <time.h>
#include <math.h>

//#include "fftw3.h"

#include <QVector>

#include <QtGlobal>
#include <QTime>


simulatedHfoGenerator::simulatedHfoGenerator() {
    seed_multiplier = 1;
}

simulatedHfoGenerator::simulatedHfoGenerator(const simulatedHfoGenerator& orig) {
}

simulatedHfoGenerator::~simulatedHfoGenerator() {
}

QVector<double> simulatedHfoGenerator::generateSignal(int size, int number_of_oscilations, int noise_amplitude){
    
    QVector<double> signal;
    signal.resize(size);
    
    double pi = atan(1)*4;
    
    //Generate low frequency components
    for(int i=0; i<size; i++)
        signal[i]=100*sin(2*pi*10*0.001*i)+100*sin(2*pi*20*0.001*i) 
                    + 100*sin(2*pi*30*0.001*i)+100*sin(2*pi*40*0.001*i);
    
    //Generate high frequency components
    number_of_oscilations = 30;
    int duration = 50;
    int interval = size / (number_of_oscilations+1);
    
    for(int i=0; i<number_of_oscilations; i++){
        int freq = 80+i*2;
        for(int j=0; j<duration; j++){
            signal[interval*i+j]+=10*sin(2*pi*freq*0.001);
        }
    }
    
    //Generate noise
    QVector<double> noise = whiteNoise(size);
    for(int i=0; i<size; i++){
        signal[i] += noise_amplitude*noise[i];
    }
    
    return signal;
}

QVector<double> simulatedHfoGenerator::whiteNoise(int size){
    //sets seed
    srand ((QTime::currentTime().msec()+QTime::currentTime().second()*(QTime::currentTime().hour()+QTime::currentTime().minute()))*seed_multiplier++);
    
    QVector<double> noise;
    noise.resize(size);
    
    int zero_count=size;
    int equal_count=size;
    double mean = 0;
    
//    while(zero_count == size || equal_count == size){
    for(int i=0; i<size; i++){
//        noise[i]=(double)(qrand()/(RAND_MAX)*2.0);
        noise[i]=(double)(rand()/((double)RAND_MAX)*2.0);
        mean+=noise[i]/size;
    }
    for(int i=0; i<size; i++){
        noise[i] = noise[i] - mean;
    }
//        zero_count=0;
//        equal_count=0;
//        for(int i=0; i<size; i++){
//            noise[i]=noise[i]-mean;
//            if(noise[i]==0){
//                zero_count++;
//            }
//            if(i>0){
//                if(noise[i]==noise[i-1]){
//                    equal_count++;
//                }
//            }
//        }
//        printf("equal count = %d\n", equal_count);
//    }
    
    return noise;
}
QVector<double> simulatedHfoGenerator::gaussianWhiteNoise(int size){
    QVector<double> noise;
    noise.resize(size);
    
    double pi = atan(1)*4;
    
    for(int i=0; i<size; i++){
        double r1 = (double)rand()/(RAND_MAX);
        double r2 = (double)rand()/(RAND_MAX);
                        
        noise[i] = sqrt(-2*log(r1))*sin(2*pi*r2);
    }
       
    return noise;
}
//QVector< QVector<double> > simulatedHfoGenerator::pinkNoise(int size, double alpha, int n_noises){
//    QVector< QVector<double> > noise;
//    noise.resize(n_noises);
////    noise.resize(size);
//    
//    double* real = (double*) fftw_malloc(sizeof(double)*size);
//    fftw_complex* complex = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*size/2+1);
//    
//    fftw_plan p = fftw_plan_dft_r2c_1d(size, real, complex, FFTW_ESTIMATE);
//    
//    fftw_plan p1 = fftw_plan_dft_c2r_1d(size, complex, real, FFTW_ESTIMATE);
//    
//    //Generate white noise
//    for(int i=0; i<n_noises; i++){
//        noise[i] = whiteNoise(size);
//        for(int j=0; j<size; j++){
//            real[j]=noise[i][j];
//        }
//
//        //Fourier transform of the noise
//        fftw_execute(p);
//
//        //Manipulate complex output
//        for(int j=0; j<size/2+1; j++){
//            complex[j][0] = complex[j][0]/pow(1+j, alpha);
//            complex[j][1] = complex[j][1]/pow(1+j, alpha);
//        }
//
//        //Inverse transform to get noise
//        //Fourier transform of the noise
//        fftw_execute(p1);
//
//        for(int j=0; j<size; j++){
//            noise[i][j] = real[j]/size;
//        }
//    }
//    
//    fftw_free(real);
//    fftw_free(complex);
//    
//    fftw_destroy_plan(p);
//    fftw_destroy_plan(p1);
//    
//    fftw_cleanup();
//    
//    return noise;
//}

QVector<double> simulatedHfoGenerator::brownNoise(int size){
    QVector<double> temp;
    
    QVector<double> noise;
    noise.resize(size);
    
    //Generate white noise
    temp = whiteNoise(size);
    
    //Integrate white noise
    double mean = 0;
    for(int i=0; i<size; i++){
        mean += temp[i]/size;
    }
    
    noise[0] = temp[0] - mean;
    for(int i=1; i<size; i++){
        noise[i]=noise[i-1]+temp[i] - mean;
    }
    
    return noise;
}