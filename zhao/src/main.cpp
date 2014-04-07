#include <stdio.h>
#include <stdlib.h>
#include "DirectionalField.h"
#include "image.h"
#include "Colony.h"

void whoLetTheAntsOut( int argc, char** argv )
{
    srand( time(NULL) );
    Image* input = imgReadPFM( (char*)"/home/keoma/Dropbox/PUC/Mestrado/aco-fault/zhao/data/sintetic_clean/sintetic_clean_attribute.pfm" );
    imgAssert( input );
    Colony* colony = new Colony( input );
    colony->run( 50 );
    imgDestroy( input );
}

int main( int argc, char** argv )
{
    whoLetTheAntsOut( argc, argv );
    return 0;
}
