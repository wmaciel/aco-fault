/*
 * File:   Point.h
 * Author: keoma
 *
 * Created on April 14, 2013, 3:01 PM
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

#endif	/* POINT_H */

