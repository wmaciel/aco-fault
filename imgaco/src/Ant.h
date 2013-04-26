/*
 * File:   Ant.h
 * Author: keoma
 *
 * Created on April 21, 2013, 11:50 AM
 */

#ifndef ANT_H
#define	ANT_H

#include <vector>

namespace aco
{



class Ant
{
public:
    /**
     * Constructor
     * @param node The initial position of the ant. It MUST be a valid position.
     */
    Ant( int node );

    /**
     * Destructor
     */
    virtual ~Ant();

    /**
     * Moves the ant from _currentNode to the given node. This function will
     * also register "node" in the array of visited nodes
     * @param node Destination node
     */
    virtual void moveTo( int node );

    /**
     * Builds a probability vector based on the pheromone and visibility vectors
     * Note that any position "i" in any of the vectors should refer to the same
     * node. i.e. pheromones[i] indicates the amount of pheromone in the edge
     * leading from _currentNode to adjacent[i].
     * @param adjacent Vector with the ids of the nodes adjacent to _currentNode
     * @param pheromones Amount of pheromone deposited in the edge leading from
     *                   _curretNode to each of the adjacent nodes
     * @param visibilities Vector with the visibilities of the adjacent nodes
     *                     from _currentNode
     * @param probabilities Output vector with the probability of the ant to
     *                      move from _current node to each of the adjacent
     *                      nodes
     */
    virtual void buildProbabilitiesVector( std::vector<int>& adjacent,
                                           std::vector<float>& pheromones,
                                           std::vector<float>& visibilities,
                                           std::vector<float>& probabilities );

    /**
     * This function will randomly choose one of the nodes in the adjacent
     * vector. Each node has a chance of being picked equal to the value in the
     * probabilities vector occupying the same position as the node.
     * @param nodes Vector with the ids of the nodes
     * @param probabilities Vector with the probabilities of each node to be
     * chosen
     * @return The chosen node
     */
    virtual int pick( std::vector<int>& nodes, std::vector<float>& probabilities );

    /**
     * Determines whether or not the ant has already visited the node "node"
     * @param node The node id that will be checked
     * @return true if the "node" has already been visited, false otherwise
     */
    virtual bool visited( int node );

protected:
    /** The node where the ant is currently in */
    int _currentNode;

    /** The array of nodes the ant has already visited */
    std::vector<int> _visited;

    /** The importance given to the pheromone value when it is time to choose a
     * node to move */
    float _pheromoneWeight;

    /** The importance given to the visibility value when it is time to choose a
     * node to move*/
    float _visibilityWeight;
};



}
#endif	/* ANT_H */

