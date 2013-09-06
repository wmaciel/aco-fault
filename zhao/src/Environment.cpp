/*
 * File:   Environment.cpp
 * Author: keoma
 *
 * Created on July 29, 2013, 6:04 PM
 */
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>

#include "Environment.h"
#include "Parameters.h"

#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

Environment::Environment( float initialPheromone, float minimumPheromone, float evaporationRate, Image* image ):
_evaporationRate(evaporationRate), _initialPheromone(initialPheromone), _minimumPheromone(minimumPheromone)
{
    _maximumPheromone = MAX_PHEROMONE;
    construct( imgGetHeight( image ), imgGetWidth( image ) );
    computeImageMatrix( image );
    clearFeromone();
    _directionalField = new DirectionalField( image, 0 );
    _directionalField->debugImages();
}



Environment::~Environment()
{
    delete[] _imageMatrix;
    delete[] _pheromoneMatrix;
}



void Environment::construct( int height, int width )
{
    _height = height;
    _width = width;

    int size = height * width;

    _imageMatrix = new float[size];
    _pheromoneMatrix = new float[size];

    memset( _imageMatrix,      0, sizeof(float)*size );
    memset( _pheromoneMatrix,  0, sizeof(float)*size );
}



void Environment::computeImageMatrix( Image* image )
{
//#pragma omp parallel for
    for (int x=0; x<_width; ++x)
    {
        for (int y=0; y<_height; ++y)
        {
            float luminance;
            imgGetPixel3f( image, x, y, &luminance, &luminance, &luminance );

            int i = id( x, y );
            _imageMatrix[i] = luminance;
        }
    }
}



void Environment::clearFeromone()
{
#pragma omp parallel for
    for (int x=0; x<_width; ++x)
    {
        for (int y=0; y<_height; ++y)
        {
            int i = id( x, y );
            _pheromoneMatrix[i] = _initialPheromone;
        }
    }
}



int Environment::id( int x, int y )
{
    if (x < 0 || y < 0)
    {
        std::cerr<<"ERROR!: Acessou fora da imagem!\n";
        exit(1);
    }

    return x + _width * y;
}



float Environment::getPheromone( int x, int y )
{
    return _pheromoneMatrix[id(x,y)];
}



float Environment::getPheromone( Point point )
{
    return getPheromone( point.x, point.y );
}



void Environment::addPheromone( float amount, int x, int y )
{
    int point = id(x,y);
    _pheromoneMatrix[point] += amount;

    if (_pheromoneMatrix[point] > _maximumPheromone)
    {
        _pheromoneMatrix[point] = _maximumPheromone;
    }
}



void Environment::addPheromone( float amount, Point point )
{
    addPheromone( amount, point.x, point.y );
}



float Environment::getMaximumPheromone()
{
    float max = 0.0f;

    for (int x=0; x<_width; ++x)
    {
        for (int y=0; y<_height; ++y)
        {
            int i = id( x, y );
            if (_pheromoneMatrix[i] > max) max = _pheromoneMatrix[i];
        }
    }

    return max;
}



float Environment::getVisibility( int x, int y )
{
    return 1.0f - _imageMatrix[id(x,y)];
}



float Environment::getVisibility( Point p )
{
    return getVisibility( p.x, p.y );
}



void Environment::getDirection( int x, int y, float& dirX, float& dirY )
{
    _directionalField->getDirection( x, y, dirX, dirY );
}



float Environment::getDirectionStrength( int x, int y )
{
    return _directionalField->getCoherence( x, y );
}



bool Environment::getDirectionStrengthMask( int x, int y )
{
    return _directionalField->getCoherenceMask( x, y );
}



void Environment::evaporatePheromone()
{
#pragma omp parallel for
    for (int x=0; x<_width; ++x)
    {
        for (int y=0; y<_height; ++y)
        {
            int i = id( x, y );
            _pheromoneMatrix[i] *= ( 1 - _evaporationRate );

             if (_pheromoneMatrix[i] < _minimumPheromone)
            {
                _pheromoneMatrix[i] = _minimumPheromone;
            }
        }
    }
}



void Environment::getAdjacent( Point point, std::vector<Point>& adjacent )
{
    for (int x = point.x-1; x <= point.x+1; ++x)
    {
        if ( x < 0 || x >= _width) continue;

        for (int y = point.y-1; y<=point.y+1; ++y)
        {
            if (y < 0 || y >= _height) continue;
            if (x == point.x && y == point.y) continue;

            adjacent.push_back( Point( x, y ) );
        }
    }
}



int Environment::getWidth()
{
    return _width;
}



int Environment::getHeight()
{
    return _height;
}



Image* Environment::getPheromoneImage()
{
    float* normalizedPheromone = normalizePheromone();

    Image* output = imgCreate( _width, _height, 3 );

#pragma omp parallel for
    for (int x = 0; x < _width; ++x)
    {
        for (int y = 0; y < _height; ++y)
        {
            float luminance = normalizedPheromone[id(x,y)];
            imgSetPixel3f( output, x, y, luminance, luminance, luminance );
        }
    }

    delete[] normalizedPheromone;

    return output;
}



float* Environment::normalizePheromone()
{
    float max = 0;

    int nElements = _height * _width;
    float* normalized = new float[nElements];

    for ( int i = 0; i < nElements; ++i )
    {
        if (max < _pheromoneMatrix[i])
        {
            max = _pheromoneMatrix[i];
        }
    }

    float factor = 1.0f / max;

    #pragma omp parallel for
    for ( int i = 0; i < nElements; ++i )
    {
       normalized[i] = _pheromoneMatrix[i] * factor;
    }

    return normalized;
}

