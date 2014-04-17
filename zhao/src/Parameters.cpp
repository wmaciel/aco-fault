#include "Parameters.h"

//ants parameters
float Parameters::pheromoneWeight   = 1.0f;
float Parameters::attributeWeight   = 2.0f;
int   Parameters::stepLength        = 5;
float Parameters::fov               = 30;
int   Parameters::maxSteps          = 100;
int   Parameters::maxAbnormalSteps  = 10;
int   Parameters::numberOfAnts      = 500;
float Parameters::pheromoneConstant = 0.02f;

//pheromone parameters
float Parameters::minPheromone    = 0.01f;
float Parameters::maxPheromone    = 1000.0f;
float Parameters::initPheromone   = 0.1f;
float Parameters::evaporationRate = 0.05f;
int   Parameters::turns           = 50;

//direction parameters
int   Parameters::widthGauss   = 1;
int   Parameters::heightGauss  = 3;
int   Parameters::kernelRadius = 1;
float Parameters::cohTreshold  = 0.5f;

//post processing parameters
float Parameters::postStdDev     = 2.0f;
float Parameters::gammaFactor    = 1.0f;
bool  Parameters::binarization   = false;
float Parameters::binThreshold   = 0.0f;
int   Parameters::openKernelRad  = 0;
int   Parameters::closeKernelRad = 0;

//pre processing parameters
float Parameters::preStdDev          = 2.0f;
bool Parameters::manualNormalization = false;
float Parameters::manualMax          = 1.0f;
float Parameters::manualMin          = 0.0f;
bool Parameters::invertColors        = false;

Image* Parameters::postProcessing(Image* img)
{
    if (!img) return NULL;
    
    Image* out = imgCopy( img );
    
    // normalization
    imgNormalize( out, postStdDev );
    
    // gamma correction
    imgGamma( out, gammaFactor );
    
    // binarization
    if (binarization)
    {
        //binarize
        imgBin( out, binThreshold );
        
        //open
        Image* openKernel = buildCircleImage( openKernelRad );
        imgErode( out, openKernel );
        imgDilate( out, openKernel );
        imgDestroy( openKernel );
        
        //close
        Image* closeKernel = buildCircleImage( closeKernelRad );
        imgDilate( out, closeKernel );
        imgErode( out, closeKernel );
        imgDestroy( closeKernel );
    }
    
    return out;
}

Image* Parameters::buildCircleImage( unsigned int radius )
{
    unsigned int diameter = radius + radius + 1;
    Image* out = imgCreate( diameter, diameter, 1 );
    float luminance = 0.0f;
    
    for (unsigned int x = 0; x < diameter; ++x)
    {
        for (unsigned int y = 0; y < diameter; ++y)
        {
            unsigned int dx = std::abs<int>( x - radius );
            unsigned int dy = std::abs<int>( y - radius );
            float distance = sqrt( dx * dx + dy * dy );
            if (distance <= (float) radius)
            {
                luminance = 1.0f;
            }
            else
            {
                luminance = 0.0f;
            }
            
            imgSetPixelf( out, x, y, luminance );
        }
    }
    
    return out;
}

