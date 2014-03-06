/* 
 * File:   GuiPresenter.cpp
 * Author: keoma
 * 
 * Created on March 5, 2014, 3:45 PM
 */

#include "GuiPresenter.h"
#include "GuiWindow.h"

GuiPresenter::GuiPresenter()
{
    _inImg = 0;
    _outImg = 0;
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
    return _inImg;
}



Image* GuiPresenter::getOutputImage()
{
    return _outImg;
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
