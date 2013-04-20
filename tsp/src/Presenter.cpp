/*
 * File:   Presenter.cpp
 * Author: walther
 *
 * Created on September 27, 2012, 6:46 PM
 */

#include "Presenter.h"

#include <GL/glut.h>
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500

TSPColony Presenter::_colony;
bool Presenter::_paused = true;

Presenter::Presenter( int argc, char** argv )
{
    initGlut( argc, argv );
    initGL();

    _paused = true;
}

Presenter::~Presenter()
{
    glutDestroyWindow( _window );
}

void Presenter::initGlut(int argc, char** argv )
{
    // initializes glut
    glutInit( &argc, argv );

    // specify the display mode to be RGB and single buffering
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    // define the size
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

    // the position where the window will appear
    glutInitWindowPosition(100,100);

    // create and window and keep the identifier
    _window = glutCreateWindow( "TSP Solver" );

    // set the callbacks
    glutDisplayFunc( cb_render );
    glutKeyboardFunc( cb_keyboard );
    glutMouseFunc( cb_mouse );
    glutIdleFunc( cb_idle );
}

void Presenter::initGL()
{
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0.0, 1.0, 0.0, 1.0 );

    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glPointSize( 8.0f );
}



void Presenter::start()
{
    glutMainLoop();
}



void Presenter::cb_render()
{
    render();
}



void Presenter::cb_keyboard( unsigned char key, int x, int y )
{
    switch( key )
    {
    case 'q':
        exit( 0 );
        break;

    case 'n':
        printf( "restarting...\n" );
        printf("removing all ants...\n");
        Presenter::_colony.removeAllAnts();
        printf("clearing pheromones...\n");
        Presenter::_colony.resetPheromones();
        break;

    case 'p':
        if (Presenter::_paused)
        {
            printf( "running...\n" );
            Presenter::_paused = false;
        }
        else
        {
            printf("pausing...\n");
            Presenter::_paused = true;
        }

        break;

    case ';':
        Presenter::_colony.printState();
        break;

    case 'c':
        printf("clearing pheromones...\n");
        Presenter::_colony.resetPheromones();
        break;

    case '+':
        printf("adding ant...\n");
        Presenter::_colony.addAnt();
        break;

    case '-':
        printf("removing ant...\n");
        Presenter::_colony.removeAnt();
        break;
    }
}



void Presenter::cb_mouse( int button, int state, int x, int y )
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        printf( "Adding city in (%d, %d)\n", x, y );

        Presenter::_colony.addCity( x / (float)SCREEN_WIDTH, 1.0 - ( y / (float)SCREEN_HEIGHT ) );
    }

    render();
}



void Presenter::cb_idle()
{
    if (!Presenter::_paused)
    {
        Presenter::_colony.update();
    }

    render();
}



void Presenter::render()
{
    glClear( GL_COLOR_BUFFER_BIT );

    int nCities = 0;
    Point* cities = 0;
    Presenter::_colony.getCities( nCities, &cities );

    renderEdges( nCities, cities );
    renderCities( nCities, cities );

    glutSwapBuffers();
}



void Presenter::renderCities( int nCities, Point* cities )
{
    glPointSize( 8.0f );

    glColor3f( 1.0f, 0.0f, 0.0f );

    glBegin( GL_POINTS );
    for (int i = 0; i < nCities; ++i)
    {
        glVertex2f( cities[i].x, cities[i].y );
    }
    glEnd();
}



void Presenter::renderEdges( int nCities, Point* cities )
{
    glLineWidth( 2.0f );

    float maxPheromone = _colony.getMaximumPheromone();

    glBegin( GL_LINES );
    {
        for (int i = 0; i < nCities; ++i)
        {
            for (int j = 0; j < i; ++j)
            {
                float intensity =  _colony.getPheromone( i, j ) / maxPheromone ;
		if (intensity < 0.3) continue;

                glColor3f( intensity, intensity, intensity );

                glVertex2f( cities[i].x, cities[i].y );
                glVertex2f( cities[j].x, cities[j].y );
            }
        }
    }
    glEnd();
}
