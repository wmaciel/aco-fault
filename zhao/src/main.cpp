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
            //if (x==3) data[i] = 0.8;
            if (y==4) data[i] = 1.0;
            //if (x==5) data[i] = 0.9;
            //data[i] = 0;
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
    DirectionalField* field = new DirectionalField( data, width, height, 10 );

    Image* dir = imgCreate( width, height, 3 );
    Image* coh = imgCreate( width, height, 3 );
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            float dirX = field->getXDirection( x, y );
            float dirY = field->getYDirection( x, y );
            float lumCoh;
            float lumX;
            float lumY;

            imgSetPixel3f( dir, x, y, 1,1,1 );

            if (dirX == 0.0f && dirY == 0.0f)
            {
                imgSetPixel3f( dir, x, y, 0.0, 0.0, 0.0 );
            }
            else
            {
                lumX = fabs( dirX );
                lumY = fabs( dirY );
                imgSetPixel3f( dir, x, y, 0.0f, lumX, lumY );
            }

            lumCoh = field->getCoherence( x, y );
            imgSetPixel3f( coh, x, y, lumCoh, lumCoh, lumCoh );
        }
    }

    imgWriteBMP( fileDir, dir );
    imgWriteBMP( fileCoh, coh );
}

int main( int argc, char** argv )
{
    testImage( "finger.bmp", "fingerDIR.bmp", "fingerCOH.bmp" );
    //basicTest();
    return 0;
}