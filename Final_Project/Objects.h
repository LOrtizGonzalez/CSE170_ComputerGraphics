#pragma once

#include "Functions.h"



// Colors
GLfloat WHITE[] = { 1, 1, 1 };
GLfloat GREY[] = { 0.5, 0.5, 0.5 };
GLfloat BLACK[] = { 0, 0, 0 };
GLfloat YELLOW[] = { 1, 1, 0 };
GLfloat CYAN[] = { 0, 1, 1 };
GLfloat RED[] = { 1, 0, 0 };
GLfloat GREEN[] = { 0, 1, 0 };
GLfloat MAGENTA[] = { 1, 0, 1 };
GLfloat ORANGE[] = { 1, 0.5, 0 };
GLfloat SILVER[] = { 0.90, 0.91, 0.98 };
GLfloat BROWN[] = { 0.5, 0.35, 0.05 };


class Camera {
	double theta;      // determines the x and z positions
	double y;          // the current y axis position
	double dTheta;     // increment in theta for moving the camera
	double dy;         // increment in y 
public:
	Camera() : theta(0), y(3), dTheta(0.04), dy(0.2) {} //The starting position of the camera
	double getX() { return bounds * cos(theta); } //Bounds of the camera
	double getY() { return y; }
	double getZ() { return bounds * sin(theta); }
	void moveRight() { theta += dTheta; } //Camera movement
	void moveLeft() { theta -= dTheta; }
	void moveUp() { y += dy; }
	void moveDown() { if (y > dy) y -= dy; }
};



// Plane class
class Plane {
	int displayListId;
	int width;
	int depth;
public:
	Plane(int width, int depth) : width(width), depth(depth) {} //Planes parameters
	double centerx() { return width / 2; } //Cameras focus point [Where the camera fixates to]
	double centerz() { return depth / 2; }
	void create() {
		displayListId = glGenLists(1);
		glNewList(displayListId, GL_COMPILE);

		GLfloat lightPosition[] = { 4, 3, 7, 1 }; //Position of lighting 

		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

		glBegin(GL_QUADS);
		glNormal3d(0, 1, 0); //Shader of plane
		for (int x = 0; x < width - 1; x++) {
			for (int z = 0; z < depth - 1; z++) { //Dimensions of the plane [Plane gets weird if parameters are changed here]
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, SILVER); //Color of plane
				glVertex3d(x, 0, z);
				glVertex3d(x + 1, 0, z);
				glVertex3d(x + 1, 0, z + 1);
				glVertex3d(x, 0, z + 1);
			}
		}
		glEnd();
		glEndList();
	}
	void draw() {
		glCallList(displayListId);
	}
};




class Sphere {
	double radius; // Defines the radius
	GLfloat* color; //Dynamically assigns the color
	double maximumHeight; //Defines the maximum height
	double x; //x
	double y; //y
	double z; //z
	int direction; // direction
public:
	Sphere(double r, GLfloat* c, double h, double x, double z) : //constructor
		radius(r), color(c), maximumHeight(h), direction(-1), y(h), x(x), z(z) {
	}

	void update() {
		y += direction * 0.005;
		if (y > maximumHeight) {
			y = maximumHeight; direction = -1;
		}
		else if (y < radius) {
			y = radius; direction = 1;
		}
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		glTranslated(x, y, z);
		glutSolidSphere(radius, 30, 30);
		glPopMatrix();
	}

	~Sphere() {}; //destructor

};


//======Down facing torus=====
class Torus {
	double radius; // Defines the radius
	double r;
	GLfloat* color; //Dynamically assigns the color
	double maximumHeight; //Defines the maximum height
	double x; //x
	double y; //y
	double z; //z
	int direction; // direction
public:
	Torus(double r, double R, GLfloat* c, double h, double x, double z) : //constructor
		radius(r), r(R), color(c), maximumHeight(h), direction(-1), y(h), x(x), z(z) {
	}

	void update() {

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		glTranslated(x, y, z);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glutSolidTorus(radius, r, 30, 30);
		glPopMatrix();
	}

	~Torus() {}; //destructor

};


//=======Front facing torus=====
class Torus2 {
	double radius; // Defines the radius
	double r;
	GLfloat* color; //Dynamically assigns the color
	double maximumHeight; //Defines the maximum height
	double x; //x
	double y; //y
	double z; //z
	int direction; // direction
public:
	Torus2(double r, double R, GLfloat* c, double h, double x, double z) : //constructor
		radius(r), r(R), color(c), maximumHeight(h), direction(-1), y(h), x(x), z(z) {
	}

	void update() {

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		glTranslated(x, y, z);
		glutSolidTorus(radius, r, 30, 30);
		glPopMatrix();
	}

	~Torus2() {}; //destructor

};


// Cube Class
class Cube {
	double leSize; // Defines the radius
	GLfloat* color; //Dynamically assigns the color
	double maximumHeight; //Defines the maximum height
	double x; //x
	double y; //y
	double z; //z
	int direction; // direction
public:
	Cube(double s, GLfloat* c, double h, double x, double z) :
		leSize(s), color(c), maximumHeight(h), direction(-1), y(h), x(x), z(z) {
	}

	void update() {

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		//glColor3ub(color[0], color[1], color[2]);
		glTranslated(x, y, z);
		glutSolidCube(leSize);

		glPopMatrix();

	}

	~Cube() {};

};

//=======Front facing Cylinder=====
class Cylinder {
	double radius;
	double leSize; // Defines the radius
	GLfloat* color; //Dynamically assigns the color
	double maximumHeight; //Defines the maximum height
	double x; //x
	double y; //y
	double z; //z
	int direction; // direction
public:
	Cylinder(double r, double s, GLfloat* c, double h, double x, double z) :
		radius(r), leSize(s), color(c), maximumHeight(h), direction(-1), y(h), x(x), z(z) {
	}

	void update() {

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

		//glRotatef(90.0, 1.0, 0.0, 0.0);
		glTranslated(x, y, z);
		glutSolidCylinder(radius, leSize, 30, 30);
		glPopMatrix();

	}

	~Cylinder() {};

};

//=======Down facing Cylinder=====
class Cylinder2 {
	double radius;
	double leSize; // Defines the radius
	GLfloat* color; //Dynamically assigns the color
	double maximumHeight; //Defines the maximum height
	double x; //x
	double y; //y
	double z; //z
	int direction; // direction
public:
	Cylinder2(double r, double s, GLfloat* c, double h, double x, double z) :
		radius(r), leSize(s), color(c), maximumHeight(h), direction(-1), y(h), x(x), z(z) {
	}

	void update() {

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

		glRotatef(90.0, 1.0, 0.0, 0.0);
		glTranslated(x, y, z);
		glutSolidCylinder(radius, leSize, 30, 30);
		glPopMatrix();

	}

	~Cylinder2() {};

};


// Pyramid Class
class Pyramid {
	GLfloat* color; //Dynamically assigns the color
	double maximumHeight; //Defines the maximum height
	double x; //x
	double y; //y
	double z; //z
	int direction; // direction
public:
	Pyramid(GLfloat* c, double h, double x, double z) :
		color(c), maximumHeight(h), direction(-1), y(h), x(x), z(z) {
	}

	void update() {

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		//glColor3ub(color[0], color[1], color[2]);
		glTranslated(x, y, z);
		glutSolidTetrahedron();

		glPopMatrix();

	}

	~Pyramid() {};

};


//Cone Class
class Cone {
	double base;
	double leSize; // Defines the radius
	GLfloat* color; //Dynamically assigns the color
	double maximumHeight; //Defines the maximum height
	double x; //x
	double y; //y
	double z; //z
	int direction; // direction
public:
	Cone(double b, double s, GLfloat* c, double h, double x, double z) :
		base(b), leSize(s), color(c), maximumHeight(h), direction(-1), y(h), x(x), z(z) {
	}

	void update() {

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

		glTranslated(x, y, z);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		glutSolidCone(base, leSize, 30, 30);
		glPopMatrix();
	}

	~Cone() {};

};

//Rectangle class
class Rect {
	double S1;
	double S2;
	double S3;
	GLfloat* color; //Dynamically assigns the color
	double maximumHeight; //Defines the maximum height
	double x; //x
	double y; //y
	double z; //z
	int direction; // direction
public:
	Rect(double s, double ss, double sss, GLfloat* c, double h, double x, double z) :
		S1(s), S2(ss), S3(sss), color(c), maximumHeight(h), direction(-1), y(h), x(x), z(z) {
	}

	void update() {

		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		//glColor3ub(color[0], color[1], color[2]);
		glTranslated(x, y, z);
		wireBox(S1, S2, S3);
		//glutSolidCube(S1);

		glPopMatrix();

	}

	~Rect() {};

};
