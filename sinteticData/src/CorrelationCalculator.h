/*
 * File:   CorrelationCalculator.h
 * Author: eliana
 *
 * Created on May 2, 2013, 8:25 AM
 */

#ifndef CORRELATION_CALCULATOR_H
#define	CORRELATION_CALCULATOR_H

class CorrelationCalculator
{
public:

    virtual float computeCorrelation( float* x, float* y ) = 0;

    virtual ~CorrelationCalculator();

protected:

    CorrelationCalculator( int subTraceSize );

    int _subTraceSize;
};

#endif	/* CORRELATIONCALCULATOR_H */
