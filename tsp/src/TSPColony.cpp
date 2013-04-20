/*
 * File:   TSPColony.cpp
 * Author: keoma
 *
 * Created on April 17, 2013, 3:41 PM
 */

// Local includes
#include "TSPColony.h"

// Library includes
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <float.h>

TSPColony::TSPColony()
{
    _steps = 0;
    _cycles = 0;
    _ants.clear();
    _bestPathLength = FLT_MAX;

    resetPheromones();
}



TSPColony::~TSPColony()
{
}



void TSPColony::addAnt()
{
    int n = _environment.getNumberOfCities();
    int node = rand() % n;
    _ants.push_back( new Ant( node ) );
}



void TSPColony::removeAnt()
{
    if (!_ants.empty())
    {
        delete _ants.back();
        _ants.pop_back();
    }
}



void TSPColony::removeAllAnts()
{
    while(!_ants.empty()) removeAnt();
}



void TSPColony::resetPheromones()
{
    _environment.clearFeromone();
}



void TSPColony::printState()
{
    printf("number of Ants: %ld\n", _ants.size());
    printf("Best path: %f\n", _bestPathLength);
    for( unsigned int i = 0; i < _bestPath.size(); ++i )
    {
        printf("%d, ", _bestPath[i]);
    }
    printf( "\n" );

    printf("Graph:\n");
    printf("Pheromone:\n");
    for ( int i = 0; i < _environment.getNumberOfCities(); ++i )
    {
        for ( int j = 0; j <= i; ++j )
        {
            printf("%0.3f ", _environment.getPheromone(i,j));
        }
        printf("\n");
    }

    printf("\n");

//    printf("Distance:\n");
//    for ( int i = 0; i < _environment.getNumberOfCities(); ++i )
//    {
//        for ( int j = 0; j < _environment.getNumberOfCities(); ++j )
//        {
//            printf("%f, ", _environment.getVisibility(i,j));
//        }
//        printf("\n");
//    }
}



void TSPColony::update()
{
    int nAnts = _ants.size();

    // If there are no ants, there's nothing to simulate
    if (!nAnts) return;

    // Move the ants
    for (int a = 0; a < nAnts; ++a )
    {
        _ants[a]->move( _environment );
    }

    // If the ants found a solution, update the pheromones and save the shortest
    if (_ants[0]->finishedCycle())
    {
        _environment.evaporatePheromone();

        for (int a = 0; a < nAnts; ++a )
        {
            computeShortestPath( a );
            _ants[a]->updatePheromones( _environment );
            _ants[a]->restart();
        }

        _cycles++;
    }

    _steps++;
}



void TSPColony::addCity( float x, float y )
{
    _environment.insertCity( x, y );
    _bestPathLength = 0;
    _bestPath.clear();
}



void TSPColony::getCities( int& size, Point** cities )
{
    _environment.getCities( size, cities );
}



float TSPColony::getPheromone( int a, int b )
{
    return _environment.getPheromone( a, b );
}



float TSPColony::getMaximumPheromone()
{
    return _environment.getMaximumPheromone();
}



void TSPColony::computeShortestPath( int ant )
{
    std::vector<int> antPath;
    _ants[ant]->getPath( antPath );
    float length = _environment.computePathLength( antPath );
    if (length <= _bestPathLength)
    {
        _bestPathLength = length;
        _bestPath.clear();
        _bestPath = antPath;
    }
}
