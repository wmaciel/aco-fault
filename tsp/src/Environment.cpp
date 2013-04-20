/*
 * File:   Environment.cpp
 * Author: keoma
 *
 * Created on April 13, 2013, 6:12 PM
 */

#include <math.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

#include "Environment.h"

#define EVAPORATION_RATE 0.4f
#define _INITIAL_PHEROMONE_ 100.0f
#define _MAX_NODES_ 100
#define _MIN_PHEROMONE_ 0.001

Environment::Environment()
{
    _nNodes = 0;
    _maxNodes = _MAX_NODES_;

    _positions = new Point[_maxNodes];
    _distanceMatrix = new float[_maxNodes * _maxNodes];
    _pheromoneMatrix = new float[_maxNodes * _maxNodes];
    clearFeromone();
    _evaporationRate = EVAPORATION_RATE;
}



Environment::~Environment()
{
    delete[] _positions;
    delete[] _pheromoneMatrix;
    delete[] _distanceMatrix;
}



int Environment::id( int x, int y )
{
    return x + _maxNodes * y;
}



void Environment::clearFeromone()
{
    for (int i = 0; i < _maxNodes; ++i)
    {
        for (int j = 0; j < _maxNodes; ++j)
        {
            _pheromoneMatrix[id(j,i)] = _INITIAL_PHEROMONE_;
            _pheromoneMatrix[id(i,j)] = _INITIAL_PHEROMONE_;
        }
    }
}



float Environment::getDistance( float xA, float yA, float xB, float yB )
{
    return sqrt( (xB-xA)*(xB-xA) + (yB-yA)*(yB-yA) );
}



float Environment::getDistance( int a, int b )
{
    float xa, ya;
    getNodePosition( a, xa, ya );

    float xb, yb;
    getNodePosition( b, xb, yb );

    return getDistance( xa, ya, xb, yb );
}



float Environment::getPheromone( int origin, int destination )
{
    return _pheromoneMatrix[id(origin, destination)];
}



float Environment::getVisibility( int origin, int destination )
{
    return 1.0f / _distanceMatrix[id(origin, destination)];
}



void Environment::insertCity( float x, float y )
{
    if (_maxNodes <= _nNodes)
    {
        printf("Can't add more than 100 cities\n");
        return;
    }

    int nodeID = _nNodes;
    _nNodes++;

    _positions[nodeID].x = x;
    _positions[nodeID].y = y;

    for (int j = 0; j < _nNodes; ++j)
    {
        float distance = getDistance( j, nodeID );
        _distanceMatrix[id(j,nodeID)] = distance;
        _distanceMatrix[id(nodeID,j)] = distance;
    }
}



void Environment::getNodePosition( int node, float& x, float& y )
{
    x = _positions[node].x;
    y = _positions[node].y;
}



void Environment::evaporatePheromone()
{
    for (int i = 0; i < _nNodes; ++i)
    {
        for (int j = 0; j < _nNodes; ++j)
        {
            _pheromoneMatrix[id(j,i)] *= ( 1 - _evaporationRate );
            if (_pheromoneMatrix[id(j,i)] <= _MIN_PHEROMONE_)
            {
                _pheromoneMatrix[id(j,i)] = _MIN_PHEROMONE_;
            }
        }
    }
}



int Environment::getNumberOfCities()
{
    return _nNodes;
}



void Environment::getCities( int& nCities, Point** cities )
{
    nCities = _nNodes;
    *(cities) = _positions;
}



void Environment::addPheromone( float amount, int origin, int destination )
{
    _pheromoneMatrix[id(origin,destination)] += amount;
    _pheromoneMatrix[id(destination,origin)] += amount;
}



float Environment::getMaximumPheromone()
{
    float max = 0.0f;

    for ( int i = 0; i < _nNodes; ++i )
    {
        for ( int j = 0; j < _nNodes; ++j )
        {
            if (_pheromoneMatrix[id(j,i)] > max) max = _pheromoneMatrix[id(j,i)];
        }
    }

    return max;
}



float Environment::computePathLength( std::vector<int>& path )
{
    float length = 0.0f;

    for (unsigned int i = 1; i < path.size(); ++i)
    {
        length += _distanceMatrix[id(path[i-1], path[i])];
    }

    return length;
}
