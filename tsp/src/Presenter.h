/*
 * File:   Presenter.h
 * Author: walther
 *
 * Created on September 27, 2012, 6:46 PM
 */

#ifndef PRESENTER_H
#define	PRESENTER_H

#include <GL/glut.h>

#include "TSPColony.h"

/**
 * Controls everything related to showing graphics on the screen
 */
class Presenter
{
public:
    /**
     * Constructor
     * @param argc Number of arguments received by the main function
     * @param argv Array of string, containing the arguments of the main function
     */
    Presenter( int argc, char** argv );

    /**
     * Destructor
     */
    virtual ~Presenter();

    /**
     * Render callback function for the GLUT library
     */
    static void cb_render();

    /**
     * Keyboard callback function for the GLUT library
     */
    static void cb_keyboard( unsigned char key, int x, int y );

    /**
     * Mouse button callback for the GLUT library
     * @param button Identifier of the pressed button
     * @param state UP or DOWN
     * @param x Horizontal position of the pointer
     * @param y Vertical position of the pointer
     */
    static void cb_mouse( int button, int state, int x, int y );

    /**
     * Updates the colony
     */
    static void cb_idle();

    /**
     * Initializes the program
     */
    void start();

    /**
     * Initializes the GLUT library
     * @param argc Number of arguments received by the main function
     * @param argv Array of string, containing the arguments of the main function
     */
    void initGlut( int argc, char** argv );

    /**
     * Initializes the OpenGL state machine
     */
    void initGL();

    /**
     * Renders what needs to be rendered
     */
    static void render();

    /**
     * Renders the cities as points in the canvas
     */
    static void renderCities( int nCities, Point* cities );

    /**
     * Renders the edges as dark as the pheromone levels on it
     */
    static void renderEdges( int nCities, Point* cities );

    /** reference to the GLUT window */
    int _window;

    /** Colony of ants */
    static TSPColony _colony;

    /** Whether the system is paused */
    static bool _paused;
};

#endif	/* PRESENTER_H */
