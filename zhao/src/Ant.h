/*
 * File:   Ant.h
 * Author: keoma
 *
 * Created on July 29, 2013, 6:04 PM
 */

#ifndef ANT_H
#define	ANT_H

#include <vector>

#include "Point.h"

class Environment;

class Ant
{
public:
    /**
     * Constructor
     * @param point Pixel where the ant will be born in
     * @param environment Environment the ant will live in
     */
    Ant( Point point, Environment* environment );
    
    /**
     * Destructor
     */
    virtual ~Ant();
    
    /**
     * Defines how much an ant can move. The stepLength corresponds to half the 
     * width of box around the the ant in which it can move.
     * @param stepLength Half width of the box in wich the ant can move
     */
    void setStepLength( int stepLength );
    
    /**
     * Defines the weight of the value of the pixel visibility when deciding 
     * which pixel to move to.
     * @param visibilityWeight
     */
    void setVisibilityWeight( float visibilityWeight );
    
    /**
     * Defines the weight of the value of the concentration of pheromone on the 
     * pixel when deciding which pixel to move to
     * @param pheromoneWeight
     */
    void setPheromoneWeight( float pheromoneWeight );
    
    /**
     * Moves the ant to the next pixel. If the ant is in an invalid pixel or has
     * nowhere to go, it gets flagged as dead
     */
    void move();
    
    /**
     * Answers if the ant is alive
     * @return true if alive, false otherwise
     */
    bool isAlive();
    
    /**
     * Deposits pheromone on the trail made by the ant
     */
    void depositPheromone();
    
    /**
     * Picks an index in an array randomly. The probability of each index 
     * getting picked is equal to the value in the vector on position equal to the index
     * @param probabilities Probabilities vector
     * @return An index in the probabilities vector
     */
    static int pickIndex( std::vector<float> probabilities );
    static int pickIndex( float* probabilities, int size );

//private:
    void getVisiblePixels( std::vector<Point>& visiblePixels );
    Point choosePixel( std::vector<Point>& visiblePixels );
    void computeProbabilities( std::vector<Point>& pixels, std::vector<float>& probabilities );
    bool isInsideFOV( Point p );
    void stopCriterion();
    bool visited( Point p );
    void lineBresenham( Point src, Point dst, std::vector<Point>& line );

    bool _isAlive;
    int _stepLength;
    float _pheromoneWeight;
    float _visibilityWeight;
    Point _position;
    float _pheromoneConstant;
    float _fieldOfView;
    float _coherenceThreshold;
    int _abnormalSteps;
    int _maxAbnormalSteps;
    int _maxSteps;
    Environment* _environment;
    std::vector<Point> _path;
};

#endif	/* ANT_H */

