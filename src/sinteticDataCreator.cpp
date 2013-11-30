#include <stdio.h>
#include<stdlib.h>
#include "image.h"

int main( int argc, char** argv )
{
    // Check input arguments
    if (argc != 6)
    {
        printf( "ERROR: Wrong number of parameters.\n" );
        printf( "Expecting: \n path to .PFM image, "
                "\n path to mask, \n path to output, "
                "\n trace to be repeated, \n fault jump\n" );
        return 1;
    }

    // Read input arguments
    Image* input = imgReadPFM( argv[1] );
    Image* mask = imgReadBMP( argv[2] );
    char* outputPath = argv[3];
    int traceId = atoi( argv[4] );
    int jump = atoi( argv[5] );

    // Fetch the dimensions of the input and output images
    int inW = imgGetWidth( input );
    int inH = imgGetHeight( input );
    int outW = imgGetWidth( mask );
    int outH = imgGetHeight( mask );

    // Check whether the trace wanted is within the input image boundaries
    if (traceId < 0 || traceId >= inW)
    {
        printf( "ERROR: Trace wanted is out of bounds!\n" );
        return 1;
    }

    // Check whether the mask is higher than the input image
    if (outH > inH)
    {
        printf( "ERROR: Mask is higher than input image!\n" );
        return 1;
    }

    // Write the output .PFM image
    Image* output = imgCreate( outW, outH, 1 );
    for (int y = 0; y < outH; ++y)
    {
        float traceValue = imgGetPixelf( input, traceId, y );
        int jumpY = (y + jump) % inH;
        float jumpedTraceValue = imgGetPixelf( input, traceId, jumpY );
        for (int x = 0; x < outW; ++x)
        {
            if (imgGetPixelf( mask, x, y ) > 0)
                imgSetPixelf( output, x, y, jumpedTraceValue );
            else
                imgSetPixelf( output, x, y, traceValue );
        }
    }
    imgWritePFM( outputPath, output );

    // Write the .BMP version of the input image for checking
    float med = imgComputeMean( input );
    float stdDev = sqrt( imgComputeVariance( input, med ) );
    imgClipPositiveOutliers( input, med + stdDev + stdDev );
    imgNormalize( input );
    imgWriteBMP( (char*) "inputView.bmp", input );

    // Write the .BMP version of the output image for checking
    med = imgComputeMean( output );
    stdDev = sqrt( imgComputeVariance( output, med ) );
    imgClipPositiveOutliers( output, med + stdDev + stdDev );
    imgNormalize( output );
    imgWriteBMP( (char*) "outputView.bmp", output );
}
