
#include <sys/types.h>
#include "inputModule.h"
#include "PLY.h"
#include "geometry.h"


/* This File contains the KeyBoard and mouse handling routines */
static int motionMode;
static int startX;
static int startY;
static GLfloat angle = 20;    /* in degrees */
static GLfloat angle2 = 30;   /* in degrees */

//static int lightX;
//static int lightY;
extern Vector4f initial_light_pos;//from main.cpp
Vector4f current_light = { -100.0, 100.0, 100.0, 0.0 };//cannot use copy constructor?
static GLfloat lightAngle = 20;    /* in degrees */
static GLfloat lightAngle2 = 30;   /* in degrees */


GLfloat current_pos[] = {0.0, 0.0, 5.0};

int flat = 0;
int light = 1;
int move = 0;
int wire = 0;

extern PLYObject* ply;

void readKeyboard(unsigned char key, int x, int y)
{
  switch(key){
  case  0x1B:
  case  'q':
  case  'Q':
    exit(0);
    break; 
  case '+':
    if (ply)
      ply->eat();
    break;
  case '-':
    if (ply)
      ply->starve();
    break;
  case 'd':
    if (ply)
      ply->dance();
    break;
  case 'i':
  case 'I':
    if (ply)
      ply->invertNormals();
    break;
  case 'l':
  case 'L':
    light = (light + 1) % 2;
		printf("%s lighting\n", (light ? "OpenGL" : "User"));
    break;
  case 't':
  case 'T':
	  move = (move + 1) % 2;
	  printf("%s %s\n", (light ? "OpenGL" : "User"),(move ? "moves lighting" : "moves object"));
	  break;
  case 'r':
  case 'R': 
   // reset initial view parameters
    angle = 20;
    angle2 = 30;
    current_pos[0] = 0.0;
    current_pos[1] = 0.0;
    current_pos[2] = 5.0;
	lightAngle = 20; 
	lightAngle2 = 30; 
	current_light[0] = -100.0;
	current_light[1] = 100.0;
	current_light[2] = 100.0;
	current_light[3] = 0.0;
    break;
  case 'h':
  case 'H':
    printf("\tPress q/Q for Quit\n");
    printf("\tPress h/H to print this help\n");
    printf("\tPress l/L to turn on/off Lighting\n");
    printf("\tPress i/I to invert the normals\n");
	printf("\tPress t/T to move light position\n");
    printf("\tPress r/R to revert ViewPoint to initial position\n");
    printf("\tPress + to make the bunny grow fatter\n");
    printf("\tPress - to make the bunny grow thinner\n");
    printf("\tPress d/D to make the bunny dance randomly\n");
  default:
    break;
  }
  glutPostRedisplay();
}


void readSpecialKeys(int key, int x, int y)
{
  switch(key){
  case GLUT_KEY_UP: 
    
    break;
  case GLUT_KEY_DOWN: 
    
    break;
  case GLUT_KEY_RIGHT: 
    
    break;
  case GLUT_KEY_LEFT:  
    
    break;
  }
  glutPostRedisplay();
}


void mouseButtHandler(int button, int state, int x, int y)
{
  motionMode = 0;
  
  switch(button){
  case GLUT_LEFT_BUTTON:  
    if(state == GLUT_DOWN) {
		motionMode = 1;		// Rotate object
			startX = x;
			startY = y;
    }
    break;
  case GLUT_MIDDLE_BUTTON:  
    if(state == GLUT_DOWN) {
      motionMode = 2;		// Translate object
		  startX = x;
		  startY = y;
    }
    break;
  case GLUT_RIGHT_BUTTON: 
    if(state == GLUT_DOWN) { 
      motionMode = 3;		// Zoom
		  startX = x;
		  startY = y;
    }
    break;
  }
  glutPostRedisplay();
}


void mouseMoveHandler(int x, int y)
{
  // No mouse button is pressed... return 
  switch(motionMode){
  case 0:
    return;
    break;

  case 1: // Calculate the rotations
	  if (move != 1) {
		  angle = angle + (x - startX);
		  angle2 = angle2 + (y - startY);
		  startX = x;
		  startY = y;
	  }
	  else {
		  lightAngle = lightAngle + (x - startX);
		  lightAngle2 = lightAngle2 + (y - startY);
		  startX = x;
		  startY = y;

	  }
    break;

  case 2:
	  if (move != 1) {
		  current_pos[0] = current_pos[0] - (x - startX) / 100.0;
		  current_pos[1] = current_pos[1] - (y - startY) / 100.0;
		  startX = x;
		  startY = y;
	  }
	  else {
		  current_light[0] = current_light[0] - (x - startX) / 100.0;
		  current_light[1] = current_light[1] - (y - startY) / 100.0;
		  startX = x;
		  startY = y;


	  }
    break;
    
  case 3:
	  if (move != 1) {
		  current_pos[2] = current_pos[2] - (y - startY) / 10.0;
		  startX = x;
		  startY = y;
	  }
	  else {
		  current_light[2] = current_light[2] - (y - startY) / 10.0;
		  startX = x;
		  startY = y;

	  }
	  break;
  }
  
  glutPostRedisplay();
}


void setUserView()
{
	//if (move != 1) {
		glLoadIdentity();

		glTranslatef(-current_pos[0], current_pos[1], -current_pos[2]);
		glRotatef(angle2, 1.0, 0.0, 0.0);
		glRotatef(angle, 0.0, 1.0, 0.0);
	}

void setLight()
{
		//glPushMatrix();

		//glLoadIdentity();

		glTranslatef(current_light[0], current_light[1], current_light[2]);
		glRotatef(lightAngle2, 1.0, 0.0, 0.0);
		glRotatef(lightAngle, 0.0, 1.0, 0.0);
		/*GLdouble M[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, M);
		Matrix4f m;
		for (GLint i = 0; i < 3; i++) {
			m[i][3] = 0.0;
			m[3][i] = 0.0;
			for (GLint j = 0; j < 3; j++) {
				m[i][j] = M[i * 4 + j];
			}
		}
		m[3][3] = 1.0;

	
		multVector(initial_light_pos, m, current_light);
		glLightfv(GL_LIGHT0, GL_POSITION, current_light);
*/
		//glPopMatrix();
}
