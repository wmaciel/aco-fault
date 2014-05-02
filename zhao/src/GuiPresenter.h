/* 
 * File:   GuiPresenter.h
 * Author: keoma
 *
 * Created on March 5, 2014, 3:45 PM
 */

#ifndef GUIPRESENTER_H
#define	GUIPRESENTER_H

#include "image.h"

class GuiWindow;

class GuiPresenter
{
public:
    GuiPresenter();
    virtual ~GuiPresenter();
    
    void showWindow();
    void loadImage( char* path );
    void run();
    Image* getInputImage();
    Image* getOutputImage();
    Image* getDirectionImage();
    Image* getConsistencyImage();
    Image* getConsistencyMask();
    
private:
    GuiWindow* _window;
    Image* _inImg;
    Image* _outImg;
    Image* _dirImg;
    Image* _cohImg;
};

#endif	/* GUIPRESENTER_H */

