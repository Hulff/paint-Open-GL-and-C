
#include "matrix.h"
#include <stdlib.h>

void translate(float (*points)[2],int num_points, float xt, float yt)
{

    // // Direto, achei mais simples e eficiente
    //  for (int i = 0; i < num_points; i++)
    //  {
    //      points[i][0] += xt;
    //      points[i][1] += yt;
    //  }

    //Com Matriz
    float translateMatrix[2][1];
    translateMatrix[0][0] = xt;
    translateMatrix[1][0] = yt;

    for (int i = 0; i < num_points; i++)
    {
        points[i][0] += translateMatrix[0][0];
        points[i][1] += translateMatrix[1][0];
    }
}