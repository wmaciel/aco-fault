/*
 * File:   Ant.cpp
 * Author: keoma
 *
 * Created on July 29, 2013, 6:04 PM
 */

#include <stdlib.h>
#include <time.h>

#include "Ant.h"
#include "Environment.h"

Ant::Ant( Point point, Environment* environment )
{
    _pheromoneConstant = 0.5f;
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

int Ant::pickIndex( std::vector<float> probabilities )
{
    // roll a dice between [0.0, 1.0)
    float roll = rand()/(float)RAND_MAX;
    while( roll == 1.0f ) roll = rand()/(float)RAND_MAX;
    // if the roll is exactly 1.0, it will never reach a negative value

    // checks which item in the vector it corresponds to
    for (unsigned int i = 0; i < probabilities.size(); ++i)
    {
        roll -= probabilities[i];
        if (roll < 0)
        {
            return i;
        }
    }

    return -1;
}

bool Ant::isAlive()
{
    return _isAlive;
}

void Ant::move()
{
    //TODO
}

void Ant::depositPheromone()
{
    int pathSize = _path.size();
    for (int i = 0; i < pathSize; ++i)
    {
        _environment->addPheromone( _pheromoneConstant, _path[i] );
    }
}
