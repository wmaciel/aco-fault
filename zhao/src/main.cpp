#include <stdio.h>
#include <stdlib.h>
#include "DirectionalField.h"
#include "image.h"
#include "Colony.h"

//void basicTest()
//{
//    // fill data
//    float* data = new float[64];
//    for (int x = 0; x < 8; ++x)
//    {
//        for (int y = 0; y < 8; ++y)
//        {
//            int i = x + y * 8;
//            //if (x==3) data[i] = 0.8;
//            if (y==4) data[i] = 1.0;
//            //if (x==5) data[i] = 0.9;
//            //data[i] = 0;
//        }
//    }
//
//    DirectionalField* field = new DirectionalField(data, 8, 8, 2);
//
//    for (int y = 0; y < 8; ++y)
//    {
//        for (int x = 0; x < 8; ++x)
//        {
//            int i = x + y * 8;
//            printf("%.3f\t", data[i]);
//        }
//        printf("\n");
//    }
//
//    field->debugPrint();
//}
//
void testImage( char* fileIn, char* fileDir, char* fileCoh, char* fileMask )
{
    Image* img = imgReadBMP( fileIn );
    Image* grey = imgGrey( img );
    Image* greyGauss = imgCopy( grey );
    imgGauss( greyGauss, grey );

    DirectionalField* field = new DirectionalField( greyGauss, 0, 8 );

    int width = imgGetWidth(greyGauss);
    int height = imgGetHeight(greyGauss);
    Image* dir = imgCreate( width, height, 3 );
    Image* coh = imgCreate( width, height, 3 );
    Image* mask = imgCreate( width, height, 3 );
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            float dirX;
            float dirY;
            field->getDirection( x, y, dirX, dirY );
            float lumCoh;
//            float lumX;
//            float lumY;

            imgSetPixel3f( dir, x, y, 1,1,1 );

            if (dirX == 0.0f && dirY == 0.0f)
            {
                imgSetPixel3f( dir, x, y, 1.0, 1.0, 0.0 );
            }
            else
            {
                if (dirX < 0)
                {
                    dirY *= -1;
                    dirX *= -1;
                }

//                lumX = fabs( dirX );
//                lumY = fabs( dirY );
                float lum = ( dirY + 1.0 ) * 0.5f;
                imgSetPixel3f( dir, x, y, lum, lum, lum);
            }

            lumCoh = field->getCoherence( x, y );
            imgSetPixel3f( coh, x, y, lumCoh, lumCoh, lumCoh );
            if (lumCoh >= 0.35f)
            {
                imgSetPixel3f( mask, x, y, 1.0f, 1.0f, 1.0f );
            }
            else
            {
                imgSetPixel3f( mask, x, y, 0.0f, 0.0f, 0.0f );
            }
        }
    }

    imgWriteBMP( (char*)"gaussiana.bmp", greyGauss );
    imgWriteBMP( fileDir, dir );
    imgWriteBMP( fileCoh, coh );
    imgWriteBMP( fileMask, mask );
}

void whoLetTheAntsOut( int argc, char** argv )
{
    //srand( time(NULL) );
    Image* imgIN = imgReadBMP( (char*)"../data/sintetico/sinteticoPiece.bmp" );
    Image* imgGray = imgGrey( imgIN );
    imgDestroy( imgIN );
    Image* imgGrayGauss = imgCopy( imgGray );
    imgGauss( imgGrayGauss, imgGray );
    imgDestroy( imgGray );
    Colony* colony = new Colony( imgGrayGauss );
    colony->run( 100 );
    imgDestroy( imgGrayGauss );
}

void testImageMorphology()
{
    Image* aux = imgReadBMP( (char*)"..data/morphology/print_mask.bmp" );
    Image* in = imgGrey( aux );
    Image* kernel = imgReadBMP( (char*)"..data/morphology/kernelBig.bmp" );

    Image* eroded = imgErode( in, kernel );
    Image* opened = imgDilate( eroded, kernel );
    Image* openedDilated = imgDilate( opened, kernel );
    Image* closed = imgErode( openedDilated, kernel );

    imgWriteBMP( (char*)"..data/morphology/eroded.bmp", eroded );
    imgWriteBMP( (char*)"..data/morphology/opened.bmp", opened );
    imgWriteBMP( (char*)"..data/morphology/openedDilated.bmp", openedDilated );
    imgWriteBMP( (char*)"..data/morphology/closed.bmp", closed );
}

int main( int argc, char** argv )
{
    //testImage( "print.bmp", "print_dir.bmp", "print_coh.bmp", "print_mask.bmp" );
    //basicTest();
    whoLetTheAntsOut( argc, argv );
    //testImageMorphology();
    return 0;
}
