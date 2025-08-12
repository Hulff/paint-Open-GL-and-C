
#include <GL/glut.h>
#include "input.h"

// Definição das variáveis globais
float r = 1.0f, g = 1.0f, b = 1.0f;

void init(void)
{
    glClearColor(r, g, b, 0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 200, 0, 200);
};
void display()
{
    glClearColor(r, g, b, 1.0f); // atualiza a cor de fundo sempre
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(200, 200);
    glutInitWindowPosition(1300, 200);
    glutCreateWindow("Paint 2025 atualizado Premium");

    init();
    glutDisplayFunc(display);

    //leituras do usuario 
    glutKeyboardFunc(teclado);
    glutSpecialFunc(tecladoEspecial);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}
