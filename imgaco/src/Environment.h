/*
 * File:   Environment.h
 * Author: keoma
 *
 * Created on April 26, 2013, 6:29 PM
 */

#ifndef ENVIRONMENT_H
#define	ENVIRONMENT_H

#include <vector>

#include "image.h"


class Environment
{
public:

    /** Constructor */
    Environment( int height, int width, Image* image );

    /** Destructor */
    virtual ~Environment();

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
     * Fetches the maximum value of pheromone in any part of the environment
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

private:

    /**
     * Initializes the internal variables and allocates space for the matrices
     * @param height Height of the matrices
     * @param width Width of the matrices
     */
    void construct( int height, int width );

    /**
     * Computes the visibility values of each element in _visibilityMatrix
     */
    void computeVisibilityMatrix();

    /**
     * Copies the given image to _imageMatrix
     * @param image Input image
     */
    void computeImageMatrix( Image* image );

    /**
     * Linearizes the coordinates
     */
    int id( int x, int y );

    /** Height of the image */
    int _height;

    /** Width of the image */
    int _width;

    /** Matrix with the luminance image */
    float* _imageMatrix;

    /** Amount of pheromones in edge between nodes */
    float* _pheromoneMatrix;

    /** Euclidean distances between nodes */
    float* _visibilityMatrix;

    /** The rate of evaporation of the pheromones */
    const float _evaporationRate;

    /** The initial small value of the pheromone intensity */
    const float _initialPheromone;
};

#endif	/* ENVIRONMENT_H */

