#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "CorrelationCalculator.h"
#include "PearsonCorrelationCalculator.h"

#define TRACE_SIZE 7

float* buildTrace( Image* img, int x, int y, int traceSize )
{
    int h = imgGetHeight( img );
    float* trace = new float[traceSize];
    int j = 0;
    for (int t = y - traceSize/2; t <= y + traceSize/2; ++t)
    {
        int i = t;
        if (i < 0) i = 0;
        if (i >= h) i = h - 1;
        trace[j] = imgGetPixelf( img, x, i );
        j++;
    }
    return trace;
}

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
    
    imgAssert(input);
    imgAssert(mask);

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
    Image* amplitudeOutput = imgCreate( outW, outH, 1 );
    for (int y = 0; y < outH; ++y)
    {
        float traceValue = imgGetPixelf( input, traceId, y );
        int jumpY = (y + jump) % inH;
        float jumpedTraceValue = imgGetPixelf( input, traceId, jumpY );
        for (int x = 0; x < outW; ++x)
        {
            float factor = 0.9f + 0.1f * rand() / (float)RAND_MAX;
            
            if (imgGetPixelf( mask, x, y ) > 0)
                imgSetPixelf( amplitudeOutput, x, y, jumpedTraceValue * factor );
            else
                imgSetPixelf( amplitudeOutput, x, y, traceValue * factor );
        }
    }
    imgAssert(amplitudeOutput);
    imgWritePFM( outputPath, amplitudeOutput );

    // Write the .BMP version of the input image for checking
    imgNormalize( input, 2 );
    imgWriteBMP( (char*) "../data/inputView.bmp", input );

    // Write the .BMP version of the amplitude image for checking
    imgNormalize( amplitudeOutput, 2 );
    imgWriteBMP( (char*) "../data/outputView.bmp", amplitudeOutput );

    //Apply cross correlation on the sintetic data
    CorrelationCalculator* calculator = new PearsonCorrelationCalculator( TRACE_SIZE );
    Image* correlationOutput = imgCreate( outW, outH, 1 );
    for (int x = 0; x < outW - 1; ++x) //Última coluna não possui vizinho
    {
        for (int y = 0; y < outH; ++y)
        {
            float* trace = buildTrace( amplitudeOutput, x, y, TRACE_SIZE );
            float* nextTrace = buildTrace( amplitudeOutput, x + 1, y, TRACE_SIZE );
            float correlation = calculator->computeCorrelation( trace, nextTrace );

            imgSetPixelf( correlationOutput, x, y, 1.0f - correlation );
        }
    }
    imgAssert(correlationOutput);

    //Write .PFM image of the attribute image
    imgWritePFM( (char*) "../data/attributeOutput.pfm", correlationOutput );

    //Write .BMP image of the attribute image
    imgNormalize( correlationOutput, 2 );
    imgWriteBMP( (char*) "../data/attributeOutput.bmp", correlationOutput );

}
