/*
 * File:   Ant.cpp
 * Author: keoma
 *
 * Created on July 29, 2013, 6:04 PM
 */

#include <stdlib.h>
#include <stdio.h>
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
    _isAlive = false;
    //_path.push_back( point );
    if (_position.x < 0 || _position.x >= _environment->getWidth()
                    || _position.y < 0 || _position.y >= _environment->getHeight())
    {
        printf( "INVALID CREATION! (%d, %d)\n", _position.x, _position.y );
    }
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

void Ant::setPosition(int x, int y)
{
    _position.x = x;
    _position.y = y;
}

void Ant::eraseMemory()
{
    _isAlive = true;
    _abnormalSteps = 0;
    _path.clear();
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

int Ant::pickIndex( float* probabilities, int size )
{
    // roll a dice between [0.0, 1.0)
    float roll = rand()/(float)RAND_MAX;
    while( roll == 1.0f ) roll = rand()/(float)RAND_MAX;
    // if the roll is exactly 1.0, it will never reach a negative value

    // checks which item in the vector it corresponds to
    while (true)
    {
        for (int i = 0; i < size; ++i)
        {
            roll -= probabilities[i];
            if (roll < 0)
            {
                return i;
            }
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
        std::vector<Point> line;
        lineBresenham( _position, chosenPoint, line );
        _position = chosenPoint;
        _path.insert( _path.end(), line.begin()+1, line.end() );
        stopCriterion();
        
        if (_position.x < 0 || _position.x >= _environment->getWidth()
                    || _position.y < 0 || _position.y >= _environment->getHeight())
        {
            printf( "INVALID MOVE! (%d, %d)\n", _position.x, _position.y );
        }
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
        //Point point = _path[i];
        //float visibility = _environment->getVisibility( point );
        //float consistency = _environment->getDirectionStrength( point.x, point.y );
        //float pheromone = ( visibility + consistency ) * _pheromoneConstant / 2.0;
        
        float pheromone = _pheromoneConstant;
        _environment->addPheromone( pheromone, _path[i] );
    }
}

void Ant::getVisiblePixels( std::vector<Point>& visiblePixels )
{
    int width = _environment->getWidth();
    int height = _environment->getHeight();
    
    for (int dx = -_stepLength; dx <= _stepLength; ++dx)
    {
        for (int dy = -_stepLength; dy <= _stepLength; ++ dy)
        {
            if (dx == 0 && dy == 0) continue;

            Point delta( dx, dy );
            Point p = delta + _position;
            if (p.x >= 0 && p.y >= 0 && p.x < width && p.y < height)
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



void Ant::lineBresenham( Point src, Point dst, std::vector<Point>& line )
{
    int x0 = src.x;
    int y0 = src.y;
    int x1 = dst.x;
    int y1 = dst.y;
    
//   dx := abs(x1-x0)
//   dy := abs(y1-y0)
//   if x0 < x1 then sx := 1 else sx := -1
//   if y0 < y1 then sy := 1 else sy := -1
//   err := dx-dy
    
    int dx = abs( x1 - x0 );
    int dy = abs( y1 - y0 );
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    
//   loop
//     plot(x0,y0)
//     if x0 = x1 and y0 = y1 exit loop
//     e2 := 2*err
//     if e2 > -dy then 
//       err := err - dy
//       x0 := x0 + sx
//     end if
//     if x0 = x1 and y0 = y1 then 
//       plot(x0,y0)
//       exit loop
//     end if
//     if e2 <  dx then 
//       err := err + dx
//       y0 := y0 + sy 
//     end if
//   end loop
    
    while (true)
    {
        line.push_back( Point( x0, y0 ) );
        if (x0 == x1 && y0 == y1) break;
        int e2= err + err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (x0 == x1 && y0 == y1)
        {
            line.push_back( Point( x0, y0 ) );
            break;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}
