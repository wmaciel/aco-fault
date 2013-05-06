/*
 * File:   Colony.h
 * Author: keoma
 *
 * Created on April 26, 2013, 6:29 PM
 */

#ifndef COLONY_H
#define	COLONY_H

#include "Environment.h"
#include "Ant.h"

class Colony
{
public:
    Colony( Image* image, int nAnts );
    virtual ~Colony();
    void addAnt();
    void addAnt( Point point );
    void addAnts( int n );
    void run( int nSteps );
    Image* getPheromoneImage();

private:
    Environment* _environment;
    std::vector<Ant> _ants;
};

#endif	/* COLONY_H */

