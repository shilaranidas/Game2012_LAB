#include <windows.h>  // For MS Windows
#include <GL/glut.h>   // (or others, depending on the system in use)
void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);  // Set display-window color to     white.
        glMatrixMode(GL_PROJECTION);       // Set projection parameters.
    gluOrtho2D(0.0, 400.0, 0.0, 400.0);
}
void drawDiamond()
{
    glBegin(GL_LINE_LOOP);
    glVertex2f(125, 350);
    glVertex2f(245, 350);
    glVertex2f(290, 300);
    glVertex2f(182, 200);
    glVertex2f(75, 300);
    glEnd();


    glBegin(GL_LINE_LOOP);
    glVertex2f(109, 333);
    glVertex2f(138, 350);
    glVertex2f(159, 337);
    glVertex2f(123, 300);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(109, 333);
    glVertex2f(123, 300);
    glVertex2f(154, 225);
    glVertex2f(92, 300);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(290, 300);
    glVertex2f(75, 300);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(123, 300);
    glVertex2f(159, 337);
    glVertex2f(154, 300);
    glVertex2f(171, 225);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(181, 300);
    glVertex2f(159, 337);
    glVertex2f(181, 350);
    glVertex2f(209, 337);
    glVertex2f(181, 300);
    glVertex2f(171, 225);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(181, 300);
    glVertex2f(209, 337);
    glVertex2f(219, 300);
    glVertex2f(195, 225);
    glVertex2f(181, 300);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(209, 337);
    glVertex2f(243, 300);
    glVertex2f(195, 225);
    glVertex2f(219, 300);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(209, 337);
    glVertex2f(229, 350);
    glVertex2f(260, 333);
    glVertex2f(243, 300);
    glVertex2f(209, 337);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(260, 333);
    glVertex2f(278, 300);
    glVertex2f(210, 225);
    glVertex2f(243, 300);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(195, 225);
    glVertex2f(182, 200);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(171, 225);
    glVertex2f(182, 200);
    glEnd();

}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);  // Clear display window.
    glColor3f(0.0, 0.0, 0.0);      // Set line segment color to blue.
    float bottomY = 200.0f;
                                   // your code goes here
    drawDiamond();
    //glTranslatef(0.0f, -150, 0.0f);
    glTranslatef(0.0f, bottomY, 0.0f);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, -bottomY, 0.0f);
    drawDiamond();
    glFlush();     // Process all OpenGL routines as quickly as possible.

}
void main(int argc, char** argv)
{
    glutInit(&argc, argv);                         // Initialize GLUT.
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);   // Set display mode.
    glutInitWindowPosition(50, 100);   // Set top-left display-window    position.
        glutInitWindowSize(400, 400);      // Set display-window width and     height.
        glutCreateWindow("Diamond Project"); // Create display window. 

    init();                            // Execute initialization procedure. 
    glutDisplayFunc(display);       // Send graphics to display window. 


    glutMainLoop();
}