
//
// Perspective view of a color cube using Perspective()
//
// Colors are assigned to each face!
//

#include <iostream>
#include "stdlib.h"
#include "time.h"
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "prepShader.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <array>
using namespace std;

#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
#define XY_AXIS glm::vec3(1,1,0)
#define YZ_AXIS glm::vec3(0,1,1)
#define XZ_AXIS glm::vec3(1,0,1)

const int NumVertices = 72; //(6 faces)(2 triangles/face)(3 vertices/triangle)
//top face/4 triangle, bottom 4 tri, side, 12 triangele
GLfloat cube_verticesP[NumVertices][3] = { 0 };
GLfloat cube_colorsP[NumVertices][3] = { 0 };

// IDs.
GLuint vao, ibo, points_vbo, colors_vbo, modelID;

// Fixed timestep variables.
int deltaTime, currentTime, lastTime = 0;

// Matrices.
glm::mat4 mvp, view, projection;

float rotAngle = 0;

// Viewing transformation parameters

GLfloat radius = 1.0;
GLfloat theta = 0.0;
GLfloat phi = 0.0;

const GLfloat  dr = 5.0 * 3.14f / 180.0f;

// Projection transformation parameters


GLfloat  zNear = 0.5, zFar = 3.0;

GLfloat  fovy = 45.0f;  // Field-of-view in Y direction angle (in degrees)
GLfloat  aspect = 4.0f / 3.0f;       // Viewport aspect ratio


static unsigned int
program,
vertexShaderId,
fragmentShaderId;

std::array<glm::vec3, NumVertices> verticesP = {};
std::array<glm::vec3, NumVertices> colorsP = {};

std::array<glm::vec3, 12> unique_verticesP = {
	glm::vec3(-0.5f, -0.9f, 0.9f),		// 0.
	glm::vec3(0.5f, -0.9f, 0.9f),		// 1.
	glm::vec3(0.9f, 0.0f, 0.9f),		// 2.
	glm::vec3(0.5f, 0.9f, 0.9f),		// 3.
	glm::vec3(-0.5f, 0.9f, 0.9f),		// 4.
	glm::vec3(-0.9f, 0.0f, 0.9f),		// 5.

	glm::vec3(-0.5f, -0.9f, -0.9f),		// 6.
	glm::vec3(0.5f, -0.9f, -0.9f),		// 7.
	glm::vec3(0.9f, 0.0f, -0.9f),		// 8.
	glm::vec3(0.5f, 0.9f, -0.9f),		// 9.
	glm::vec3(-0.5f, 0.9f, -0.9f),		// 10.
	glm::vec3(-0.9f, 0.0f, -0.9f),		// 11.

};

std::array<glm::vec3, 12> unique_colorsP = {
	glm::vec3(1.0f, 0.0f, 0.0f),//0
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),

	glm::vec3(1.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 1.0f, 1.0f),
	glm::vec3(0.5f, 0.0f, 1.0f),//6
	glm::vec3(1.0f, 0.5f, 0.0f),//7

	glm::vec3(0.0f, 0.0f, 0.5f),
	glm::vec3(0.5f, 0.5f, 0.0f),
	glm::vec3(0.5f, 1.0f, 0.5f),//10
	glm::vec3(0.0f, 0.5f, 0.5f)
};


//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices

int Index = 0;

void quad(int a, int b, int c, int d, int e, int f)
{
	colorsP[Index] = unique_colorsP[a];
	verticesP[Index] = unique_verticesP[a]; Index++;
	colorsP[Index] = unique_colorsP[a];
	verticesP[Index] = unique_verticesP[b]; Index++;
	colorsP[Index] = unique_colorsP[a];
	verticesP[Index] = unique_verticesP[c]; Index++;
	colorsP[Index] = unique_colorsP[a];
	verticesP[Index] = unique_verticesP[d]; Index++;
	colorsP[Index] = unique_colorsP[a];
	verticesP[Index] = unique_verticesP[e]; Index++;
	colorsP[Index] = unique_colorsP[a];
	verticesP[Index] = unique_verticesP[f]; Index++;

}
//void hexa(int a, int b, int c, int d
//	, int e, int f, int g, int h, int i, int j, int k, int l
//	, int m, int n, int o)
//{
//	colors[Index] = unique_colors[a];
//	vertices[Index] = unique_vertices[a]; Index++;
//	colors[Index] = unique_colors[a];
//	vertices[Index] = unique_vertices[b]; Index++;
//	colors[Index] = unique_colors[a];
//	vertices[Index] = unique_vertices[c]; Index++;
//	colors[Index] = unique_colors[a];
//	vertices[Index] = unique_vertices[d]; Index++;
//	colors[Index] = unique_colors[a];
//	vertices[Index] = unique_vertices[e]; Index++;
//	colors[Index] = unique_colors[a];
//	vertices[Index] = unique_vertices[f]; Index++;
//	colors[Index] = unique_colors[a];
//	vertices[Index] = unique_vertices[g]; Index++;
//	colors[Index] = unique_colors[a];
//	vertices[Index] = unique_vertices[h]; Index++;
//	colors[Index] = unique_colors[a];
//	vertices[Index] = unique_vertices[i]; Index++;
//	colors[Index] = unique_colors[a];
//	vertices[Index] = unique_vertices[j]; Index++;
//	colors[Index] = unique_colors[a];
//	vertices[Index] = unique_vertices[k]; Index++;
//	colors[Index] = unique_colors[a];
//	vertices[Index] = unique_vertices[l]; Index++;
//	colors[Index] = unique_colors[a];
//	vertices[Index] = unique_vertices[m]; Index++;
//	colors[Index] = unique_colors[a];
//	vertices[Index] = unique_vertices[n]; Index++;
//	colors[Index] = unique_colors[a];
//	vertices[Index] = unique_vertices[o]; Index++;
//}


//----------------------------------------------------------------------------

// generate 18 triangles: 54 vertices and 54 colors
void colorcube()
{
	//bottom
	//hexa(0, 1, 2, 2, 3, 4, 4, 5, 0, 0, 2, 4, 4, 0, 5);
	quad(0, 1, 2, 3, 4, 5);
	quad(0, 2, 3, 3, 5, 0);
	//front left
	quad(6, 0, 5, 5, 11, 6);
	//front 
	quad(7, 1, 0, 0, 6, 7);
	//front right
	quad(1, 7, 8, 8, 2, 1);
	//back right
	quad(3, 2, 8, 8, 9, 3);
	//back
	quad(3, 9, 10, 10, 4, 3);
	//back left
	quad(11, 5, 4, 4, 10, 11);
	//top
	quad(6, 7, 8, 9, 10, 11);
	quad(6, 8, 9, 9, 11, 6);
	
	cout << Index << endl;

	
	




	
	

	//top
	//hexa(6, 7, 8, 8, 9, 10, 10, 11, 6, 6, 8, 10, 10, 6, 11);
	for (int i = 0; i < NumVertices; ++i) {

		cube_verticesP[i][0] = verticesP[i][0];
		cube_verticesP[i][1] = verticesP[i][1];
		cube_verticesP[i][2] = verticesP[i][2];

		cube_colorsP[i][0] = colorsP[i][0];
		cube_colorsP[i][1] = colorsP[i][1];
		cube_colorsP[i][2] = colorsP[i][2];
	}
}

void init(void)
{
	vertexShaderId = setShader((char*)"vertex", (char*)"cubeProjection.vert");
	fragmentShaderId = setShader((char*)"fragment", (char*)"cubeProjection.frag");
	program = glCreateProgram();
	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);
	glLinkProgram(program);
	glUseProgram(program);

	colorcube();

	modelID = glGetUniformLocation(program, "mvp");

	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_verticesP), cube_verticesP, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colorsP), cube_colorsP, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

// Enable depth test.
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
}





//---------------------------------------------------------------------
//
// transformModel
//
void transformObject(float scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, glm::vec3(scale));
	mvp = projection * view * Model;
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &mvp[0][0]);
}

//---------------------------------------------------------------------
//
// display
//
void display(void)
{
	// Delta time stuff.
	currentTime = glutGet(GLUT_ELAPSED_TIME); // Gets elapsed time in milliseconds.
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::vec3  eye(radius * sin(theta) * cos(phi),
		radius * sin(theta) * sin(phi),
		radius * cos(theta)
	);

	//glm::vec3  eye(0.0, 0.0, 3.0);
	glm::vec3  at(0.0, 0.0, 0.0);
	glm::vec3  up(0.0, 1.0, 0.0);
	view = glm::lookAt(eye, at, up);


	glBindVertexArray(vao);


	//projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.5f, 3.0f);
	projection = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);

	//transformObject(0.2f, X_AXIS, rotAngle = 0.0 , glm::vec3(0.0f, 0.0f, 0.0f));
	transformObject(0.2f, YZ_AXIS, rotAngle += ((float)45 / (float)1000 * deltaTime), glm::vec3(0.0f, 0.0f, 0.0f));


	//Ordering the GPU to start the pipeline GL_TRIANGLES
	glDrawArrays(GL_TRIANGLES, 0, NumVertices); // Try GL_LINE_STRIP too!

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

	glutSwapBuffers(); // Now for a potentially smoother render.
}

void
keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;

	case 'z': zNear *= 1.1; zFar *= 1.1; break;
	case 'Z': zNear *= 0.9; zFar *= 0.9; break;
	case 'r': radius *= 2.0; break;
	case 'R': radius *= 0.5; break;
	case 'o': theta += dr; break;
	case 'O': theta -= dr; break;
	case 'p': phi += dr; break;
	case 'P': phi -= dr; break;

	case ' ':  // reset values to their defaults
		zNear = 0.5;
		zFar = 3.0;

		radius = 1.0;
		theta = 0.0;
		phi = 0.0;
		break;
	}

	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	aspect = GLfloat(width) / height;
}


void idle()
{
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

int
main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Color Cube");

	glewInit();

	init();

	glutIdleFunc(idle);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);

	glutMainLoop();
	return 0;
}
