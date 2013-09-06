/*
 * File:   Colony.h
 * Author: keoma
 *
 * Created on July 29, 2013, 6:01 PM
 */

#ifndef COLONY_H
#define	COLONY_H

#include <vector>

#include "image.h"

class Point;
class Ant;
class Environment;

class Colony
{
public:
    Colony( Image* image );
    virtual ~Colony();
    void run( int nSteps );
    Image* getPheromoneImage();

private:
    void distributeAnts();
    void distributeAntsByBlock();
    void addAntInBlock( Point pMin, Point pMax );
    void moveAnts();
    void updatePheromone();
    bool available( Point point, Ant& ant );
    void printDebugImage();

    Environment* _environment;
    std::vector<Ant*> _ants;
};

#endif	/* COLONY_H */

