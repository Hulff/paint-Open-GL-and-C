#include <GL/freeglut.h>
#include "shape.h"
#include <stdlib.h>
#include <math.h>

// Criar um shape
Shape *createShape(ShapeType type, int num_points) {
    Shape *s = malloc(sizeof(Shape));
    if (!s) return NULL;

    s->type = type;
    s->num_points = num_points;
    s->id = rand();
    s->points = malloc(sizeof(float[num_points][2]));

    // cor padrão = preto
    s->color[0] = 0.0f;
    s->color[1] = 0.0f;
    s->color[2] = 0.0f;

    return s;
}

// Liberar memória
void freeShape(Shape *s) {
    if (!s) return;
    free(s->points);
    free(s);
}

// Adicionar ponto
void addPoint(Shape *s, int index, float x, float y) {
    if (!s) return;
    if (index < s->num_points) {
        s->points[index][0] = x;
        s->points[index][1] = y;
    }
}

// ------------------- Funções de desenho -------------------

// Desenhar ponto
void drawPoint(const Shape *s) {
    if (!s || s->type != SHAPE_POINT) return;

    glColor3fv(s->color);
    glPointSize(6);
    glBegin(GL_POINTS);
        glVertex2f(s->points[0][0], s->points[0][1]);
    glEnd();
}

// Desenhar linha
void drawLine(const Shape *s) {
    if (!s || s->type != LINE) return;

    glColor3fv(s->color);
    glLineWidth(2);
    glBegin(GL_LINES);
        glVertex2f(s->points[0][0], s->points[0][1]);
        glVertex2f(s->points[1][0], s->points[1][1]);
    glEnd();
}

// Desenhar quadrado
void drawSquare(const Shape *s) {
    if (!s || s->type != SQUARE) return;
    if (s->num_points < 2) return;

    float x1 = s->points[0][0];
    float y1 = s->points[0][1];
    float x2 = s->points[1][0];
    float y2 = s->points[1][1];

    glColor3fv(s->color);
    glBegin(GL_QUADS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();

    // contorno
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}

// Desenhar triângulo
void drawTriangle(const Shape *s) {
    if (!s || s->type != TRIANGLE) return;
    if (s->num_points < 3) return;

    glColor3fv(s->color);
    glBegin(GL_TRIANGLES);
        glVertex2f(s->points[0][0], s->points[0][1]);
        glVertex2f(s->points[1][0], s->points[1][1]);
        glVertex2f(s->points[2][0], s->points[2][1]);
    glEnd();

    // contorno
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(s->points[0][0], s->points[0][1]);
        glVertex2f(s->points[1][0], s->points[1][1]);
        glVertex2f(s->points[2][0], s->points[2][1]);
    glEnd();
}

// Desenhar polígono
void drawPolygon(const Shape *s) {
    if (!s || s->type != POLYGON) return;
    if (s->num_points < 3) return;

    glColor3fv(s->color);
    glBegin(GL_POLYGON);
        for (int i = 0; i < s->num_points; i++) {
            glVertex2f(s->points[i][0], s->points[i][1]);
        }
    glEnd();

    // contorno
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < s->num_points; i++) {
            glVertex2f(s->points[i][0], s->points[i][1]);
        }
    glEnd();
}
