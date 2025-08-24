#include "select.h"
#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#include "helper.h"

// dist�ncia euclidiana
static float dist(float x1, float y1, float x2, float y2)
{
    return sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// distância ponto-reta (segmento)
static float pointToSegmentDist(float px, float py, float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    if (dx == 0 && dy == 0)
    {
        return dist(px, py, x1, y1);
    }
    float t = ((px - x1) * dx + (py - y1) * dy) / (dx * dx + dy * dy);
    if (t < 0)
        t = 0;
    else if (t > 1)
        t = 1;
    float projX = x1 + t * dx;
    float projY = y1 + t * dy;
    return dist(px, py, projX, projY);
}

// algoritmo ray casting: ponto dentro do pol�gono?
static int pointInPolygon(float px, float py, Shape *poly)
{
    int inside = 0;
    for (int i = 0, j = calcRealNumPoints(poly) - 1; i < calcRealNumPoints(poly); j = i++)
    {
        if (poly->points[i][2] == 1 && poly->points[j][2] == 1) // se os dois pontos sao utilizados pelo poligono
        {
            float xi = poly->points[i][0], yi = poly->points[i][1];
            float xj = poly->points[j][0], yj = poly->points[j][1];

            int intersect = ((yi > py) != (yj > py)) &&
                            (px < (xj - xi) * (py - yi) / (yj - yi + 0.00001f) + xi);
            if (intersect)
                inside = !inside;
        }
    }
    return inside;
}

// testa se (x,y) esta sobre/na figura
static int isInsideShape(float x, float y, Shape *s)
{
    switch (s->type)
    {
    case SHAPE_POINT:
        return (dist(x, y, s->points[0][0], s->points[0][1]) < 10.0f);

    case LINE:
        return (pointToSegmentDist(x, y,
                                   s->points[0][0], s->points[0][1],
                                   s->points[1][0], s->points[1][1]) < 5.0f);

    case POLYGON:
        return pointInPolygon(x, y, s);
    case FREE_DRAW:
        return pointInPolygon(x, y, s);

    default:
        return 0;
    }
}

void initSelector(Selector *sel, int maxPos)
{
    sel->selected = NULL;
    sel->index = maxPos;
    sel->active = 0;
    sel->action = ACTION_NONE;
}

void setSelectionMode(Selector *sel, int active)
{
    sel->active = active;
    sel->selected = NULL;
    sel->index = -1;
    sel->action = ACTION_NONE;
}

// Seleciona uma figura clicando em qualquer parte dela
void selectShape(Selector *sel, Shape **shapes, int shapeCount, float x, float y)
{
    if (!sel->active)
    {

        return;
    }

    sel->selected = NULL;
    sel->index = -1;


    // percorre de tras pra frente (ultimo desenhado tem prioridade)
    for (int i = shapeCount; i >= 0; i--)
    {
        Shape *s = shapes[i];
        if (isInsideShape(x, y, s))
        {
            sel->selected = s;
            sel->index = i;
            printf("Figura selecionada (tipo=%c, index=%d)\n", s->type, i);
            return;
        }
    }
}

void deleteSelected(Selector *sel, Shape **shapes, int *shapeCount)
{
    if (!sel->selected || sel->index < 0)
        return;

    freeShape(shapes[sel->index]);

    for (int i = sel->index; i < (*shapeCount) - 1; i++)
    {
        shapes[i] = shapes[i + 1];
    }
    (*shapeCount)--;

    printf("Figura apagada!\n");

    sel->selected = NULL;
    sel->index = -1;
    sel->action = ACTION_NONE;
}

void drawSelection(const Selector *sel)
{
    if (!sel->selected)
    {
        return;
    }
    Shape *s = sel->selected;

    glColor3f(1, 0, 0);
    glLineWidth(2);

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < s->num_points; i++)
    {
        if (s->points[i][2] == 1) // faz a marcação da selecao apenas para os pontos renderizados da figura
        {
            float x = s->points[i][0];
            float y = s->points[i][1];
            glVertex2f(x -  5, y - 5);
            glVertex2f(x + 5, y - 5);
            glVertex2f(x + 5, y + 5);
            glVertex2f(x - 5, y + 5);
        }
    }
    glEnd();
}