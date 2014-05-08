#include <cstdio>
#include <stdlib.h>
#include <vector>

#include "image.h"
#include "Parameters.h"
#include "DirectionalField.h"

#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

int scale = 1;

bool isLocalMaximum( Image* img, int x, int y, float dx, float dy )
{   
    if (x - scale <= 0 ||
        x + scale >= imgGetWidth(img)-1 ||
        y - scale <= 0 ||
        y + scale >= imgGetHeight(img)-1 ||
        (dx == 0 && dy == 0) )
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
    if (p==0) return false;
    
    std::vector<float> pixels;
    
    if (cosN == maxCos)
    {   
        for ( int s = scale; s > 0; --s )
        {
            pixels.push_back( imgGetPixelf( img, x, y+s ) );
            pixels.push_back( imgGetPixelf( img, x, y-s ) );
        }
    }
    else if(cosE == maxCos)
    {
        for ( int s = scale; s > 0; --s )
        {
            pixels.push_back( imgGetPixelf( img, x+s, y ) );
            pixels.push_back( imgGetPixelf( img, x-s, y ) );
        }
    }
    else if(cosNE == maxCos)
    {
        for ( int s = scale; s > 0; --s )
        {
            pixels.push_back( imgGetPixelf( img, x+s, y+s ) );
            pixels.push_back( imgGetPixelf( img, x-s, y-s ) );
        }
    }
    else if(cosNW == maxCos)
    {
        for ( int s = scale; s > 0; --s )
        {
            pixels.push_back( imgGetPixelf( img, x-s, y+s ) );
            pixels.push_back( imgGetPixelf( img, x+s, y-s ) );
        }
    }
    else
    {
        printf( "Well, that is embarrassing...\n" );
        exit(-1);
    }
    
    for (unsigned int i = 0; i < pixels.size(); ++i)
    {
        if (pixels[i] >= p)
        {
            return false;
        }
    }
    
    return true;
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
    if (argc != 7)
    {
        printf( "Wrong usage\n" );
        printf( "Expecting:\n" );
        printf( "- input image path\n" );//1
        printf( "- output image path\n" );//2
        printf( "- gauss width\n" );//3
        printf( "- gauss height\n" );//4
        printf( "- gauss radius\n" );//5
	printf( "- edge half width\n" );//6
        exit( 0 );
    }
    
    std::string inputPath = argv[1];
    Image* img = 0;
    if (std::string::npos == inputPath.find(".pfm"))
    {
        img = imgReadBMP( argv[1] );
    }
    else
    {
        img = imgReadPFM( argv[1] );
    }
    
    Parameters::widthGauss = atoi( argv[3] );
    Parameters::heightGauss = atoi( argv[4] );
    Parameters::radiusGauss = atoi( argv[5] );
    scale = atoi( argv[6] );
    
    if(imgGetDimColorSpace(img)!= 1)
    {
        Image* grey = imgGrey( img );
        imgDestroy( img );
        img = grey;
    }
    
    //imgInvertColors( img );
    
    Image* out = applyEdgeThinnig( img );
    imgDestroy( img );
    imgWriteBMP( argv[2], out );
    
    return 0;
}
