/*
 * File:   Ant.h
 * Author: keoma
 *
 * Created on April 17, 2013, 5:11 PM
 */

#ifndef ANT_H
#define	ANT_H

#include "Environment.h"


class Ant
{
public:
    Ant( int startingNode );
    virtual ~Ant();
    bool finishedCycle();
    void restart();
    void move( Environment& environment );
    void updatePheromones( Environment& environment );
    void getPath( std::vector<int>& path );

private:

    void buildProbabilitiesVector( Environment& environment, std::vector<float>& probabilities );

    int chooseItem( std::vector<float>& probabilities  );

    bool isVisited( int node );

    /** id of the node the ant is currently in */
    int _currentNode;

    /** Nodes already visited */
    std::vector<int> _tabu;
};

#endif	/* ANT_H */

