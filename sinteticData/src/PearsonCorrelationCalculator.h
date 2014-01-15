/*
 * File:   PearsonCorrelationCalculator.h
 * Author: eliana
 *
 * Created on May 2, 2013, 8:33 AM
 */

#ifndef PEARSON_CORRELATION_CALCULATOR_H
#define	PEARSON_CORRELATION_CALCULATOR_H

#include "CorrelationCalculator.h"

class PearsonCorrelationCalculator: public CorrelationCalculator
{
public:

    PearsonCorrelationCalculator( int subTraceSize );

    virtual ~PearsonCorrelationCalculator();

    float computeCorrelation( float* x, float* y );

private:

};

#endif	/* PEARSON_CORRELATION_CALCULATOR_H */
