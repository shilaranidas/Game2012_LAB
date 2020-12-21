
///////////////////////////////////////////////////////////////////////
//
// Sierpenski.cpp
//
///////////////////////////////////////////////////////////////////////

using namespace std;

#include "stdlib.h"
#include "time.h"
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "prepShader.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <array>


GLuint Buffers[1];

const GLuint NumVertices = 30000;
const GLfloat scale = 0.5f;
GLfloat vertices[NumVertices][2] = { 0 };
std::array<glm::vec2, NumVertices> vertices1;

#define BUFFER_OFFSET(x)  ((const void*) (x))
static unsigned int
program,
vertexShaderId,
fragmentShaderId;


void init(void)
{

	// Create shader program executable.

	vertexShaderId = setShader((char*)"vertex", (char*)"cube.vert");
	fragmentShaderId = setShader((char*)"fragment", (char*)"cube.frag");
	program = glCreateProgram();
	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);
	glLinkProgram(program);
	glUseProgram(program);

	//Next, we create a buffer object on the GPU and place our data in that object.
	glGenBuffers(2, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBindAttribLocation(program, 0, "vPosition");
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);


	// Select an arbitrary initial point inside of the triangle
		vertices[0][0] = 0.25; 
		vertices[0][1] = 0.50;
		vertices1[0][0] = 0.25;
		vertices1[0][1] = 0.50;

	//// Specifiy the vertices for a triangle
	//GLfloat points[3][2] = {
	//	{-1.0, -1.0}, {0.0, 1.0}, {1.0, -1.0}
	//};


	//// compute and store N-1 new points
	//for (int i = 1; i < NumVertices; ++i) {
	//	int j = (int)(rand() % 3);   // pick a vertex at random

	//	// Compute the point halfway between the selected vertex
	//	//   and the previous point
	//	vertices[i][0] = (vertices[i - 1][0] + points[j][0]) / 2.0;
	//	vertices[i][1] = (vertices[i - 1][1] + points[j][1]) / 2.0;
	//}

	//using glm and array - this does exactly the samething as above
	std::array<glm::vec2, 3> points1 ={ glm::vec2(-1.0, -1.0), glm::vec2(0.0, 1.0), glm::vec2(1.0, -1.0) };

	//// compute and store N-1 new points
	for (int i = 1; i < NumVertices; ++i) {
		int j = (int)(rand() % 3);   // pick a vertex at random
		vertices1[i] = (vertices1[i - 1] + points1[j]) / 2.0f;
		
		vertices[i][0] = vertices1[i][0];
		vertices[i][1] = vertices1[i][1];
	}

}


//---------------------------------------------------------------------
//
// display
//

void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//Selecting the buffer
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);


	//Pushing the coordinates of the vertices into the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Ordering the GPU to start the pipeline
	glDrawArrays(GL_POINTS, 0, NumVertices);

	glFlush();
}


void idle()
{
	//glutPostRedisplay();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

//---------------------------------------------------------------------
//
// main
//

int main(int argc, char** argv)
{
	//Before we can open a window, theremust be interaction between the windowing systemand OpenGL.In GLUT, this interaction is initiated by the following function call :
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);

	//if you comment out this line, a window is created with a default size
	glutInitWindowSize(512, 512);

	//the top-left corner of the display
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Hello World");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	//If there are events in the queue, our program responds to them through functions
	//called callbacks.A callback function is associated with a specific type of event.
	//A display callback is generated when the application programm or the
	//operating system determines that the graphics in a window need to be redrawn.
	glutDisplayFunc(display);

	glutIdleFunc(idle);
	glutKeyboardFunc(keyInput);

	//begin an event-processing loop
	glutMainLoop();



}