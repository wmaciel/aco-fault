/*
 * File:   Ant.cpp
 * Author: keoma
 *
 * Created on April 21, 2013, 11:50 AM
 */

#include <stdlib.h>
#include <math.h>

#include "Ant.h"

Ant::Ant( Point point )
{
    moveTo( point );
}



Ant::~Ant()
{
}



void Ant::moveTo( Point point )
{
    _currentPosition = point;
    _visited.push_back( point );
}



Point Ant::pick(std::vector<Point>& nodes, std::vector<float>& probabilities)
{
    // roll a dice between [0.0, 1.0)
    float roll = rand()/(float)RAND_MAX;
    while( roll == 1.0f ) roll = rand()/(float)RAND_MAX;
    // if the roll is exactly 1.0, it will never reach a negative value

    // checks which item in the vector it corresponds to
    for (unsigned int i = 0; i < probabilities.size(); ++i)
    {
        roll -= probabilities[i];
        if (roll < 0)
        {
            return nodes[i];
        }
    }

    // This will happen when all the probabilities are zero
    Point error;
    error.x = -1;
    error.y = -1;
    return error;
}



void Ant::buildProbabilitiesVector( Environment& environment, std::vector<float> probabilities )
{
    std::vector<Point> adjacent;
    environment.getAdjacent( _currentPosition, adjacent );

    int nNodes = adjacent.size();
    float totalWeight = 0;

    for (int i = 0; i < nNodes; ++i)
    {
        if (visited( adjacent[i] ))
        {
            probabilities.push_back( 0.0f );
        }
        else
        {
            float pheromone = environment.getPheromone( adjacent[i] );
            float visibility = environment.getVisibility( adjacent[i] );
            float edgeWeight = pow( pheromone, _pheromoneWeight ) *
                               pow( visibility, _visibilityWeight );
            totalWeight += edgeWeight;
            probabilities.push_back( edgeWeight );
        }
    }
}



bool Ant::visited( Point point )
{
    for (unsigned int i = 0; i < _visited.size(); ++i)
    {
        if (point.x == _visited[i].x && point.y == _visited[i].y) return true;
    }

    return false;
}
