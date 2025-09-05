
#include "matrix.h"
#include "operations.h"

#include <math.h>
#include <stdlib.h>
#include "shape.h"
#include <string.h>

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
    multiplicar_matrizes_3x3_3x3(rotate, translateToOrigin, temp); // temp = R * T_toOrigin
    multiplicar_matrizes_3x3_3x3(translateBack, temp, transform);  // transform = T_back * (R * T_toOrigin)

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

void escala(float (*points)[3], float (*originalPoints)[3], int num_points, float xf, float yf, float sx, float sy)
{

    float transladarOrigem[3][3] = {
        {1, 0, -xf},
        {0, 1, -yf},
        {0, 0, 1}};

    float matrizEscala[3][3] = {
        {sx, 0, 0},
        {0, sy, 0},
        {0, 0, 1}};

    float voltarLocal[3][3] = {
        {1, 0, xf},
        {0, 1, yf},
        {0, 0, 1}};

    float resultadoEscala[3][3];
    float voltarResultado[3][3];

    multiplicar_matrizes_3x3_3x3(matrizEscala, transladarOrigem, resultadoEscala);
    multiplicar_matrizes_3x3_3x3(voltarLocal, resultadoEscala, voltarResultado);

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

void cisalhamento_h(float (*points)[3], float (*originalPoints)[3], int num_points, float cx, float cy, float shx)
{
    if (!originalPoints || num_points <= 0)
        return;

    // Matriz de cisalhamento horizontal
    float shearMatrix[3][3] = {
        {1, shx, 0},
        {0, 1, 0},
        {0, 0, 1}};

    // Aplicar a transformação para cada ponto
    for (int i = 0; i < num_points; i++)
    {
        // Coordenadas homogêneas do ponto
        float p[3][1] = {
            {originalPoints[i][0] - cx}, // transladar para a origem
            {originalPoints[i][1] - cy},
            {1}};

        float result[3][1];

        // multiplicar pelo cisalhamento
        multiplicar_matrizes_3x3_3x1(p, shearMatrix, result);

        // transladar de volta para a posição original
        points[i][0] = result[0][0] + cx;
        points[i][1] = result[1][0] + cy;
        points[i][2] = originalPoints[i][2]; // mantém z original
    }
}

void cisalhamento_v(float (*points)[3], float (*originalPoints)[3], int num_points, float cx, float cy, float shy)
{
    if (!originalPoints || num_points <= 0)
        return;

    // Matriz de cisalhamento vertical
    float shearMatrix[3][3] = {
        {1, 0, 0},
        {shy, 1, 0},
        {0, 0, 1}};

    // Aplicar a transformação para cada ponto
    for (int i = 0; i < num_points; i++)
    {
        // Coordenadas homogêneas do ponto
        float p[3][1] = {
            {originalPoints[i][0] - cx}, // transladar para a origem
            {originalPoints[i][1] - cy},
            {1}};

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
void reflexao(float (*points)[3], int num_points, float cx, float cy, int tipo)
{
    // Transladar para a origem
    float transladarOrigem[3][3] = {
        {1, 0, -cx},
        {0, 1, -cy},
        {0, 0, 1}};

    // Matriz de reflexão de acordo com o tipo
    float matrizReflexao[3][3];
    if (tipo == 0)
    { // Reflexão em relação ao eixo X
        float m[3][3] = {
            {1, 0, 0},
            {0, -1, 0},
            {0, 0, 1}};
        memcpy(matrizReflexao, m, sizeof(m));
    }
    else if (tipo == 1)
    { // Reflexão em relação ao eixo Y
        float m[3][3] = {
            {-1, 0, 0},
            {0, 1, 0},
            {0, 0, 1}};
        memcpy(matrizReflexao, m, sizeof(m));
    }
    else if (tipo == 2)
    { // Reflexão na origem
        float m[3][3] = {
            {-1, 0, 0},
            {0, -1, 0},
            {0, 0, 1}};
        memcpy(matrizReflexao, m, sizeof(m));
    }

    // Transladar de volta ao local original
    float voltarLocal[3][3] = {
        {1, 0, cx},
        {0, 1, cy},
        {0, 0, 1}};

    // Calcular transformação final: voltarLocal * matrizReflexao * transladarOrigem
    float temp[3][3];
    float transformacaoFinal[3][3];
    multiplicar_matrizes_3x3_3x3(matrizReflexao, transladarOrigem, temp);
    multiplicar_matrizes_3x3_3x3(voltarLocal, temp, transformacaoFinal);

    // Aplicar transformação diretamente aos pontos
    for (int i = 0; i < num_points; i++)
    {
        float p[3][1] = {{points[i][0]}, {points[i][1]}, {1}};
        float result[3][1];
        multiplicar_matrizes_3x3_3x1(p, transformacaoFinal, result);
        points[i][0] = result[0][0];
        points[i][1] = result[1][0];
    }
}

// Funções do quickhull, sempre que adicionar um ponto ao polígono convexo,
// sempre colocar no sentido horário

float distancia(Point3 p1, Point3 p2, Point3 p3)
{
    float a = p1[1] - p2[1];
    float b = p2[0] - p1[0];
    float c = p1[0] * p2[1] - p2[0] * p1[1];
    return fabs(a * p3[0] + b * p3[1] + c) / sqrt(a * a + b * b);
}

Point3 *pontos_acima(Point3 p1, Point3 p2, Point3 *points, int num_points, int *out_count)
{
    Point3 *acima = malloc(15 * sizeof(Point3));
    int count = 0;

    for (int i = 0; i < num_points; i++)
    {
        if (points[i][2] == 1)
        {
            float d = (p2[0] - p1[0]) * (points[i][1] - p1[1]) - (p2[1] - p1[1]) * (points[i][0] - p1[0]);
            if (d > 0)
            {
                memcpy(acima[count++], points[i], sizeof(Point3));
            }
        }
    }

    *out_count = count;
    return acima;
}

Point3 *pontos_abaixo(Point3 p1, Point3 p2, Point3 *points, int num_points, int *out_count)
{
    Point3 *abaixo = malloc(15 * sizeof(Point3));
    int count = 0;

    for (int i = 0; i < num_points; i++)
    {
        if (points[i][2] == 1)
        {
            float d = (p2[0] - p1[0]) * (points[i][1] - p1[1]) - (p2[1] - p1[1]) * (points[i][0] - p1[0]);
            if (d < 0)
            {
                memcpy(abaixo[count++], points[i], sizeof(Point3));
            }
        }
    }

    *out_count = count;
    return abaixo;
}

Point3 *quickhull2(Point3 p1, Point3 p2, Point3 *points, int num_points, int lado, int *out_count)
{
    Point3 *feixo_convexo = malloc(15 * sizeof(Point3));
    int count_feixo = 0;

    if (num_points == 0 || points == NULL)
    {
        *out_count = 0;
        return feixo_convexo;
    }

    // ponto mais distante da linha
    float max_dist = -1;
    int idx = -1;
    for (int i = 0; i < num_points; i++)
    {
        if (points[i][2] == 1)
        {
            float d = distancia(p1, p2, points[i]);
            if (d > max_dist)
            {
                max_dist = d;
                idx = i;
            }
        }
    }

    Point3 far;
    memcpy(far, points[idx], sizeof(Point3));

    // criar resto dos pontos sem ponto mais distante
    Point3 resto[15];
    int count_resto = 0;
    for (int i = 0; i < num_points; i++)
    {
        if (i != idx && points[i][2] == 1) // só visíveis
        {
            memcpy(resto[count_resto++], points[i], sizeof(Point3));
        }
    }

    int count1, count2;
    Point3 *set1;
    Point3 *set2;

    if (lado == 1)
    {
        set1 = pontos_acima(p1, far, resto, count_resto, &count1);
        set2 = pontos_acima(far, p2, resto, count_resto, &count2);
    }
    else
    {
        set1 = pontos_abaixo(p1, far, resto, count_resto, &count1);
        set2 = pontos_abaixo(far, p2, resto, count_resto, &count2);
    }

    int out1_count, out2_count;
    Point3 *feixo1 = quickhull2(p1, far, set1, count1, lado, &out1_count);
    Point3 *feixo2 = quickhull2(far, p2, set2, count2, lado, &out2_count);

    // combinar feixo1 + ponto mais distante + feixo2
    for (int i = 0; i < out1_count; i++)
        memcpy(feixo_convexo[count_feixo++], feixo1[i], sizeof(Point3));
    memcpy(feixo_convexo[count_feixo++], far, sizeof(Point3));
    for (int i = 0; i < out2_count; i++)
        memcpy(feixo_convexo[count_feixo++], feixo2[i], sizeof(Point3));

    *out_count = count_feixo;


    free(set1);
    free(set2);
    free(feixo1);
    free(feixo2);

    return feixo_convexo;
}
Point3 *quickhull(Point3 *points, int num_points, int *out_count)
{
    int min_x = 0, max_x = 0;
    for (int i = 1; i < num_points; i++)
    {
        if (points[i][2] == 1) // leva em consideração apenas pontos visíveis
        {
            if (points[i][0] < points[min_x][0])
                min_x = i;
            if (points[i][0] > points[max_x][0])
                max_x = i;
        }
    }

    int count1, count2;
    Point3 *top = pontos_acima(points[min_x], points[max_x], points, num_points, &count1);
    Point3 *bottom = pontos_abaixo(points[min_x], points[max_x], points, num_points, &count2);

    int out_count1, out_count2;
    Point3 *feixo_top = quickhull2(points[min_x], points[max_x], top, count1, 1, &out_count1);
    Point3 *feixo_bottom = quickhull2(points[min_x], points[max_x], bottom, count2, -1, &out_count2);

    Point3 *feixo_convexo = malloc(15 * sizeof(Point3));
    int c = 0;
    for (int i = 0; i < out_count1; i++)
        memcpy(feixo_convexo[c++], feixo_top[i], sizeof(Point3));
    memcpy(feixo_convexo[c++], points[max_x], sizeof(Point3));
    for (int i = 0; i < out_count2; i++)
        memcpy(feixo_convexo[c++], feixo_bottom[i], sizeof(Point3));
    memcpy(feixo_convexo[c++], points[min_x], sizeof(Point3));

    *out_count = c;

    free(top);
    free(bottom);
    free(feixo_top);
    free(feixo_bottom);

    // --- Ordenar o casco convexo antes de retornar ---
    float cx = 0, cy = 0;
    for (int i = 0; i < *out_count; i++)
    {
        cx += feixo_convexo[i][0];
        cy += feixo_convexo[i][1];
    }
    cx /= *out_count;
    cy /= *out_count;

    for (int i = 0; i < *out_count - 1; i++)
    {
        for (int j = i + 1; j < *out_count; j++)
        {
            float ai = atan2f(feixo_convexo[i][1] - cy, feixo_convexo[i][0] - cx);
            float aj = atan2f(feixo_convexo[j][1] - cy, feixo_convexo[j][0] - cx);
            if (ai > aj)
            {
                float tmp[3];
                memcpy(tmp, feixo_convexo[i], sizeof(Point3));
                memcpy(feixo_convexo[i], feixo_convexo[j], sizeof(Point3));
                memcpy(feixo_convexo[j], tmp, sizeof(Point3));
            }
        }
    }

    return feixo_convexo;
}
