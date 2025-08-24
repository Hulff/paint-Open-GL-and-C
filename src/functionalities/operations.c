
#include "matrix.h"
#include <math.h>
#include <stdlib.h>
#include "shape.h"

void translate(float (*points)[3], int num_points, float xt, float yt)
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
        {0, 0, 1}};
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
void rotate(float (*points)[3], int num_points, double angle, float cx, float cy)
{
    // Matriz de translação para a origem (centro da figura)
    float translateToOrigin[3][3] = {
        {1, 0, -cx},
        {0, 1, -cy},
        {0, 0, 1}};

    // Matriz de rotação
    float rotate[3][3] = {
        {cos(angle), -sin(angle), 0},
        {sin(angle), cos(angle), 0},
        {0, 0, 1}};

    // Matriz de translação de volta
    float translateBack[3][3] = {
        {1, 0, cx},
        {0, 1, cy},
        {0, 0, 1}};

    float temp[3][3];
    float transform[3][3];

    // MT= T_back * R * T_toOrigin
    multiplicar_matrizes_3x3_3x3(rotate, translateToOrigin, temp);     //temp = R * T_toOrigin
    multiplicar_matrizes_3x3_3x3(translateBack, temp, transform); // transform = T_back * (R * T_toOrigin)

    // Aplicar a transformação a todos os pontos
    for (int i = 0; i < num_points; i++)
    {
        float p[3][1] = {{points[i][0]}, {points[i][1]}, {1}}; // ponto em coordenadas homogêneas
        float result[3][1];
        multiplicar_matrizes_3x3_3x1(p, transform, result);

        points[i][0] = result[0][0];
        points[i][1] = result[1][0];
    }
}

void escala(float (*points)[3],float (*originalPoints)[3], int num_points,float xf,float yf,float sx,float sy){

   float transladarOrigem[3][3]={
     {1,0,-xf},
     {0,1,-yf},
     {0,0,1}
   };
   
   float matrizEscala[3][3]={
     {sx,0,0},
     {0,sy,0},
     {0,0,1}
   };

    float voltarLocal[3][3]={
     {1,0,xf},
     {0,1,yf},
     {0,0,1}
   };

   float resultadoEscala[3][3];
   float voltarResultado[3][3];

 multiplicar_matrizes_3x3_3x3(matrizEscala,transladarOrigem,resultadoEscala);
 multiplicar_matrizes_3x3_3x3(voltarLocal,resultadoEscala,voltarResultado);

 // Aplicar a transformação a todos os pontos
    for (int i = 0; i < num_points; i++)
    {
        float p[3][1] = {{originalPoints[i][0]}, {originalPoints[i][1]}, {1}}; // ponto em coordenadas homogêneas
        float result[3][1];
        multiplicar_matrizes_3x3_3x1(p, voltarResultado, result);

        points[i][0] = result[0][0];
        points[i][1] = result[1][0];
    }

}

void cisalhamento_h(float (*points)[3], float (*originalPoints)[3], int num_points, float cx, float cy, float shx){
    if (!originalPoints || num_points <= 0) return;

    // Matriz de cisalhamento horizontal
    float shearMatrix[3][3] = {
        {1, shx, 0},
        {0, 1, 0},
        {0, 0, 1}
    };

    // Aplicar a transformação para cada ponto
    for (int i = 0; i < num_points; i++) {
        // Coordenadas homogêneas do ponto
        float p[3][1] = {
            {originalPoints[i][0] - cx}, // transladar para a origem
            {originalPoints[i][1] - cy},
            {1}
        };

        float result[3][1];

        // multiplicar pelo cisalhamento
        multiplicar_matrizes_3x3_3x1(p, shearMatrix, result);

        // transladar de volta para a posição original
        points[i][0] = result[0][0] + cx;
        points[i][1] = result[1][0] + cy;
        points[i][2] = originalPoints[i][2]; // mantém z original
    }

}

void cisalhamento_v(float (*points)[3], float (*originalPoints)[3], int num_points, float cx, float cy, float shy){
    if (!originalPoints || num_points <= 0) return;

    // Matriz de cisalhamento vertical
    float shearMatrix[3][3] = {
        {1, 0, 0},
        {shy, 1, 0},
        {0, 0, 1}
    };

    // Aplicar a transformação para cada ponto
    for (int i = 0; i < num_points; i++) {
        // Coordenadas homogêneas do ponto
        float p[3][1] = {
            {originalPoints[i][0] - cx}, // transladar para a origem
            {originalPoints[i][1] - cy},
            {1}
        };

        float result[3][1];

        // multiplicar pelo cisalhamento
        multiplicar_matrizes_3x3_3x1(p, shearMatrix, result);

        // transladar de volta para a posição original
        points[i][0] = result[0][0] + cx;
        points[i][1] = result[1][0] + cy;
        points[i][2] = originalPoints[i][2]; // mantém z original
    }
}

// Função para refletir a figura
void reflexao(Shape *s, float cx, float cy, int type)
{
    // Matriz de reflexão
    float reflexao_matrix[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}};

    // Define a matriz baseada no tipo de reflexão
    switch (type)
    {
    case 0: // Reflexão em torno do eixo X (horizontal)
        reflexao_matrix[1][1] = -1.0f;
        break;
    case 1: // Reflexão em torno do eixo Y (vertical)
        reflexao_matrix[0][0] = -1.0f;
        break;
    case 2: // Reflexão em torno da origem
        reflexao_matrix[0][0] = -1.0f;
        reflexao_matrix[1][1] = -1.0f;
        break;
    }
    // Itera por todos os pontos da figura para aplicar a transformação
    for (int i = 0; i < s->num_points; i++)
    {
        float px = s->points[i][0];
        float py = s->points[i][1];

        // Transladar para a origem (baseado no centro da figura)
        px -= cx;
        py -= cy;

        // Aplicar a reflexão (multiplicação de matriz)
        float new_px = px * reflexao_matrix[0][0];
        float new_py = py * reflexao_matrix[1][1];

        // Transladar de volta para a posição original
        px = new_px + cx;
        py = new_py + cy;

        // Atualiza os pontos da figura
        s->points[i][0] = px;
        s->points[i][1] = py;
    }
}
