#include <math.h>

#include "Point.h"

Point::Point()
{
    x = -1;
    y = -1;
}



Point::Point( int x, int y )
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

bool operator==(Point p1, Point p2)
{
    return (p1.x == p2.x) && (p1.y == p2.y);
}

Point operator+(Point p1, Point p2)
{
    Point pSum( p1.x + p2.x, p1.y + p2.y );
    return pSum;
}
