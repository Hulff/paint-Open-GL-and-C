
#include <GL/freeglut.h>
#include <stdbool.h>
#include "input.h"
#include "menu.h"
#include "storage.h"
#include "shape.h"
#include "config.h"
#include "select.h"

// Definição das variáveis globais
float r = 1.0f, g = 1.0f, b = 1.0f;
ShapeStack *storage;
Selector *selector;

void init(void)
{
    glClearColor(r, g, b, 0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, windW, 0, windH);
};
void display()
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i <= storage->top; i++)
    {
        Shape *s = storage->items[i];
        switch (s->type)
        {
        case SHAPE_POINT:
            glPointSize(4.0f);
            glColor3f(s->r, s->g, s->b);
            glBegin(GL_POINTS);
            glVertex2f(s->points[0][0], s->points[0][1]);
            glEnd();
            break;
        case LINE:
            glBegin(GL_LINES);
            for (int j = 0; j < 2; j++)
            {
                glColor3f(s->r, s->g, s->b); // cor da linha
                glVertex2f(s->points[j][0], s->points[j][1]);
            }
            glEnd();
            break;
        case TRIANGLE:
            glBegin(GL_TRIANGLES);
            for (int j = 0; j < 3; j++)
            {
                glColor3f(s->r, s->g, s->b); // cor do triangulo
                glVertex2f(s->points[j][0], s->points[j][1]);
            }
            glEnd();
            break;
        case FREE_DRAW:
            glBegin(GL_LINE_LOOP);
            for (int j = 0; j < s->num_points; j++)
            {
                if (s->points[j][2] == 1)
                {
                    glColor3f(s->r, s->g, s->b);
                    glVertex2f(s->points[j][0], s->points[j][1]);
                }
            }
            glEnd();
            break;
        case POLYGON:
            glBegin(GL_POLYGON);
            for (int j = 0; j < s->num_points; j++)
            {
                if (s->points[j][2] == 1)
                {
                    glColor3f(s->r, s->g, s->b);
                    glVertex2f(s->points[j][0], s->points[j][1]);
                }
            }
            glEnd();
            break;
        default:
            //
            break; // evita warning se esquecer algum
        }
    }
    if (selector->selected != NULL)
    {
        drawSelection(selector);
    }
    glFlush();
}

int main(int argc, char **argv)
{
    int option;
    bool control = true;
    storage = criarPilha(maxFig); // pilha de tamanho fixo, 10 figuras no maximo

    selector = malloc(sizeof(Selector));
    initSelector(selector, maxFig); // incializa os valores para o selector

    while (control)
    {
        startUI(&option);
        switch (option)
        {
        case 1: // iniciar programa
            control = false;
            break;
        case 4: // atalhos para o paint
            keyBindsUI();
            break;
        case 3:
            return 0;
        default:
            break;
        }
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windW, windH);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Paint 2025 atualizado Premium");

    init();
    glutDisplayFunc(display);

    glutKeyboardFunc(teclado);
    glutSpecialFunc(tecladoEspecial);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mouseMove);
    glutMouseWheelFunc(mouseWheel);
    programUI();

    glutMainLoop();
    return 0;
}
