/*
 * File:   Graph.cpp
 * Author: keoma
 *
 * Created on April 21, 2013, 11:51 AM
 */

#include "Graph.h"

void aco::Graph::evaporatePheromone( float evaporationRate )
{
    int nNodes = getNumberOfNodes();

    for (int i = 0; i < nNodes; ++i)
    {
        std::vector<int> adjacent;
        getAdjacentNodes( i, adjacent );
        int nAdjacent = adjacent.size();
        for (int j = 0; j < nAdjacent; ++j)
        {
            float oldPheromone = getPheromone( i, j );
            float newPheromone = oldPheromone * ( 1.0f - evaporationRate );
            addPheromone( i, j, newPheromone - oldPheromone );
        }
    }
}