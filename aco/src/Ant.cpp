/*
 * File:   Ant.cpp
 * Author: keoma
 *
 * Created on April 21, 2013, 11:50 AM
 */

#include <stdlib.h>
#include <math.h>

#include "Ant.h"

aco::Ant::Ant( int node )
{
    moveTo( node );
}



aco::Ant::~Ant()
{
}



void aco::Ant::moveTo( int node )
{
    _currentNode = node;
    _visited.push_back( node );
}



int aco::Ant::pick( std::vector<int>& nodes, std::vector<float>& probabilities )
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
    return -1;
}



void aco::Ant::buildProbabilitiesVector( std::vector<int>& adjacent,
                                         std::vector<float>& pheromones,
                                         std::vector<float>& visibilities,
                                         std::vector<float>& probabilities )
{
    int nNodes = adjacent.size();
    float totalWeight = 0;

    for (int i = 0; i < nNodes; ++i)
    {
        // finds out which node are we talking about
        int node = adjacent[i];

        // if the node was already visited, the probability to go there is zero
        if (visited( node ))
        {
            probabilities.push_back( 0.0f );
        }
        else
        {
            float edgeWeight = pow( pheromones[i], _pheromoneWeight ) *
                               pow( visibilities[i], _visibilityWeight );
            totalWeight += edgeWeight;
            probabilities.push_back( edgeWeight );
        }
    }

    // if total weight is zero, the ant completed its tour
    if (totalWeight == 0.0) return;

    // normalizes the vector, so the probabilities sum 1.0
    float normalizingFactor = 1.0f/totalWeight;
    for (int i = 0; i < nNodes; ++i)
    {
        probabilities[i] *= normalizingFactor;
    }
}



bool aco::Ant::visited( int node )
{
    for (unsigned int i = 0; i < _visited.size(); ++i)
    {
        if (node == _visited[i]) return true;
    }

    return false;
}
