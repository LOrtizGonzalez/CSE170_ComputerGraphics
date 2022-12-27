#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include "shader.h"
#include "shaderprogram.h"

#include <vector> //
#include <math.h> //

int bounds = 10;

static float year = 0, day = 0;

// curve parameter for comet pos
static GLfloat u = 0.0;
static GLfloat du = 0.1;
float n = 0.1, o = .2;

void myWireSphere(GLfloat radius, int slices, int stacks) {
	glPushMatrix();
	glRotatef(-30.0, 5.0, 0.0, 0.0); //changed from -30.00,1.00,,,
	glutWireSphere(radius, slices, stacks);
	glPopMatrix();
}

void wireBox(double width, double height, double depth) {
	glPushMatrix();
	glScalef(width, height, depth);
	glutSolidCube(1.0);
	glPopMatrix();
}

void timer(int v) {
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, v);
}

void timer2(int v) {
	u += du;
	day = (day + n); // % 360;
	year = (year + n); // % 360;
	glLoadIdentity();
	gluLookAt(7, 0, 0, 0, 0, 0, 0, 1, 0); //Changed from 5,1,1,, , , , 
	//gluLookAt(20 * cos(u / 8.0) + 12, 5 * sin(u / 8.0) + 1, 10 * cos(u / 8.0) + 2, 0, 0, 0, 0, 1, 0);
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer2, v);
}

void planet() {
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	glTranslatef(4.0, 4.0, 7.0); //modifying to move sphere
	glRotatef((GLfloat)day, 0.0, 1.0 + n, 0.0);
	myWireSphere(1.0, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glRotatef((GLfloat)year, 3.0, 2.0, 0.0);//--Checking what this does
	glTranslatef(5.0 + n, 5.0 + o, 8.0 + o); //adjusting the smaller sphere to approximate the larger one
	glRotatef((GLfloat)day, 0.0 + o, 1.0 + o, 2.0 + o); //-------angle rotation of the sphere while orbiting
	glColor3f(0.0, 0.0, 1.0);
	myWireSphere(0.2, 15, 15);
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex3f(0, -0.3, 0);
	glVertex3f(0, 0.3, 0);
	glEnd();
	glPopMatrix();
}