#include <math.h>

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

void Point::normalize()
{
    float norm = sqrt( x*x + y*y );
    if (norm == 0.0f) return;
    x /= norm;
    y /= norm;
}

bool operator==(Point p1, Point p2)
{
    return (p1.x == p2.x) && (p1.y == p2.y);
}

Point& operator+(Point p1, Point p2)
{
    Point* pSum = new Point( p1.x + p2.x, p1.y + p2.y );
    return *pSum;
}
