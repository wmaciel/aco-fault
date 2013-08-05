/*
 * File:   Ant.cpp
 * Author: keoma
 *
 * Created on July 29, 2013, 6:04 PM
 */

#include "Ant.h"
#include "Environment.h"

Ant::Ant( Point point, Environment* environment )
{
    _position = point;
    _environment = environment;
}

Ant::~Ant()
{
}

void Ant::setStepLength( int stepLength )
{
    _stepLength = stepLength;
}

void Ant::setVisibilityWeight( float visibilityWeight )
{
    _visibilityWeight = visibilityWeight;
}

void Ant::setPheromoneWeight( float pheromoneWeight )
{
    _pheromoneWeight = pheromoneWeight;
}
