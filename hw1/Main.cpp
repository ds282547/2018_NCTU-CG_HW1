/*HW1-Double Pendulum Systems

<insert TA's words...>

*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

/**
 * this needs to be at the top, since some headers might already includes the
 * cmath header. So to prevent including cmath without _USE_MATH_DEFINES, put it on top of everything else.
 */
#define _USE_MATH_DEFINES

#include "gl/glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>


#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>


#include "Image.hpp"
#include "DoublePendulum.hpp"
#include "TriplePendulum.hpp"

//number of textures desired, you may want to change it to get bonus point
#define TEXTURE_NUM 5
//directories of image files
char* texture_name[TEXTURE_NUM] = {
	"./Resource/sun.bmp" ,
	"./Resource/ceiling.bmp",
	"./Resource/earth.bmp",
	"./Resource/mars.bmp",
	"./Resource/moon.bmp"
};
//texture id array
GLuint texture[TEXTURE_NUM];


GLuint mProgramID;

//passTime parameter for helping coordinate your animation, you may utilize it to help perform animation
#define deltaTime 10 // in ms (1e-3 second)
double passTime = 0.0;

//light sorce parameter, no need to modify
//actually, modification of these parameter is forbidden in this homework
float LightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };//Light position
float LightAmb[] = { 0.1f, 0.1f, 0.1f, 1.0f };//Ambient Light Values
float LightDif[] = { 1.0f, 1.0f, 1.0f, 1.0f };//Diffuse Light Values
float LightSpc[] = { 1.0f, 1.0f, 1.0f, 1.0f };//Specular Light Values

float CameraPosition[] = { 0.0f, 0.0f, -36.0f };
float CameraRotation[] = { 0.0f, 0.0f , 0.0f };

// The double pendulum systems
DoublePendulum* doublePendulumSystems[3];
TriplePendulum*  triplePendulumSystem;

void drawPlanes(Vector3f pt0, Vector3f pt2,const unsigned c0, const unsigned c1, const unsigned c2) {
	// 2--3
	// | /|
	// |/ |
	// 1--0
	
	Vector3f pt1, pt3;
	pt1[c0] = pt2[c0]; pt1[c1] = pt0[c1]; pt1[c2] = pt0[c2];
	pt3[c0] = pt0[c0]; pt3[c1] = pt2[c1]; pt3[c2] = pt0[c2];

	glBegin(GL_QUADS); // 0123

	glTexCoord2d(0, 0);
	glVertex3fv(pt0());

	glTexCoord2d(0, 1);
	glVertex3fv(pt3());

	glTexCoord2d(1, 1);
	glVertex3fv(pt2());
	
	glTexCoord2d(1, 0);
	glVertex3fv(pt1());
	glEnd();
	
}
void RenderPlanes()
{
	// Draw 5 plane or boxes, the five planes shall display like a cube excluded the front face.

	
	// write your code here ...

	
	// Example: Draw One Red Plane by Draw Two Triangles
	
	// set Color to RGB (1, 0, 0) which is red


	//   0 -- 1
	//  /|    |\
	// 4 3 -- 2 5
	// |/      \|
	// 7--------6
	static const float u = 0.57735026919;
	static const float v = 3;
	static Vector3f boxNormals[8] = { {-u,-u,-u},{u,-u,-u},{u,u, -u},{-u,u,-u},{-u,-u,-v},{u,-u,-v},{u,u,-v},{-u,u,-v} };
	

	drawPlanes({ -10, -10, 10 }, { 10, 10, 10 }, 0, 1, 2);		// Back
	drawPlanes({ -10, 10, 10 }, { 10, 10, -10 }, 0, 2, 1);	// Top
	drawPlanes({ 10, -10, 10 }, { 10, 10, -10 }, 2, 1, 0);	// Left
	drawPlanes({ -10, -10, -10 }, { 10, -10, 10 }, 0, 2, 1);		// Bottom
	drawPlanes({ -10, -10, -10 }, { -10, 10, 10 }, 2, 1, 0);		// Right
}

//callback function for drawing a frame
void Display(void)
{
	// Alway Clear Color and Depth Bit Before Drawing!
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	// Load Identity Matrix
	glLoadIdentity();

	// Setup Camera Configurations
	
	glRotatef(CameraRotation[0], 1.0f, 0.0f, 0.0f);
	glRotatef(CameraRotation[1], 0.0f, 1.0f, 0.0f);
	glRotatef(CameraRotation[2], 0.0f, 0.0f, 1.0f);

	glTranslatef(CameraPosition[0], CameraPosition[1], CameraPosition[2]);
	glRotatef(180, 0.0f, 1.0f, 0.0f);

	LightPos[0] = doublePendulumSystems[1]->GetBottomBobPosition()[0];
	LightPos[1] = doublePendulumSystems[1]->GetBottomBobPosition()[1];
	LightPos[2] = doublePendulumSystems[1]->GetBottomBobPosition()[2];
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);

	glColor3f(1.0, 1.0, 1.0);

	glBindTexture(GL_TEXTURE_2D, texture[1]);

	RenderPlanes();


	glDisable(GL_TEXTURE_2D);
	// Render the double pendulum systems
	for (int i = 0; i < 3; i++)
	{
		doublePendulumSystems[i]->render();	
	}
	triplePendulumSystem->render();
	glEnable(GL_TEXTURE_2D);
	// Swap the drawn buffer to the window
	glutSwapBuffers();
}

//callback funtion as a passTimer, no need to modify it
void Tick(int id)
{
	double d = deltaTime / 1000.0;
	passTime += d;

	// update your physics here
	for (int i = 0; i < 3; i++)
	{
		// here, we can multiply d with a constant to speed up simulation
		if(doublePendulumSystems[i])
			doublePendulumSystems[i]->updatePhysics(d * 3);
	}
	triplePendulumSystem->updatePhysics(d * 3);
	glutPostRedisplay();
	glutTimerFunc(deltaTime, Tick, 0); // 100ms for passTime step size
}

//callback function when the window size is changed, no need to modify it
void WindowSize(int w, int h)
{
	glViewport(0, 0, w, h);							//changing the buffer size the same to the window size
	glMatrixMode(GL_PROJECTION);					//choose the projection matrix
	glLoadIdentity();
	gluPerspective(60.0, (double)w /(double)h, 1.0, 1000.0);//set the projection matrix as perspective mode
	glMatrixMode(GL_MODELVIEW);						//it is suggested that modelview matrix is chosen if nothing specific being performed
	glLoadIdentity();
}




//initialization for parameters of this program, you must perform something here
void init()
{
	glClearColor(1.0, 0.0, 0.0, 1.0);//set what color should be used when we clean the color buffer

	glEnable(GL_LIGHT1);//Enable Light1
	glEnable(GL_LIGHTING);//Enable Lighting
	//***********
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);//Set Light1 Position, this setting function should be at another place
	//***********
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmb);//Set Light1 Ambience
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDif);//Set Light1 Diffuse
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpc);//Set Light1 Specular
	//since a light source is also an object in the 3D scene, we set its position in the display function after gluLookAt()
	//you should know that straight texture mapping(without modifying shader) may not have shading effect
	//you need to tweak certain parameter(when building texture) to obtain a lit and textured object

	glShadeModel(GL_SMOOTH);//shading model

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);//you can choose which part of lighting model should be modified by texture mapping
	glEnable(GL_COLOR_MATERIAL);//enable this parameter to use glColor() as material of lighting model

	//please load all the textures here
	glGenTextures(TEXTURE_NUM, texture);

	for (unsigned i = 0; i < TEXTURE_NUM; ++i ) {
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		Image *img = loadTexture(texture_name[i]);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, img->sizeX, img->sizeY, GL_RGB,
			GL_UNSIGNED_BYTE, img->data);
	}
	glEnable(GL_TEXTURE_2D);
	


	// write your code here ...

	doublePendulumSystems[0] = new DoublePendulum({ 5,10,3 }, 3, 4, 1, 3);
	doublePendulumSystems[1] = new DoublePendulum({ 0,10, -2 }, 4, 4, 0.2, 0.7, true, texture[0]);
	doublePendulumSystems[2] = new DoublePendulum({ -5,10,-7 }, 5, 3, 0.3, 0.5);
	triplePendulumSystem = new TriplePendulum({ 0,10,8 }, 5, 6,7, 1, 2, 5, texture);

	glLineWidth(3);
	// write your code here ...
}

void DealKeyboardInput(unsigned char key, int x, int y)
{	
	// Deal with Inputs from Keyboard
	static const float step = 0.3f;
	static const float rstep = 5.0f;
	switch (key) {
		case 'a':
			CameraPosition[0] += step;
			break;
		case 'd':
			CameraPosition[0] -= step;
			break;
		case 'w':
			CameraPosition[1] -= step;
			break;
		case 's':
			CameraPosition[1] += step;
			break;
		case 'q':
			CameraPosition[2] += step;
			break;
		case 'e':
			CameraPosition[2] -= step;
			break;
		case 'j':
			CameraRotation[0] += rstep;
			break;
		case 'l':
			CameraRotation[0] -= rstep;
			break;
		case 'i':
			CameraRotation[1] += rstep;
			break;
		case 'k':
			CameraRotation[1] -= rstep;
			break;
		case 'u':
			CameraRotation[2] += rstep;
			break;
		case 'o':
			CameraRotation[2] -= rstep;
			break;
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);//glut function for simplifying OpenGL initialization
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//demanding: double-framed buffer | RGB colorbuffer | depth buffer
	glutInitWindowPosition(100, 50);//set the initial window position
	glutInitWindowSize(800, 600);//set the initial window size
	//**************************
	glutCreateWindow("CG_HW1_0756078");//IMPORTANT!! Create the window and set its title, please replace 12345678 with your own student ID
	//**************************
	glutDisplayFunc(Display);//callback funtion for drawing a frame
	glutReshapeFunc(WindowSize);//callback function when the window size is changed
	glutTimerFunc(deltaTime, Tick, 0);//passTimer function
	glutKeyboardFunc(DealKeyboardInput);

	//you may want to write your own callback funtion for other events(not demanded nor forbidden)
	init();//self-defined initialization function for the elegance of your code

	glutMainLoop();
	return 0;
}