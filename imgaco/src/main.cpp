
#include <iostream>
#include <stdlib.h>

#include "image.h"
#include "Colony.h"

int main( int argc, char** argv )
{
    srand(time(0));

    if (argc != 4)
    {
        std::cerr << "Usage: imgaco [input.bmp] [output.bmp] [number of iterations]\n";
        return 0;
    }

    Image* input = imgReadBMP( argv[1] );
    if ( !input )
    {
        std::cerr << "Image " << argv[1] << "could not be loaded\n";
        return 0;
    }

    int nAnts = ( sqrt( imgGetWidth( input ) * imgGetHeight( input ) ) + 0.5f );

    std::cerr << "Running with " << nAnts << " ants...\n";

    Colony colony( input, nAnts );
    int nIterations = strtol( argv[3], NULL, 10 );
    colony.run( nIterations );

    Image* pheromone = colony.getPheromoneImage();

    imgWriteBMP( argv[2], pheromone );

    return 0;
}
