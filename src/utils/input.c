// detecta teclado e mouse

#include <GL/glut.h>
#include <stdio.h>

extern float r, g, b;

// ler teclado
void teclado(unsigned char key, int x, int y)
{
    printf("Tecla: %c\n", key);
    if (key == 'b')
    {
        r = 0.0f;
        g = 0.0f;
        b = 0.0f;
    }
    if (key == 'w')
    {
        r = 1.0f;
        g = 1.0f;
        b = 1.0f;
    }
    glutPostRedisplay(); // pede pra redesenhar a tela
    if (key == 27)
        exit(0); // ESC
}
// ler as setas
void tecladoEspecial(int key, int x, int y)
{
    if (key == GLUT_KEY_UP)
        printf("Seta ↑\n");
    if (key == GLUT_KEY_DOWN)
        printf("Seta ↓\n");
    if (key == GLUT_KEY_LEFT)
        printf("Seta ←\n");
    if (key == GLUT_KEY_RIGHT)
        printf("Seta →\n");
}
// ler pos do clique do mouse no canva
void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        printf("Clique esquerdo (%d,%d)\n", x, y);
}
