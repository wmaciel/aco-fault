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
#include "Point.h"


class Environment
{
public:

    /** Constructor */
    Environment( float initialPheromone, float minimumPheromone, float evaporationRate, Image* image );

    /** Destructor */
    virtual ~Environment();

    /**
     * Clears the pheromones values
     */
    void clearFeromone();

    /**
     * Returns the pheromone level in the pixel
     */
    float getPheromone( int x, int y );
    float getPheromone( Point point );

    /**
     * Deposit amount of pheromone in the pixel
     */
    void addPheromone( float amount, int x, int y );
    void addPheromone( float amount, Point point );

    /**
     * Fetches the maximum value of pheromone in any part of the environment
     */
    float getMaximumPheromone();

    /**
     * Returns the visibility of the pixel
     */
    float getVisibility( int x, int y );
    float getVisibility( Point point );

    /**
     * Creates the negative feedback of pheromone evaporation
     */
    void evaporatePheromone();

    /**
     * Populates a vector with the position of the adjacent pixels
     */
    void getAdjacent( Point point, std::vector<Point>& adjacent );

    /**
     * Fetches the width of the environment
     */
    int getWidth();

    /**
     * Fetches de height of the environment
     */
    int getHeight();

    Image* getPheromoneImage();

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

    /**
     * Divides every element of the matrix by the greatest value in the matrix
     * @param matrix
     */
    void normalizePheromone();

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

    /** Minimum value of the pheromone level, to avoid shutdown of search paths */
    const float _minimumPheromone;
};

#endif	/* ENVIRONMENT_H */

