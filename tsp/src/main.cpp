#include "Presenter.h"

#include <time.h>

int main( int argc, char** argv )
{
    srand(time(NULL));

    Presenter presenter( argc, argv );

//    for ( float i = 0.4f; i < 0.75; i+=0.1 )
//    {
//        for ( float j=0.4f; j < 0.75; j+=0.1)
//        {
//            presenter._colony.addCity( i,j );
//        }
//    }

    presenter.start();
    return 0;
}
