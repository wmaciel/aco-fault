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
#include "Parameters.h"

Ant::Ant( Point point, Environment* environment )
{
    _pheromoneConstant = PHEROMONE_CONSTANT;
    _fieldOfView = FOV_ANGLE;
    _coherenceThreshold = COHERENCE_TRESHOLD;
    _maxAbnormalSteps = MAX_ABNORMAL_STEPS;
    _maxSteps = MAX_STEPS;
    _abnormalSteps = 0;
    _position = point;
    _environment = environment;
    //_path.push_back( point );
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
    if (visiblePixels.size() > 0)
    {
        Point chosenPoint = choosePixel( visiblePixels );
        _position = chosenPoint;
        _path.push_back( _position );
        stopCriterion();
    }
    else
    {
        _isAlive = false;
    }
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
            if (dx == 0 && dy == 0) continue;

            Point delta( dx, dy );
            Point p = delta + _position;
            if (p.x >= 0 && p.y >= 0)
            {
                float visibility = _environment->getVisibility( p );
                if (!visited( p ) && isInsideFOV( p ) && visibility > 0)
                {
                    visiblePixels.push_back( p );
                }
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
    if (p.x < 0 || p.y < 0 || p.x >= _environment->getWidth() || p.y >= _environment->getHeight())
    {
        return false;
    }

    float eyeDirX, eyeDirY;
    _environment->getDirection( _position.x, _position.y, eyeDirX, eyeDirY );

    float pDirX = p.x - _position.x;
    float pDirY = p.y - _position.y;

    float norm = sqrt( pDirX*pDirX + pDirY*pDirY );
    pDirX /= norm;
    pDirY /= norm;

    norm = sqrt( eyeDirX*eyeDirX + eyeDirY*eyeDirY );
    eyeDirX /= norm;
    eyeDirY /= norm;

    float angle = acos( eyeDirX * pDirX + eyeDirY *pDirY );
    angle = angle * 180.0f / M_PI;
    if (angle <= _fieldOfView)
    {
        return true;
    }
    else // testa no outro sentido
    {
        eyeDirX = -eyeDirX;
        eyeDirY = -eyeDirY;
        angle = acos( eyeDirX * pDirX + eyeDirY * pDirY );
        angle = angle * 180.0f / M_PI;
        return angle <= _fieldOfView;
    }
}

void Ant::stopCriterion()
{
    if (!_environment->getDirectionStrengthMask( _position.x, _position.y ) )
    {
        _abnormalSteps++;
    }
    
    if (_abnormalSteps > _maxAbnormalSteps)
    {
        _isAlive = false;
    }

    if (_maxSteps <= (int)_path.size())
    {
        _isAlive = false;
    }
}

bool Ant::visited( Point p )
{
    for (unsigned int i = 0; i < _path.size(); ++i)
    {
        if (p == _path[i])
        {
            return true;
        }
    }

    return false;
}
