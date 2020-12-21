
///////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////         
// TextureDemo1.cpp
// We are using stb_image to load image data so we can create OpenGL textures. 
//https://github.com/nothings/stb
// UV origin for OpenGL is bottom-left
// Hooman Salamat
///////////////////////////////////////////////////////////////////// 


#include <iostream>
#include "stdlib.h"
#include "time.h"
#include <GL/glew.h>
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

using namespace std;

// IDs.
GLuint vao, ibo, points_vbo, colors_vbo, modelID;

// Horizontal and vertical ortho offsets.
float osH = 0.0f, osV = 0.0f, scrollSpd = 0.25f, zoom = 10.0f;

// Fixed timestep variables.
int deltaTime, currentTime, lastTime = 0;

// Matrices.
glm::mat4 mvp, view, projection;

// Camera and transform variables.
glm::vec3 position, frontVec, worldUp, upVec, rightVec; // Set by function.
GLfloat pitch, yaw,
moveSpeed = 0.1f,
turnSpeed = 1.0f;
float rotAngle = 0.0f;

// Mouse variables.
bool mouseFirst = true, mouseClicked = false;
int lastX, lastY;

// Texture variables.
GLuint textureID;
GLint width, height, bitDepth;

GLshort cube_indices[] = {
	// Front.
	0, 3, 2,
	2, 0, 1
};
//////////////x,y=u,v/////////////////
///D//-1,1=0,1        C//1,1=1,1//////////
/////////////          ///////////////
/////////////          //////////////
///A//-1,-1=0,0       B//1,-1=1,0///////
//////////////////////////////////////

//simple vertics, here use z plane, maped between triangle and image by u,v 
GLfloat cube_vertices[] = {
	//	x,		y,		z,	  u,	v
		-1.0f, -1.0f, 1.0f,	0.0f, 0.0f,		// 0.
		1.0f,  -1.0f, 1.0f, 1.0f, 0.0f,		// 1.
		1.0f,   1.0f, 1.0f, 1.0f, 1.0f,		// 2.
		-1.0f,  1.0f, 1.0f, 0.0f, 1.0f		// 3.
};

//step2: try this - pay attention to texture coordinate: (u,v) = (0.5f, 1.0f) //it now triangle 
/////////E/////////
///////     ///////
/////A       B//////
///////////////////
//GLfloat cube_vertices[] = {
//	//	x,		y,		z,	  u,	v
//		-1.0f, -1.0f, 1.0f,	0.0f, 0.0f,		// 0.
//		1.0f,  -1.0f, 1.0f, 1.0f, 0.0f,		// 1.
//		0.0f,   1.0f, 1.0f, 0.5f, 1.0f,		// 2.
//		//-1.0f,  1.0f, 1.0f, 0.0f, 1.0f		// 3.
//};
////////////////////////////////////
/////////////////////   F//1,2=1,1////////////
/////////////////       /////////////////
///D//-1,1=0,1          /////////////
//////////////          /////////////
//////////////   0,0=   ///////////////////
//////////////          ////////////////
///A//-1,-1=0,0        B//1,-1=1,0///////
//step3: try this - pay attention to vertex coordinate: (x,y,z) = (1.0f,   2.0f, 1.0f)
//GLfloat cube_vertices[] = {
//	//	x,		y,		z,	  u,	v
//		-1.0f, -1.0f, 1.0f,	0.0f, 0.0f,		// 0.
//		1.0f,  -1.0f, 1.0f, 1.0f, 0.0f,		// 1.
//		1.0f,   2.0f, 1.0f, 1.0f, 1.0f,		// 2.
//		-1.0f,  1.0f, 1.0f, 0.0f, 1.0f		// 3.
//};

//////////////x,y=u,v/////////////////
///D//-1,1=-1,2      0,2     1,2     C//1,1=2,2//////////
/////////////                        ///////////////
/////////=-1,1       0,1     1,1     ////2,1//////////
/////////////                        //////////////
///A//-1,-1=-1,0     0,0    1,0      B//1,-1=2,0///////
//////////////////////////////////////
////step4: the world-space polygon is mapped to a 2x3 3 X 2 rectangle in texture space
//GLfloat cube_vertices[] = {
//	//	x,		y,		z,	  u,	v
//		-1.0f, -1.0f, 1.0f,	-1.0f, 0.0f,	// 0.
//		1.0f,  -1.0f, 1.0f, 2.0f, 0.0f,	    // 1.
//		1.0f,   1.0f, 1.0f, 2.0f, 2.0f,		// 2.
//		-1.0f,  1.0f, 1.0f, -1.0f, 2.0f		// 3.
//};

//////////////x,y=u,v/////////////////
///D//-1,1=-1,2      0,2     C//1,1=1,2//////////
/////////////                ///////////////
/////////=-1,1       0,1     //1,1//////////////
/////////////                //////////////
///A//-1,-1=-1,0     0,0     B//1,-1=1,0///////
//////////////////////////////////////
////step5: the world-space polygon is mapped to a 2 X 2 rectangle in texture space
//GLfloat cube_vertices[] = {
//	//	x,		y,		z,	  u,	v
//		-1.0f, -1.0f, 1.0f,	-1.0f, 0.0f,	// 0.
//		1.0f,  -1.0f, 1.0f, 1.0f, 0.0f,	    // 1.
//		1.0f,   1.0f, 1.0f, 1.0f, 2.0f,		// 2.
//		-1.0f,  1.0f, 1.0f, -1.0f, 2.0f		// 3.
//};

GLfloat colors[] = {
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f
};

static unsigned int
program,
vertexShaderId,
fragmentShaderId;


// Globals.
//static int isWire = 0; // Is wireframe?

void resetView()
{
	position = glm::vec3(0.0f, 0.0f, 5.0f);
	frontVec = glm::vec3(0.0f, 0.0f, -1.0f);
	worldUp = glm::vec3(0, 1, 0);
	pitch = 0.0f;
	yaw = -90.0f;
}

void loadTexture()
{
	stbi_set_flip_vertically_on_load(true);//flip from upside down

	unsigned char* image = stbi_load("Media/spheremap.png", &width, &height, &bitDepth, 0);//load an image
	if (!image) {
		cout << "Unable to load file!" << endl;
		// Could add a return too if you modify init.
	}
	//image is rgb need to set to gpu
	//bind with texture id
	glGenTextures(1, &textureID);//is like glGenBuffers, connect texture object to the GPU
	//can handle many texture same time gl text ure 0,1,2.... has some limitation
	//gl_texture0-31 exist
	glActiveTexture(GL_TEXTURE0);
	//bind 2d image
	glBindTexture(GL_TEXTURE_2D, textureID);//bind the texture//what kind of texture. Texture 2D one type of texture
	//load texture image to GPU
	//target,level, internal format, w,h, border,format(external), type(data type of rgb component), pixel
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, 
		height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	//give me the address where to sent the picture
	//target,pname, param
	//S and T just mean U and V or X and Y //vec4.xyzw == vec4.rgba == vec4.strq
	//The GL_REPEAT mode has textures repeat when you go past (0,0) to (1,1) range
	//The GL_CLAMP_TO_EDGE mode has textures stop at the last pixel when you fall off the edge.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//setting co-ordinate of image and triange polygon mapped here
	glUniform1i(glGetUniformLocation(program, "texture0"), 0);

	glGenerateMipmap(GL_TEXTURE_2D);
	//free the cpu now, as it already loaded in gpu
	// Clean up. But we don't want to unbind the texture or we cannot use it.
	stbi_image_free(image);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	//change this line from previous code in last 2 parameter
	//previously those are 0,Null
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(cube_vertices[0]) * 5, 0);
	glEnableVertexAttribArray(0);
	//load u v form 4,5 index of all vertices
	// Now for the UV/ST values.
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(cube_vertices[0]) * 5, (void*)(sizeof(cube_vertices[0]) * 3));
	glEnableVertexAttribArray(2);

	colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);//0,NULL);
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

	loadTexture();

	createBuffer();

	// Enable depth test.
	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0, 1.0, 1.0, 1.0); // black background
}

//---------------------------------------------------------------------
//
// calculateView
//
void calculateView()
{
	frontVec.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontVec.y = sin(glm::radians(pitch));
	frontVec.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontVec = glm::normalize(frontVec);
	rightVec = glm::normalize(glm::cross(frontVec, worldUp));
	upVec = glm::normalize(glm::cross(rightVec, frontVec));

	view = glm::lookAt(position, position + frontVec, upVec);
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

	glBindVertexArray(vao);

	// Update the projection or view if perspective.
	projection = glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	calculateView();
	transformObject(0.4f, YZ_AXIS, rotAngle -= 0, glm::vec3(0.0f, 0.0f, 0.0f));

	//Ordering the GPU to start the pipeline
	//draw 2 trangle
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	//step1: try the following to see different texture maps effect
	//draw topleft triangle
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
	//draw bottom right triangle
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)(sizeof(cube_indices[0]) * 3));

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

	glutSwapBuffers(); // Now for a potentially smoother render.
}

void idle()
{
	//glutPostRedisplay();
}

void timer(int id)
{
	glutPostRedisplay();
	glutTimerFunc(33, timer, 0);
}

void keyDown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		position += frontVec * moveSpeed;
		break;
	case 's':
		position -= frontVec * moveSpeed;
		break;
	case 'a':
		position += rightVec * moveSpeed;
		break;
	case 'd':
		position -= rightVec * moveSpeed;
		break;
	case ' ':
		resetView();
		break;
	}
}

void keyDownSpecial(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		pitch -= turnSpeed;
		break;
	case GLUT_KEY_DOWN:
		pitch += turnSpeed;
		break;
	case GLUT_KEY_LEFT:
		yaw += turnSpeed;
		break;
	case GLUT_KEY_RIGHT:
		yaw -= turnSpeed;
		break;
	}
}

void mouseMove(int x, int y)
{
	//cout << "Mouse pos: " << x << "," << y << endl;
	if (mouseClicked)
	{
		pitch += (GLfloat)((y - lastY) * 0.1);
		yaw -= (GLfloat)((x - lastX) * 0.1);
		lastY = y;
		lastX = x;
	}
}

void mouseClick(int btn, int state, int x, int y)
{
	/*cout << "Clicked: " << (btn == 0 ? "left " : "right ") << (state == 0 ? "down " : "up ") <<
		"at " << x << "," << y << endl;*/
	if (state == 0)
	{
		lastX = x;
		lastY = y;
		mouseClicked = true;
		glutSetCursor(GLUT_CURSOR_NONE);
		cout << "Mouse clicked." << endl;
	}
	else
	{
		mouseClicked = false;
		glutSetCursor(GLUT_CURSOR_INHERIT);
		cout << "Mouse released." << endl;
	}
}

void clean()
{
	cout << "Cleaning up!" << endl;
	glDeleteTextures(1, &textureID);
}

//---------------------------------------------------------------------
//
// main
//
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1024, 768);
	glutCreateWindow("Texture Demo 1");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.
	init();

	// Set all our glut functions.
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutTimerFunc(33, timer, 0);
	glutKeyboardFunc(keyDown);
	glutSpecialFunc(keyDownSpecial);
	glutMouseFunc(mouseClick);
	//glutPassiveMotionFunc(mouseMove); // or...
	glutMotionFunc(mouseMove); // Requires click to register.
	atexit(clean); // This GLUT function calls specified function before terminating program. Useful!
	glutMainLoop();
}
