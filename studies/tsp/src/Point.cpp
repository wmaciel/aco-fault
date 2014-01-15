/*
 * File:   Point.cpp
 * Author: keoma
 *
 * Created on April 14, 2013, 3:01 PM
 */

#include "Point.h"

Point::Point()
{
    x = -1;
    y = -1;
}



Point::Point( float x, float y )
{
    this->x = x;
    this->y = y;
}



Point::Point(const Point& orig)
{
    x = orig.x;
    y = orig.y;
}



Point::~Point()
{
    // do nothing
}
