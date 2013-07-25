#include <stdio.h>
#include <stdlib.h>
#include "DirectionalField.h"
#include "image.h"

void basicTest()
{
    // fill data
    float* data = new float[64];
    for (int x = 0; x < 8; ++x)
    {
        for (int y = 0; y < 8; ++y)
        {
            int i = x + y * 8;
            if (x==y) data[i] = 0.9;
        }
    }

    DirectionalField* field = new DirectionalField(data, 8, 8, 2);

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            int i = x + y * 8;
            printf("%.3f\t", data[i]);
        }
        printf("\n");
    }

    field->debugPrint();
}

void testImage( char* fileIn, char* fileDir, char* fileCoh )
{
    Image* img = imgReadBMP( fileIn );
    Image* grey = imgGrey( img );
    
    float* data = imgGetData( grey );
    int width = imgGetWidth(grey);
    int height = imgGetHeight(grey);
    DirectionalField* field = new DirectionalField( data, width, height, 3 );
    
    Image* dir = imgCreate( width, height, 3 );
    Image* coh = imgCreate( width, height, 3 );
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            //float dirX = field->getXDirection( x, y );
            float dirY = field->getYDirection( x, y );
            float lum = 0.5 * (dirY+1.0);
            imgSetPixel3f( dir, x, y, lum, lum, lum );
            
            lum = field->getCoherence( x, y );
            imgSetPixel3f( coh, x, y, lum, lum, lum );
        }
    }
    
    imgWriteBMP( fileDir, dir );
    imgWriteBMP( fileCoh, coh );
}

int main( int argc, char** argv )
{
    testImage( "/home/eliana/Desktop/wmaciel/img.bmp", "/home/eliana/Desktop/wmaciel/imgDIR.bmp", "/home/eliana/Desktop/wmaciel/imgCOH.bmp" );
    //basicTest();
    return 0;
}