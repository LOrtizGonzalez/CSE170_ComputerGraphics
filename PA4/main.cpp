#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

//#define cimg_use_bmp
#include<CImg.h>
//#include <../../CImg-3.2.0/CImg.h>
using namespace cimg_library;

#include <iostream>
#include "shader.h"
#include "shaderprogram.h"

#include <vector> //
//#include <math.h> //
#define _USE_MATH_DEFINES
#include <math.h>

CImg <unsigned char> texture;

/*=================================================================================================
	DOMAIN
=================================================================================================*/

// Window dimensions
const int InitWindowWidth = 800;
const int InitWindowHeight = 800;
int WindowWidth = InitWindowWidth;
int WindowHeight = InitWindowHeight;

// Last mouse cursor position
int LastMousePosX = 0;
int LastMousePosY = 0;

// Arrays that track which keys are currently pressed
bool key_states[256];
bool key_special_states[256];
bool mouse_states[8];

// Other parameters
bool draw_wireframe = false;

float n = 8.0f; //Number of sides donut has //Controls number of triangles
float r = 0.25f; //Small radius   //Controlls the size of the donut
float R = 0.5f; //Big radius //Controlls the size of the donut
std::vector <glm::vec4> colors;

GLuint textureId = 0; //Texture ID

void init(void);


float p(int i, int n) {  //Calculates paramater with position times 2pi / number of sides
	float p = i * (2*M_PI / (float)n);
	return p;
}

float p_t(int i, int n) {  //Calculates [0,1] division for texture implimentation
	float p_t = (i * ((2 * M_PI) / (float)n)) / (2*M_PI);
	return p_t;
}

/*=================================================================================================
	SHADERS & TRANSFORMATIONS
=================================================================================================*/

ShaderProgram PassthroughShader;
ShaderProgram PerspectiveShader;
//ShaderProgram PerspectiveTextureShader;
ShaderProgram PerspectiveTextureLightShader;

glm::mat4 PerspProjectionMatrix(1.0f);
glm::mat4 PerspViewMatrix(1.0f);
glm::mat4 PerspModelMatrix(1.0f);

float perspZoom = 1.0f, perspSensitivity = 0.35f;
float perspRotationX = 0.0f, perspRotationY = 0.0f;

/*=================================================================================================
	OBJECTS
=================================================================================================*/

GLuint axis_VAO;
GLuint axis_VBO[2];

GLuint torus_VAO;
GLuint torus_VBO[4];


float axis_vertices[] = {
	//x axis
	-1.0f,  0.0f,  0.0f, 1.0f,
	1.0f,  0.0f,  0.0f, 1.0f,
	//y axis
	0.0f, -1.0f,  0.0f, 1.0f,
	0.0f,  1.0f,  0.0f, 1.0f,
	//z axis
	0.0f,  0.0f, -1.0f, 1.0f,
	0.0f,  0.0f,  1.0f, 1.0f
};

float axis_colors[] = { //r g b alpha
	//x axis
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	//y axis
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	//z axis
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f
};

float torus_colors[] = {

	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,

	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,

	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};

float* torus(float n, float r, float R) {

	std::vector<float> t; //t used for temporarily storing vectors
	int count = 0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {

			t.push_back((R + (r * cos(p(j, n)))) * cos(p(i, n)));
			t.push_back((R + (r * cos(p(j, n)))) * sin(p(i, n)));
			t.push_back(r * sin(p(j, n)));
			t.push_back(0);

			t.push_back((R + (r * cos(p(j, n)))) * cos(p(i + 1, n)));
			t.push_back((R + (r * cos(p(j, n)))) * sin(p(i + 1, n)));
			t.push_back(r * sin(p(j, n)));
			t.push_back(0);

			t.push_back((R + (r * cos(p(j + 1, n)))) * cos(p(i, n)));
			t.push_back((R + (r * cos(p(j + 1, n)))) * sin(p(i, n)));
			t.push_back(r * sin(p(j + 1, n)));
			t.push_back(0);

			t.push_back((R + (r * cos(p(j + 1, n)))) * cos(p(i + 1, n)));
			t.push_back((R + (r * cos(p(j + 1, n)))) * sin(p(i + 1, n)));
			t.push_back(r * sin(p(j + 1, n)));
			t.push_back(0);

			t.push_back((R + (r * cos(p(j + 1, n)))) * cos(p(i, n)));
			t.push_back((R + (r * cos(p(j + 1, n)))) * sin(p(i, n)));
			t.push_back(r * sin(p(j + 1, n)));
			t.push_back(0);

			t.push_back((R + (r * cos(p(j, n)))) * cos(p(i + 1, n)));
			t.push_back((R + (r * cos(p(j, n)))) * sin(p(i + 1, n)));
			t.push_back(r * sin(p(j, n)));
			t.push_back(0);

			count += 24;
		}
	}
	for (int i = 0; i < t.size() / 4; i++) {
		colors.push_back(glm::vec4(1.0, 1.0, 1.0, 1.0));
	}

	float* torus = new float[count]; //Stores vectors from temp vecs into one vector utilizing the count as the size
	for (int i = 0; i < count; i++) {
		torus[i] = t[i];
	}
	return torus;
}

float* smooth(float n) {

	//------
	std::vector<float> t; //t used for temporarily storing vectors
	int count = 0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			//va
			t.push_back(((cos(p(j, n)))) * cos(p(i, n)));
			t.push_back(((cos(p(j, n)))) * sin(p(i, n)));
			t.push_back(sin(p(j, n)));
			t.push_back(0);

			//v1
			t.push_back(((cos(p(j, n)))) * cos(p(i + 1, n)));
			t.push_back(((cos(p(j, n)))) * sin(p(i + 1, n)));
			t.push_back(sin(p(j, n)));
			t.push_back(0);

			//v2
			t.push_back(((cos(p(j + 1, n)))) * cos(p(i, n)));
			t.push_back(((cos(p(j + 1, n)))) * sin(p(i, n)));
			t.push_back(sin(p(j + 1, n)));
			t.push_back(0);


			//vb
			t.push_back(((cos(p(j + 1, n)))) * cos(p(i + 1, n)));
			t.push_back(((cos(p(j + 1, n)))) * sin(p(i + 1, n)));
			t.push_back(sin(p(j + 1, n)));
			t.push_back(0);

			//v2
			t.push_back(((cos(p(j + 1, n)))) * cos(p(i, n)));
			t.push_back(((cos(p(j + 1, n)))) * sin(p(i, n)));
			t.push_back(sin(p(j + 1, n)));
			t.push_back(0);

			//v1
			t.push_back(((cos(p(j, n)))) * cos(p(i + 1, n)));
			t.push_back(((cos(p(j, n)))) * sin(p(i + 1, n)));
			t.push_back(sin(p(j, n)));
			t.push_back(0);


			count += 24;
		}
	}

	float* torus = new float[count]; //Stores vectors from temp vecs into one vector utilizing the count as the size
	for (int i = 0; i < count; i++) {
		torus[i] = t[i];
	}
	return torus;
}

float* torus_texture(float n, float r, float R)
{

	std::vector<float> vec;
	int count = 0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {

			vec.push_back(p_t(i, n));
			vec.push_back(p_t(j, n));

			vec.push_back(p_t(i + 1, n));
			vec.push_back(p_t(j, n));

			vec.push_back(p_t(i, n));
			vec.push_back(p_t(j + 1, n));

			vec.push_back(p_t(i + 1, n));
			vec.push_back(p_t(j + 1, n));

			vec.push_back(p_t(i, n));
			vec.push_back(p_t(j + 1, n));

			vec.push_back(p_t(i + 1, n));
			vec.push_back(p_t(j, n));

			count += 12;
		}
	}

	float* torus = new float[count];
	for (int i = 0; i < count; i++) {
		torus[i] = vec[i];
	}
	return torus;
}


/*=================================================================================================
	HELPER FUNCTIONS
=================================================================================================*/

void window_to_scene(int wx, int wy, float& sx, float& sy)
{
	sx = (2.0f * (float)wx / WindowWidth) - 1.0f;
	sy = 1.0f - (2.0f * (float)wy / WindowHeight);
}

/*=================================================================================================
	SHADERS
=================================================================================================*/

void CreateTransformationMatrices(void)
{
	// PROJECTION MATRIX
	PerspProjectionMatrix = glm::perspective<float>(glm::radians(60.0f), (float)WindowWidth / (float)WindowHeight, 0.01f, 1000.0f);

	// VIEW MATRIX
	glm::vec3 eye(0.0, 0.0, 2.0);
	glm::vec3 center(0.0, 0.0, 0.0);
	glm::vec3 up(0.0, 1.0, 0.0);

	PerspViewMatrix = glm::lookAt(eye, center, up);

	// MODEL MATRIX
	PerspModelMatrix = glm::mat4(1.0);
	PerspModelMatrix = glm::rotate(PerspModelMatrix, glm::radians(perspRotationX), glm::vec3(1.0, 0.0, 0.0));
	PerspModelMatrix = glm::rotate(PerspModelMatrix, glm::radians(perspRotationY), glm::vec3(0.0, 1.0, 0.0));
	PerspModelMatrix = glm::scale(PerspModelMatrix, glm::vec3(perspZoom));
}

void CreateShaders(void)
{
	// Renders without any transformations
	PassthroughShader.Create("./shaders/simple.vert", "./shaders/simple.frag");

	// Renders using perspective projection
	PerspectiveShader.Create("./shaders/persp.vert", "./shaders/persp.frag");

	//texpersp.frag/vert
	//PerspectiveTextureShader.Create("./shaders/texpersp.vert", "./shaders/texpersp.frag");

	//texpersplight.frag/vert
	PerspectiveTextureLightShader.Create("./shaders/texpersplight.vert", "./shaders/texpersplight.frag");

}

/*=================================================================================================
	BUFFERS
=================================================================================================*/

void CreateAxisBuffers(void)
{
	glGenVertexArrays(1, &axis_VAO);
	glBindVertexArray(axis_VAO);

	glGenBuffers(2, &axis_VBO[0]);


	//vertex data 
	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axis_vertices), axis_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color data
	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axis_colors), axis_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void CreateTextureBuffers(void) {

	int size = texture.width() * texture.height();
	unsigned char* rgb = new unsigned char[3 * size];
	for (int i = 0; i < size; i++) {
		rgb[3 * i + 0] = texture.data()[0 * size + i];
		rgb[3 * i + 1] = texture.data()[1 * size + i];
		rgb[3 * i + 2] = texture.data()[2 * size + i];
	}

	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width(), texture.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, rgb);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glGenVertexArrays(1, &torus_VAO);
	glBindVertexArray(torus_VAO);
	glGenBuffers(4, &torus_VBO[0]);

	//Vertices
	float* vertices = torus(n, r, R); //Torus function

	glBindBuffer(GL_ARRAY_BUFFER, torus_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, n * n * 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Colors
	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), &colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	//Normals
	float* normals = smooth(n);

	glBindBuffer(GL_ARRAY_BUFFER, torus_VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, n * n * 24 * sizeof(float), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);


	//Texture
	float* center_texture = torus_texture(n, r, R); //For textures to be implemented

	glBindBuffer(GL_ARRAY_BUFFER, torus_VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, n * n * 12 * sizeof(float), center_texture, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(3);
}

/*=================================================================================================
	CALLBACKS
=================================================================================================*/

//-----------------------------------------------------------------------------
// CALLBACK DOCUMENTATION
// https://www.opengl.org/resources/libraries/glut/spec3/node45.html
// http://freeglut.sourceforge.net/docs/api.php#WindowCallback
//-----------------------------------------------------------------------------

void idle_func()
{
	//uncomment below to repeatedly draw new frames
	glutPostRedisplay();
}

void reshape_func(int width, int height)
{
	WindowWidth = width;
	WindowHeight = height;

	glViewport(0, 0, width, height);
	glutPostRedisplay();
}

void keyboard_func(unsigned char key, int x, int y)
{
	key_states[key] = true;

	switch (key)
	{
	case 'q':
	{
		n = n + 1.0f;
		CreateTextureBuffers();
		break;
	}
	case 'a':
	{
		n = n - 1.0f;
		CreateTextureBuffers();
		break;
	}
	case 'w':
	{
		r = r + 0.1f;
		CreateTextureBuffers();
		break;
	}
	case 's':
	{
		r = r - 0.1f;
		CreateTextureBuffers();
		break;
	}
	case 'e':
	{
		R = R + 0.1f;
		CreateTextureBuffers();
		break;
	}
	case 'd':
	{
		R = R - 0.1f;
		CreateTextureBuffers();
		break;
	}

	case 'f':
	{
		draw_wireframe = !draw_wireframe;
		if (draw_wireframe == true)
			std::cout << "Wireframes on.\n";
		else
			std::cout << "Wireframes off.\n";
		break;
	}

	case ' ':
	{
		if (textureId == 0) {
			texture.load("Big_Blue.bmp");
			CreateTextureBuffers();
			textureId++;
			break;

		}

		if (textureId == 1) {

			texture.load("milla.bmp");
			CreateTextureBuffers();
			textureId++;
			break;
		}

		if (textureId == 2) {

			texture.load("logo.bmp");
			CreateTextureBuffers();
			textureId = 0;
			break;
		}
		break;


	}

	// Exit on escape key press
	case '\x1B':
	{
		exit(EXIT_SUCCESS);
		break;
	}
	}
}

void key_released(unsigned char key, int x, int y)
{
	key_states[key] = false;
}

void key_special_pressed(int key, int x, int y)
{
	key_special_states[key] = true;
}

void key_special_released(int key, int x, int y)
{
	key_special_states[key] = false;
}

void mouse_func(int button, int state, int x, int y)
{
	// Key 0: left button
	// Key 1: middle button
	// Key 2: right button
	// Key 3: scroll up
	// Key 4: scroll down

	if (x < 0 || x > WindowWidth || y < 0 || y > WindowHeight)
		return;

	float px, py;
	window_to_scene(x, y, px, py);

	if (button == 3)
	{
		perspZoom += 0.03f;
	}
	else if (button == 4)
	{
		if (perspZoom - 0.03f > 0.0f)
			perspZoom -= 0.03f;
	}

	mouse_states[button] = (state == GLUT_DOWN);

	LastMousePosX = x;
	LastMousePosY = y;
}

void passive_motion_func(int x, int y)
{
	if (x < 0 || x > WindowWidth || y < 0 || y > WindowHeight)
		return;

	float px, py;
	window_to_scene(x, y, px, py);

	LastMousePosX = x;
	LastMousePosY = y;
}

void active_motion_func(int x, int y)
{
	if (x < 0 || x > WindowWidth || y < 0 || y > WindowHeight)
		return;

	float px, py;
	window_to_scene(x, y, px, py);

	if (mouse_states[0] == true)
	{
		perspRotationY += (x - LastMousePosX) * perspSensitivity;
		perspRotationX += (y - LastMousePosY) * perspSensitivity;
	}

	LastMousePosX = x;
	LastMousePosY = y;
}

/*=================================================================================================
	RENDERING
=================================================================================================*/

void display_func(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CreateTransformationMatrices();
	/*
	PerspectiveShader.Use();
	PerspectiveShader.SetUniform("projectionMatrix", glm::value_ptr(PerspProjectionMatrix), 4, GL_FALSE, 1);
	PerspectiveShader.SetUniform("viewMatrix", glm::value_ptr(PerspViewMatrix), 4, GL_FALSE, 1);
	PerspectiveShader.SetUniform("modelMatrix", glm::value_ptr(PerspModelMatrix), 4, GL_FALSE, 1);

	if (draw_wireframe == true)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(axis_VAO);
	glDrawArrays(GL_LINES, 0, 6);

	glBindVertexArray(torus_VAO);
	glDrawArrays(GL_TRIANGLES, 0, n * n * 6);

	glBindVertexArray(0);
	*/
	//For torus texture
	//----------------------------------------------------------- 
	PerspectiveTextureLightShader.Use();
	PerspectiveTextureLightShader.SetUniform("projectionMatrix", glm::value_ptr(PerspProjectionMatrix), 4, GL_FALSE, 1);
	PerspectiveTextureLightShader.SetUniform("viewMatrix", glm::value_ptr(PerspViewMatrix), 4, GL_FALSE, 1);
	PerspectiveTextureLightShader.SetUniform("modelMatrix", glm::value_ptr(PerspModelMatrix), 4, GL_FALSE, 1);

	PerspectiveTextureLightShader.SetUniform("texId", textureId);


	glBindVertexArray(torus_VAO);
	glDrawArrays(GL_TRIANGLES, 0, n * n * 6);
	glBindVertexArray(0);
	//------------------------------------------------------------


	if (draw_wireframe == true)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glutSwapBuffers();
}

/*=================================================================================================
	INIT
=================================================================================================*/

void init(void) //=-------------------------------------------------------------------------------------------------------------------------------------------
{
	// Print some info
	std::cout << "Vendor:         " << glGetString(GL_VENDOR) << "\n";
	std::cout << "Renderer:       " << glGetString(GL_RENDERER) << "\n";
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "GLSL Version:   " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n\n";

	std::cout << "Note: System will crash once q or a is pressed 8 times \n" << std::endl;

	// Set OpenGL settings
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // background color
	glEnable(GL_DEPTH_TEST); // enable depth test
	glEnable(GL_CULL_FACE); // enable back-face culling

	// Create shaders
	CreateShaders();

	// Create axis
	CreateAxisBuffers();

	// Create Torus with image
	CreateTextureBuffers();

	std::cout << "Finished initializing...\n\n";
}

/*=================================================================================================
	MAIN
=================================================================================================*/

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(InitWindowWidth, InitWindowHeight);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	texture.load("Big_Blue.bmp");

	//texture.display();

	glutCreateWindow("CSE-170 Computer Graphics");

	// Initialize GLEW
	GLenum ret = glewInit();
	if (ret != GLEW_OK) {
		std::cerr << "GLEW initialization error." << std::endl;
		glewGetErrorString(ret);
		return -1;
	}

	glutDisplayFunc(display_func);
	glutIdleFunc(idle_func);
	glutReshapeFunc(reshape_func);
	glutKeyboardFunc(keyboard_func);
	glutKeyboardUpFunc(key_released);
	glutSpecialFunc(key_special_pressed);
	glutSpecialUpFunc(key_special_released);
	glutMouseFunc(mouse_func);
	glutMotionFunc(active_motion_func);
	glutPassiveMotionFunc(passive_motion_func);

	init();

	glutMainLoop();

	return EXIT_SUCCESS;
}
