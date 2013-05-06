/*
 * File:   Environment.cpp
 * Author: keoma
 *
 * Created on April 26, 2013, 6:29 PM
 */

#include <string.h>

#include "Environment.h"

#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

Environment::Environment( float initialPheromone, float evaporationRate, Image* image ):
_evaporationRate(evaporationRate), _initialPheromone(initialPheromone)
{
    construct( imgGetHeight( image ), imgGetWidth( image ) );
    computeImageMatrix( image );
    computeVisibilityMatrix();
    clearFeromone();
}



Environment::~Environment()
{
    delete[] _imageMatrix;
    delete[] _pheromoneMatrix;
    delete[] _visibilityMatrix;
}



void Environment::construct( int height, int width )
{
    _height = height;
    _width = width;

    int size = height * width;

    _imageMatrix = new float[size];
    _pheromoneMatrix = new float[size];
    _visibilityMatrix = new float[size];

    memset( _imageMatrix,      0, sizeof(float)*size );
    memset( _pheromoneMatrix,  0, sizeof(float)*size );
    memset( _visibilityMatrix, 0, sizeof(float)*size );
}



void Environment::computeImageMatrix( Image* image )
{
    Image* gray = imgGrey( image );

    for (int x=0; x<_width; ++x)
    {
        for (int y=0; y<_height; ++y)
        {
            float luminance;
            imgGetPixel3f( gray, x, y, &luminance, &luminance, &luminance );

            int i = id( x, y );
            _imageMatrix[i] = luminance;
        }
    }
}



void Environment::computeVisibilityMatrix()
{
    for (int x=1; x<_width-1; ++x)
    {
        for (int y=1; y<_height-1; ++y)
        {
            float leftRight = fabs( _imageMatrix[id(x-1,y)]   - _imageMatrix[id(x+1,y)] );
            float upDown    = fabs( _imageMatrix[id(x,y-1)]   - _imageMatrix[id(x,y+1)] );
            float diagonal1 = fabs( _imageMatrix[id(x-1,y-1)] - _imageMatrix[id(x+1,y+1)] );
            float diagonal2 = fabs( _imageMatrix[id(x-1,y+1)] - _imageMatrix[id(x+1,y-1)] );
            float max = MAX( leftRight, MAX( upDown, MAX( diagonal1, diagonal2 ) ) );

            _visibilityMatrix[id(x,y)] = max;
        }
    }
}



void Environment::clearFeromone()
{
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
    _pheromoneMatrix[id(x,y)] += amount;
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
    return _visibilityMatrix[id(x,y)];
}



float Environment::getVisibility( Point point )
{
    return getVisibility( point.x, point.y );
}



void Environment::evaporatePheromone()
{
    for (int x=0; x<_width; ++x)
    {
        for (int y=0; y<_height; ++y)
        {
            int i = id( x, y );
            _pheromoneMatrix[i] *= ( 1 - _evaporationRate );
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
    Image* output = imgCreate( _width, _height, 1 );

    for (int x = 0; x < _width; ++x)
    {
        for (int y = 0; y < _height; ++y)
        {
            float luminance = _pheromoneMatrix[id(x,y)];
            imgSetPixel3f( output, x, y, luminance, luminance, luminance );
        }
    }

    return output;
}
