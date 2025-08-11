
#include <GL/glut.h>

void init(void) {
    glClearColor(1,1,1,0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 200, 0, 200);

};
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(1, 0, 0);
    glBegin(GL_POLYGON);
        glVertex2i(30,50);
        glVertex2i(30,150);
        glVertex2i(170,150);
        glVertex2i(170,50);
    glEnd();


    glColor3f(1, 1, 1);

    glBegin(GL_POLYGON);
        glVertex2i(70,90);
        glVertex2i(70,110);
        glVertex2i(130,110);
        glVertex2i(130,90);
    glEnd();

    glColor3f(1, 1, 1);
    
    glBegin(GL_POLYGON);
        glVertex2i(90,70);
        glVertex2i(90,130);
        glVertex2i(110,130);
        glVertex2i(110,70);
    glEnd();


    



    glFlush();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(200, 200);
    glutInitWindowPosition(1300, 200);
    glutCreateWindow("Tarefa CG boneco quadrado");
    
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
