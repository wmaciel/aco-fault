#include "Presenter.h"

#include <time.h>

int main( int argc, char** argv )
{
    srand(time(NULL));

    Presenter presenter( argc, argv );

    presenter.start();
    return 0;
}
