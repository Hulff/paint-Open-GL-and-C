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
    float r;
    float g;
    float b;
    int id; //para podermos identificar a figura na busca na pilha
    float (*points)[3]; // array de pontos [x, y, z] z indica se o ponto deve ser renderizado ou não
} Shape;

// Funções para manipular shapes
Shape *createShape(int num_points, ShapeType type);

#endif
