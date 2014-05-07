/*
 * File:   DirectionalField.cpp
 * Author: keoma
 *
 * Created on July 19, 2013, 6:41 PM
 */

#include "DirectionalField.h"
#include "image.h"
#include "Parameters.h"

#include <math.h>
#include <stdio.h>

DirectionalField::DirectionalField( Image* img, int openKernelRadius, int closeKernelRadius )
{
    _width = imgGetWidth( img );
    _height = imgGetHeight( img );
    _coherence = imgCreate( _width, _height, 1 );
    _coherenceMask = imgCreate( _width, _height, 1 );
    _direction = imgCreate( _width, _height, 3 );
    _ortho = imgCreate( _width, _height, 3 );
    _coherenceThreshold = COHERENCE_TRESHOLD;
    _openKernelRadius = openKernelRadius;
    _closeKernelRadius = closeKernelRadius;
    
    _gaussianKernel = imgCreateGaussianKernel( DIR_FIELD_GAUSSIAN_WINDOW_HALF_WIDTH, DIR_FIELD_GAUSSIAN_WINDOW_HALF_HEIGHT, Parameters::radiusGauss );

    int size = _width * _height;
    _gxMatrix = new float[size];
    _gyMatrix = new float[size];
    _gxxMatrix = new float[size];
    _gyyMatrix = new float[size];
    _gxyMatrix = new float[size];

    float* data = imgGetData( img );

    buildHorizontalDerivativeMatrix( data );
    buildHorizontalWindowedDerivativeMatrix();
    buildVerticalDerivativeMatrix( data );
    buildVerticalWindowedDerivativeMatrix();
    buildCrossedWindowedDerivativeMatrix();
    buildCoherenceImage();
    buildCoherenceMask();
    buildDirectionImage();
    buildGradientDirectionImage();
    
    delete[] _gxMatrix;
    delete[] _gyMatrix;
    delete[] _gxxMatrix;
    delete[] _gyyMatrix;
    delete[] _gxyMatrix;
    
    _gxMatrix  = 0;
    _gyMatrix  = 0;
    _gxxMatrix = 0;
    _gyyMatrix = 0;
    _gxyMatrix = 0;
}



DirectionalField::~DirectionalField()
{
    imgDestroy(_coherence);
    imgDestroy(_coherenceMask);
    imgDestroy(_direction);
    imgDestroy(_gaussianKernel);
    
    if (_gxMatrix)  delete[] _gxMatrix;
    if (_gyMatrix)  delete[] _gyMatrix;
    if (_gxxMatrix) delete[] _gxxMatrix;
    if (_gyyMatrix) delete[] _gyyMatrix;
    if (_gxyMatrix) delete[] _gxyMatrix;
}



int DirectionalField::pixel( int x, int y )
{
    return x + _width * y;
}



float DirectionalField::dx( int x, int y, float* data )
{
    int front  = pixel( x + 1, y );
    int back   = pixel( x - 1, y );
    int center = pixel( x, y );

    if ( x == _width-1 )// backward difference
    {
        return data[center] - data[back];
    }
    else if ( x == 0 )  // forward difference
    {
        return data[front] - data[center];
    }
    else                 // central difference
    {
        return ( data[front] - data[back] ) / 2.0f;
    }
}

float DirectionalField::dy( int x, int y, float* data )
{
    int top    = pixel( x, y + 1  );
    int bottom = pixel( x, y - 1  );
    int center = pixel( x, y );

    if ( y == _height-1 ) // backward difference
    {
        return data[center] - data[bottom];
    }
    else if ( y == 0 )    // forward difference
    {
        return data[top] - data[center];
    }
    else                  // central difference
    {
        return ( data[top] - data[bottom] ) / 2.0f;
    }
}

float DirectionalField::computeCoherence( int pixel )
{
    float gxxMinusGyy = _gxxMatrix[pixel] - _gyyMatrix[pixel];
    float hxy = gxxMinusGyy * gxxMinusGyy + 4 * _gxyMatrix[pixel] * _gxyMatrix[pixel];

    float divisor = ( _gxxMatrix[pixel] + _gyyMatrix[pixel] );
    if (divisor == 0.0f)
    {
        return 0.0f;
    }

    return sqrt( hxy ) / divisor;
}

void DirectionalField::computeDirection( int pixel, float& dirX, float& dirY )
{
    float gxxMinusGyy = _gxxMatrix[pixel] - _gyyMatrix[pixel];
    float hxy = gxxMinusGyy * gxxMinusGyy + 4 * _gxyMatrix[pixel] * _gxyMatrix[pixel];

    dirX = 0.5 * ( gxxMinusGyy ) - 0.5 * sqrt( hxy );
    dirY = _gxyMatrix[pixel];
}

void DirectionalField::computeGradientDirection(int pixel, float& dirX, float& dirY)
{
    float gxxMinusGyy = _gxxMatrix[pixel] - _gyyMatrix[pixel];
    float hxy = gxxMinusGyy * gxxMinusGyy + 4 * _gxyMatrix[pixel] * _gxyMatrix[pixel];

    dirX = 0.5 * ( gxxMinusGyy ) + 0.5 * sqrt( hxy );
    dirY = _gxyMatrix[pixel];
}


void DirectionalField::buildHorizontalDerivativeMatrix( float* data )
{
    #pragma omp parallel for
    for (int x = 0; x < _width; ++x)
    {
        for (int y = 0; y < _height; ++y)
        {
            int i = pixel(x, y);
            _gxMatrix[i] = dx( x, y, data );
        }
    }
}



void DirectionalField::buildVerticalDerivativeMatrix( float* data )
{
    #pragma omp parallel for
    for (int x = 0; x < _width; ++x)
    {
        for (int y = 0; y < _height; ++y)
        {
            int i = pixel(x, y);
            _gyMatrix[i] = dy( x, y, data );
        }
    }
}



void DirectionalField::buildHorizontalWindowedDerivativeMatrix()
{
    #pragma omp parallel for
    for (int x = 0; x < _width; ++x)
    {
        for (int y = 0; y < _height; ++y)
        {
            _gxxMatrix[pixel(x,y)] = getHorizontalWindowedDerivative( x, y );
        }
    }
}



void DirectionalField::buildVerticalWindowedDerivativeMatrix()
{
    #pragma omp parallel for
    for (int x = 0; x < _width; ++x)
    {
        for (int y = 0; y < _height; ++y)
        {
            _gyyMatrix[pixel(x,y)] = getVerticalWindowedDerivative( x, y );
        }
    }
}



void DirectionalField::buildCrossedWindowedDerivativeMatrix()
{
    #pragma omp parallel for
    for (int x = 0; x < _width; ++x)
    {
        for (int y = 0; y < _height; ++y)
        {
            _gxyMatrix[pixel(x,y)] = getCrossedWindowedDerivative( x, y );
        }
    }
}



void DirectionalField::buildCoherenceImage()
{
    float max = 0.0f;
    #pragma omp parallel for
    for (int x = 0; x < _width; ++x)
    {
        for (int y = 0; y < _height; ++y)
        {
            float coherenceValue = computeCoherence( pixel( x, y ) );
            imgSetPixelf( _coherence, x, y, coherenceValue );
            if (coherenceValue > max) max = coherenceValue;
        }
    }

    // normalize coherence Image
    imgNormalize( _coherence );
    
}



void DirectionalField::buildCoherenceMask()
{
    // build raw mask
    #pragma omp parallel for
    for (int x = 0; x < _width; ++x)
    {
        for (int y = 0; y < _height; ++y)
        {
            float lum = imgGetPixelf( _coherence, x, y );
            float mask = (lum >= _coherenceThreshold) ? 1 : 0;
            imgSetPixelf( _coherenceMask, x, y, mask );
        }
    }
    
    // morphological treatment
    // open
    Image* openKernel = Parameters::buildCircleImage( _openKernelRadius );
    imgErode( _coherenceMask, openKernel );
    imgDilate( _coherenceMask, openKernel );
    imgDestroy( openKernel );

    //close
    Image* closeKernel = Parameters::buildCircleImage( _closeKernelRadius );
    imgDilate( _coherenceMask, closeKernel );
    imgErode( _coherenceMask, closeKernel );
    imgDestroy( closeKernel );
}



void DirectionalField::buildDirectionImage()
{
    #pragma omp parallel for
    for (int x = 0; x < _width; ++x)
    {
        for (int y = 0; y < _height; ++y)
        {
            float dirX, dirY;
            computeDirection( pixel(x,y), dirX, dirY );

            // normalize directions
            float lenght = sqrt( dirX * dirX + dirY * dirY );
            if (lenght > 0)
            {
                dirX /= lenght;
                dirY /= lenght;
            }
            if (dirY < 0)
            {
                dirX = -dirX;
                dirY = -dirY;
            }

            imgSetPixel3f( _direction, x, y, 0.0f, dirX, dirY );
        }
    }
}

void DirectionalField::buildGradientDirectionImage()
{
    #pragma omp parallel for
    for (int x = 0; x < _width; ++x)
    {
        for (int y = 0; y < _height; ++y)
        {
            float dirX, dirY;
            computeGradientDirection( pixel(x,y), dirX, dirY );

            // normalize directions
            float lenght = sqrt( dirX * dirX + dirY * dirY );
            if (lenght > 0)
            {
                dirX /= lenght;
                dirY /= lenght;
            }
            if (dirY < 0)
            {
                dirX = -dirX;
                dirY = -dirY;
            }

            imgSetPixel3f( _ortho, x, y, 0.0f, dirX, dirY );
        }
    }
}

float DirectionalField::getHorizontalWindowedDerivative( int px, int py )
{
    int windowWidth = imgGetWidth( _gaussianKernel );
    int windowHeight = imgGetHeight( _gaussianKernel );
    int windowHalfWidth = windowWidth / 2;
    int windowHalfHeight = windowHeight / 2;
    
    float sum = 0.0f;
    
    for (int wY = 0; wY < windowHeight; ++wY)
    {
        int imgY = py + wY - windowHalfHeight;
        if (imgY < 0 || imgY >= _height) continue;
        
        for (int wX = 0; wX < windowWidth; ++wX)
        {
            int imgX = px + wX - windowHalfWidth;
            if (imgX < 0 || imgX >= _width) continue;
            
            float value = _gxMatrix[pixel(imgX, imgY)];
            value *= value;
            
            sum += imgGetPixelf( _gaussianKernel, wX, wY ) * value;
        }
    }
    
    return sum;
}



float DirectionalField::getVerticalWindowedDerivative( int px, int py )
{
    int windowWidth = imgGetWidth( _gaussianKernel );
    int windowHeight = imgGetHeight( _gaussianKernel );
    int windowHalfWidth = windowWidth / 2;
    int windowHalfHeight = windowHeight / 2;
    
    float sum = 0.0f;
    
    for (int wY = 0; wY < windowHeight; ++wY)
    {
        int imgY = py + wY - windowHalfHeight;
        if (imgY < 0 || imgY >= _height) continue;
        
        for (int wX = 0; wX < windowWidth; ++wX)
        {
            int imgX = px + wX - windowHalfWidth;
            if (imgX < 0 || imgX >= _width) continue;
            
            float value = _gyMatrix[pixel(imgX, imgY)];
            value *= value;
            
            sum += imgGetPixelf( _gaussianKernel, wX, wY ) * value;
        }
    }
    
    return sum;
}



float DirectionalField::getCrossedWindowedDerivative( int px, int py )
{
    int windowWidth = imgGetWidth( _gaussianKernel );
    int windowHeight = imgGetHeight( _gaussianKernel );
    int windowHalfWidth = windowWidth / 2;
    int windowHalfHeight = windowHeight / 2;
    
    float sum = 0.0f;
    
    for (int wY = 0; wY < windowHeight; ++wY)
    {
        int imgY = py + wY - windowHalfHeight;
        if (imgY < 0 || imgY >= _height) continue;
        
        for (int wX = 0; wX < windowWidth; ++wX)
        {
            int imgX = px + wX - windowHalfWidth;
            if (imgX < 0 || imgX >= _width) continue;
            
            float value = _gyMatrix[pixel(imgX, imgY)];
            value *= _gxMatrix[pixel(imgX, imgY)];
            
            sum += imgGetPixelf( _gaussianKernel, wX, wY ) * value;
        }
    }
    
    return sum;
}

void DirectionalField::getDirection( int x, int y, float& dirX, float& dirY )
{
    float aux;
    imgGetPixel3f( _direction, x, y, &aux, &dirX, &dirY );
}

float DirectionalField::getCoherence( int x, int y )
{
    return imgGetPixelf( _coherence, x, y );
}

void DirectionalField::getGrandientDirection(int x, int y, float& dirX, float& dirY)
{
    float aux;
    imgGetPixel3f( _ortho, x, y, &aux, &dirX, &dirY );
}


bool DirectionalField::getCoherenceMask(int x, int y)
{
    // The values on the mask images should only be 0 or 1. I test against 0.5 
    // just to make sure little approximation errors don't mess up the results.
    return imgGetPixelf( _coherenceMask, x, y ) > 0.5f;
}

void DirectionalField::debugPrint()
{
//    printf( "gx:\n" );
//    for (int y = 0; y < _height; ++y)
//    {
//        for (int x = 0; x < _width; ++x)
//        {
//            printf("%.2f\t", _gxMatrix[pixel(x,y)]);
//        }
//        printf("\n");
//    }
//
//    printf( "gy:\n" );
//    for (int y = 0; y < _height; ++y)
//    {
//        for (int x = 0; x < _width; ++x)
//        {
//            printf("%.2f\t", _gyMatrix[pixel(x,y)]);
//        }
//        printf("\n");
//    }
//
//    printf( "gxx:\n" );
//    for (int y = 0; y < _height; ++y)
//    {
//        for (int x = 0; x < _width; ++x)
//        {
//            printf("%.2f\t", _gxxMatrix[pixel(x,y)]);
//        }
//        printf("\n");
//    }
//
//    printf( "gyy:\n" );
//    for (int y = 0; y < _height; ++y)
//    {
//        for (int x = 0; x < _width; ++x)
//        {
//            printf("%.2f\t", _gyyMatrix[pixel(x,y)]);
//        }
//        printf("\n");
//    }
//
//    printf( "gxy:\n" );
//    for (int y = 0; y < _height; ++y)
//    {
//        for (int x = 0; x < _width; ++x)
//        {
//            printf("%.2f\t", _gxyMatrix[pixel(x,y)]);
//        }
//        printf("\n");
//    }
//
//    printf( "direction:\n" );
//    for (int y = 0; y < _height; ++y)
//    {
//        for (int x = 0; x < _width; ++x)
//        {
//            printf("(%.2f, %.2f)\t", _directionX[pixel(x,y)], _directionY[pixel(x,y)]);
//        }
//        printf("\n");
//    }
//
//    printf( "coherence:\n" );
//    for (int y = 0; y < _height; ++y)
//    {
//        for (int x = 0; x < _width; ++x)
//        {
//            printf("%.3f\t", _coherence[pixel(x,y)]);
//        }
//        printf("\n");
//    }
}

void DirectionalField::debugImages()
{
    //directional field
    Image* dbgImg = imgCreate( _width, _height, 3 );
    for (int x = 0; x < _width; ++x)
    {
        for (int y = 0; y < _height; ++y)
        {
            float dirX, dirY;
            getGrandientDirection( x, y, dirX, dirY );

            imgSetPixel3f( dbgImg, x, y, 1,1,1 );

            if (dirX == 0.0f && dirY == 0.0f)
            {
                imgSetPixel3f( dbgImg, x, y, 1.0, 1.0, 0.0 );
            }
            else
            {
                if (dirY < 0)
                {
                    dirY *= -1;
                    dirX *= -1;
                }

                float lumX = fabs( dirX );
                float lumY = fabs( dirY );
                imgSetPixel3f( dbgImg, x, y, 0.0f, lumX, lumY );
            }
        }
    }
    imgWriteBMP( (char*)"directionalFieldDebugImage.bmp", dbgImg );
    imgDestroy( dbgImg );
}

Image* DirectionalField::getCoherence()
{
    return imgCopy( _coherence );
}

Image* DirectionalField::getCoherenceMask()
{
    return imgCopy(_coherenceMask);
}

Image* DirectionalField::getDirection()
{
    return imgCopy(_direction);
}

Image* DirectionalField::getGrandientDirection()
{
    return imgCopy( _ortho );
}
