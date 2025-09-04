
#include "matrix.h"
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

float distancia(float p1[3], float p2[3], float p3[3])
{
    // calcula distância de p3 à linha p1-p2
    float a = p1[1] - p2[1];
    float b = p2[0] - p1[0];
    float c = p1[0] * p2[1] - p2[0] * p1[1];
    return fabs(a*p3[0] + b*p3[1] + c) / sqrt(a*a + b*b);

}

float* pontos_acima(float p1[3], float p2[3], float (*points)[3], int num_points) {
    // retorna pontos acima da linha p1-p2
    float (*acima)[3] = malloc(15 * sizeof *acima); // aloca espaço para 15 pontos
    int count = 0;

    if (p2[0] - p1[0] == 0) {
        return NULL;
    }

    float m = (p2[1] - p1[1]) / (p2[0] - p1[0]);
    float c = - m * p1[0] + p1[1];

    for (int i = 0; i < num_points; i++) {
        if (points[i][1] > m * points[i][0] + c) {
            acima[count][0] = points[i][0];
            acima[count][1] = points[i][1];
            acima[count][2] = points[i][2];
            count++;
        }
    }

    return acima;
}

float* pontos_abaixo(float p1[3], float p2[3], float (*points)[3], int num_points) {
    // retorna pontos abaixo da linha p1-p2
    float (*abaixo)[3] = malloc(15 * sizeof *abaixo); // aloca espaço para 15 pontos
    int count = 0;

    if (p2[0] - p1[0] == 0) {
        return NULL;
    }

    float m = (p2[1] - p1[1]) / (p2[0] - p1[0]);
    float c = - m * p1[0] + p1[1];

    for (int i = 0; i < num_points; i++) {
        if (points[i][1] < m * points[i][0] + c) {
            abaixo[count][0] = points[i][0];
            abaixo[count][1] = points[i][1];
            abaixo[count][2] = points[i][2];
            count++;
        }
    }

    return abaixo;
}

float* quickhull2 (float p1[3], float p2[3], float (*points)[3], int num_points, int lado) {
    // retorna pontos do casco convexo entre p1 e p2
    // se lado = 1, pontos acima da linha p1-p2
    // se lado = -1, pontos abaixo da linha p1-p2

    if (num_points == 0 || points == NULL || p1 == NULL || p2 == NULL) {
        return NULL;
    }

    float (*feixo_convexo)[3] = malloc(15 * sizeof *feixo_convexo); // aloca espaço para 15 pontos

    float maior_distancia = -1;
    float ponto_mais_distante[3];

    for (int i = 0; i < num_points; i++) {
        float d = distancia(p1, p2, points[i]);
        if (d > maior_distancia) {
            maior_distancia = d;
            ponto_mais_distante[0] = points[i][0];
            ponto_mais_distante[1] = points[i][1];
            ponto_mais_distante[2] = points[i][2];
        }
    }

    // remover ponto_mais_distante de points
    float (*resto_pontos)[3] = malloc(15 * sizeof *resto_pontos);
    int count_resto = 0;
    for (int i = 0; i < num_points; i++) {
        if (points[i][0] != ponto_mais_distante[0] || points[i][1] != ponto_mais_distante[1]) {
            resto_pontos[count_resto][0] = points[i][0];
            resto_pontos[count_resto][1] = points[i][1];
            resto_pontos[count_resto][2] = points[i][2];
            count_resto++;
        }
    }

    float* ponto1_acima = pontos_acima(p1, ponto_mais_distante, resto_pontos, count_resto);
    float* ponto1_abaixo = pontos_abaixo(p1, ponto_mais_distante, resto_pontos, count_resto);
    float* ponto2_acima = pontos_acima(ponto_mais_distante, p2, resto_pontos, count_resto);
    float* ponto2_abaixo = pontos_abaixo(ponto_mais_distante, p2, resto_pontos, count_resto);

    if (lado == 1) {
        float* feixo1[3] = quickhull2(p1, ponto_mais_distante, ponto1_acima, count_resto, lado);
        float* feixo2[3] = quickhull2(ponto_mais_distante, p2, ponto2_acima, count_resto, lado);

        // combinar feixo1 + ponto_mais_distante + feixo2
        int count_feixo = 0;
        if (feixo1 != NULL) {
            for (int i = 0; i < 15; i++) {
                if (feixo1[i][0] == 0 && feixo1[i][1] == 0) break;
                feixo_convexo[count_feixo][0] = feixo1[i][0];
                feixo_convexo[count_feixo][1] = feixo1[i][1];
                feixo_convexo[count_feixo][2] = feixo1[i][2];
                count_feixo++;
            }
            free(feixo1);
        }

        feixo_convexo[count_feixo][0] = ponto_mais_distante[0];
        feixo_convexo[count_feixo][1] = ponto_mais_distante[1];
        feixo_convexo[count_feixo][2] = ponto_mais_distante[2];
        count_feixo++;

        if (feixo2 != NULL) {
            for (int i = 0; i < 15; i++) {
                if (feixo2[i][0] == 0 && feixo2[i][1] == 0) break;
                feixo_convexo[count_feixo][0] = feixo2[i][0];
                feixo_convexo[count_feixo][1] = feixo2[i][1];
                feixo_convexo[count_feixo][2] = feixo2[i][2];
                count_feixo++;
            }
            free(feixo2);
        }

    } else if (lado == -1) {
        float* feixo1[3] = quickhull2(p1, ponto_mais_distante, ponto1_abaixo, count_resto, lado);
        float* feixo2[3] = quickhull2(ponto_mais_distante, p2, ponto2_abaixo, count_resto, lado);

        // combinar feixo1 + ponto_mais_dist
        int count_feixo = 0;
        if (feixo1 != NULL) {
            for (int i = 0; i < 15; i++) {
                if (feixo1[i][0] == 0 && feixo1[i][1] == 0) break;
                feixo_convexo[count_feixo][0] = feixo1[i][0];
                feixo_convexo[count_feixo][1] = feixo1[i][1];
                feixo_convexo[count_feixo][2] = feixo1[i][2];
                count_feixo++;
            }
            free(feixo1);
        }

        feixo_convexo[count_feixo][0] = ponto_mais_distante[0];
        feixo_convexo[count_feixo][1] = ponto_mais_distante[1];
        feixo_convexo[count_feixo][2] = ponto_mais_distante[2];
        count_feixo++;

        if (feixo2 != NULL) {
            for (int i = 0; i < 15; i++) {
                if (feixo2[i][0] == 0 && feixo2[i][1] == 0) break;
                feixo_convexo[count_feixo][0] = feixo2[i][0];
                feixo_convexo[count_feixo][1] = feixo2[i][1];
                feixo_convexo[count_feixo][2] = feixo2[i][2];
                count_feixo++;
            }
            free(feixo2);
        }
    }

    free(resto_pontos);
    free(ponto1_acima);
    free(ponto1_abaixo);
    free(ponto2_acima);
    free(ponto2_abaixo);

    return feixo_convexo;
    
}

float* quickhull(float (*points)[3], int num_points) {
    // retorna pontos do casco convexo
    if (num_points < 3 || points == NULL) {
        return NULL;
    }

    float* feixo_convexo[3] = malloc(15 * sizeof *feixo_convexo); // aloca espaço para 15 pontos

    // encontrar pontos com x mínimo e máximo
    float ponto_esq[3][1];
    ponto_esq[0][0] = points[0][0];
    ponto_esq[1][0] = points[0][1];
    ponto_esq[2][0] = points[0][2];
    float ponto_dir[3][1];
    ponto_dir[0][0] = points[0][0];
    ponto_dir[1][0] = points[0][1];
    ponto_dir[2][0] = points[0][2];

    for (int i = 0; i < num_points; i++) {
        if (points[i][0] < ponto_esq[0][0]) {
            ponto_esq[0][0] = points[i][0];
            ponto_esq[1][0] = points[i][1];
            ponto_esq[2][0] = points[i][2];
        }
        if (points[i][0] > ponto_dir[0][0]) {
            ponto_dir[0][0] = points[i][0];
            ponto_dir[1][0] = points[i][1];
            ponto_dir[2][0] = points[i][2];
        }
    }

    // remover ponto_esq e ponto_dir de points
    float (*resto_pontos)[3] = malloc(15 * sizeof *resto_pontos);
    int count_resto = 0;
    for (int i = 0; i < num_points; i++) {
        if ((points[i][0] != ponto_esq[0][0] || points[i][1] != ponto_esq[1][0]) &&
            (points[i][0] != ponto_dir[0][0] || points[i][1] != ponto_dir[1][0])) {
            resto_pontos[count_resto][0] = points[i][0];
            resto_pontos[count_resto][1] = points[i][1];
            resto_pontos[count_resto][2] = points[i][2];
            count_resto++;
        }
    }

    float* acima = pontos_acima(ponto_esq[0], ponto_dir[0], resto_pontos, count_resto);
    float* abaixo = pontos_abaixo(ponto_esq[0], ponto_dir[0], resto_pontos, count_resto);

    float* feixo1[3] = quickhull2(ponto_esq[0], ponto_dir[0], acima, count_resto, 1);
    float* feixo2[3] = quickhull2(ponto_dir[0], ponto_esq[0], abaixo, count_resto, -1);

    // combinar ponto_esq + feixo1 + ponto_dir + feixo2
    int count_feixo = 0;
    feixo_convexo[count_feixo][0] = ponto_esq[0][0];
    feixo_convexo[count_feixo][1] = ponto_esq[1][0];
    feixo_convexo[count_feixo][2] = ponto_esq[2][0];
    count_feixo++;

    if (feixo1 != NULL) {
        for (int i = 0; i < 15; i++) {
            if (feixo1[i][0] == 0 && feixo1[i][1] == 0) break;
            feixo_convexo[count_feixo][0] = feixo1[i][0];
            feixo_convexo[count_feixo][1] = feixo1[i][1];
            feixo_convexo[count_feixo][2] = feixo1[i][2];
            count_feixo++;
        }
        free(feixo1);
    }

    feixo_convexo[count_feixo][0] = ponto_dir[0][0];
    feixo_convexo[count_feixo][1] = ponto_dir[1][0];
    feixo_convexo[count_feixo][2] = ponto_dir[2][0];
    count_feixo++;

    if (feixo2 != NULL) {
        for (int i = 0; i < 15; i++) {
            if (feixo2[i][0] == 0 && feixo2[i][1] == 0) break;
            feixo_convexo[count_feixo][0] = feixo2[i][0];
            feixo_convexo[count_feixo][1] = feixo2[i][1];
            feixo_convexo[count_feixo][2] = feixo2[i][2];
            count_feixo++;
        }
        free(feixo2);
    }

    free(resto_pontos);
    free(acima);
    free(abaixo);

    return feixo_convexo;
}