#include "Objects.h"

int choice = 1;
int night = 0;


// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 2.0f, z = 5.0f;
// all variables initialized to 1.0, meaning
float angle = 0.0f;


void changeSize(int w, int h)
{

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset Matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}


//Plane plane(bounds, bounds);
Plane plane(20, 20);

Camera camera;

Sphere sphere[] = {
	//radius, color, initial starting position on the y axis [x], positon on board[x,z]
   Sphere(0.3, ORANGE, 6, 7.5, 1.5)
   //Sphere(0.3, BLACK, 5, 7.5, 7.5)
};

Cube cube[] = {
	//size, initial starting position on the y axis [x], positon on board[x,z]
  Cube(2, BLACK,  -0.9, 7.5, 7.5),
  Cube(14.9, ORANGE,  -6.93, 7.5, 7.5),
  Cube(2,BLACK,5,7.5,0),
  Cube(40, BLACK,-19.5,20,20),
  Cube(15, BROWN,-6.93,8,32),
  Cube(15, WHITE,-6.93,32,32),
  Cube(15, GREEN,-6.93,32,8)
  //
};

Cylinder cylinder[] = {
	//radius, size, color, initial starting position on the y axis [x], positon on board[x,z]
  Cylinder(0.8, 15, YELLOW, 0, 14.9, 0),
  Cylinder(0.8, 15, YELLOW, 0, 0, 0),
  Cylinder(10, 0.4, RED, 0, 7.5, 0.1),
  //
  Cylinder(4, 5.5, RED, 6, 5, 33.2)
};

//======Down facing torus=====
Torus torus[] = {
	//radius 1, radius 2, color, initial starting position on the y axis [x], positon on board[x,z]
   Torus(0.15, 0.8, GREY, 4.5, 7.5, 1.5),
   Torus(.2, .75,WHITE,.5,27,35)
   //
};

//=======Front facing torus=====
Torus2 torus2[] = {
	//radius 1, radius 2, color, initial starting position on the y axis [x], positon on board[x,z]
   Torus2(0.2, .5, WHITE, 5.2, 7.5, .82)
   //
};


//Pyramid pyramid[] = {
//	//color, initial starting position on the y axis [x], positon on board[x,z]
//	Pyramid(YELLOW, 3, 2, 15)
//};

Cone cone[] = {
	//color, initial starting position on the y axis [x], positon on board[x,z]
	//Cone(1,1,ORANGE, 0, 2, -4),
	Cone(2,2,ORANGE, 7.5, 27.5, 4.2),
	Cone(2,2,ORANGE, 7.5, 33, 4.2),
	Cone(1,1,YELLOW,0,2,20),
	Cone(1,1,YELLOW,0,3.5,20),
	Cone(1,1,YELLOW,0,5,20),
	Cone(1,1,YELLOW,0,6.5,20),
	Cone(1,1,YELLOW,0,8,20),
	Cone(1,1,YELLOW,0,9.5,20),
	Cone(1,1,YELLOW,0,11,20),
	Cone(1,1,YELLOW,0,12.5,20),
	Cone(1,1,YELLOW,0,14,20),
	Cone(1,1,YELLOW,0,15.5,20),
	/*Cone(1,1,YELLOW,0,17,20),
	Cone(1,1,YELLOW,0,18.5,20),*/
	Cone(1,1,YELLOW,0,20,20),
	/*Cone(1,1,YELLOW,0,21.5,20),
	Cone(1,1,YELLOW,0,23,20),*/
	Cone(1,1,YELLOW,0,24.5,20),
	Cone(1,1,YELLOW,0,26,20),
	Cone(1,1,YELLOW,0,27.5,20),
	Cone(1,1,YELLOW,0,29,20),
	Cone(1,1,YELLOW,0,30.5,20),
	Cone(1,1,YELLOW,0,32,20),
	Cone(1,1,YELLOW,0,33.5,20),
	Cone(1,1,YELLOW,0,35,20),
	Cone(1,1,YELLOW,0,36.5,20),
	Cone(1,1,YELLOW,0,38,20),


	Cone(1,1,YELLOW,0,20,1),
	Cone(1,1,YELLOW,0,20,2.5),
	Cone(1,1,YELLOW,0,20,4),
	Cone(1,1,YELLOW,0,20,5.5),
	Cone(1,1,YELLOW,0,20,7),
	Cone(1,1,YELLOW,0,20,8.5),
	Cone(1,1,YELLOW,0,20,10),
	Cone(1,1,YELLOW,0,20,11.5),
	Cone(1,1,YELLOW,0,20,13),
	Cone(1,1,YELLOW,0,20,14.5),
	/*Cone(1,1,YELLOW,0,20,16),
	Cone(1,1,YELLOW,0,20,17.5),
	Cone(1,1,YELLOW,0,20,19),
	Cone(1,1,YELLOW,0,20,20.5),
	Cone(1,1,YELLOW,0,20,22),
	Cone(1,1,YELLOW,0,20,23.5),*/
	Cone(1,1,YELLOW,0,20,25),
	Cone(1,1,YELLOW,0,20,26.5),
	Cone(1,1,YELLOW,0,20,28),
	Cone(1,1,YELLOW,0,20,29.5),
	Cone(1,1,YELLOW,0,20,31),
	Cone(1,1,YELLOW,0,20,32.5),
	Cone(1,1,YELLOW,0,20,34),
	Cone(1,1,YELLOW,0,20,35.5),
	Cone(1,1,YELLOW,0,20,37)
};

Rect rect[] = {
	//width, height, depth, color, initial starting position on the y axis [x], positon on board[x,z] 
	Rect(8,12,6,RED, 1.5 / 2, 5, 36),
	Rect(2,1,1,YELLOW, 1.5 / 2, 12, 36),
	Rect(2,1,1,YELLOW, 1.5 / 2, 11.9, 37.2),
	Rect(2,0.6,1,YELLOW, 1.2, 11.95, 36.8),
	Rect(10,7,6,WHITE, 4, 30, 4),
	Rect(8,3,4,WHITE, 2, 30,30),
	Rect(3,3,2,ORANGE,1,38,30),
	Rect(6,2,2,YELLOW,1,35,36),
	Rect(2,1,6,RED,4,32,12),
	Rect(6,1,2,RED,4,32,12)
};



void init() {

	glClearColor(0.1, 0.39, 0.88, 1.0); //Changes color of background


	glEnable(GL_DEPTH_TEST); //Objects will clip in cameras view if disabled
	glEnable(GL_LIGHTING); //Everything turns pure white if disabled
	glEnable(GL_LIGHT0); //Everything turns pure dark if disabled

	glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
	glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
	glMaterialf(GL_FRONT, GL_SHININESS, 30);

	plane.create();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();


	// Set the camera
	if (choice == 0) { // Rotational
		gluLookAt(x, 1.0f, z, x + lx, 1.0f, z + lz, 0.0f, 1.0f, 0.0f);
	}
	if (choice == 1) { // First person
		gluLookAt(camera.getX(), camera.getY(), camera.getZ(), plane.centerx(), 0.0, plane.centerz(), 0.0, 1.0, 0.0);
	}
	if (choice == 2) { //Eagle eye
		gluLookAt(x, 75.0f, z, x + lx, 1.0f, z + lz, 0.0f, 1.0f, 0.0f);
	}
	if (choice == 3) { //Poke out
		gluLookAt(camera.getX(), camera.getY(), camera.getZ(), x + lx, 1.0f, z + lz, 1.0f, 1.0f, 1.0f);
	}


	// Draw ground
	plane.draw();

	planet();


	for (int i = 0; i < sizeof sphere / sizeof(Sphere); i++) {
		sphere[i].update();
	}
	for (int i = 0; i < sizeof torus / sizeof(Torus); i++) {
		torus[i].update();
	}
	for (int i = 0; i < sizeof torus2 / sizeof(Torus2); i++) {
		torus2[i].update();
	}
	for (int i = 0; i < sizeof cube / sizeof(Cube); i++) {
		cube[i].update();
	}
	for (int i = 0; i < sizeof cylinder / sizeof(Cylinder); i++) {
		cylinder[i].update();
	}
	
	/*for (int i = 0; i < sizeof pyramid / sizeof(Pyramid); i++) {
		pyramid[i].update();
	}*/
	for (int i = 0; i < sizeof cone / sizeof(Cone); i++) {
		cone[i].update();
	}
	
	for (int i = 0; i < sizeof rect / sizeof(Rect); i++) {
		rect[i].update();
	}

	glFlush();
	glutSwapBuffers();
}

void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, GLfloat(w) / GLfloat(h), 1.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
}

void special(int key, int, int) {
	switch (key) {
	case GLUT_KEY_RIGHT: camera.moveLeft(); break;
	case GLUT_KEY_LEFT: camera.moveRight(); break;
	case GLUT_KEY_UP: camera.moveUp(); break;
	case GLUT_KEY_DOWN: camera.moveDown(); break;
	}
	glutPostRedisplay();
}


void processSpecialKeys(int key, int xx, int yy)
{

	float fraction = 0.1f;

	switch (key) {
	case GLUT_KEY_LEFT:
		angle -= 0.05f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.05f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_UP:
		x += lx * fraction;
		z += lz * fraction;
		break;
	case GLUT_KEY_DOWN:
		x -= lx * fraction;
		z -= lz * fraction;
		break;
	}
}

void keyboard_func(unsigned char key, int xx, int yy)
{

	float fraction = 0.1f;

	switch (key) {

	case 'w':
	{
		x += lx * fraction;
		z += lz * fraction;
		break;
	}
	case 's':
	{
		x -= lx * fraction;
		z -= lz * fraction;
		break;
	}
	case ' ':
	{
		if (choice == 0) {
			choice++;
			glutReshapeFunc(reshape);
			glutSpecialFunc(special);
			break;
		}

		if (choice == 1) {
			choice++;
			glutSpecialFunc(processSpecialKeys);
			break;
		}
		if (choice == 2) {
			choice++;
			glutReshapeFunc(reshape);
			glutSpecialFunc(processSpecialKeys);
			break;
		}
		if (choice == 3) {
			choice = 0;
			glutReshapeFunc(reshape);
			glutSpecialFunc(processSpecialKeys);
			break;
		}
	}

	case 'n':
	{
		if (night == 0) {
			glEnable(GL_LIGHT0);
			night++;
			break;
		}
		if (night == 1) {
			glDisable(GL_LIGHT0);
			night = 0;
			break;
		}
	}

	case 'z':
	{
		bounds++;
		break;
	}
	case 'x':
	{
		bounds--;
		break;
	}
	case'p': //to increase or decrease rate of orbit and spin
	{
		if (n <= 1.5) {
			n += 0.1;
		}
		if (o <= 15.0) {//angle of orbit
			o += 2;
		}
		glutReshapeFunc(reshape);
		glutSpecialFunc(special);
		break;
	}
	case'o':
	{
		if (n >= -1.5) {
			n -= 0.1;
		}
		if (o >= -15.0) { //will change angle of orbit
			o -= 2;
		}
		glutReshapeFunc(reshape);
		glutSpecialFunc(special);
		break;
	}

	}
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Project");
	glutDisplayFunc(display);
	glutReshapeFunc(changeSize);
	glutIdleFunc(display);
	glutSpecialFunc(processSpecialKeys);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutTimerFunc(100, timer, 0);
	glutKeyboardFunc(keyboard_func);
	// OpenGL init
	glutTimerFunc(100, timer2, 0);
	glEnable(GL_DEPTH_TEST);
	init();
	glutMainLoop();

}