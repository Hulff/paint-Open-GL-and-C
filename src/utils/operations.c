
#include "matrix.h"
#include <math.h>
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
    float translateMatrix[3][3] = {
        {1, 0, xt},
        {0, 1, yt},
        {0, 0, 1}
    };
    // converter points para coordenadas homogêneas
    float pointsHomogeneos[3][1];
    float pointsResultado[3][1];

    for (int i = 0; i < num_points; i++)
    {
        pointsHomogeneos[0][0] = points[i][0];
        pointsHomogeneos[1][0] = points[i][1];
        pointsHomogeneos[2][0] = 1;

        // p'i = MT(tx,ty)*pi
        multiplicar_matrizes_3x3_3x1(pointsHomogeneos, translateMatrix, pointsResultado);

        points[i][0] = pointsResultado[0][0];
        points[i][1] = pointsResultado[1][0];
    }
}
void rotate(float (*points)[2], int num_points, double angle, float xt, float yt)
{
    float translateMatrix[3][3] = {
        {1, 0, xt},
        {0, 1, yt},
        {0, 0, 1}
    };
    float translateBackMatrix[3][3] = {
        {1, 0, -xt},
        {0, 1, -yt},
        {0, 0, 1}
    };
    float  rotateMatrix[3][3] = {
        {cos(angle), -sin(angle), xt},
        {sin(angle), cos(angle), yt},
        {0, 0, 1}
    };
    float transformMatrix[3][3];

    // gerar matriz de transformação composta
    // MTransform = MT*MR*MT
    multiplicar_matrizes_3x3_3x3(translateMatrix, rotateMatrix, transformMatrix);
    // MTransform2 = MTransform*MT
    multiplicar_matrizes_3x3_3x3(transformMatrix, translateBackMatrix, transformMatrix);

    // converter points para coordenadas homogêneas
    float pointsHomogeneos[3][1];
    float pointsResultado[3][1];


    for (int i = 0; i < num_points; i++)
    {
        pointsHomogeneos[0][0] = points[i][0];
        pointsHomogeneos[1][0] = points[i][1];
        pointsHomogeneos[2][0] = 1;

        // p'i = M*pi, aplica a matriz de transformação nos pontos
        multiplicar_matrizes_3x3_3x1(pointsHomogeneos, transformMatrix, pointsResultado);

        points[i][0] = pointsResultado[0][0];
        points[i][1] = pointsResultado[1][0];
    }
}