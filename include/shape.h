#ifndef SHAPE_H
#define SHAPE_H

typedef enum
{
    SHAPE_POINT,
    LINE,
    SQUARE,
    TRIANGLE,
    POLYGON,
    FREE_DRAW
} ShapeType;

typedef struct
{
    ShapeType type;
    int num_points;
    int id; //para podermos identificar a figura na busca na pilha
    float (*points)[2]; // array de pontos [x, y]
} Shape;

// Funções para manipular shapes
Shape *createShape(int num_points);


#endif
