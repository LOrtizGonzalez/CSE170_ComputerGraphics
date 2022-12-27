#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <iostream>
#include <ctime>

//=================================================================================================
// CALLBACKS
//=================================================================================================

//-----------------------------------------------------------------------------
// CALLBACK DOCUMENTATION
// https://www.opengl.org/resources/libraries/glut/spec3/node45.html
// http://freeglut.sourceforge.net/docs/api.php#WindowCallback
//-----------------------------------------------------------------------------

float xr = 0, yr = 0;
float col1 = 0, col2 = 0, col3 = 0;

void resetColors() {
	col1 = 0;
	col2 = 0;
	col3 = 0;
}

void helpMenu(int x)
{
	std::cout << "Menu" << std::endl;
	std::cout << x << std::endl;
}

void displayKeys()
{
	std::cout << "+===============================================+" << std::endl;
	std::cout << "	UP ARROW : Green Text\n";
	std::cout << "	LEFT ARROW : Red Text\n";
	std::cout << "	RIGHT ARROW : Blue Text\n";
	std::cout << "	DOWN ARROW : Orignal Tex Color\n\n";
	std::cout << "	W : Up\n";
	std::cout << "	A : Left\n";
	std::cout << "	S : Down\n";
	std::cout << "	D : Right\n\n";
	std::cout << "	Right-Mouse-Click : Rotate Right\n";
	std::cout << "	Left-Mouse-Click : Rotate Left\n";
	std::cout << "+===============================================+" << std::endl;
}

void idle_func()
{
	//uncomment below to repeatedly draw new frames
	//glutPostRedisplay();
}

void reshape_func( int width, int height )
{
	glViewport( 0, 0, width, height );
	glutPostRedisplay();
}

void keyboard_func( unsigned char key, int x, int y )
{
	switch (key)
	{
		case 'w':
		{
			if (yr < 1.0)
				yr = yr + .1;
			break;
		}

		case 'a':
		{
			if (xr > -0.8)
				xr = xr - .1;
			break;
		}

		case 's':
		{
			if (yr > -1.0)
				yr = yr - .1;
			break;
		}

		case 'd':
		{
			if (xr < 0.8)
				xr = xr + .1;
			break;
		}

		case 'h':
		{
			displayKeys();
			break;
		}

		// Exit on escape key press
		case '\x1B':
		{
			exit(EXIT_SUCCESS);
			break;
		}
	}


	glutPostRedisplay();
}

void key_released( unsigned char key, int x, int y )
{
}

void key_special_pressed( int key, int x, int y )
{
	switch (key)
	{
		case GLUT_KEY_UP: {
			resetColors();
			col2 = col2 - 1;
			col3 = col3 - 1;
			break; 
		}
		case GLUT_KEY_LEFT: {
			resetColors();
			col1 = col1 - 1;
			col3 = col3 - 1;
			break; 
		}
		case GLUT_KEY_RIGHT: {
			resetColors();
			col1 = col1 - 1;
			col2 = col2 - 1;
			break; 
		}
		case GLUT_KEY_DOWN: {
			resetColors();
			break; 
		}
	}
	glutPostRedisplay();
}



void key_special_released( int key, int x, int y )
{
}

void mouse_func( int button, int state, int x, int y )
{
	//std::cout << x << " " << y << std::endl;
	switch(button) {
		case GLUT_LEFT_BUTTON: {
			glRotatef(30.0, 0.0, 0.0, 1.0);
			state = GLUT_DOWN;
			break;
		}
		case GLUT_RIGHT_BUTTON: {
			glRotatef(-30.0, 0.0, 0.0, 1.0);
			break;
		}
	}

}

void passive_motion_func( int x, int y )
{
}

void active_motion_func( int x, int y )
{
}

//=================================================================================================
// RENDERING
//=================================================================================================

void display_func( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//---------- LUCA ----------
	//L =====================
	glColor3f(col1 + 1.0f, col2 + 1.0f, col3 + 1.0f); //LEFT
	glBegin(GL_TRIANGLES);
		glVertex2f( xr + (-0.67f), yr + (-0.1f)); //left
		glVertex2f( xr + (-0.61f), yr + (-0.05f)); //right
		glVertex2f(xr + (-0.62f), yr + 0.4f); //middle
	glEnd();

	glColor3f(col1 + 1.0f, col2 + 1.0f, col3 + 1.0f); //BOTTOM
	glBegin(GL_TRIANGLES);
		glVertex2f(xr + (-0.65f), yr + (-0.1f)); //left
		glVertex2f(xr + (-0.45f), yr + (-0.1f)); //right
		glVertex2f(xr + (-0.59f), yr + (-0.05f)); //middle
	glEnd();
	//U =====================
	glColor3f(col1 + 1.0f, col2 + 1.0f, col3 + 1.0f); //LEFT
	glBegin(GL_TRIANGLES);
		glVertex2f(xr + (-0.43f), yr + (-0.1f)); //left 
		glVertex2f(xr + (-0.33f),yr + (-0.1f)); //right
		glVertex2f(xr + (-0.41f), yr + (0.4f)); //middle
	glEnd();
	glColor3f(col1 + 1.0f, col2 + 1.0f, col3 + 1.0f); //RIGHT
	glBegin(GL_TRIANGLES);
		glVertex2f(xr + (-0.31f),yr + (-0.1f)); //left
		glVertex2f(xr + (-0.21f), yr + (-0.1f)); //right
		glVertex2f(xr + (-0.23f), yr + 0.4f); //middle
	glEnd();
	//C =====================
	glColor3f(col1 + 1.0f, col2 + 1.0f, col3 + 1.0f);
	glBegin(GL_TRIANGLES); //BOTTOM-LEFT
		glVertex2f(xr + (-0.19f), yr + 0.06f); //left
		glVertex2f(xr + (-0.05f), yr + (-0.1f)); //right
		glVertex2f(xr + (-0.16f), yr + 0.13f); //middle
	glEnd();
	glColor3f(col1 + 1.0f, col2 + 1.0f, col3 +  1.0f); //BOTTOM-RIGHT
	glBegin(GL_TRIANGLES);
		glVertex2f(xr + (-0.06f), yr + (-0.05f)); //left
		glVertex2f(xr + 0.02f, yr + 0.0f); //right
		glVertex2f(xr + (-0.035f), yr + (-0.1f)); //middle
	glEnd();
	glColor3f(col1 + 1.0f, col2 + 1.0f, col3 + 1.0f); //TOP-LEFT
	glBegin(GL_TRIANGLES);
		glVertex2f(xr + (-0.2f), yr + 0.1f); //left
		glVertex2f(xr + (-0.065f),yr + 0.3f); //right
		glVertex2f(xr + (-0.08f), yr + 0.4f); //middle
	glEnd();
	glColor3f(col1 + 1.0f, col2 + 1.0f, col3 + 1.0f); //TOP-RIGHT
	glBegin(GL_TRIANGLES);
		glVertex2f(xr + (-0.07f), yr + 0.4f); //left
		glVertex2f(xr + 0.02f, yr + 0.3f); //right
		glVertex2f(xr + (-0.02f), yr + 0.28f); //middle
	glEnd();
	//A =====================
	glColor3f(col1 + 1.0f, col2 + 1.0f, col3 + 1.0f); //LEFT
	glBegin(GL_TRIANGLES);
		glVertex2f(xr + 0.05f, yr + (-0.1f)); //left
		glVertex2f(xr + 0.2f, yr + 0.4f); //right
		glVertex2f(xr + 0.2f, yr + 0.25f); //middle
	glEnd();
	glColor3f(col1 + 1.0f, col2 + 1.0f, col3 + 1.0f); //RIGHT
	glBegin(GL_TRIANGLES);
		glVertex2f(xr + 0.21f, yr + 0.4f); //left
		glVertex2f(xr + 0.31f, yr + (-0.02f)); //right
		glVertex2f(xr + 0.27f, yr + (-0.1f)); //middle
	glEnd();
	glColor3f(col1 + 1.0f, col2 + 1.0f, col3 + 1.0f); //MIDDLE
	glBegin(GL_TRIANGLES);
		glVertex2f(xr + 0.14f, yr + 0.1f); //left
		glVertex2f(xr + 0.23f,yr + .13f); //right
		glVertex2f(xr + 0.18f, yr + 0.05f); //middle
	glEnd();

	glutSwapBuffers();
}

//=================================================================================================
// INIT
//=================================================================================================

void init( void )
{

	// Print some info
	std::cout << "Vendor:         " << glGetString( GL_VENDOR   ) << "\n";
	std::cout << "Renderer:       " << glGetString( GL_RENDERER ) << "\n";
	std::cout << "OpenGL Version: " << glGetString( GL_VERSION  ) << "\n\n";


	// Set the background color
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	std::cout << "Finished initializing...\n\n";
}

//=================================================================================================
// MAIN
//=================================================================================================

int main( int argc, char** argv )
{
	glutInit( &argc, argv );

	glutInitWindowPosition( 100, 100 );
	glutInitWindowSize( 800, 600 );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

	glutCreateWindow( "CSE-170 Computer Graphics" );

	glutDisplayFunc( display_func ); //display call back function
	glutIdleFunc( idle_func );
	glutReshapeFunc( reshape_func );
	glutKeyboardFunc( keyboard_func ); //keyboard call back function
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
