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
    Point( float x, float y );

    /**
     * Copy Constructor
     */
    Point(const Point& orig);

    /**
     * Destructor
     */
    virtual ~Point();

    /** X coordinate */
    float x;

    /** Y coordinate */
    float y;
};

bool operator== (Point p1, Point p2);

Point& operator+ (Point p1, Point p2);

#endif	/* POINT_H */
