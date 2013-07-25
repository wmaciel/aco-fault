/*
 * File:   DirectionalField.cpp
 * Author: keoma
 *
 * Created on July 19, 2013, 6:41 PM
 */

#include "DirectionalField.h"

#include <math.h>
#include <stdio.h>

DirectionalField::DirectionalField( float* data, int width, int height, int window )
{
    int size = width * height;
    _width = width;
    _height = height;
    _coherence = new float[size];
    _directionX = new float[size];
    _directionY = new float[size];
    _gxMatrix = new float[size];
    _gyMatrix = new float[size];
    _gxxMatrix = new float[size];
    _gyyMatrix = new float[size];
    _gxyMatrix = new float[size];

    buildHorizontalDerivativeMatrix( data );
    buildHorizontalWindowedDerivativeMatrix( window );
    buildVerticalDerivativeMatrix( data );
    buildVerticalWindowedDerivativeMatrix( window );
    buildCrossedWindowedDerivativeMatrix( window );
    buildCoherenceMatrix();
    buildDirectionMatrices();
    normalizeDirections();
    normalizeCoherence();
}



DirectionalField::DirectionalField( const DirectionalField& orig )
{
}



DirectionalField::~DirectionalField()
{
}



int DirectionalField::pixel( int x, int y )
{
    return x + _width * y;
}



float DirectionalField::dx( int x, int y, float* data )
{
    int front  = pixel( x + 1, y );
    int back   = pixel( x - 1, y );
    int center = pixel( x, y );

    if ( x == _width-1 )// backward difference
    {
        return data[center] - data[back];
    }
    else if ( x == 0 )  // forward difference
    {
        return data[front] - data[center];
    }
    else                  // central difference
    {
        return ( data[front] - data[back] ) / 2.0f;
    }
}

float DirectionalField::dy( int x, int y, float* data )
{
    int top    = pixel( x, y + 1  );
    int bottom = pixel( x, y - 1  );
    int center = pixel( x, y );

    if ( y == _height-1 ) // backward difference
    {
        return data[center] - data[bottom];
    }
    else if ( y == 0 )// forward difference
    {
        return data[top] - data[center];
    }
    else                  // central difference
    {
        return ( data[top] - data[bottom] ) / 2.0f;
    }
}

float DirectionalField::coherence( int pixel )
{
    float gxxMinusGyy = _gxxMatrix[pixel] - _gyyMatrix[pixel];
    float hxy = gxxMinusGyy * gxxMinusGyy + 4 * _gxyMatrix[pixel] * _gxyMatrix[pixel];
    
    float divisor = ( _gxxMatrix[pixel] + _gyyMatrix[pixel] );
    if (divisor == 0.0f)
    {
        return 0.0f;
    }
    
    return sqrt( hxy ) / divisor;
}

void DirectionalField::direction( int pixel, float& dirX, float& dirY )
{
    float gxxMinusGyy = _gxxMatrix[pixel] - _gyyMatrix[pixel];
    float hxy = gxxMinusGyy * gxxMinusGyy + 4 * _gxyMatrix[pixel] * _gxyMatrix[pixel];

    dirX = 0.5 * ( _gxxMatrix[pixel] - _gyyMatrix[pixel] - sqrt( hxy ) );
    dirY = _gxyMatrix[pixel];
}

void DirectionalField::buildHorizontalDerivativeMatrix( float* data )
{
    for (int x = 0; x < _width; ++x)
    {
        for (int y = 0; y < _height; ++y)
        {
            int i = pixel(x, y);
            _gxMatrix[i] = dx( x, y, data );
        }
    }
}



void DirectionalField::buildVerticalDerivativeMatrix( float* data )
{
    for (int x = 0; x < _width; ++x)
    {
        for (int y = 0; y < _height; ++y)
        {
            int i = pixel(x, y);
            _gyMatrix[i] = dy( x, y, data );
        }
    }
}



void DirectionalField::buildHorizontalWindowedDerivativeMatrix( int windowSize )
{
    for (int x = 0; x < _width; ++x)
    {
        for (int y = 0; y < _height; ++y)
        {
            _gxxMatrix[pixel(x,y)] = getHorizontalWindowedDerivative( x, y, windowSize );
        }
    }
}



void DirectionalField::buildVerticalWindowedDerivativeMatrix( int windowSize )
{
    for (int x = 0; x < _width; ++x)
    {
        for (int y = 0; y < _height; ++y)
        {
            _gyyMatrix[pixel(x,y)] = getVerticalWindowedDerivative( x, y, windowSize );
        }
    }
}



void DirectionalField::buildCrossedWindowedDerivativeMatrix( int windowSize )
{
    for (int x = 0; x < _width; ++x)
    {
        for (int y = 0; y < _height; ++y)
        {
            _gxyMatrix[pixel(x,y)] = getCrossedWindowedDerivative( x, y, windowSize );
        }
    }
}



void DirectionalField::buildCoherenceMatrix()
{
    for (int x = 0; x < _width; ++x)
    {
        for (int y = 0; y < _height; ++y)
        {
            int i = pixel( x, y );
            _coherence[i] = coherence( i );
        }
    }
}



void DirectionalField::buildDirectionMatrices()
{
    for (int x = 0; x < _width; ++x)
    {
        for (int y = 0; y < _height; ++y)
        {
            int i = pixel( x, y );
            direction( i, _directionX[i], _directionY[i] );
        }
    }
}



float DirectionalField::getHorizontalWindowedDerivative( int px, int py, int windowSize )
{
    float sum = 0;

    for (int x = px - windowSize; x <= px + windowSize; ++x)
    {
        if (x < 0) continue;
        if (x >= _width) continue;

        for (int y = py - windowSize; y <= py + windowSize; ++y)
        {
            if (y < 0) continue;
            if (y >= _height) continue;

            sum += _gxMatrix[pixel(x,y)] * _gxMatrix[pixel(x,y)];
        }
    }

    return sum;
}



float DirectionalField::getVerticalWindowedDerivative(int px, int py, int windowSize)
{
    float sum = 0;

    for (int x = px - windowSize; x <= px + windowSize; ++x)
    {
        if (x < 0) continue;
        if (x >= _width) continue;

        for (int y = py - windowSize; y <= py + windowSize; ++y)
        {
            if (y < 0) continue;
            if (y >= _height) continue;

            sum += _gyMatrix[pixel(x,y)] * _gyMatrix[pixel(x,y)];
        }
    }

    return sum;
}



float DirectionalField::getCrossedWindowedDerivative( int px, int py, int windowSize )
{
    float sum = 0;

    for (int x = px - windowSize; x <= px + windowSize; ++x)
    {
        if (x < 0) continue;
        if (x >= _width) continue;

        for (int y = py - windowSize; y <= py + windowSize; ++y)
        {
            if (y < 0) continue;
            if (y >= _height) continue;

            sum += _gxMatrix[pixel(x,y)] * _gyMatrix[pixel(x,y)];
        }
    }

    return sum;
}

float DirectionalField::getXDirection( int x, int y )
{
    return _directionX[pixel(x,y)];
}

float DirectionalField::getYDirection( int x, int y )
{
    return _directionY[pixel(x,y)];
}

float DirectionalField::getCoherence( int x, int y )
{
    return _coherence[pixel(x,y)];
}

void DirectionalField::normalizeDirections()
{
    int size = _width*_height;

    for (int i = 0; i < size; ++i)
    {
        if (_directionX[i] < 0)
        {
            _directionX[i] *= -1;
            _directionY[i] *= -1;
        }
    }

    for (int i = 0; i < size; ++i)
    {
        float lenght = sqrt( _directionX[i] * _directionX[i] + _directionY[i] * _directionY[i] );
        _directionX[i] /= lenght;
        _directionY[i] /= lenght;
    }
}

void DirectionalField::normalizeCoherence()
{
    int size = _width * _height;
    
    float min = _coherence[0];
    for (int i = 0; i < size; ++i)
    {
        if (_coherence[i] < min)
        {
            min = _coherence[i];
        }
    }
    
    if (min < 0)
    {
        for (int i = 0; i < size; ++i)
        {
            _coherence[i] += min;
        }
    }
    
    float max = _coherence[0];
    for (int i = 0; i < size; ++i)
    {
        if (_coherence[i] > max)
        {
            max = _coherence[i];
        }
    }
    
    for (int i = 0; i < size; ++i)
    {
        _coherence[i] /= max;
    }
}

void DirectionalField::debugPrint()
{
    printf( "gx:\n" );
    for (int y = 0; y < _width; ++y)
    {
        for (int x = 0; x < _height; ++x)
        {
            printf("%.2f\t", _gxMatrix[pixel(x,y)]);
        }
        printf("\n");
    }

    printf( "gy:\n" );
    for (int y = 0; y < _width; ++y)
    {
        for (int x = 0; x < _height; ++x)
        {
            printf("%.2f\t", _gyMatrix[pixel(x,y)]);
        }
        printf("\n");
    }

    printf( "gxx:\n" );
    for (int y = 0; y < _width; ++y)
    {
        for (int x = 0; x < _height; ++x)
        {
            printf("%.2f\t", _gxxMatrix[pixel(x,y)]);
        }
        printf("\n");
    }

    printf( "gyy:\n" );
    for (int y = 0; y < _width; ++y)
    {
        for (int x = 0; x < _height; ++x)
        {
            printf("%.2f\t", _gyyMatrix[pixel(x,y)]);
        }
        printf("\n");
    }

    printf( "gxy:\n" );
    for (int y = 0; y < _width; ++y)
    {
        for (int x = 0; x < _height; ++x)
        {
            printf("%.2f\t", _gxyMatrix[pixel(x,y)]);
        }
        printf("\n");
    }

    printf( "direction:\n" );
    for (int y = 0; y < _width; ++y)
    {
        for (int x = 0; x < _height; ++x)
        {
            printf("(%.2f, %.2f)\t", _directionX[pixel(x,y)], _directionY[pixel(x,y)]);
        }
        printf("\n");
    }
    
    printf( "coherence:\n" );
    for (int y = 0; y < _width; ++y)
    {
        for (int x = 0; x < _height; ++x)
        {
            printf("%.3f\t", _coherence[pixel(x,y)]);
        }
        printf("\n");
    }
}
