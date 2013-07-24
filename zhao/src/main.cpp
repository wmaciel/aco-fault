#include <stdio.h>
#include <stdlib.h>
#include "DirectionalField.h"

int main( int argc, char** argv )
{
//    // fill data
    float* data = new float[64];
    for (int x = 0; x < 8; ++x)
    {
        for (int y = 0; y < 8; ++y)
        {
            int i = x + y * 8;
            if (x==y) data[i] = 0.9;
        }
    }

    DirectionalField* field = new DirectionalField(data, 8, 8, 2);

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            int i = x + y * 8;
            printf("%.3f\t", data[i]);
        }
        printf("\n");
    }

    field->debugPrint();

    return 0;
}