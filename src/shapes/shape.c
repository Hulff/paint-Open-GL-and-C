// Manipulações das Figuras
#include <stdlib.h>
#include "shape.h"

// criar uma figura
Shape *createShape(int num_points)
{
    Shape *s = malloc(sizeof(Shape));
    switch (num_points)
    {
    case 1:
        s->type = SHAPE_POINT;
        break;
    case 2:
        s->type = LINE;
        break;
    case 3:
        s->type = TRIANGLE;
        break;
    default:
        s->type = POLYGON;
        break;
    }
    s->num_points = num_points;
    s->id = rand();                                   // id aleatório
    s->points = malloc(sizeof(float[num_points][3])); // aloca todos os pontos
    for (int i = 0; i < num_points; i++)
    {
        s->points[i][0] = 0; // x
        s->points[i][1] = 0; // y
        s->points[i][2] = 0; // z
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
