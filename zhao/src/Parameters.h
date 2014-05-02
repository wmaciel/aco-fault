/*
 * File:   Parameters.h
 * Author: keoma
 *
 * Created on September 1, 2013, 1:18 PM
 */

#ifndef PARAMETERS_H
#define	PARAMETERS_H

#include "image.h"

class Parameters
{
public:
    Parameters();;
    ~Parameters(){};
    
    static Image* preProcessing( Image* img );
    static Image* postProcessing( Image* img );
    static Image* buildCircleImage( unsigned int radius );
    
    //ants parameters
    static float pheromoneWeight;
    static float attributeWeight;
    static int stepLength;
    static float fov;
    static int maxSteps;
    static int maxAbnormalSteps;
    static int numberOfAnts;
    static float pheromoneConstant;
    
    //pheromone parameters
    static float minPheromone;
    static float maxPheromone;
    static float initPheromone;
    static float evaporationRate;
    static int turns;
    
    //direction parameters
    static int widthGauss;
    static int heightGauss;
    static int radiusGauss;
    static int kernelRadius;
    static float cohTreshold;
    
    //post processing parameters
    static float postStdDev;
    static float gammaFactor;
    static bool binarization;
    static float binThreshold;
    static int openKernelRad;
    static int closeKernelRad;
    
    //pre processing parameters
    static float preStdDev;
    static bool manualNormalization;
    static float manualMax;
    static float manualMin;
    static bool invertColors;
};

/**
 * Pheromone present in all of the pixels of the pheromone image. This must be a
 * non-zero value, otherwise it would shutdown the searches on the pixels
 */
#define INITIAL_PHEROMONE (Parameters::initPheromone)

/**
 * How much of the pheromone evaporates after each step. Must be between 0 and 1
 */
#define EVAPORATION_RATE (Parameters::evaporationRate)

/**
 * The minimum value of pheromone present at any given pixel. This is used to 
 * not shut down search paths
 */
#define MIN_PHEROMONE (Parameters::minPheromone)

/**
 * The relative importance of the pheromone value in comparison with the 
 * visibility value
 */
#define PHEROMONE_WEIGHT (Parameters::pheromoneWeight)

/**
 * The relative importance of the visibility value in comparison with the 
 * pheromone value
 */
#define VISIBILITY_WEIGHT (Parameters::attributeWeight)

/**
 * Number of steps an ant remembers taking.
 */
#define MEMORY_ 100

/**
 * Used for the block wise ant distribution technique presented by Zhao. This is
 * the size of one side of the block
 */
#define BLOCK_SIZE 30

/**
 * How many long can an ant "leap" in one step
 */
#define STEP_LENGTH (Parameters::stepLength)

/**
 * The amount of pheromone each ant deposits at each pixel it has traversed
 */
#define PHEROMONE_CONSTANT (Parameters::pheromoneConstant)

/**
 * How wide around the direction of the direction field can an ant see
 */
#define FOV_ANGLE (Parameters::fov)

/**
 * Used by the DIrectional Field to create the coherence mask. Values below the 
 * treshold are seen as dead ends. Any ant that steps on a pixel like that will 
 * increase it's abnormal step counter
 */
#define COHERENCE_TRESHOLD (Parameters::cohTreshold)

/**
 * Maximum number of abnormal steps an ant can have before dying
 */
#define MAX_ABNORMAL_STEPS (Parameters::maxAbnormalSteps)

/**
 * Maximum number of steps an Ant can have. It is used as a timeout for ants 
 * that may end up on an infinite loop
 */
#define MAX_STEPS (Parameters::maxSteps)

/**
 * The maximum amount of pheromone allowed to accumulate in a single pixel
 */
#define MAX_PHEROMONE (Parameters::maxPheromone)

/**
 * Number of ants inserted into the environment. This value is not used if the 
 * ants a distributed by blocks.
 */
#define NUMBER_OF_ANTS (Parameters::numberOfAnts)

/**
 * Width of the Gaussian window used to build the directional field
 */
#define DIR_FIELD_GAUSSIAN_WINDOW_HALF_WIDTH (Parameters::widthGauss)

/**
 * Height of the Gaussian window used to build the directional field
 */
#define DIR_FIELD_GAUSSIAN_WINDOW_HALF_HEIGHT (Parameters::heightGauss)

#endif	/* PARAMETERS_H */

