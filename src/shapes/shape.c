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
        s->type = POINT;
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
    s->id = rand();    //id aleatório
    s->points = malloc(sizeof(float[num_points][2])); // aloca todos os pontos
    return s;
}

void freeShape(Shape *s)
{
    if (!s)
        return;
    free(s->points); // libera os pontos primeiro
    free(s);         // depois libera a struct
}

