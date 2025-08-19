// detecta teclado e mouse

#include <GL/glut.h>
#include <stdio.h>

#include "shape.h"
#include "storage.h"
#include "menu.h"
#include "config.h"

extern float r, g, b;
extern ShapeStack *storage; // pilha global de figuras

bool waitingForClick = false; // controla captura de ponto
ShapeType currentShapeType;   // guarda qual figura está sendo criada

// ler teclado
void teclado(unsigned char key, int x, int y)
{
    printf("Tecla: %c\n", key);
    (void)x;
    (void)y;

    switch (key)
    {
    case 'b':
        r = 0;
        g = 0;
        b = 0;
        break;
    case 'w':
        r = 1;
        g = 1;
        b = 1;
        break;
    case 'q':
        exit(0); // ESC ou sair
    case 'p':    // criar ponto
        printf("Clique no canvas para criar o ponto\n");
        waitingForClick = true;
        currentShapeType = SHAPE_POINT;
        break;
    }

    glutPostRedisplay();
}
// ler as setas (sem uso ainda)
void tecladoEspecial(int key, int x, int y)
{
    (void)x;
    (void)y;
    
    if (key == GLUT_KEY_UP)
        printf("Seta ↑\n");
    if (key == GLUT_KEY_DOWN)
        printf("Seta ↓\n");
    if (key == GLUT_KEY_LEFT)
        printf("Seta ←\n");
    if (key == GLUT_KEY_RIGHT)
        printf("Seta →\n");
}

// ler pos do clique do mouse 
void mouse(int button, int state, int x, int y)
{
    if (waitingForClick && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        float fx = (float)x;
        float fy = (float)(windH - y); 

        // cria a figura
        Shape *s = createShape(1);
        s->points[0][0] = fx;
        s->points[0][1] = fy;

        // adiciona à pilha
        adicionarFigura(storage, s);

        waitingForClick = false; // resetar captura para que a proxima figura seja selecionada

        programUI();
        printf("Ponto adicionado em (%.2f, %.2f)\n", fx, fy);

        glutPostRedisplay();
    }
}