/*
 * File:   Graph.h
 * Author: Walther Maciel
 *
 * Created on April 21, 2013, 11:51 AM
 */

#ifndef GRAPH_H
#define	GRAPH_H

#include <vector>

namespace aco
{


/**
 * The Graph class is a pure virtual class to create an interface with the rest
 * of the ACO library. It abstracts whatever discrete environment the ants must
 * navigate as a graph.
 * It uses integers to identify each of it's nodes, so they must have unique
 * node ids.
 */
class Graph
{
public:
    /**
     * Destructor
     */
    virtual ~Graph(){};

    /**
     * Given a node, fetches all the other nodes it can reach directly and
     * overwrites it on the given vector
     * @param node Node of origin
     * @param adjacent [out]Will be filled with all the nodes reachable directly
     *        from node. If node is not valid, adjacent is emptied
     */
    virtual void getAdjacentNodes( int node, std::vector<int>& adjacent ) = 0;

    /**
     * Fetches the visibility value between nodes origin and destination.
     * If the nodes are adjacent or if any of them is invalid, returns 0.0f
     * @param origin Node from which the visibility will be given
     * @param destination Node to which the visibility will be given
     * @return The value of visibility from node origin to node destination
     */
    virtual float getVisibility( int origin, int destination ) = 0;

    /**
     * Fetches the amount of pheromone in the edge between nodes origin and
     * destination
     * @param origin Node of origin of the edge being inquired
     * @param destination Node at the end of the edge being inquired
     * @return The pheromone value in the edge from destination to origin
     */
    virtual float getPheromone( int origin, int destination ) = 0;

    /**
     * Fetches the number of nodes in the graph
     * @return The number of nodes in the graph
     */
    virtual int getNumberOfNodes() = 0;

    /**
     * Updates the whole graph diminishing the amount of pheromone in each edge.
     * The evaporation of pheromone is important for it stops the pheromone
     * values to accumulate indefinitely
     * @param evaporationRate The rate at the pheromone evaporates. Normally
     *                        this would be a value between 0 and 1.
     */
    virtual void evaporatePheromone( float evaporationRate );

    /**
     * Increases the amount of pheromone on the edge between nodes origin and
     * destination by amount
     * @param origin Beginning of the edge being modified
     * @param destination End of the edge being modified
     * @param amount Amount of pheromone to be added on the edge
     */
    virtual void addPheromone( int origin, int destination, float amount ) = 0;

private:
    /**
     * Private constructor
     */
    Graph();
};



}
#endif	/* GRAPH_H */

