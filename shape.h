#ifndef SHAPE_H
#define SHAPE_H

typedef enum {
    SHAPE_POINT,
    LINE,
    SQUARE,
    TRIANGLE,
    POLYGON,
    FREE_DRAW
} ShapeType;

typedef struct {
    ShapeType type;
    int num_points;
    int id;
    float (*points)[2];
    float color[3];
} Shape;

// ---- Criação / destruição ----
Shape *createShape(ShapeType type, int num_points);
void freeShape(Shape *s);

// ---- Manipulação de pontos ----
void addPoint(Shape *s, int index, float x, float y);

// ---- Desenho ----
void drawPoint(const Shape *s);
void drawLine(const Shape *s);
void drawSquare(const Shape *s);
void drawTriangle(const Shape *s);
void drawPolygon(const Shape *s);

#endif
