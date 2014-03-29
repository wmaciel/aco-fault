#include <cstdio>
#include <string.h>
#include <math.h>
#include <float.h>
#include <signal.h>
#include "PFMImage.h"


PFMImage::PFMImage()
{
    _buffer = 0;
    _width  = 0;
    _height = 0;    
}



PFMImage::PFMImage( int width, int height )
{
    _width = width;
    _height = height; 
    
    _buffer = new float[_width*_height];
}



PFMImage::PFMImage(PFMImage& original)
{
    _buffer = 0;
    copy( original );
}



PFMImage::~PFMImage()
{
    if (_buffer)
    {
        delete[] _buffer;
        _buffer = 0;
    }
}



void PFMImage::getPixel( int x, int y, float& luminanceValue )
{
    // Valida as coordenadas do pixel
    if (x < 0 || x >= _width || y < 0 || y >= _height)
        return;
    
    int index = getIndex( x, y );

     luminanceValue = _buffer[index];
}



float PFMImage::getPixel(int x, int y)
{
    // Valida as coordenadas do pixel
    if (x < 0 || x >= _width || y < 0 || y >= _height)
    {
        return -FLT_MAX;
    }
    
    int index = getIndex( x, y );
    return _buffer[index];
}
    


void PFMImage::setPixel( int x, int y, float luminance )
{
        // Valida as coordenadas do pixel
    if (x < 0 || x >= _width || y < 0 || y >= _height)
        return;
    
    int index = getIndex( x, y );

    _buffer[index] = luminance;
}



void PFMImage::getDimension( int& width, int& height )
{
    width = _width;
    height = _height;
}



bool PFMImage::exportPFMFile( std::string filePath )
{
    FILE* filePointer = fopen( filePath.c_str(), "wb" ); 
            
    // Testa se o arquivo foi aberto
    if (!filePointer)
    {
        printf("\nN�o foi poss�vel abrir o arquivo %s\n", filePath.c_str() );
        return false;
    }

    // Salva "header"
    fwrite( &_width, sizeof(int), 1, filePointer );
    fwrite( &_height, sizeof(int), 1, filePointer );
    
    // Salva dados
    fwrite( _buffer, _width*_height, sizeof(float), filePointer );

    // Feedback
    printf( "exportPFMFile: %s successfuly created\n", filePath.c_str() );
    
    // Fecha o arquivo
    fclose(filePointer);
    
    return false;
}



bool PFMImage::loadPFMFile( std::string filePath )
{
    FILE* filePointer = fopen( filePath.c_str(), "rb" );
    size_t numRead = 0;
    
    // Testa se o arquivo foi aberto
    if (!filePointer)
    {  
        printf("\nN�o foi poss�vel abrir o arquivo %s\n", filePath.c_str() );
        return false;
    }

    // L� "header"
    numRead += fread( (void*)&_width, sizeof(int), 1, filePointer );
    numRead += fread( (void*)&_height, sizeof(int), 1, filePointer );
    
    // Testa se os dados do header foram recuperados corretamente
    if (numRead != 2)
    {
        printf("\nErro de leitura do arquivo %s\n", filePath.c_str() );
        fclose( filePointer );
        return false;
    }
    
    if (_buffer)
    {
        delete[] _buffer;
        _buffer = 0;
    }
    
    // L� a imagem
    _buffer = new float[_width*_height];
    fread( _buffer, _width*_height, sizeof(float), filePointer );
    
    fclose(filePointer);
    
    return true;
}



void PFMImage::fill( float luminance )
{
    if (luminance == 0.0)
    {
        memset( _buffer, 0, sizeof(float)*_width*_height );
    }
    else
    {
        int size = _width * _height;
        for (int i = 0; i < size; ++i)
        {
            _buffer[i] = luminance;
        }
    }
}



void PFMImage::copy( PFMImage& original )
{
    _width = original._width;
    _height = original._height;
    
    if (_buffer)
    {
        delete[] _buffer;
        _buffer = 0;
    }
    
    _buffer = new float[_width*_height];
    memcpy( _buffer, original._buffer, sizeof(float)*_width*_height );
}



int PFMImage::getIndex( int x, int y )
{
    return y*_width + x;
}


float apply(float c[9], float v[9])
{
    return c[0]*v[0]+c[1]*v[1]+c[2]*v[2]+c[3]*v[3]+c[4]*v[4]+c[5]*v[5]+c[6]*v[6]+c[7]*v[7]+c[8]*v[8];
}



void PFMImage::generateGauss( int scale )
{
    if (_width <= 0 || _height <= 0) return;
    if (scale <= 0) return;
    
    float oldBuffer[_width*_height];
    
    for (int i=0; i < scale; ++i)
    {
        memcpy( oldBuffer, _buffer, sizeof(float)*_width*_height );

        int w = _width;
        int h = _height;

        for (int y=1;y<h-1;y++)
        {
            for (int x=1;x<w-1;x++)
            {
                int k = getIndex( x, y );

                float v[9] = {oldBuffer[k+w-1], oldBuffer[k+w], oldBuffer[k+w+1],
                              oldBuffer[k-1]  , oldBuffer[k]  , oldBuffer[k+1],
                              oldBuffer[k-w-1], oldBuffer[k-w], oldBuffer[k-w+1]};

                float gauss[9]={1.f/16, 2.f/16, 1.f/16,  2.f/16, 4.f/16, 2.f/16,  1.f/16, 2.f/16, 1.f/16 };

                _buffer[k] = apply( gauss, v );
            }
        }
    }
}
