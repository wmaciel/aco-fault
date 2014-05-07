#include <cstdio>
#include <stdlib.h>

#include "image.h"
#include "Parameters.h"
#include "DirectionalField.h"

#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

bool isLocalMaximum( Image* img, int x, int y, float dx, float dy )
{
    if (x <= 0 ||
        x >= imgGetWidth(img)-1 ||
        y <= 0 ||
        y >= imgGetHeight(img)-1)
    {
        return false;
    }
    
    float cosN,  // same as cosS
          cosE,  // same as cosW
          cosNE, // same as cosSW
          cosNW; // same as cosSE
    
    cosN  =  dx*0     + dy*1;
    cosE  =  dx*1     + dy*0;
    cosNE =  dx*0.707 + dy*0.707;
    cosNW = -dx*0.707 + dy*0.707;
    
    float maxCos = MAX( cosN, MAX(cosE, MAX(cosNE, cosNW)));
    
    float p  = imgGetPixelf( img, x, y );
    if (cosN == maxCos)
    {    
        float pN = imgGetPixelf( img, x, y + 1 );
        float pS = imgGetPixelf( img, x, y - 1 );
        if (p > pN && p > pS)
        {
            return true;
        }
    }
    else if(cosE == maxCos)
    {
        float pE = imgGetPixelf( img, x+1, y );
        float pW = imgGetPixelf( img, x-1, y );
        if (p > pE && p > pW)
        {
            return true;
        }
    }
    else if(cosNE == maxCos)
    {
        float pNE = imgGetPixelf( img, x+1, y+1 );
        float pSW = imgGetPixelf( img, x-1, y-1 );
        if (p > pNE && p > pSW)
        {
            return true;
        }
    }
    else if(cosNW == maxCos)
    {
        float pNW = imgGetPixelf( img, x-1, y+1 );
        float pSE = imgGetPixelf( img, x+1, y-1 );
        if (p > pNW && p > pSE)
        {
            return true;
        }
    }
    else
    {
        printf( "Well, that is embarrassing...\n" );
        exit(-1);
    }
    
    return false;
}

Image* applyEdgeThinnig( Image* imgIn )
{
    // The radiuses are not important, since we'll not use the mask
    DirectionalField* dirField = new DirectionalField( imgIn, 1, 1 );
    
    dirField->debugImages();
    
    Image* imgOut = imgCopy( imgIn );
    
    int imgWidth  = imgGetWidth( imgIn );
    int imgHeight = imgGetHeight( imgIn );
    for (int x = 0; x < imgWidth; ++x)
    {
        for (int y = 0; y < imgHeight; ++y)
        {
            float dx, dy;
            dirField->getGrandientDirection( x, y, dx, dy );
            if (!isLocalMaximum( imgIn, x, y, dx, dy ))
            {
                imgSetPixelf( imgOut, x, y, 0 );
            }
        }
    }
    
    return imgOut;
}

int main(int argc, char** argv)
{
    if (argc != 6)
    {
        printf( "Wrong usage\n" );
        printf( "Expecting:\n" );
        printf( "- input image path\n" );//1
        printf( "- output image path\n" );//2
        printf( "- gauss width\n" );//3
        printf( "- gauss height\n" );//4
        printf( "- gauss radius\n" );//5
        exit( 0 );
    }
    
    Image* img = imgReadBMP( argv[1] );
    Parameters::widthGauss = atoi( argv[3] );
    Parameters::heightGauss = atoi( argv[4] );
    Parameters::radiusGauss = atoi( argv[5] );
    
    if(imgGetDimColorSpace(img)!= 1)
    {
        Image* grey = imgGrey( img );
        imgDestroy( img );
        img = grey;
    }
    
    Image* out = applyEdgeThinnig( img );
    
    imgWriteBMP( argv[2], out );
    
    return 0;
}