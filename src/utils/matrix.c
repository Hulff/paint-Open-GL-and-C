

#include <stdlib.h>
#include <stdio.h>

// matriz2 (3x3) * matriz1 (3x1) = resultado (3x1)
void multiplicar_matrizes_3x3_3x1(float matriz1[3][1], float matriz2[3][3], float resultado[3][1])
{
    for (int i = 0; i < 3; i++)
    {
        resultado[i][0] = 0;
        for (int k = 0; k < 3; k++)
        {
            resultado[i][0] += matriz2[i][k] * matriz1[k][0];
        }
    }
}
void multiplicar_matrizes_3x3_3x3(float matriz1[3][3], float matriz2[3][3], float resultado[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            resultado[i][j] = 0;
            for (int k = 0; k < 3; k++)
            {
                resultado[i][j] += matriz2[i][k] * matriz1[k][j];
            }
        }
    }
}
