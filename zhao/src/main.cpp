#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gtk/gtkgl.h>
#include "GuiPresenter.h"

int main( int argc, char** argv )
{
    gtk_init( &argc, &argv );
    gtk_gl_init( &argc, &argv );
    srand( time(NULL) );
    GuiPresenter* presenter = new GuiPresenter();
    presenter->showWindow();
    gtk_main();
    return 0;
}
