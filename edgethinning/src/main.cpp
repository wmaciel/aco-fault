#include <cstdio>

#include "image.h"

int main(int argc, char** argv)
{
    if (argc != 6)
    {
        printf( "Wrong usage\n" );
        printf( "Expecting:\n" );
        printf( "- input image path\n" );
        printf( "- output image path\n" );
        printf( "- gauss width\n" );
        printf( "- gauss height\n" );
        printf( "- gauss radius\n" );
        exit( 0 );
    }
    
    Image* img = imgReadPFM( argv[1] );
    
}