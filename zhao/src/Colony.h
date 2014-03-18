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
    void clearAnts();
    void distributeAnts();
    Image* generateProbabilityImage( Image* input );
    void generateProbabilityImages();
    void spawnAnt( int a );
    void moveAnts();
    void moveUntilAllDead();
    void moveOneStep();
    void updatePheromone();
    bool available( Point point, Ant& ant );
    void printDebugImage();
    Image* postProcessing( Image* img );

    std::vector<Image*> _probabilityDistributions;
    Environment* _environment;
    std::vector<Ant*> _ants;
};

#endif	/* COLONY_H */

