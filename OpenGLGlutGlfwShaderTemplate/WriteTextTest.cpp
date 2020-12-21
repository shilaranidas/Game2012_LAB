//#include <windows.h>
//#include <GL/glut.h>
//#include <GL\freeglut_ext.h>
#include <iostream>
#include "stdlib.h"
#include "time.h"
#include <GL\glew.h>
#include <GL/freeglut.h> 
#include "prepShader.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <array>
using namespace std;

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
#define XY_AXIS glm::vec3(1,1,0)
#define YZ_AXIS glm::vec3(0,1,1)
#define XZ_AXIS glm::vec3(1,0,1)
//#include <stdio.h> 
int w, h;
const int font = (int)GLUT_BITMAP_8_BY_13;
char s[30];
double t;
static void resize(int width, int height)
{
    const float ar = (float)width / (float)height;
    w = width;
    h = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);     glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
//void setOrthographicProjection() {
//    glMatrixMode(GL_PROJECTION);
//    glPushMatrix();
//    glLoadIdentity();
//    gluOrtho2D(0, w, 0, h);
//    glScalef(1, -1, 1);
//    glTranslatef(0, -h, 0);
//    glMatrixMode(GL_MODELVIEW);
//}
void resetPerspectiveProjection() {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
void renderBitmapString(float x, float y, void* font, const char* string) {
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}
static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1.0, 0.0, 0.0);
    //setOrthographicProjection();
    glPushMatrix();
    glLoadIdentity();
    renderBitmapString(200, 200, (void*)font, "Font Rendering - Programming Techniques");
    renderBitmapString(300, 220, (void*)font, s);
    renderBitmapString(300, 240, (void*)font, "Esc - Quit");
    glPopMatrix();
    resetPerspectiveProjection();
    glutSwapBuffers();
}
void update(int value) {
    t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    int time = (int)t;
    //sprintf(s, "TIME : %2d Sec", time);
    glutTimerFunc(1000, update, 0);
    glutPostRedisplay();
}
void printInteraction(void)
{
    std::cout << "Interaction:" << std::endl;
    std::cout << "Press the up and down arrow keys to move the viewpoint over the field." << std::endl
        << "Press the left and right arrow keys to cycle through the filters." << std::endl;
}
static unsigned int
program,
vertexShaderId,
fragmentShaderId;
// IDs.
GLuint vao, ibo, points_vbo, colors_vbo, modelID;
glm::vec3 position, frontVec, worldUp, upVec, rightVec; // Set by function.
GLfloat pitch, yaw,
moveSpeed = 0.1f,
turnSpeed = 1.0f;
float rotAngle = 0.0f;
GLshort cube_indices[] = {
    // Front.
    0, 3, 2,
    2, 0, 1
};

GLfloat grass_vertices[] = {
    //	x,		y,		z,	  u,	v
        -100.0f, 0.0f, 100.0f,	0.0f, 0.0f,		// 0.
        100.0f,  0.0f, 100.0f, 8.0f, 0.0f,		// 1.
        100.0f,   0.0f, -100.0f, 8.0f, 8.0f,		// 2.
        -100.0f,  0.0f, -100.0f, 0.0f, 8.0f		// 3.
};

GLfloat sky_vertices[] = {
    //	x,		y,		z,	  u,	v
        -100.0f, 0.0f, -70.0f,	0.0f, 0.0f,		// 0.
        100.0f,  0.0f, -70.0f, 1.0f, 0.0f,		// 1.
        100.0f,   120.0f, -70.0f, 1.0f, 1.0f,		// 2.
        -100.0f,  120.0f, -70.0f, 0.0f, 1.0f		// 3.
};

GLfloat colors[] = {
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f
};

void resetView()
{
    position = glm::vec3(0.0f, 0.0f, 5.0f);
    frontVec = glm::vec3(0.0f, 0.0f, -1.0f);
    worldUp = glm::vec3(0, 1, 0);
    pitch = 0.0f;
    yaw = -90.0f;
}
void createBuffer()
{
    vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grass_vertices), grass_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(grass_vertices[0]) * 5, 0);
    glEnableVertexAttribArray(0);

    // Now for the UV/ST values.
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(grass_vertices[0]) * 5, (void*)(sizeof(grass_vertices[0]) * 3));
    glEnableVertexAttribArray(2);

    colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.

    glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.


    vao = 1;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sky_vertices), sky_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(sky_vertices[0]) * 5, 0);
    glEnableVertexAttribArray(0);

    // Now for the UV/ST values.
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(sky_vertices[0]) * 5, (void*)(sizeof(sky_vertices[0]) * 3));
    glEnableVertexAttribArray(2);

    colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.

    glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.


}
void init(void)
{
    vertexShaderId = setShader((char*)"vertex", (char*)"triangles9.vert");
    fragmentShaderId = setShader((char*)"fragment", (char*)"triangles9.frag");
    program = glCreateProgram();
    glAttachShader(program, vertexShaderId);
    glAttachShader(program, fragmentShaderId);
    glLinkProgram(program);
    glUseProgram(program);

    modelID = glGetUniformLocation(program, "mvp");

    resetView();

    createBuffer();

    // Enable depth test.
    glEnable(GL_DEPTH_TEST);

    glClearColor(1.0, 1.0, 1.0, 1.0); // black background
}
int main(int argc, char** argv)
{
    printInteraction();
    glutInit(&argc, argv);
    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
   
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);  
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Font Rendering Using Bitmap Font - Programming Techniques0");   
    glutReshapeFunc(resize);

    glutDisplayFunc(display);
    //glutTimerFunc(25, update, 0);   
    init();
    glutMainLoop();
    return EXIT_SUCCESS;
}