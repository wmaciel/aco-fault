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
    _fieldOfView = 15.0;
    _coherenceThreshold = 0.3f;
    _abnormalSteps = 0;
    _maxAbnormalSteps = 3;
    _position = point;
    _environment = environment;
}

Ant::~Ant()
{
    _path.clear();
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
    std::vector<Point> visiblePixels;
    getVisiblePixels( visiblePixels );
    Point chosenPoint = choosePixel( visiblePixels );
    _position = chosenPoint;
    _path.push_back( _position );
    stopCriterion();
}

void Ant::depositPheromone()
{
    int pathSize = _path.size();
    for (int i = 0; i < pathSize; ++i)
    {
        _environment->addPheromone( _pheromoneConstant, _path[i] );
    }
}

void Ant::getVisiblePixels( std::vector<Point>& visiblePixels )
{
    for (int dx = -_stepLength; dx <= _stepLength; ++dx)
    {
        for (int dy = -_stepLength; dy <= _stepLength; ++ dy)
        {
            Point delta( dx, dy );
            Point p = delta + _position;
            if (isInsideFOV( p ))
            {
                visiblePixels.push_back( p );
            }
        }
    }
}

Point Ant::choosePixel( std::vector<Point>& visiblePixels )
{
    std::vector<float> probabilities;
    computeProbabilities( visiblePixels, probabilities );
    int chosenIndex = pickIndex( probabilities );
    return visiblePixels[chosenIndex];
}

void Ant::computeProbabilities( std::vector<Point>& pixels, std::vector<float>& probabilities )
{
    // compute normalizing factor
    float factor = 0;
    for (unsigned int i = 0; i < pixels.size(); ++i)
    {
        float pheromone = _environment->getPheromone( pixels[i] );
        float visibility = _environment->getVisibility( pixels[i] );
        factor += pow( pheromone, _pheromoneWeight ) * pow( visibility, _visibilityWeight );
    }
    factor = 1.0f/factor;

    //compute the probabilities
    for (unsigned int i = 0; i < pixels.size(); ++i)
    {
        float pheromone = _environment->getPheromone( pixels[i] );
        float visibility = _environment->getVisibility( pixels[i] );

        probabilities.push_back( factor * pow( pheromone, _pheromoneWeight ) * pow( visibility, _visibilityWeight ) );
    }
}

bool Ant::isInsideFOV( Point p )
{
    Point eyeDir;
    _environment->getDirection( p.x, p.y, eyeDir.x, eyeDir.y );

    p.normalize();
    eyeDir.normalize();

    float angle = acos( eyeDir.x * p.x + eyeDir.y * p.y );

    return angle <= _fieldOfView;
}

void Ant::stopCriterion()
{
    float directionCoherence = _environment->getDirectionStrength( _position.x, _position.y );
    if (directionCoherence < _coherenceThreshold)
    {
        _abnormalSteps++;
    }

    if (_abnormalSteps > _maxAbnormalSteps)
    {
        _isAlive = false;
    }
}
