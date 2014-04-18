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

GuiPresenter::GuiPresenter()
{
    _inImg = 0;
    _outImg = 0;
    //_preProcImg = 0;
    //_postProcImg = 0;
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
