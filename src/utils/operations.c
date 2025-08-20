
#include "matrix.h"
#include <stdlib.h>

void translate(float (*points)[2], int num_points, float xt, float yt)
{

    // // Direto, achei mais simples e eficiente
    //  for (int i = 0; i < num_points; i++)
    //  {
    //      points[i][0] += xt;
    //      points[i][1] += yt;
    //  }

    // // Com Matriz
    // float translateMatrix[2][1];
    // translateMatrix[0][0] = xt;
    // translateMatrix[1][0] = yt;

    // for (int i = 0; i < num_points; i++)
    // {
    //     points[i][0] += translateMatrix[0][0];
    //     points[i][1] += translateMatrix[1][0];
    // }

    // com Matriz e coordenadas homogêneas p' = MT(tx,ty)*p
    float translateMatrix[3][3];
    translateMatrix[0][0] = 1;
    translateMatrix[0][1] = 0;
    translateMatrix[0][2] = xt;
    translateMatrix[1][0] = 0;
    translateMatrix[1][1] = 1;
    translateMatrix[1][2] = yt;
    translateMatrix[2][0] = 0;
    translateMatrix[2][1] = 0;
    translateMatrix[2][2] = 1;
    // converter points para coordenadas homogêneas
    float pointsHomogeneos[3][1];
    float pointsResultado[3][1];
    for (int i = 0; i < num_points; i++)
    {
        pointsHomogeneos[0][0] = points[i][0];
        pointsHomogeneos[1][0] = points[i][1];
        pointsHomogeneos[2][0] = 1;

        multiplicar_matrizes_3x3_3x1(pointsHomogeneos, translateMatrix, pointsResultado);
        
        points[i][0] = pointsResultado[0][0];
        points[i][1] = pointsResultado[1][0];
    }
}
void rotate(float (*points)[2], int num_points, float angle)
{
}