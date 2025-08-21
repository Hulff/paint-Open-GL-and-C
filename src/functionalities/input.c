// detecta teclado e mouse

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#include "shape.h"
#include "storage.h"
#include "menu.h"
#include "config.h"
#include "operations.h"
#include "helper.h"

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
bool rotationStarted = false; // controla início da rotação
float last_angle = 0;         // ângulo anterior para rotação
int n_points = 0;             // número de pontos criados
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
        n_points = 0;
        break;
    case 'l': // criar reta
        printf("Clique no canvas para escolher o primeiro ponto da reta\n");
        waitingForClick = true;
        createShapeMode = true;
        currentShapeType = LINE;
        n_points = 0;
        break;
    case 't': // transladar
        if (storage->top < 0)
        {
            // Não há figuras para transformar
            printf("Nenhuma figura para transformar.\n");
        }
        else
        {

            currentOperation = TRANSLATE;
            printf("Clique no canvas para transladar a figura\n");
            waitingForClick = true;
            createShapeMode = false;
        }
        break;
    case 'r': // rotacionar
        if (storage->top < 0)
        {
            // Não há figuras para transformar
            printf("Nenhuma figura para transformar.\n");
        }
        else
        {

            currentOperation = ROTATE;
            printf("Clique no canvas para rotacionar a figura\n");
            waitingForClick = true;
            createShapeMode = false;
        }
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
    if (n_points < 1 && currentShapeType == SHAPE_POINT && waitingForClick && createShapeMode && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        float fx = (float)x;
        float fy = (float)(windH - y);

        // cria a figura
        Shape *s = createShape(1, SHAPE_POINT);
        s->points[n_points][n_points] = fx;
        s->points[n_points][1] = fy;
        s->points[n_points][2] = 1; // z = 1 indica que o ponto deve ser renderizado
        n_points++;
        // adiciona à pilha
        adicionarFigura(storage, s);

        waitingForClick = false; // resetar captura para que a proxima figura seja selecionada
        createShapeMode = false;

        programUI();
        printf("Ponto adicionado em (%.2f, %.2f)\n", fx, fy);

        glutPostRedisplay();
    }
    else if (n_points < 2 && currentShapeType == LINE && waitingForClick && createShapeMode && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        float fx = (float)x;
        float fy = (float)(windH - y);
        Shape *s;
        if (n_points == 0)
        {
            s = createShape(2, SHAPE_POINT);
        }
        else
        {
            s = storage->items[storage->top];
            s->type = LINE;
        };
        // cria a figura
        s->points[n_points][0] = fx;
        s->points[n_points][1] = fy;
        s->points[n_points][2] = 1; // z = 1 indica que o ponto deve ser renderizado
        // adiciona à pilha
        if (n_points == 0)
        {
            adicionarFigura(storage, s);
        };
        n_points++;

        if (n_points == 2)
        {
            waitingForClick = false; // resetar captura para que a proxima figura seja selecionada
            createShapeMode = false;
        }

        programUI();
        printf("Ponto adicionado em (%.2f, %.2f)\n", fx, fy);
        if (n_points == 1)
        {
            printf("selecione a posição do segundo ponto\n");
        }

        glutPostRedisplay();
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        waitingForClick = false; // cancelar captura de ponto
        createShapeMode = false; // cancelar modo de criação de forma
        currentOperation = NONE; // cancelar operação atual
        n_points = 0;            // reseta o numero de pontos
        rotationStarted = false; // reseta o estado de rotação
        last_angle = 0;          // reseta o ângulo da rotação incremental
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
    else if (currentOperation == ROTATE && waitingForClick && !createShapeMode)
    {
        int pos = storage->top;
        float fx = (float)x;
        float fy = (float)(windH - y);
        Shape *s = storage->items[pos];

        // calcula o centro da figura
        float cx = 0, cy = 0;
        for (int i = 0; i < s->num_points; i++)
        {
            cx += s->points[i][0];
            cy += s->points[i][1];
        }
        cx = cx / s->num_points;
        cy = cy / s->num_points;

        // ângulo atual do mouse em relação ao centro
        float current_angle = atan2(fy - cy, fx - cx); // retorna o ângulo desse vetor (fy - cy, fx - cx) em relação ao eixo X positivo.

        if (!rotationStarted)
        {
            last_angle = current_angle;
            rotationStarted = true;
        }

        // diferença incremental para ser mais suave
        float delta_angle = current_angle - last_angle;

        // aplica rotação em torno do centro
        rotate(s->points, s->num_points, delta_angle, cx, cy);

        last_angle = current_angle;

        programUI();
        printf("Clique com o botao direito para confirmar a rotação\n");
        glutPostRedisplay();
    }
}