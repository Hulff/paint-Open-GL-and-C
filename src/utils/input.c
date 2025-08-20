// detecta teclado e mouse

#include <GL/glut.h>
#include <stdio.h>

#include "shape.h"
#include "storage.h"
#include "menu.h"
#include "config.h"
#include "operations.h"

extern float r, g, b;
extern ShapeStack *storage; // pilha global de figuras

typedef enum
{
    TRANSLATE,
    ROTATE,
    NONE,
} Operation;
bool waitingForClick = false; // controla captura de ponto
bool createShapeMode = false; // controla criação de forma
Operation currentOperation;   // guarda qual operação está sendo feita
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
        createShapeMode = true;
        currentShapeType = SHAPE_POINT;
        break;
    case 't': // transladar
        currentOperation = TRANSLATE;
        printf("Clique no canvas para transladar a figura\n");
        waitingForClick = true;
        createShapeMode = false;
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
    if (waitingForClick && createShapeMode && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
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
        createShapeMode = false;

        programUI();
        printf("Ponto adicionado em (%.2f, %.2f)\n", fx, fy);

        glutPostRedisplay();
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        printf("Botão direito do mouse pressionado\n");
        waitingForClick = false; // cancelar captura de ponto
        createShapeMode = false; // cancelar modo de criação de forma
        currentOperation = NONE; // cancelar operação atual
    }
}
// ler pos do mouse sempre
void mouseMove(int x, int y)
{
    if (currentOperation == TRANSLATE && waitingForClick && !createShapeMode)
    {
        int pos = storage->top; // posição da figura a ser transladada
        float fx = (float)x;
        float fy = (float)(windH - y);

        Shape *s = storage->items[pos];
        // deslocamento dos pontos
        float dx = fx - s->points[0][0];
        float dy = fy - s->points[0][1];

        translate(s->points, s->num_points, dx, dy);

        programUI();
        printf("Clique com o botao direito para confirmar a translação\n");
        printf("Figura transladada para (%.2f, %.2f)\n", fx, fy);

        glutPostRedisplay();
    }
}