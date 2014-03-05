/* 
 * File:   GuiPresenter.cpp
 * Author: keoma
 * 
 * Created on March 5, 2014, 3:45 PM
 */

#include "GuiPresenter.h"

GuiPresenter::GuiPresenter()
{
    _inImg = 0;
    _outImg = 0;
}



GuiPresenter::~GuiPresenter()
{
    if (_inImg)  imgDestroy( _inImg );
    if (_outImg) imgDestroy( _outImg );
}



Image* GuiPresenter::getInputImage()
{
    return _inImg;
}



Image* GuiPresenter::getOutputImage()
{
    return _outImg;
}

