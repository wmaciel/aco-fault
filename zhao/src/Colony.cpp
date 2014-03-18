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
    
    imgWriteBMP( (char*)"debugImg/inputAfterGaussian.bmp", image );
    
    _environment = new Environment( INITIAL_PHEROMONE, MIN_PHEROMONE, EVAPORATION_RATE, image );
    
    generateProbabilityImages();
}



Colony::~Colony()
{
}



void Colony::clearAnts()
{
    int nAnts = _ants.size();
    
    #pragma omp parallel for
    for( int a = 0; a < nAnts; ++a)
    {
        Ant* ant = _ants[a];
        delete ant;
    }
    
    _ants.clear();
}

void Colony::distributeAnts()
{
    int nAnts = Parameters::numberOfAnts;
    
    //#pragma omp parallel for
    for (int a = 0; a < nAnts; ++a)
    {   
        if (!_ants[a]->isAlive())
        {
            spawnAnt( a );
        }
    }
}

void Colony::generateProbabilityImages()
{
     Image* vis = _environment->getVisibilityImage(); imgWriteBMP( (char*)"debugImg/gammaone.bmp", vis );
     
     Image* visGammaDown = imgCopy( vis );
     imgGamma( visGammaDown, 0.5f ); imgWriteBMP( (char*)"debugImg/gammadown.bmp", visGammaDown );
     Image* gammaDownProb = generateProbabilityImage( visGammaDown );
     imgDestroy( visGammaDown );
     _probabilityDistributions.push_back( gammaDownProb );
     
     Image* visGammaUp = imgCopy( vis );
     imgGamma( visGammaUp, 1.5f ); imgWriteBMP( (char*)"debugImg/gammaup.bmp", visGammaUp );
     Image* gammaUpProb = generateProbabilityImage( visGammaUp );
     imgDestroy( visGammaUp );
     _probabilityDistributions.push_back( gammaUpProb );
     
     Image* prob = generateProbabilityImage( vis );
     imgDestroy( vis );
     _probabilityDistributions.push_back( prob );
}

Image* Colony::generateProbabilityImage( Image* input )
{
    if (imgGetDimColorSpace( input ) != 1) return 0;
    
    Image* output = imgCopy( input );
    float* data = imgGetData( output );
    int size = imgGetWidth( output ) * imgGetHeight( output );
    float sum = 0;
    for (int i = 0; i < size; ++i)
    {
//        if (isnan(data[i]))
//        {
//            printf( "Cannot generate probability image, NAN detected! on pixel %d\n", i );
//        }
        sum += data[i];
    }
    for (int i = 0; i < size; ++i)
    {
        data[i] /= sum;
    }
    
    return output;
}



void Colony::spawnAnt( int a )
{
    //chooses new spawn point for ant number "a"
    int nGammaImages = _probabilityDistributions.size();
    Image* probabilityImage = _probabilityDistributions[a % nGammaImages];
    float* probabilities = imgGetData( probabilityImage );
    int pixel = Ant::pickIndex( probabilities, _environment->getWidth()*_environment->getHeight() );
    int width = imgGetWidth( probabilityImage );
    int chosenX = pixel % width;
    int chosenY = pixel / width;
    
    //teleports the ant there
    _ants[a]->setPosition( chosenX, chosenY );
    _ants[a]->eraseMemory();
}



void Colony::run( int nSteps )
{
    // saves parameter list
    FILE* paramFile = fopen( "params.txt", "w" );
    
    fprintf(paramFile, "Parâmetros utilizados na última execução:\n");
    
    fprintf(paramFile, "Formiga:\n" );
    fprintf(paramFile, "\tQuantidade: %d\n", Parameters::numberOfAnts);
    fprintf(paramFile, "\tAngulo de visão: %f\n", Parameters::fov);
    fprintf(paramFile, "\tPeso Feromônio: %f\n", Parameters::pheromoneWeight);
    fprintf(paramFile, "\tPeso Atributo: %f\n", Parameters::attributeWeight);
    fprintf(paramFile, "\tConstante Feromônio: %f\n", Parameters::pheromoneConstant);
    fprintf(paramFile, "\tTamanho Passo: %d\n", Parameters::stepLength);
    fprintf(paramFile, "\tNumero passos: %d\n", Parameters::maxSteps);
    fprintf(paramFile, "\tNumero passos erroneos: %d\n", Parameters::maxAbnormalSteps);
    
    fprintf(paramFile, "Feromônio:\n");
    fprintf(paramFile, "\tTurnos: %d\n", Parameters::turns);
    fprintf(paramFile, "\tTaxa de Evaporação: %f\n", Parameters::evaporationRate);
    fprintf(paramFile, "\tFeromonio minimo: %f\n", Parameters::minPheromone);
    fprintf(paramFile, "\tFeromonio maximo: %f\n", Parameters::maxPheromone);
    fprintf(paramFile, "\tFeromonio inicial: %f\n", Parameters::initPheromone);
    
    fprintf(paramFile, "Direção:\n");
    fprintf(paramFile, "\tLargura Gauss: %d\n", Parameters::widthGauss);
    fprintf(paramFile, "\tAltura Gauss: %d\n", Parameters::heightGauss);
    fprintf(paramFile, "\tLimiar Consistencia: %f\n", Parameters::cohTreshold);
    fprintf(paramFile, "\tRaio do Kernel: %d\n", Parameters::kernelRadius);
    
    fclose( paramFile );
    
    // initialize ants
    clearAnts();
    Point p( 0, 0 );
    for (int a = 0; a < Parameters::numberOfAnts; ++a)
    {
        Ant* ant = new Ant( p, _environment );
        _ants.push_back( ant );
    }
    
    // run simulation
    for (int currentStep = 0; currentStep < nSteps; ++currentStep)
    {
        distributeAnts();
        moveAnts();
        updatePheromone();
    }
}



void Colony::moveAnts()
{
    //moveUntilAllDead();
    moveOneStep();
    printDebugImage();
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
    //printDebugImage();
}

void Colony::moveOneStep()
{
    int nAnts = Parameters::numberOfAnts;
    
    #pragma omp parallel for
    for (int a = 0; a < nAnts; ++a)
    {
        //printf("moving ant #%d...\n", a);
        Ant* ant = _ants[a];

        if (ant->isAlive())
        {
            ant->move();
        }
    }
}

void Colony::updatePheromone()
{
    _environment->evaporatePheromone();
    
    int nAnts = _ants.size();
    for (int a = 0; a < nAnts; ++a)
    {
        _ants[a]->depositPheromone();
    }
}

Image* Colony::getPheromoneImage()
{
    
    Image* img = _environment->getPheromoneImage();
    return img;
}

bool Colony::available( Point point, Ant& ant )
{
    return false;
}

Image* Colony::postProcessing( Image* img )
{
    Image* out = imgCopy( img );
    
    // normalization
    imgNormalize( out, Parameters::stdDev );
    
    // gamma correction
    imgGamma( out, Parameters::gammaFactor );
    
    // binarization
    if (Parameters::binarization)
    {
        imgBin( out, Parameters::binThreshold );
    }
    
    return out;
}

void Colony::printDebugImage()
{
    static int step = 0;
    Image* imgIn = _environment->getPheromoneImage();
    imgNormalize( imgIn, 2 );
    
    Image* img = imgCreate( _environment->getWidth(), _environment->getHeight(), 3 );
    
    for (int x = 0; x < _environment->getWidth(); ++x)
    {
        for (int y = 0; y < _environment->getHeight(); ++y)
        {
            float lum = imgGetPixelf( imgIn, x, y );
            imgSetPixelf( img, x, y, lum );
        }
    }  
    
    int nAnts = _ants.size();
    for (int i = 0; i < nAnts; ++i)
    {
        Ant* ant = _ants[i];
        if (ant->isAlive())
        {
            // each ant has a color
            imgSetPixel3f( img, ant->_position.x, ant->_position.y, 1.0f - i/(float)nAnts, (0.0f + i) / nAnts, 0.0f );
            
            //all ants are red
            //imgSetPixel3f( img, ant->_position.x, ant->_position.y, 1,0,0 );
        }
        else
        {
            imgSetPixel3f( img, ant->_position.x, ant->_position.y, 0.0f, 0.0f, 1.0f );
        }
    }

    
    //postProcessing( &img );
    char filename[150];
    sprintf( filename, "debugImg/debugImage%04d.bmp", ++step );
    imgWriteBMP( filename, img );
    imgDestroy( img );
}
