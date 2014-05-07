/*
 * File:   Point.h
 * Author: keoma
 *
 * Created on May 1, 2013, 4:31 PM
 */

#ifndef POINT_H
#define	POINT_H

class Point
{
public:

    /**
     * Constructor
     */
    Point();

    /**
     * Constructor
     */
    Point( int x, int y );

    /**
     * Copy Constructor
     */
    Point(const Point& orig);

    /**
     * Destructor
     */
    virtual ~Point();

    /** X coordinate */
    int x;

    /** Y coordinate */
    int y;
};

bool operator== (Point p1, Point p2);

Point operator+ (Point p1, Point p2);

Point operator- (Point p1, Point p2);

float dot( Point& p1, Point& p2 );

#endif	/* POINT_H */
