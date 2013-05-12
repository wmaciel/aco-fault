/*
 * File:   Ant.h
 * Author: keoma
 *
 * Created on April 21, 2013, 11:50 AM
 */

#ifndef ANT_H
#define	ANT_H

#include <vector>

#include "Point.h"
#include "Environment.h"

class Ant
{
public:
    /**
     * Constructor
     * @param point The initial position of the ant. It MUST be a valid position.
     */
    Ant( Point point, int memorySize, float pheromoneWeight, float visibilityWeight );

    /**
     * Destructor
     */
    virtual ~Ant();

    /**
     * Moves the ant from _currentNode to the given node. This function will
     * also register "node" in the array of visited nodes
     * @param node Destination node
     */
    virtual void moveTo( Point point );

    /**
     * This function will randomly choose one of the adjacent nodes. Each node
     * has a chance of being picked proportional to its visibility and pheromone
     * levels.
     * @param environment
     * @return The chosen node
     */
    virtual Point pick( Environment& environment );

    /**
     * Determines whether or not the ant has already visited the node "node"
     * @param node The node id that will be checked
     * @return true if the "node" has already been visited, false otherwise
     */
    virtual bool visited( Point point );

    /**
     * Fetches the current position of the ant
     */
    Point getPosition();

protected:

    /**
     * Builds a probability vector based on the visibility and pheromone levels
     * of the adjacent pixels
     */
    virtual void buildProbabilitiesVector( Environment& environment, std::vector<float>& probabilities, std::vector<Point>& nodes );

    /**
     * This function will randomly choose one of the nodes in the adjacent
     * vector. Each node has a chance of being picked equal to the value in the
     * probabilities vector occupying the same position as the node.
     * @param nodes Vector with the ids of the nodes
     * @param probabilities Vector with the probabilities of each node to be
     * chosen
     * @return The chosen node
     */
    virtual Point pickWithProbability( std::vector<Point>& nodes, std::vector<float>& probabilities );



    /** The position of the pixel the ant is currently in */
    Point _currentPosition;

    /** The array of pixels the ant has already visited */
    std::vector<Point> _visited;

    /** The importance given to the pheromone value when it is time to choose a
     * node to move */
    float _pheromoneWeight;

    /** The importance given to the visibility value when it is time to choose a
     * node to move*/
    float _visibilityWeight;

    /** The number of places the ant remembers visiting */
    unsigned int _memorySize;
};
#endif	/* ANT_H */
