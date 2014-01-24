/*
 * File:   Parameters.h
 * Author: keoma
 *
 * Created on September 1, 2013, 1:18 PM
 */

#ifndef PARAMETERS_H
#define	PARAMETERS_H

/**
 * Pheromone present in all of the pixels of the pheromone image. This must be a
 * non-zero value, otherwise it would shutdown the searches on the pixels
 */
#define INITIAL_PHEROMONE 0.1f

/**
 * How much of the pheromone evaporates after each step. Must be between 0 and 1
 */
#define EVAPORATION_RATE 0.1f

/**
 * The minimum value of pheromone present at any given pixel. This is used to 
 * not shut down search paths
 */
#define MIN_PHEROMONE 0.01f

/**
 * The relative importance of the pheromone value in comparison with the 
 * visibility value
 */
#define PHEROMONE_WEIGHT 1.0f

/**
 * The relative importance of the visibility value in comparison with the 
 * pheromone value
 */
#define VISIBILITY_WEIGHT 5.0f

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
#define STEP_LENGTH 5

/**
 * The amount of pheromone each ant deposits at each pixel it has traversed
 */
#define PHEROMONE_CONSTANT 0.02f

/**
 * How wide around the direction of the direction field can an ant see
 */
#define FOV_ANGLE 15.0f

/**
 * Used by the DIrectional Field to create the coherence mask. Values below the 
 * treshold are seen as dead ends. Any ant that steps on a pixel like that will 
 * increase it's abnormal step counter
 */
#define COHERENCE_TRESHOLD 0.5f

/**
 * Maximum number of abnormal steps an ant can have before dying
 */
#define MAX_ABNORMAL_STEPS 5

/**
 * Maximum number of steps an Ant can have. It is used as a timeout for ants 
 * that may end up on an infinite loop
 */
#define MAX_STEPS 100

/**
 * The maximum amount of pheromone allowed to accumulate in a single pixel
 */
#define MAX_PHEROMONE 1000.0f

/**
 * Number of ants inserted into the environment. This value is not used if the 
 * ants a distributed by blocks.
 */
#define NUMBER_OF_ANTS 200

/**
 * Path to the kernel image used for the opening and closing of the coherence 
 * mask image
 */
#define KERNEL_PATH ((char*)"/home/keoma/Dropbox/PUC/Mestrado/antColonyOptimization/src/zhao/data/masks/kernel3.bmp")

/**
 * Width of the Gaussian window used to build the directional field
 */
#define DIR_FIELD_GAUSSIAN_WINDOW_HALF_WIDTH 2

/**
 * Height of the Gaussian window used to build the directional field
 */
#define DIR_FIELD_GAUSSIAN_WINDOW_HALF_HEIGHT 2

#endif	/* PARAMETERS_H */

