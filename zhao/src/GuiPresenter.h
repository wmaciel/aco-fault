/* 
 * File:   GuiPresenter.h
 * Author: keoma
 *
 * Created on March 5, 2014, 3:45 PM
 */

#ifndef GUIPRESENTER_H
#define	GUIPRESENTER_H

#include "image.h"


class GuiPresenter
{
public:
    GuiPresenter();
    virtual ~GuiPresenter();
    Image* getInputImage();
    Image* getOutputImage();
    
private:
    Image* _inImg;
    Image* _outImg;
};

#endif	/* GUIPRESENTER_H */

