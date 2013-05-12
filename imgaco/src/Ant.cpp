/*
 * File:   Ant.cpp
 * Author: keoma
 *
 * Created on April 21, 2013, 11:50 AM
 */

#include <stdlib.h>
#include <math.h>

#include "Ant.h"

Ant::Ant( Point point, int memorySize, float pheromoneWeight, float visibilityWeight )
{
    _pheromoneWeight = pheromoneWeight;
    _visibilityWeight = visibilityWeight;
    _memorySize = memorySize;
    moveTo( point );
}



Ant::~Ant()
{
}



void Ant::moveTo( Point point )
{
    _currentPosition = point;

    _visited.push_back( point );
    while (_visited.size() >= _memorySize) _visited.erase(_visited.begin());
}



Point Ant::pickWithProbability(std::vector<Point>& nodes, std::vector<float>& probabilities)
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



void Ant::buildProbabilitiesVector( Environment& environment, std::vector<float>& probabilities, std::vector<Point>& nodes )
{
    environment.getAdjacent( _currentPosition, nodes );

    int nNodes = nodes.size();
    float totalWeight = 0;

    for (int i = 0; i < nNodes; ++i)
    {
        if (visited( nodes[i] ))
        {
            probabilities.push_back( 0.0f );
        }
        else
        {
            float pheromone = environment.getPheromone( nodes[i] );
            float visibility = environment.getVisibility( nodes[i] );
            float edgeWeight = pow( pheromone, _pheromoneWeight ) *
                               pow( visibility, _visibilityWeight );
            totalWeight += edgeWeight;
            probabilities.push_back( edgeWeight );
        }
    }
}



bool Ant::visited( Point point )
{
    bool visited = false;

#pragma omp parallel for
    for (unsigned int i = 0; i < _visited.size(); ++i)
    {
        if (point.x == _visited[i].x && point.y == _visited[i].y)
        {
            visited = true;
        }
    }

    return visited;
}



Point Ant::getPosition()
{
    return _currentPosition;
}



Point Ant::pick( Environment& environment )
{
    std::vector<float> probabilities;
    std::vector<Point> adjacentNodes;
    buildProbabilitiesVector( environment, probabilities, adjacentNodes );

    return pickWithProbability( adjacentNodes, probabilities );
}
