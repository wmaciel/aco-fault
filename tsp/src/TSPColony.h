/*
 * File:   TSPColony.h
 * Author: keoma
 *
 * Created on April 17, 2013, 3:41 PM
 */

#ifndef TSPCOLONY_H
#define	TSPCOLONY_H

#include "Environment.h"
#include "Ant.h"
#include "Point.h"

class TSPColony
{
public:
    TSPColony();
    virtual ~TSPColony();
    void addCity( float x, float y );
    void addAnt();
    void removeAnt();
    void removeAllAnts();
    void resetPheromones();
    void printState();
    void update();
    void getCities( int& size, Point** cities );
    void getBestPath( std::vector<int>& path );
    float getPheromone( int a, int b );
    float getMaximumPheromone();

private:

    /**
     * Computes the shortest path by checking the path each ant has made and
     * comparing it with the previous results
     */
    void computeShortestPath( int ant );

    /**
     * Number of simulation steps
     */
    int _steps;

    /**
     * Number of completed cycles
     * A cycle is completed when the ants finds a solution
     */
    int _cycles;

    /**
     * Array with the id of the nodes in the shortest path found so far
     */
    std::vector<int> _bestPath;

    /**
     * Length of the best path found so far
     */
    float _bestPathLength;

    /**
     * Graph with the information about the nodes and its edges
     * Is where the ants move
     */
    Environment _environment;

    /**
     * Array of ants in the simulation
     */
    std::vector<Ant*> _ants;
};

#endif	/* TSPCOLONY_H */

