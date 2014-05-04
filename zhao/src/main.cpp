#include <stdio.h>
#include <stdlib.h>
#include "DirectionalField.h"
#include "image.h"
#include "Colony.h"

void whoLetTheAntsOut( int argc, char** argv )
{
    srand( time(NULL) );
    Image* input = imgReadPFM( (char*)"/home/keoma/Desktop/tst/inline_279/variance/mine/04_dist/input.pfm" );
    imgAssert( input );
    imgInvertColors( input );
    Colony* colony = new Colony( input );
    colony->run( 1000 );
    imgDestroy( input );
}

int main( int argc, char** argv )
{
    whoLetTheAntsOut( argc, argv );
    return 0;
}
