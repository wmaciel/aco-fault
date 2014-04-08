/*
 * File:   Colony.cpp
 * Author: keoma
 *
 * Created on July 29, 2013, 6:01 PM
 */

#include "Colony.h"
#include "Point.h"
#include "Environment.h"
#include "Ant.h"
#include "Parameters.h"

#include <stdio.h>
#include <stdlib.h>

Colony::Colony( Image* image )
{
    // Make sure the input is a grey scale image
    if (imgGetDimColorSpace( image ) != 1)
    {
        Image* aux = imgGrey( image );
        imgDestroy( image );
        image = aux;
        aux = 0;
    }
    
    // Normalize image
    imgNormalize( image, 2 );
    
    // Apply gaussian filter for noise reduction
    imgGauss( image );
    
    // Outputs the image that will actually be used on the tracking
    imgWriteBMP( (char*)"trueInput.bmp", image );
    
    _environment = new Environment( INITIAL_PHEROMONE, MIN_PHEROMONE, EVAPORATION_RATE, image );
}



Colony::~Colony()
{
}



void Colony::clearAnts()
{
    #pragma omp parallel for
    for( int a = 0; a < NUMBER_OF_ANTS; ++a)
    {
        delete _ants[a];
    }
    
    _ants.clear();
}



void Colony::distributeAnts()
{
    distributeAntsByBlock();
}



void Colony::distributeAntsByBlock()
{
    int nHorizontalBlocks = _environment->getWidth() / BLOCK_SIZE;
    int nVerticalBlocks   = _environment->getHeight() / BLOCK_SIZE;
    
    int nAnts = nVerticalBlocks * nHorizontalBlocks;
    
    for (int hb = 0; hb < nHorizontalBlocks; ++hb)
    {
        for (int vb = 0; vb < nVerticalBlocks; ++vb)
        {
            Point pMin( hb * BLOCK_SIZE, vb * BLOCK_SIZE );
            Point pMax( pMin.x + BLOCK_SIZE - 1, pMin.y + BLOCK_SIZE - 1 );
            for (int a = 0; a < nAnts; ++a)
            {
                addAntInBlock( pMin, pMax );
            }
        }
    }
}



void Colony::addAntInBlock( Point pMin, Point pMax )
{
    // compute the probabilities
    std::vector<float> probabilities;

    float sum = 0.0f;
    for (int y = pMin.y; y <= pMax.y; ++y)
    {
        for (int x = pMin.x; x <= pMax.x; ++x)
        {
            sum += _environment->getVisibility( x, y );
        }
    }

    for (int y = pMin.y; y <= pMax.y; ++y)
    {
        for (int x = pMin.x; x <= pMax.x; ++x)
        {
            float pixelValue = _environment->getVisibility( x, y );
            probabilities.push_back( pixelValue / sum );
        }
    }
    
    int blockSize = pMax.x - pMin.x + 1;

    int pixel = Ant::pickIndex( probabilities );
    Point chosenPoint( pMin.x + pixel % blockSize, pMin.y + pixel / blockSize );
    Ant* ant = new Ant( chosenPoint, _environment );
    ant->setStepLength( STEP_LENGTH );
    ant->setPheromoneWeight( PHEROMONE_WEIGHT );
    ant->setVisibilityWeight( VISIBILITY_WEIGHT );
    _ants.push_back( ant );
}



void Colony::run( int nSteps )
{
    for (int currentStep = 0; currentStep < nSteps; ++currentStep)
    {
        distributeAnts();
        moveAnts();
        updatePheromone();
        clearAnts();
    }
}



void Colony::moveAnts()
{
    moveUntilAllDead();
}



void Colony::moveUntilAllDead()
{
    bool allDead = false;
    const int nAnts = _ants.size();
    
    while (!allDead)
    {
        allDead = true;
        #pragma omp parallel for
        for (int a = 0; a < nAnts; ++a)
        {
            Ant* ant = _ants[a];
            
            if (ant->isAlive())
            {
                allDead = false;
                ant->move();
            }
        }
    }
    printDebugImage();
}



void Colony::updatePheromone()
{   
    int nAnts = _ants.size();

    for (int a = 0; a < nAnts; ++a)
    {
        _ants[a]->depositPheromone();
    }
}



bool Colony::available( Point point, Ant& ant )
{
    return false;
}


void Colony::postProcessing( Image** img )
{
    Image* binImg = imgBinOtsu( *img );
    imgDestroy(*img);
    *img = binImg;
    
    //Morphology Close
    Image* kernel = imgReadBMP( KERNEL_PATH );
    imgDilate( *img, kernel );
    imgErode(  *img, kernel );
    
    imgDestroy( kernel );
}



void Colony::printDebugImage()
{
    static int step = 0;
    Image* img = _environment->getPheromoneImage();//imgCreate( _environment->getWidth(), _environment->getHeight(), 3 );
    
//    for (int x = 0; x < _environment->getWidth(); ++x)
//    {
//        for (int y = 0; y < _environment->getHeight(); ++y)
//        {
//            float lum = imgGetPixelf( img, x, y );
//            imgSetPixelf( img, x, y, lum );
//        }
//    }  
    
//    int nAnts = _ants.size();
//    for (int i = 0; i < nAnts; ++i)
//    {
//        Ant* ant = _ants[i];
//        if (ant->isAlive())
//        {
//            // each ant has a color
//            //imgSetPixel3f( img, ant->_position.x, ant->_position.y, 1.0f - i/(float)nAnts, (0.0f + i) / nAnts, 0.0f );
//            
//            //all ants are red
//            imgSetPixel3f( img, ant->_position.x, ant->_position.y, 1,0,0 );
//        }
//        else
//        {
//            imgSetPixel3f( img, ant->_position.x, ant->_position.y, 0.0f, 0.0f, 1.0f );
//        }
//    }
    
//    Image* aux = imgBinOtsu( img );
//    imgDestroy( img );
//    img = aux;
//    aux = 0;

    imgNormalize( img );
    //postProcessing( &img );
    char filename[150];
    sprintf( filename, "/home/keoma/Dropbox/PUC/Mestrado/antColonyOptimization/src/zhao/data/debugImages/debugImage%04d.bmp", ++step );
    imgWriteBMP( filename, img );
    imgDestroy( img );
}
