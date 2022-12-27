#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include "shader.h"
#include "shaderprogram.h"

#include <vector>
#define _USE_MATH_DEFINES
#include <math.h> //Allows for M_PI use

/*=================================================================================================
	DOMAIN
=================================================================================================*/

// Window dimensions
const int InitWindowWidth  = 800;
const int InitWindowHeight = 800;
int WindowWidth  = InitWindowWidth;
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

/*=================================================================================================
	SHADERS & TRANSFORMATIONS
=================================================================================================*/

ShaderProgram PassthroughShader;
ShaderProgram PerspectiveShader;

glm::mat4 PerspProjectionMatrix( 1.0f );
glm::mat4 PerspViewMatrix( 1.0f );
glm::mat4 PerspModelMatrix( 1.0f );

float perspZoom = 1.0f, perspSensitivity = 0.35f;
float perspRotationX = 0.0f, perspRotationY = 0.0f;

/*=================================================================================================
	OBJECTS
=================================================================================================*/

GLuint axis_VAO;
GLuint axis_VBO[2];
float R = 1.0f, r = 0.5; //Use to generate x, y , and z points from TORUS func
int n = 10; //Use to generate x, y , and z points from TORUS func
GLuint torus_VAO; // for TORUS object
GLuint torus_VBO[2]; // for TORUS object

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

float axis_colors[] = {
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

//--------xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx-------- +

float prmtr(int a, int y)
{
	return a * ((2 * M_PI) / y);
}

float* torus_Vertex(float outR,float inr, int n)
{
	std::vector<float> temp;
	int count = 0;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			//First Triangle coordinates: x1, y1, z1, 1; x2, y2, z2, 1; x3, y3, z3, 1;
			temp.push_back((outR + inr * cos(prmtr(j, n))) * cos(prmtr(i, n)));
			temp.push_back((outR + inr * cos(prmtr(j, n))) * sin(prmtr(i, n)));
			temp.push_back(inr * sin(prmtr(j, n)));
			temp.push_back(1);

			temp.push_back((outR + inr * cos(prmtr(j, n))) * cos(prmtr(i + 1, n)));
			temp.push_back((outR + inr * cos(prmtr(j, n))) * sin(prmtr(i + 1, n)));
			temp.push_back(inr * sin(prmtr(j, n)));
			temp.push_back(1);

			temp.push_back((outR + inr * cos(prmtr(j + 1, n))) * cos(prmtr(i, n)));
			temp.push_back((outR + inr * cos(prmtr(j + 1, n))) * sin(prmtr(i, n)));
			temp.push_back(inr * sin(prmtr(j + 1, n)));
			temp.push_back(1);
			//Second Triangle
			temp.push_back((outR + inr * cos(prmtr(j + 1, n))) * cos(prmtr(i + 1, n)));
			temp.push_back((outR + inr * cos(prmtr(j + 1, n))) * sin(prmtr(i + 1, n)));
			temp.push_back(inr * sin(prmtr(j + 1, n)));
			temp.push_back(1);

			temp.push_back((outR + inr * cos(prmtr(j + 1, n))) * cos(prmtr(i, n)));
			temp.push_back((outR + inr * cos(prmtr(j + 1, n))) * sin(prmtr(i, n)));
			temp.push_back(inr * sin(prmtr(j + 1, n)));
			temp.push_back(1);

			temp.push_back((outR + inr * cos(prmtr(j, n))) * cos(prmtr(i + 1, n)));
			temp.push_back((outR + inr * cos(prmtr(j, n))) * sin(prmtr(i + 1, n)));
			temp.push_back(inr * sin(prmtr(j, n)));
			temp.push_back(1);

			count += 24;
		}
	}
	float* sample = new float[count];
	for (int i = 0; i < count; i++) {
		sample[i] = temp[i];
	}
	return sample;
}
//---------xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx--------- +

/*=================================================================================================
	HELPER FUNCTIONS
=================================================================================================*/

void window_to_scene( int wx, int wy, float& sx, float& sy )
{
	sx = ( 2.0f * (float)wx / WindowWidth ) - 1.0f;
	sy = 1.0f - ( 2.0f * (float)wy / WindowHeight );
}

/*=================================================================================================
	SHADERS
=================================================================================================*/

void CreateTransformationMatrices( void )
{
	// PROJECTION MATRIX
	PerspProjectionMatrix = glm::perspective<float>( glm::radians( 60.0f ), (float)WindowWidth / (float)WindowHeight, 0.01f, 1000.0f );

	// VIEW MATRIX
	glm::vec3 eye   ( 0.0, 0.0, 2.0 );
	glm::vec3 center( 0.0, 0.0, 0.0 );
	glm::vec3 up    ( 0.0, 1.0, 0.0 );

	PerspViewMatrix = glm::lookAt( eye, center, up );

	// MODEL MATRIX
	PerspModelMatrix = glm::mat4( 1.0 );
	PerspModelMatrix = glm::rotate( PerspModelMatrix, glm::radians( perspRotationX ), glm::vec3( 1.0, 0.0, 0.0 ) );
	PerspModelMatrix = glm::rotate( PerspModelMatrix, glm::radians( perspRotationY ), glm::vec3( 0.0, 1.0, 0.0 ) );
	PerspModelMatrix = glm::scale( PerspModelMatrix, glm::vec3( perspZoom ) );
}

void CreateShaders( void )
{
	// Renders without any transformations
	PassthroughShader.Create( "./shaders/simple.vert", "./shaders/simple.frag" );

	// Renders using perspective projection
	PerspectiveShader.Create( "./shaders/persp.vert", "./shaders/persp.frag" );
}

/*=================================================================================================
	BUFFERS
=================================================================================================*/

void CreateAxisBuffers( void )
{
	glGenVertexArrays( 1, &axis_VAO );
	glBindVertexArray( axis_VAO );

	glGenBuffers( 2, &axis_VBO[0] );

	glBindBuffer( GL_ARRAY_BUFFER, axis_VBO[0] );
	glBufferData( GL_ARRAY_BUFFER, sizeof( axis_vertices ), axis_vertices, GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof( float ), (void*)0 );
	glEnableVertexAttribArray( 0 );

	glBindBuffer( GL_ARRAY_BUFFER, axis_VBO[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof( axis_colors ), axis_colors, GL_STATIC_DRAW );
	glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof( float ), (void*)0 );
	glEnableVertexAttribArray( 1 );

	glBindVertexArray( 0 );
}

void CreateTorusBuffers(void)
{
	glGenVertexArrays(1, &torus_VAO);
	glBindVertexArray(torus_VAO);

	glGenBuffers(2, &torus_VBO[0]);

	float* torus = torus_Vertex(R, r, n);

	glBindBuffer(GL_ARRAY_BUFFER, torus_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, n * n * 24 * sizeof(float), torus, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, torus_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, n * n * 24 * sizeof(float), torus, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
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

void reshape_func( int width, int height )
{
	WindowWidth  = width;
	WindowHeight = height;

	glViewport( 0, 0, width, height );
	glutPostRedisplay();
}

void keyboard_func( unsigned char key, int x, int y )
{
	key_states[ key ] = true;

	switch( key )
	{
		case 'f':
		{
			draw_wireframe = !draw_wireframe;
			if( draw_wireframe == true )
				std::cout << "Wireframes on.\n";
			else
				std::cout << "Wireframes off.\n";
			break;
		}
		case 'q':
		{
			if (n < 75)
				n++;
			CreateTorusBuffers();
			glutPostRedisplay();
			break;
		}
		case 'a':
		{
			if (n > 3) {
				n--;
				CreateTorusBuffers();
				glutPostRedisplay();
			}
			break;
		}
		case'w':
		{
			if (r <= 1.5)
				r = r + 0.01;
			CreateTorusBuffers();
			glutPostRedisplay();
			break;
		}
		case's':
		{
			if (r > 0.1) 
				r = r - 0.01;
			CreateTorusBuffers();
			glutPostRedisplay();
			break;
		}
		case'e':
		{
			if (R <= 5)
				R = R + 0.01;
			glutPostRedisplay();
			CreateTorusBuffers();
			break;
		}
		case'd':
		{
			if (R > 0.5)
				R = R - 0.01;
			glutPostRedisplay();
			CreateTorusBuffers();
			break;
		}

		// Exit on escape key press
		case '\x1B':
		{
			exit( EXIT_SUCCESS );
			break;
		}
	}
	glutPostRedisplay();
}

void key_released( unsigned char key, int x, int y )
{
	key_states[ key ] = false;
}

void key_special_pressed( int key, int x, int y )
{
	key_special_states[ key ] = true;
}

void key_special_released( int key, int x, int y )
{
	key_special_states[ key ] = false;
}

void mouse_func( int button, int state, int x, int y )
{
	// Key 0: left button
	// Key 1: middle button
	// Key 2: right button
	// Key 3: scroll up
	// Key 4: scroll down

	if( x < 0 || x > WindowWidth || y < 0 || y > WindowHeight )
		return;

	float px, py;
	window_to_scene( x, y, px, py );

	if( button == 3 )
	{
		perspZoom += 0.03f;
	}
	else if( button == 4 )
	{
		if( perspZoom - 0.03f > 0.0f )
			perspZoom -= 0.03f;
	}

	mouse_states[ button ] = ( state == GLUT_DOWN );

	LastMousePosX = x;
	LastMousePosY = y;
}

void passive_motion_func( int x, int y )
{
	if( x < 0 || x > WindowWidth || y < 0 || y > WindowHeight )
		return;

	float px, py;
	window_to_scene( x, y, px, py );

	LastMousePosX = x;
	LastMousePosY = y;
}

void active_motion_func( int x, int y )
{
	if( x < 0 || x > WindowWidth || y < 0 || y > WindowHeight )
		return;

	float px, py;
	window_to_scene( x, y, px, py );

	if( mouse_states[0] == true )
	{
		perspRotationY += ( x - LastMousePosX ) * perspSensitivity;
		perspRotationX += ( y - LastMousePosY ) * perspSensitivity;
	}

	LastMousePosX = x;
	LastMousePosY = y;
}

/*=================================================================================================
	RENDERING
=================================================================================================*/

void display_func( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	CreateTransformationMatrices();

	PerspectiveShader.Use();
	PerspectiveShader.SetUniform( "projectionMatrix", glm::value_ptr( PerspProjectionMatrix ), 4, GL_FALSE, 1 );
	PerspectiveShader.SetUniform( "viewMatrix", glm::value_ptr( PerspViewMatrix ), 4, GL_FALSE, 1 );
	PerspectiveShader.SetUniform( "modelMatrix", glm::value_ptr( PerspModelMatrix ), 4, GL_FALSE, 1 );

	if( draw_wireframe == true )
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glBindVertexArray( axis_VAO );
	glDrawArrays( GL_LINES, 0, 6 );

	glBindVertexArray( 0 );
	//--------------------------------------------------------+
	glBindVertexArray( torus_VAO ); //Added for TORUS
	glDrawArrays(GL_TRIANGLES, 0, n * n * 6); //Added for TORUS
	glBindVertexArray(0);
	//--------------------------------------------------------+
	if( draw_wireframe == true )
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	glutSwapBuffers();
}

/*=================================================================================================
	INIT
=================================================================================================*/

void init( void )
{
	// Print some info
	std::cout << "Vendor:         " << glGetString( GL_VENDOR   ) << "\n";
	std::cout << "Renderer:       " << glGetString( GL_RENDERER ) << "\n";
	std::cout << "OpenGL Version: " << glGetString( GL_VERSION  ) << "\n";
	std::cout << "GLSL Version:   " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << "\n\n";

	// Set OpenGL settings
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); // background color
	glEnable( GL_DEPTH_TEST ); // enable depth test
	//glEnable( GL_CULL_FACE ); // enable back-face culling

	// Create shaders
	CreateShaders();

	// Create buffers
	CreateAxisBuffers();
	CreateTorusBuffers(); //From the torus buffers

	std::cout << "Finished initializing...\n\n";
}

/*=================================================================================================
	MAIN
=================================================================================================*/

int main( int argc, char** argv )
{
	glutInit( &argc, argv );

	glutInitWindowPosition( 100, 100 );
	glutInitWindowSize( InitWindowWidth, InitWindowHeight );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

	glutCreateWindow( "CSE-170 Computer Graphics" );

	// Initialize GLEW
	GLenum ret = glewInit();
	if( ret != GLEW_OK ) {
		std::cerr << "GLEW initialization error." << std::endl;
		glewGetErrorString( ret );
		return -1;
	}

	glutDisplayFunc( display_func );
	glutIdleFunc( idle_func );
	glutReshapeFunc( reshape_func );
	glutKeyboardFunc( keyboard_func );
	glutKeyboardUpFunc( key_released );
	glutSpecialFunc( key_special_pressed );
	glutSpecialUpFunc( key_special_released );
	glutMouseFunc( mouse_func );
	glutMotionFunc( active_motion_func );
	glutPassiveMotionFunc( passive_motion_func );

	init();

	glutMainLoop();

	return EXIT_SUCCESS;
}
