/*
 * File:   Colony.cpp
 * Author: keoma
 *
 * Created on July 29, 2013, 6:01 PM
 */

#include "Colony.h"
#include "Point.h"
#include "Environment.h"
#include "Ant.h"

#define INITIAL_PHEROMONE_ 0.1f
#define EVAPORATION_RATE_ 0.05f
#define MEMORY_ 100
#define VISIBILITY_THRESHOLD_ 0.08f
#define MINIMUM_PHEROMONE_ 0.01f
#define _PHEROMONE_WEIGHT_ 1.0f
#define _VISIBILITY_WEIGHT_ 10.0f
#define BLOCK_SIZE 10

Colony::Colony( Image* image, int nAnts )
{
    _environment = new Environment( INITIAL_PHEROMONE_, MINIMUM_PHEROMONE_, EVAPORATION_RATE_, image );
    distributeAnts();
}

Colony::~Colony()
{
}

void Colony::addAnt( Point point )
{

}

void Colony::distributeAnts()
{
    int nHorizontalBlocks = _environment->getWidth() / BLOCK_SIZE;
    int nVerticalBlocks   = _environment->getHeight() / BLOCK_SIZE;

    for (int hb = 0; hb < nHorizontalBlocks; ++hb)
    {
        for (int vb = 0; vb < nVerticalBlocks; ++vb)
        {
            Point pMin( hb * BLOCK_SIZE, vb * BLOCK_SIZE );
            Point pMax( pMin.x + BLOCK_SIZE, pMin.y + BLOCK_SIZE );

            addAntInBlock( pMin, pMax );
        }
    }
}

void Colony::addAntInBlock( Point pMin, Point pMax )
{
    // compute the probabilities
    std::vector<float> probabilities;

    float sum = 0.0f;
    for (int y = pMin.y; y <= pMax.y; ++y)
    {
        for (int x = pMin.x; x <= pMax.x; ++x)
        {
            sum += 1.0f - _environment->getVisibility( x, y );
        }
    }

    for (int y = pMin.y; y <= pMax.y; ++y)
    {
        for (int x = pMin.x; x <= pMax.x; ++x)
        {
            float pixelValue = _environment->getVisibility( x, y );
            probabilities.push_back( pixelValue / sum );
        }
    }

    int pixel = Ant::pickIndex( probabilities );
    int width = _environment->getWidth();
    Point chosenPoint( pixel % width, pixel / width );
    Ant* ant = new Ant( chosenPoint, _environment );
    _ants.push_back( ant );
}

void Colony::run( int nSteps )
{

}

Image* Colony::getPheromoneImage()
{
    return 0;
}

bool Colony::available( Point point, Ant& ant )
{
    return false;
}
