/*
 * File:   PearsonCorrelationCalculator.cpp
 * Author: eliana
 *
 * Created on May 2, 2013, 8:33 AM
 */

#include <math.h>
#include "PearsonCorrelationCalculator.h"

PearsonCorrelationCalculator::PearsonCorrelationCalculator( int subTraceSize ) :
        CorrelationCalculator( subTraceSize )
{
}

PearsonCorrelationCalculator::~PearsonCorrelationCalculator()
{
}

float PearsonCorrelationCalculator::computeCorrelation( float* x, float* y )
{
    double firstAcc = 0, secoundAcc = 0, firstFirstAcc = 0, firstSecoundAcc = 0,
            secoundSecoundAcc = 0;

    // Percorre todo o sub-tra�o definido.
    for (int i = 0; i < _subTraceSize; i++)
    {
        // Verifica se a amostra � valida antes de ler do tra�o.
        float firstValue = x[i];
        float secoundValue = y[i];

        // Acumula o resultado desta amostra
        firstAcc += firstValue;
        secoundAcc += secoundValue;
        firstFirstAcc += firstValue * firstValue;
        firstSecoundAcc += firstValue * secoundValue;
        secoundSecoundAcc += secoundValue * secoundValue;
    }

    // Normaliza os acumuladores
    firstAcc /= _subTraceSize;
    secoundAcc /= _subTraceSize;
    firstFirstAcc /= _subTraceSize;
    firstSecoundAcc /= _subTraceSize;
    secoundSecoundAcc /= _subTraceSize;

    // Computa os dois divisores da equa��o
    double firstDiv = firstFirstAcc - firstAcc * firstAcc;
    double secoundDiv = secoundSecoundAcc - secoundAcc * secoundAcc;

    // Como vamos dividir pelo produto destes divisores, garante que ambos s�o diferentes de 0.
    if (firstDiv < 1.0e-36 || secoundDiv < 1.0e-36) return -1.0;

    // Caso seja "seguro" computa a correla��o
    return (float) ((firstSecoundAcc - firstAcc * secoundAcc)
            / (sqrt( firstDiv ) * sqrt( secoundDiv )));
}

