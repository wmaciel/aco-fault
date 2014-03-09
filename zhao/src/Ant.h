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
    
    void setPosition( int x, int y );
    
    void eraseMemory();
    
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
    /**
     * Fetches all the pixels visible to the ant
     * @param visiblePixels Array that will be filled with all the pixels 
     * visible by the ant at this moment
     */
    void getVisiblePixels( std::vector<Point>& visiblePixels );
    
    /**
     * Picks a random pixel from the array of pixels given to it. The 
     * probability of each individual pixel to be picked is proportional to the 
     * value of of the total attractiveness of the pixel
     * @param visiblePixels
     * @return 
     */
    Point choosePixel( std::vector<Point>& visiblePixels );
    
    /**
     * Computes the probabilities of each pixel in the vector "pixels" and fills
     * the vector "probabilities" with it
     * @param pixels
     * @param probabilities
     */
    void computeProbabilities( std::vector<Point>& pixels, std::vector<float>& probabilities );
    
    /**
     * Checks if the pixel "p" is inside the field of vision of the ant
     * @param p
     * @return true if visible, false otherwise
     */
    bool isInsideFOV( Point p );
    
    /**
     * Checks each of the stop criterion. If any of them fail, the ant is 
     * flagged as dead
     */
    void stopCriterion();
    
    /**
     * Checks if the pixel "p" was already visited by the ant
     * @param p
     * @return true if pixel "p" was visited, false otherwise
     */
    bool visited( Point p );
    
    /**
     * Uses the line rendering algorithm of Bresenham to find out the pixels 
     * that belong to the straight line that goes from pixel "src" to pixel 
     * "dst".
     * @param src
     * @param dst
     * @param line Vector that will hold the answer
     */
    void lineBresenham( Point src, Point dst, std::vector<Point>& line );

    /** Flag signaling if the ant is still alive */
    bool _isAlive;
    
    /** The amount the ant can move */
    int _stepLength;
    
    /** Importance of the pheromone concentration */
    float _pheromoneWeight;
    
    /** Importance of the pixel visibility */
    float _visibilityWeight;
    
    /** Current position of the ant */
    Point _position;
    
    /** The amount of pheromone deposited at each pixel by the ant. This 
     * constant is only used when the amount of pheromone deposited by the ant 
     * is constant.*/
    float _pheromoneConstant;
    
    /** The angle of the field of view of the ant in degrees */
    float _fieldOfView;
    
    /** DEPRECATED */
    float _coherenceThreshold;
    
    /** DEPRECATED */
    int _abnormalSteps;
    
    /** DEPRECATED */
    int _maxAbnormalSteps;
    
    /** Maximum number of steps an ant can take. This is used as a timeout to 
     * ensure the death of all ants */
    int _maxSteps;
    
    /** The environment in which the ant is moving */
    Environment* _environment;
    
    /** The pixels the ant has moved trough so far */
    std::vector<Point> _path;
};

#endif	/* ANT_H */

