/*
 * File:   Environment.h
 * Author: keoma
 *
 * Created on April 13, 2013, 6:12 PM
 */

#ifndef ENVIRONMENT_H
#define	ENVIRONMENT_H

#include "Point.h"

#include <vector>


class Environment
{
public:
    /** Constructor */
    Environment();

    /** Destructor */
    virtual ~Environment();

    /**
     * Inserts a city in the environment
     * @param x
     * @param y
     */
    void insertCity( float x, float y );

    /**
     * Clears the pheromones values
     */
    void clearFeromone();

    /**
     * Returns the pheromone level in the edge leading from the node origin to
     * destination
     */
    float getPheromone( int origin, int destination );

    /**
     * Deposit amount of pheromone in the edge from origin to destination
     */
    void addPheromone( float amount, int origin, int destination );

    /**
     * Fetches the maximum value of pheromone in the pheromone matrix
     * @return
     */
    float getMaximumPheromone();

    /**
     * Returns the visibility of the node destination from origin
     */
    float getVisibility( int origin, int destination );

    /**
     * Creates the negative feedback of pheromone evaporation
     */
    void evaporatePheromone();

    /**
     * Fetches the number of cities in the environment
     */
    int getNumberOfCities();

    /**
     * Fetches the array with the coordinates of the cities
     */
    void getCities( int& nCities, Point** cities );

    /**
     * Computes the length of the given path
     */
    float computePathLength( std::vector<int>& path );

private:
    /**
     * Linearizes the coordinates
     */
    int id( int x, int y );

    /**
     * Gets the id of the city in the coordinates x and y
     */
    int getCityId( float x, float y );

    /**
     * Gets the pixel position of a given node
     */
    void getNodePosition( int node, float& x, float& y );

    /**
     * Gets the euclidean distance between two points
     */
    float getDistance( float xA, float yA, float xB, float yB );

    /**
     * Gets the euclidean distance between two nodes
     */
    float getDistance( int a, int b );

    /** Number of nodes in the graph */
    int _nNodes;

    /** Maximum number of nodes */
    int _maxNodes;

    /** Position array */
    Point* _positions;

    /** Amount of pheromones in edge between nodes */
    float* _pheromoneMatrix;

    /** Euclidean distances between nodes */
    float* _distanceMatrix;

    /** The rate of evaporation of the pheromones */
    float _evaporationRate;
};

#endif	/* ENVIRONMENT_H */

