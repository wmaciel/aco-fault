/*
 * File:   Environment.cpp
 * Author: keoma
 *
 * Created on April 26, 2013, 6:29 PM
 */

#include <string.h>

#include "Environment.h"

Environment::Environment( float initialPheromone, float evaporationRate, int height, int width, Image* image ):
_evaporationRate(evaporationRate), _initialPheromone(initialPheromone)
{
    construct( height, width );
    computeImageMatrix( image );
    computeVisibilityMatrix();
    clearFeromone();
}



Environment::~Environment()
{
    //TODO:
}



void Environment::construct( int height, int width )
{
    _height = height;
    _width = width;

    int size = height * width;

    _imageMatrix = new float[size];
    _pheromoneMatrix = new float[size];
    _visibilityMatrix = new float[size];
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
