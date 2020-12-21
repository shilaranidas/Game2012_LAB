#include <GL\freeglut_std.h>

char keyboardbuffer[20][40];
int keyboardindex_x[20];
int keyboardindex_y;

// I use this to put text on the screen
void Sprint(int x, int y, char* st)
{
	int l, i;

	l = strlen(st); // see how many characters are in text string.
	glRasterPos2i(x, y); // location to start printing text
	for (i = 0; i < l; i++) // loop until i is greater then l
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]); // Print a character on the screen
	}

}

static void TimeEvent(int te)
{
	glutPostRedisplay(); // Update screen with new data
	glutTimerFunc(100, TimeEvent, 1); // Reset our timmer.
}

// Setup our Opengl world, called once at startup.
void init(void)
{
	int i;
	glClearColor(0.0, 0.0, 0.0, 0.0); // When screen cleared, use black.
	glShadeModel(GL_SMOOTH); // How the object color will be rendered smooth or flat
	glEnable(GL_DEPTH_TEST); // Check depth when rendering
	for (i = 0; i < 20; i++)
	{
		keyboardindex_x[i] = 0;
	}
	keyboardindex_y = 8;
}

// Draw our world
void display(void)
{
	int i;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the screen

	glMatrixMode(GL_PROJECTION); // Tell opengl that we are doing project matrix work
	glLoadIdentity(); // Clear the matrix
	glOrtho(-8.0, 8.0, -8.0, 8.0, 0.0, 30.0); // Setup an Ortho view
	glMatrixMode(GL_MODELVIEW); // Tell opengl that we are doing model matrix work. (drawing)
	glLoadIdentity(); // Clear the model matrix

	glColor3f(1.0, 0.0, 0.0);
	for (i = 0; i < 20; i++)
	{
		Sprint(-8, 8 - i, keyboardbuffer[i]);
	}

	glutSwapBuffers();
}

// This is called when the window has been resized.
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void arrows(int key, int x, int y)
{

	if (key == GLUT_KEY_UP)
	{
		keyboardindex_y--;
		if (keyboardindex_y < 0) keyboardindex_y = 0;
	}

	if (key == GLUT_KEY_DOWN)
	{
		keyboardindex_y++;
		if (keyboardindex_y > 19) keyboardindex_y = 19;
	}
}

// Read the keyboard
void keyboard(unsigned char key, int x, int y)
{

	if (isalnum(key) || key == ' ') // Check for a letter of the alphabet
	{
		keyboardbuffer[keyboardindex_y][keyboardindex_x[keyboardindex_y]] = key;
			keyboardindex_x[keyboardindex_y]++;
			if (keyboardindex_x[keyboardindex_y] > 39) keyboardindex_x[keyboardindex_y] = 39; // Stop buffer overflow
	}
	if (key == 8)
	{
		keyboardindex_x[keyboardindex_y]= keyboardindex_x[keyboardindex_y] - 1;
			if (keyboardindex_x[keyboardindex_y] < 0) keyboardindex_x[keyboardindex_y] = 0; // Stop index from being negitive
			keyboardbuffer[keyboardindex_y][keyboardindex_x[keyboardindex_y]] = 0;
	}

	switch (key)
	{
	case 27:
		exit(0); // exit program when [ESC] key presseed
		break;
	default:
		break;
	}
}

// Main program
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(10, 10);
	glutCreateWindow(argv[0]);
	glutSetWindowTitle("GlutWindow");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(arrows);
	glutTimerFunc(10, TimeEvent, 1);
	glutMainLoop();
	return 0;
}

