/*
 * File:   Ant.cpp
 * Author: keoma
 *
 * Created on April 17, 2013, 5:11 PM
 */

#include <vector>
#include <math.h>
#include <stdlib.h>
#include <algorithm>

#include "Ant.h"

#define _ALPHA_ 1.0f //phromone
#define _BETA_ 2.0f //visibility
#define _PHEROMONE_STASH_ 100.0f // how much pheromone an ant has

Ant::Ant( int startingNode )
{
    _currentNode = startingNode;
    _tabu.clear();
}



Ant::~Ant()
{
}



void Ant::restart()
{
    _currentNode = _tabu.front();
    _tabu.clear();
}



bool Ant::finishedCycle()
{
    return (_tabu.size() > 1) && ((_tabu.front() - _tabu.back()) == 0);
}



void Ant::move( Environment& environment )
{
    // Mark the current node as visited
    _tabu.push_back( _currentNode );

    // Pick a node to move to
    std::vector<float> probabilities;
    buildProbabilitiesVector( environment, probabilities );
    int chosen = chooseItem( probabilities );

    // move there
    if (chosen >= 0)
    {
        _currentNode = chosen;
    }
    else
    {
        _tabu.push_back( _tabu.front() );
        _currentNode = _tabu.front();
    }
}



void Ant::buildProbabilitiesVector( Environment& environment, std::vector<float>& probabilities )
{
    int nNodes = environment.getNumberOfCities();
    float totalWeight = 0;

    for (int i = 0; i < nNodes; ++i)
    {
        // if the node was already visited, the probability to go there is zero
        if (isVisited(i))
        {
            probabilities.push_back( 0.0f );
        }
        else
        {
            float visibility = environment.getVisibility(_currentNode, i);
            float pheromone = environment.getPheromone( _currentNode, i );
            float edgeWeight = pow( pheromone, _ALPHA_ ) * pow( visibility, _BETA_ );
            totalWeight += edgeWeight;
            probabilities.push_back( edgeWeight );
        }
    }

    // is total weight is zero, the ant completed its tour
    if (totalWeight == 0.0) return;

    // normalizes the vector, so the probabilities sum 1.0
    for (int i = 0; i < nNodes; ++i)
    {
        probabilities[i] /= totalWeight;
    }
}



int Ant::chooseItem( std::vector<float>& probabilities )
{
    // roll a dice between [0.0, 1.0)
    float roll = rand()/(float)RAND_MAX;
    while( roll == 1.0f ) roll = rand()/(float)RAND_MAX;
    // if the roll is exactly 1.0, it will never reach a negative value

    // checks wich item in the vector it corresponds to
    for (unsigned int i = 0; i < probabilities.size(); ++i)
    {
        roll -= probabilities[i];
        if (roll < 0)
        {
            return i;
        }
    }

    // This will happen when all the probabilities are zero
    return -1;
}



void Ant::updatePheromones( Environment& environment )
{
    float length = environment.computePathLength( _tabu );
    float edgeDeposit = _PHEROMONE_STASH_ / length;

    for (unsigned int i = 1; i < _tabu.size(); ++i)
    {
        environment.addPheromone( edgeDeposit, _tabu[i-1], _tabu[i] );
    }
}



bool Ant::isVisited( int node )
{
    for (unsigned int i = 0; i < _tabu.size(); ++i)
    {
        if (node == _tabu[i]) return true;
    }

    return false;
}



void Ant::getPath( std::vector<int>& path )
{
    path = _tabu;
}
