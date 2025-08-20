
#include <GL/glut.h>
#include <stdbool.h>
#include "input.h"
#include "menu.h"
#include "storage.h"
#include "shape.h"
#include "config.h"


// Definição das variáveis globais
float r = 1.0f, g = 1.0f, b = 1.0f;
ShapeStack *storage;

void init(void)
{
    glClearColor(r, g, b, 0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, windW,0, windH);
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
            glPointSize(5.0f);
            glColor3f(0.0f, 0.0f, 0.0f); // cor preta para o ponto
            glBegin(GL_POINTS);
            glVertex2f(s->points[0][0], s->points[0][1]);
            glEnd();
            break;
        case LINE:
            glBegin(GL_LINES);
            glVertex2f(s->points[0][0], s->points[0][1]);
            glVertex2f(s->points[1][0], s->points[1][1]);
            glEnd();
            break;
            // outros tipos: TRIANGLE, SQUARE, POLYGON
        case TRIANGLE:
            //...
            break;
        case SQUARE:
            //
            break;
        case POLYGON:
            //
            break;
        case FREE_DRAW:
            //
            break;
        default:
            //
            break; // evita warning se esquecer algum
        }
    }
    glFlush();
}

int main(int argc, char **argv)
{
    int option;
    bool control = true;
    storage = criarPilha(10); // pilha de tamanho fixo, 10 figuras no maximo

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
    programUI();

    glutMainLoop();
    return 0;
}

