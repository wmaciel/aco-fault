/* 
 * File:   GuiWindow.h
 * Author: keoma
 *
 * Created on March 5, 2014, 11:53 AM
 */

#ifndef GUIWINDOW_H
#define	GUIWINDOW_H

extern "C"
{
    #include <gtk/gtk.h>
    #include <gtk/gtkgl.h>
}

class GuiPresenter;

class GuiWindow
{
public:
    GuiWindow();
    virtual ~GuiWindow();
    void setPresenter( GuiPresenter* presenter );
    void show();

private:
    GtkWidget* build();
    GtkWidget* buildMainBox();
    GtkWidget* buildCanvasBox();
    GtkWidget* buildControlsBox();
    GtkWidget* buildParameterBox( const char* name, double min, double max, double step );
    
    //CALLBACKS
    /**
     * Callback called once to initialize the openGL state of the canvas
     * @param canvas is the GtkDrawingArea that will be configured
     * @param event holds specific data related to this event
     * @param user_data is not being used but is required by the API
     * @return TRUE if went ok, FALSE otherwise.
     */
    static gboolean cb_configGLCanvas( GtkWidget *canvas, GdkEventConfigure *event, gpointer user_data );
    
    /**
     * Callback called when any part of the canvas is exposed and it needs to be
     * redrawn.
     * @param canvas is the GtkDrawingAres that will be redrawn
     * @param event holds specific data related to this event
     * @param user_data is not being used but is required by the API
     * @return TRUE if went ok, FALSE otherwise.
     */
    static gboolean cb_exposeGLCanvas( GtkWidget *canvas, GdkEventExpose *event, gpointer user_data );
    
    GuiPresenter* _presenter;
    GtkWidget* _gtkWindow;
    
    /** Drawing Area will hold the OpenGL drawing of the output image */
    GtkWidget* _dstCanvas;
    
    /** Drawing Area will hold the OpenGL drawing of the input image */
    GtkWidget* _srcCanvas;
    
    /** Spin button defining the number of turns to run */
    GtkWidget* _spinTurns;
    
    GtkWidget* _spinAlpha;
};

#endif	/* GUIWINDOW_H */

