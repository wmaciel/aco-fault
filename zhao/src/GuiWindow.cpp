/* 
 * File:   GuiWindow.cpp
 * Author: keoma
 * 
 * Created on March 5, 2014, 11:53 AM
 */

#include "GuiWindow.h"
#include "GuiPresenter.h"
#include <GL/gl.h>

GuiWindow::GuiWindow()
{
    _gtkWindow = build();
    
    g_signal_connect( _dstCanvas    , "configure-event", G_CALLBACK( cb_configGLCanvas )    , NULL );
    g_signal_connect( _dstCanvas    , "expose-event"   , G_CALLBACK( cb_exposeGLCanvas )    , this );
    g_signal_connect( _srcCanvas    , "configure-event", G_CALLBACK( cb_configGLCanvas )    , NULL );
    g_signal_connect( _srcCanvas    , "expose-event"   , G_CALLBACK( cb_exposeGLCanvas )    , this );
    
    _presenter = 0;
}



GuiWindow::~GuiWindow()
{
}

void GuiWindow::setPresenter( GuiPresenter* presenter )
{
    _presenter = presenter;
}

void GuiWindow::show()
{
    gtk_widget_show_all( _gtkWindow );
}

GtkWidget* GuiWindow::build()
{
    //create window
    GtkWidget* window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    
    //create mainBox
    GtkWidget* mainBox = buildMainBox();
    
    //insert mainBox as outer most layout
    gtk_container_add( GTK_CONTAINER( window ), mainBox );
    
    return window;
}


GtkWidget* GuiWindow::buildMainBox()
{
    GtkWidget* mainBox = gtk_hbox_new( FALSE, 5 );
    
    GtkWidget* canvasBox = buildCanvasBox();
    gtk_box_pack_start( GTK_BOX( mainBox ), canvasBox, TRUE, TRUE, 5 );
    
    GtkWidget* controlsBox = buildControlsBox();
    gtk_box_pack_start( GTK_BOX( mainBox ), controlsBox, FALSE, FALSE, 5 );
    
    return mainBox;
}


GtkWidget* GuiWindow::buildCanvasBox()
{
    GtkWidget* canvasBox = gtk_hbox_new( TRUE, 1 );
    
    // create the drawing areas
    _dstCanvas = gtk_drawing_area_new();
    gtk_drawing_area_size( GTK_DRAWING_AREA( _dstCanvas ), 400, 100 );
    _srcCanvas = gtk_drawing_area_new();
    gtk_drawing_area_size( GTK_DRAWING_AREA( _srcCanvas ), 400, 100 );
    
    // OpenGL configuration for the canvas
    GdkGLConfig* glconfig = gdk_gl_config_new_by_mode (static_cast<GdkGLConfigMode>
                                                       (GDK_GL_MODE_RGB  |
                                                        GDK_GL_MODE_DEPTH|
                                                        GDK_GL_MODE_DOUBLE) );
    
    // Define the canvas as capable of rendering OpenGL graphics
    gtk_widget_set_gl_capability( _dstCanvas, glconfig, NULL, TRUE, GDK_GL_RGBA_TYPE );
    gtk_widget_set_gl_capability( _srcCanvas, glconfig, NULL, TRUE, GDK_GL_RGBA_TYPE );
    
    // Add Drawing Areas to the box
    gtk_box_pack_start_defaults( GTK_BOX( canvasBox ), _srcCanvas );
    gtk_box_pack_start_defaults( GTK_BOX( canvasBox ), _dstCanvas );
    
    return canvasBox;
}



GtkWidget* GuiWindow::buildControlsBox()
{
    GtkWidget* controlsBox = gtk_vbox_new( FALSE, 3 );
    
    GtkWidget* parametersLabel = gtk_label_new( "Parâmetros:" );
    gtk_box_pack_start( GTK_BOX( controlsBox ), parametersLabel, FALSE, TRUE, 5 );
    
    GtkWidget* separator01 = gtk_hseparator_new();
    gtk_box_pack_start( GTK_BOX( controlsBox ), separator01, FALSE, TRUE, 5 );
    
    GtkWidget* turnsBox = buildParameterBox("Turnos:", 1, 1000, 1 );
    gtk_box_pack_start( GTK_BOX( controlsBox ), turnsBox, FALSE, TRUE, 2 );
    
    GtkWidget* alphaBox = buildParameterBox("Peso Feromônio:", 1, 10, 1 );
    gtk_box_pack_start( GTK_BOX( controlsBox ), alphaBox, FALSE, TRUE, 2 );
    
    GtkWidget* etaBox = buildParameterBox("Peso Atributo:", 1, 10, 1 );
    gtk_box_pack_start( GTK_BOX( controlsBox ), etaBox, FALSE, TRUE, 2 );
    
    GtkWidget* antsBox = buildParameterBox("Formigas:", 1, 10000, 5 );
    gtk_box_pack_start( GTK_BOX( controlsBox ), antsBox, FALSE, TRUE, 2 );
    
    GtkWidget* freedomBox = buildParameterBox("Ângulo Visão:", 1, 180, 1 );
    gtk_box_pack_start( GTK_BOX( controlsBox ), freedomBox, FALSE, TRUE, 2 );
    
    GtkWidget* constantBox = buildParameterBox("Constante Feromônio:", 0.1, 100, 0.5 );
    gtk_box_pack_start( GTK_BOX( controlsBox ), constantBox, FALSE, TRUE, 2 );
    
    GtkWidget* evaporationBox = buildParameterBox("Taxa Evaporação:", 0, 1, 0.1 );
    gtk_box_pack_start( GTK_BOX( controlsBox ), evaporationBox, FALSE, TRUE, 2 );
    
    GtkWidget* separator02 = gtk_hseparator_new();
    gtk_box_pack_start( GTK_BOX( controlsBox ), separator02, FALSE, TRUE, 5 );
    
    GtkWidget* loadButton = gtk_file_chooser_button_new( "Selecione faria de atributo sísmico", GTK_FILE_CHOOSER_ACTION_OPEN );
    gtk_box_pack_start( GTK_BOX( controlsBox ), loadButton, TRUE, TRUE, 2 );
    
    GtkWidget* runButton = gtk_button_new_with_label( "Executar" );
    gtk_box_pack_start( GTK_BOX( controlsBox ), runButton, TRUE, TRUE, 2 );
    
    return controlsBox;
}



GtkWidget* GuiWindow::buildParameterBox( const char* name, double min, double max, double step )
{
    GtkWidget* paramBox = gtk_vbox_new( FALSE, 1 );
    GtkWidget* label = gtk_label_new( name );
    GtkWidget* spin = gtk_spin_button_new_with_range( min, max, step );
    
    gtk_box_pack_start( GTK_BOX(paramBox), label, FALSE, TRUE, 1 );
    gtk_box_pack_start( GTK_BOX(paramBox), spin,  TRUE, TRUE, 1 );
    
    return paramBox;
}



gboolean GuiWindow::cb_configGLCanvas( GtkWidget* canvas, GdkEventConfigure* event, gpointer user_data )
{
    GdkGLContext *glContext = gtk_widget_get_gl_context( canvas );
    GdkGLDrawable *glDrawable = gtk_widget_get_gl_drawable( canvas );
    
    // start opengl commands
    if( !gdk_gl_drawable_gl_begin( glDrawable, glContext ) )
    {
        g_assert_not_reached();
    }
    
    glLoadIdentity();
    glViewport( 0, 0, canvas->allocation.width, canvas->allocation.height );
    glMatrixMode( GL_PROJECTION );
    glOrtho ( 0, canvas->allocation.width, 0, canvas->allocation.height, -10, 10 );
    
    // end opengl commands
    gdk_gl_drawable_gl_end( glDrawable );
    
    //everything fine
    return TRUE;
}



gboolean GuiWindow::cb_exposeGLCanvas( GtkWidget* canvas, GdkEventExpose* event, gpointer user_data )
{
    GdkGLContext *glContext = gtk_widget_get_gl_context( canvas );
    GdkGLDrawable *glDrawable = gtk_widget_get_gl_drawable( canvas );
 
    //tell gtk that we will start calling opengl functions
    if( !gdk_gl_drawable_gl_begin( glDrawable, glContext ) )
    {
        g_assert_not_reached();
    }
    
    //Clear the canvas with black
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    //Recover access to the image
    GuiWindow* window = ( GuiWindow* )user_data;
    
    Image* image = NULL;
    
    if( canvas == window->_dstCanvas )
    {
        image = ( Image* )window->_presenter->getOutputImage();
    }
    else
    {
        image = ( Image* )window->_presenter->getInputImage();
    }
    
    //If Image loaded OK, start drawing
    if( image )
    {
        glBegin( GL_POINTS );
            int width = imgGetWidth( image );
            int height = imgGetHeight( image );

            for (int w = 0; w < width; ++w)
            {
                for (int h = 0; h < height; ++h)
                {
                    float r,g,b;
                    imgGetPixel3f( image, w, h, &r, &g, &b );
                    glColor3f( r,g,b );
                    glVertex2i( w, h );
                }
            }
        glEnd();
    }
    
    //Update image
    gdk_gl_drawable_swap_buffers (glDrawable);
    
    //Tell GTK we stopped messing with OpenGL
    gdk_gl_drawable_gl_end( glDrawable );
    
    //everything went fine
    return TRUE;
}
