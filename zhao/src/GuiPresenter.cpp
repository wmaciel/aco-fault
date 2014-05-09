/* 
 * File:   GuiPresenter.cpp
 * Author: keoma
 * 
 * Created on March 5, 2014, 3:45 PM
 */

#include "GuiPresenter.h"
#include "GuiWindow.h"
#include "Parameters.h"
#include "Colony.h"
#include "DirectionalField.h"

GuiPresenter::GuiPresenter()
{
    _inImg = 0;
    _outImg = 0;
    _dirImg = 0;
    _cohImg = 0;
    _window = new GuiWindow();
    _window->setPresenter( this );
}



GuiPresenter::~GuiPresenter()
{
    if (_inImg)  imgDestroy( _inImg );
    if (_outImg) imgDestroy( _outImg );
}



void GuiPresenter::showWindow()
{
    _window->show();
}



Image* GuiPresenter::getInputImage()
{
//    if (_preProcImg)
//    {
//        imgDestroy( _preProcImg );
//    }
//    
//    _preProcImg = Parameters::preProcessing( _inImg );
//    
//    return _preProcImg;
    
    return Parameters::preProcessing( _inImg );
}



Image* GuiPresenter::getOutputImage()
{
//    if (_postProcImg)
//    {
//        imgDestroy( _postProcImg );
//    }
//    
//    _postProcImg = Parameters::postProcessing( _outImg );
//    
//    return _postProcImg;
    
    return Parameters::postProcessing( _outImg );
}

Image* GuiPresenter::getDirectionImage()
{
    Image* inputImage = getInputImage();
    if (!inputImage) return NULL;
    
    DirectionalField* dirField = new DirectionalField(inputImage, Parameters::dirOpenKernelRadius, Parameters::dirCloseKernelRadius);
    Image* dirImg = dirField->getDirection();
    delete dirField;
    
    for (int x = 0; x < imgGetWidth(dirImg); ++x)
    {
        for (int y = 0; y < imgGetHeight(dirImg); ++y)
        {
            float aux, dx, dy;
            imgGetPixel3f(dirImg, x, y, &aux, &dx, &dy);
            if (( dx == dy ) && ( dx == 0 ))
                imgSetPixel3f( dirImg, x, y, 1.0, 0.0, 0.0 );
            else
                imgSetPixel3f( dirImg, x, y, aux, fabs(dx), fabs(dy) );
        }
    }
    
    return dirImg;
}

Image* GuiPresenter::getConsistencyImage()
{
    Image* inputImage = getInputImage();
    if (!inputImage) return NULL;
    
    DirectionalField* dirField = new DirectionalField(inputImage, Parameters::dirOpenKernelRadius, Parameters::dirCloseKernelRadius);
    Image* cohImg = dirField->getCoherence();
    delete dirField;
    //imgNormalize( cohImg );
    return cohImg;
}

Image* GuiPresenter::getConsistencyMask()
{
    Image* inputImage = getInputImage();
    if (!inputImage) return NULL;
    
    DirectionalField* dirField = new DirectionalField(inputImage, Parameters::dirOpenKernelRadius, Parameters::dirCloseKernelRadius);
    Image* mskImg = dirField->getCoherenceMask();
    delete dirField;
    //imgNormalize( mskImg );
    return mskImg;
}


void GuiPresenter::loadImage( char* path )
{
    if (_inImg)
    {
        imgDestroy( _inImg );
        _inImg = 0;
    }
    
    if (_outImg)
    {
        imgDestroy(_outImg);
        _outImg = 0;
    }
    
    _inImg = imgReadPFM( path );
    
    _window->redraw();
}



void GuiPresenter::run()
{
    Image* copyInImg = getInputImage();
    Colony* colony = new Colony( copyInImg );
    colony->run( Parameters::turns );
    imgDestroy( copyInImg );
    
    if (_outImg)
    {
        imgDestroy(_outImg);
        _outImg = 0;
    }
    
    _outImg = colony->getPheromoneImage();
    //imgNormalize( _outImg );
    _window->redraw();
    
    delete colony;
}
