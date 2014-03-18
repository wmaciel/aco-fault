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
float Parameters::stdDev       = 2.0f;
float Parameters::gammaFactor  = 1.0f;
bool  Parameters::binarization = false;
float Parameters::binThreshold = 0.0f;

Image* Parameters::postProcessing(Image* img)
{
    if (!img) return NULL;
    
    Image* out = imgCopy( img );
    
    // normalization
    imgNormalize( out, stdDev );
    
    // gamma correction
    imgGamma( out, gammaFactor );
    
    // binarization
    if (binarization)
    {
        imgBin( out, binThreshold );
    }
    
    return out;
}

