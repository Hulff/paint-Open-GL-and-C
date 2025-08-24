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
#include "select.h"

extern float r, g, b;
extern ShapeStack *storage; // pilha global de figuras (criado na Main)
extern Selector *selector;  // selector global (criado na Main)

typedef enum
{
    TRANSLATE,
    ROTATE,
    SCALE,
    SHEAR,
    COLOR,
    NONE,
    SELECTION,
} Operation;

typedef enum {
    SHEAR_HORIZONTAL,
    SHEAR_VERTICAL
} ShearType;

bool waitingForClick = false; // controla captura de ponto
bool createShapeMode = false; // controla criação de forma
bool rotationStarted = false; // controla início da rotação
bool shearingStarted = false; // controla início do cisalhamento

int n_points = 0; // número de pontos criados

float last_angle = 0; // ângulo anterior para rotação

ShearType currentShearType = SHEAR_HORIZONTAL; //padrão
float last_mouse_x = 0; // posição anterior do mouse para cisalhamento horizontal
float current_shx = 0.0f; // valor atual do cisalhamento horizontal
float last_mouse_y = 0; // posição anterior do mouse para cisalhamento vertical
float current_shy = 0.0f; // valor atual do cisalhamento vertical
float shear_center_x = 0.0f, shear_center_y = 0.0f;
Shape *beforeShearFig = NULL;

float current_scale = 0;      // escala incial
float center_scale_x = 0;     // centro x da escala
float center_scale_y = 0;     // centro y da escala
Shape *beforeScaleFig = NULL; // guarda figura original antes de escalar

Operation currentOperation; // guarda qual operação está sendo feita
ShapeType currentShapeType; // guarda qual figura está sendo criada

int selectedColorPos = 0; // guarda a posição da cor escolhida
float colors[8][3] = {
    {0.0f, 0.0f, 0.0f}, // preto
    {1.0f, 0.0f, 0.0f}, // vermelho
    {0.0f, 1.0f, 0.0f}, // verde
    {0.0f, 0.0f, 1.0f}, // azul
    {1.0f, 1.0f, 0.0f}, // amarelo
    {1.0f, 0.0f, 1.0f}, // magenta
    {0.0f, 1.0f, 1.0f}, // ciano
    {1.0f, 1.0f, 1.0f}, // branco
};
// helper para resetar estados
void resetStates()
{
    waitingForClick = false; // cancelar captura de ponto
    createShapeMode = false; // cancelar modo de criação de forma
    currentOperation = NONE; // cancelar operação atual
    n_points = 0;            // reseta o numero de pontos
    rotationStarted = false; // reseta o estado de rotação
    shearingStarted = false; // reseta o estado de cisalhamento
    last_angle = 0;          // reseta o ângulo da rotação incremental
    current_scale = 0;       // reseta a escala
    center_scale_x = 0;      // reseta o centro x da escala
    center_scale_y = 0;      // reseta o centro y da escala
    selectedColorPos = 0;    // reseta a posição da cor escolhida

    if (beforeShearFig != NULL)
    {
    free(beforeShearFig->points);
    free(beforeShearFig);
    beforeShearFig = NULL;
    }

    if (beforeScaleFig != NULL)
    {
        // libera memória da figura original antes de escalar
        free(beforeScaleFig->points);
        free(beforeScaleFig);
        beforeScaleFig = NULL;
    }
}

// ler teclado
void teclado(unsigned char key, int x, int y)
{
    printf("Tecla: %c\n", key);

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
    case 'c':
        resetStates(); // resetar estados
        printf("use o scroll para mudar as cores\n");
        waitingForClick = true;
        currentOperation = COLOR;
        break;
    case 'p':          // criar ponto
        resetStates(); // resetar estados

        printf("Clique no canvas para criar o ponto\n");
        waitingForClick = true;
        createShapeMode = true;
        currentShapeType = SHAPE_POINT;
        n_points = 0;
        break;
    case 'l': // criar reta
        printf("Clique no canvas para escolher o primeiro ponto da reta\n");
        resetStates(); // resetar estados
        waitingForClick = true;
        createShapeMode = true;
        currentShapeType = LINE;
        n_points = 0;
        break;
    case 'k': // criação livre
        printf("Clique no canvas para escolher o primeiro ponto do desenho livre\n");
        resetStates(); // resetar estados
        waitingForClick = true;
        createShapeMode = true;
        currentShapeType = FREE_DRAW;
        n_points = 0;
        break;
    case 'j': // criação livre preenchida
        printf("Clique no canvas para escolher o primeiro ponto do desenho livre\n");
        resetStates(); // resetar estados
        waitingForClick = true;
        createShapeMode = true;
        currentShapeType = POLYGON;
        n_points = 0;
        break;
    case 't': // transladar
        if (verifyAvailability(storage, selector))
        {

            resetStates(); // resetar estados

            currentOperation = TRANSLATE;
            printf("Mova o mouse no canvas para transladar a figura\n");
            waitingForClick = true;
            createShapeMode = false;
        }
        break;
    case 'r': // rotacionar
        if (verifyAvailability(storage, selector))
        {

            resetStates(); // resetar estados

            currentOperation = ROTATE;
            printf("Mova o mouse no canvas para rotacionar a figura\n");
            waitingForClick = true;
            createShapeMode = false;
        }
        break;
    case 'e': // escalar
        if (verifyAvailability(storage, selector))
        {

            resetStates(); // resetar estados
            currentOperation = SCALE;
            printf("Use o scroll para controlar a escala\n");
        }

        break;
    case 'z': // cisalhar horizontal
        if (storage->top < 0)
        {
            // Não há figuras para transformar
            printf("Nenhuma figura para transformar.\n");
        }
        else
        {

            resetStates(); // resetar estados

            currentOperation = SHEAR;
            printf("Mova o mouse no canvas para cisalhar a figura\n");
            waitingForClick = true;
            createShapeMode = false;
        }

        break;
    case 's': // seleção
        if (storage->top < 0)
        {
            // Não há figuras para transformar
            printf("Nenhuma figura para selecionar.\n");
        }
        else
        {

            resetStates(); // resetar estados
            currentOperation = SELECTION;
            waitingForClick = true;
            setSelectionMode(selector, 1);
            printf("Clique na figura que deseja selecionar\n");
        }
        break;
    }

    glutPostRedisplay();
}
// ler as setas (sem uso ainda)
void tecladoEspecial(int key, int x, int y)
{

    if (key == GLUT_KEY_UP) {
        printf("Seta ↑\n");
        currentShearType = SHEAR_VERTICAL;
    }
    if (key == GLUT_KEY_DOWN) {
        printf("Seta ↓\n");
        currentShearType = SHEAR_VERTICAL;
    }
    if (key == GLUT_KEY_LEFT) {
        printf("Seta ←\n");
        currentShearType = SHEAR_HORIZONTAL;
    }
    if (key == GLUT_KEY_RIGHT) {
        printf("Seta →\n");
        currentShearType = SHEAR_HORIZONTAL;
    }
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
        s->points[n_points][0] = fx;
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
    else if (n_points < 15 && currentShapeType == FREE_DRAW && waitingForClick && createShapeMode && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        float fx = (float)x;
        float fy = (float)(windH - y);
        Shape *s;
        if (n_points == 0)
        {
            s = createShape(15, SHAPE_POINT); // 15 pontos sao alocados
        }
        else // GL_LINE_LOOP aceita a partir de 2 pontos
        {
            s = storage->items[storage->top];
            s->type = FREE_DRAW;
        }
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

        if (n_points == 15)
        {
            waitingForClick = false; // resetar captura para que a proxima figura seja selecionada
            createShapeMode = false;
        }

        programUI();
        printf("Ponto adicionado em (%.2f, %.2f)\n", fx, fy);
        if (n_points != 15)
        {
            printf("selecione a posição do %d ponto\n", n_points + 1);
            printf("o maximo sao 15 vertices \n");
        }

        glutPostRedisplay();
    }
    else if (n_points < 15 && currentShapeType == POLYGON && waitingForClick && createShapeMode && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        float fx = (float)x;
        float fy = (float)(windH - y);
        Shape *s;
        if (n_points == 0)
        {
            s = createShape(15, SHAPE_POINT); // 15 pontos sao alocados
        }
        else if (n_points == 1)
        {
            s = storage->items[storage->top];
            s->type = LINE;
        }
        else if (n_points >= 2) // GL_POLYGON aceita a partir de 3 pontos
        {
            s = storage->items[storage->top];
            s->type = POLYGON;
        }
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

        if (n_points == 15)
        {
            waitingForClick = false; // resetar captura para que a proxima figura seja selecionada
            createShapeMode = false;
        }

        programUI();
        printf("Ponto adicionado em (%.2f, %.2f)\n", fx, fy);
        if (n_points != 15)
        {
            printf("selecione a posição do %d ponto\n", n_points + 1);
            printf("o maximo sao 15 vertices \n");
        }

        glutPostRedisplay();
    }
    else if (currentOperation == SELECTION && waitingForClick && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        float fx = (float)x;
        float fy = (float)(windH - y);

        programUI();
        printf("buscando figura mais proxima de (%.2f, %.2f)\n", fx, fy);
        // seleciona um elemento
        selectShape(selector, storage->items, storage->top, fx, fy);

        glutPostRedisplay();
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        resetStates(); // cancelar captura de ponto
    }
}
// ler pos do mouse sempre
void mouseMove(int x, int y)
{

    if (currentOperation == TRANSLATE && waitingForClick && !createShapeMode && selector->selected != NULL)
    {
        int pos = selector->index; // posição da figura a ser transladada
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
    else if (currentOperation == ROTATE && waitingForClick && !createShapeMode && selector->selected != NULL)
    {
        int pos = selector->index; // posição da figura a ser transladada

        float fx = (float)x;
        float fy = (float)(windH - y);
        Shape *s = storage->items[pos];

        // calcula o centro da figura
        float cx = 0, cy = 0;
        calcRealCenter(s, &cx, &cy);

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
    else if (currentOperation == SHEAR && waitingForClick && !createShapeMode)
    {
        int pos = storage->top;
        Shape *s = storage->items[pos];

        float fx = (float)x;
        float fy = (float)(windH - y);

        // inicia controle do cisalhamento
        if (!shearingStarted)
        {
            if (s->num_points <= 0) return; // nada a fazer

            beforeShearFig = malloc(sizeof(Shape));
            beforeShearFig->type = s->type;
            beforeShearFig->num_points = s->num_points;
            beforeShearFig->id = s->id;
            beforeShearFig->points = malloc(s->num_points * sizeof(float[3]));

            if (!beforeShearFig->points) { free(beforeShearFig); return; } // checa malloc

            for (int i = 0; i < s->num_points; i++)
            {
                beforeShearFig->points[i][0] = s->points[i][0];
                beforeShearFig->points[i][1] = s->points[i][1];
                beforeShearFig->points[i][2] = s->points[i][2];
            }

            calcRealCenter(beforeShearFig, &shear_center_x, &shear_center_y);
            last_mouse_x = fx;
            last_mouse_y = fy;
            current_shx = 0.0f;
            current_shy = 0.0f;
            shearingStarted = true;
        }

        // deslocamento horizontal desde o último movimento
        float delta_x = fx - last_mouse_x;
        float delta_y = fy - last_mouse_y;

        // sensibilidade: ajuste conforme achar confortável
        float sensibilidade = 0.01f;

        if (currentShearType == SHEAR_HORIZONTAL)
        {
            // atualiza cisalhamento acumulado
            current_shx += delta_x * sensibilidade;
            // aplica o cisalhamento em torno do centro (usando a figura congelada)
            cisalhamento_h(s->points, beforeShearFig->points, s->num_points, shear_center_x, shear_center_y, current_shx);
        }
        else if (currentShearType == SHEAR_VERTICAL)
        {
            // atualiza cisalhamento acumulado
            current_shy += delta_y * sensibilidade;
            // aplica o cisalhamento em torno do centro (usando a figura congelada)
            cisalhamento_v(s->points, beforeShearFig->points, s->num_points, shear_center_x, shear_center_y, current_shy);
        }

        last_mouse_x = fx;
        last_mouse_y = fy;

        programUI();
        printf("Clique com o botao direito para confirmar o cisalhamento\n");
        printf("Clique nas setas para alternar entre cisalhamento horizontal e vertical\n");
        if (currentShearType == SHEAR_HORIZONTAL) {
            printf("shx atual: %.3f\n", current_shx);
        } else if (currentShearType == SHEAR_VERTICAL) {
            printf("shy atual: %.3f\n", current_shy);
        }
        glutPostRedisplay();
    }
}

void mouseWheel(int wheel, int direction, int x, int y)
{

    if (currentOperation == SCALE && !createShapeMode && selector->selected != NULL)
    {
        int pos = selector->index; // posição da figura a ser transladada
        Shape *s = storage->items[pos];
        // calcula o centro da figura original
        if (center_scale_x == 0 && center_scale_y == 0) // garante apenas um cálculo por figura
        {
            // guardar os valores iniciais da figura original antes de escalar
            beforeScaleFig = malloc(sizeof(Shape));
            beforeScaleFig->type = s->type;
            beforeScaleFig->num_points = s->num_points;
            beforeScaleFig->id = s->id;
            beforeScaleFig->points = malloc(s->num_points * sizeof(float[3])); // bloco contínuo

            // copia os pontos
            for (int i = 0; i < s->num_points; i++)
            {
                beforeScaleFig->points[i][0] = s->points[i][0];
                beforeScaleFig->points[i][1] = s->points[i][1];
                beforeScaleFig->points[i][2] = s->points[i][2];
            }

            // calcula o centro real usando a função
            calcRealCenter(s, &center_scale_x, &center_scale_y);
        }

        if (direction > 0 && (1 + current_scale) < 2.0) // limite máximo
        {
            current_scale += 0.1;
        }
        else if (direction < 0 && (1 + current_scale) > 0.2) // limite mínimo
        {
            current_scale -= 0.1;
        }
        escala(s->points, beforeScaleFig->points, beforeScaleFig->num_points, center_scale_x, center_scale_y, 1 + current_scale, 1 + current_scale);

        programUI();
        printf("Use o scroll para controlar a escala\n");
        printf("Use o botao direito para confirmar\n");
        printf("Escala atual %2.f%% \n", (100 * current_scale));

        glutPostRedisplay();
    }
    if (currentOperation == COLOR && !createShapeMode && selector->selected != NULL)
    {
        int pos = selector->index; // posição da figura a ser transladada
        Shape *s = storage->items[pos];
        if (direction > 0)
        {
            selectedColorPos++;
            if (selectedColorPos > 8)
                selectedColorPos = 0;
        }
        else
        {
            selectedColorPos--;
            if (selectedColorPos < 0)
                selectedColorPos = 8;
        }
        s->r = colors[selectedColorPos][0];
        s->g = colors[selectedColorPos][1];
        s->b = colors[selectedColorPos][2];

        programUI();
        printf("Use o scroll para controlar a cor\n");
        printf("Use o botao direito para confirmar\n");
        printf("Cor atual: (%.2f, %.2f, %.2f)\n", s->r, s->g, s->b);

        glutPostRedisplay();
    }
}
