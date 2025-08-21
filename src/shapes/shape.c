// Manipulações das Figuras
#include <stdlib.h>
#include "shape.h"

// criar uma figura
Shape *createShape(int num_points, ShapeType type)
{
    Shape *s = malloc(sizeof(Shape));
    s->type = type;
    s->num_points = num_points;
    s->id = rand();                                   // id aleatório
    s->points = malloc(sizeof(float[num_points][3])); // aloca todos os pontos
    s->r = 0.0f;                                      // cor inicial
    s->g = 0.0f;                                      // cor inicial
    s->b = 0.0f;                                      // cor inicial

    for (int i = 0; i < num_points; i++)
    {
        s->points[i][0] = 0; // x
        s->points[i][1] = 0; // y
        s->points[i][2] = 0; // z (indica se o ponto deve ser renderizado ou não)
    }
    return s;
}

void freeShape(Shape *s)
{
    if (!s)
        return;
    free(s->points); // libera os pontos primeiro
    free(s);         // depois libera a struct
}
