/*
 * OpenGL demonstration program for UCI ICS Computer Graphics courses
 * sceneModule.cpp,v 2.2 2016/10/05 11:38 pm
 *
 */
#include <cstdio>
#include <iostream>
#include "sceneModule.h"
#include "my_gl.h"
#include "I_my_gl.h"



#define PI 3.14159265359

GLfloat angley = 20;    /* in degrees */
GLfloat anglex = 30;   /* in degrees */
GLfloat distanceX = 0.0;
GLfloat distanceY = 0.0;
GLfloat distanceZ = 5.0;
void I_my_glGetMatrixd(GLdouble *m);


static float cubeColors[6][4] =
{
  {0.8, 0.8, 0.8, 1.0},
  {0.8, 0.0, 0.0, 1.0},
  {0.0, 0.8, 0.0, 1.0},
  {0.0, 0.0, 0.8, 1.0},
  {0.0, 0.8, 0.8, 1.0},
  {0.8, 0.0, 0.8, 1.0},
};

static float cubeVertexes[6][4][4] =
{
  {
    {-1.0, -1.0, -1.0, 1.0},
    {-1.0, -1.0, 1.0, 1.0},
    {-1.0, 1.0, 1.0, 1.0},
    {-1.0, 1.0, -1.0, 1.0}},

  {
    {1.0, 1.0, 1.0, 1.0},
    {1.0, -1.0, 1.0, 1.0},
    {1.0, -1.0, -1.0, 1.0},
    {1.0, 1.0, -1.0, 1.0}},

  {
    {-1.0, -1.0, -1.0, 1.0},
    {1.0, -1.0, -1.0, 1.0},
    {1.0, -1.0, 1.0, 1.0},
    {-1.0, -1.0, 1.0, 1.0}},

  {
    {1.0, 1.0, 1.0, 1.0},
    {1.0, 1.0, -1.0, 1.0},
    {-1.0, 1.0, -1.0, 1.0},
    {-1.0, 1.0, 1.0, 1.0}},

  {
    {-1.0, -1.0, -1.0, 1.0},
    {-1.0, 1.0, -1.0, 1.0},
    {1.0, 1.0, -1.0, 1.0},
    {1.0, -1.0, -1.0, 1.0}},

  {
    {1.0, 1.0, 1.0, 1.0},
    {-1.0, 1.0, 1.0, 1.0},
    {-1.0, -1.0, 1.0, 1.0},
    {1.0, -1.0, 1.0, 1.0}}
};

void drawScene(){

  for (int i = 0; i < 6; i++) {
    glBegin(GL_POLYGON);
		glColor3fv(&cubeColors[i][0]);
		glVertex4fv(&cubeVertexes[i][0][0]);
		glVertex4fv(&cubeVertexes[i][1][0]);
		glVertex4fv(&cubeVertexes[i][2][0]);
		glVertex4fv(&cubeVertexes[i][3][0]);
    glEnd();
  }
}

void sceneTransformation(){

  //glTranslatef(-distanceX, distanceY, -distanceZ);
  //glRotatef( anglex, 1.0, 0.0, 0.0 );
  //glRotatef( angley, 0.0, 1.0, 0.0 );

  //gluLookAt(eyeX, eyeY, eyeZ, atX, atY, atZ, upX, upY, upZ);
  glLoadIdentity();

  //  glTranslatef(-distanceX, distanceY, -distanceZ);
  //  glRotatef( anglex, 1.0, 0.0, 0.0 );
  //  glRotatef( angley, 0.0, 1.0, 0.0 );
  //    return;

  // The code below should have exactly the same behavior as the three lines above.
  GLdouble eyeX, eyeY, eyeZ;
  GLdouble atX = 0, atY = 0, atZ = 0;
  GLdouble upX = 0, upY = 1, upZ = 0;

  while (anglex < 0)
	  anglex += 360;
  while (anglex >= 360)
	  anglex -= 360;
  if (anglex > 90 && anglex < 270)
	  upY = -1;

  double radian_x = anglex * (PI / 180), radian_y = angley * (PI / 180);
  eyeX = distanceX * cos(radian_y) - sin(radian_y) * sin(radian_x) * distanceY - sin(radian_y) * cos(radian_x) * distanceZ;
  eyeY = -cos(radian_x) * distanceY + sin(radian_x) * distanceZ;
  eyeZ = sin(radian_y) * distanceX - cos(radian_y) * sin(radian_x) * distanceY + cos(radian_y) * cos(radian_x) * distanceZ;
  gluLookAt(eyeX, eyeY, eyeZ, atX, atY, atZ, upX, upY, upZ);

}


void testcase1() {
	//Test LoadMatrix
	//GLdouble tempmatrix0[16], newmatrix0[16];
	//GLdouble m[16] = {
	//	1, 2, 3, 4,
	//	2, 3, 5, 6,
	//	7, 5, 3, 2,
	//	2, 3, 4, 3
	//};
	//// This operates the OpenGL way, retrieves the result and undoes it
	//glPushMatrix();
	//glLoadMatrixd(m);
	//glGetDoublev(GL_MODELVIEW_MATRIX, tempmatrix0);
	//glPopMatrix();
	//// This uses your my_gl* functions, and will have effect
	//my_glLoadMatrixd(m);
	//I_my_glGetMatrixd(newmatrix0);
	//if (!compareMat(newmatrix0, tempmatrix0)) {
	//	printf("Matrix is wrong");
	//}
	//else {
	//	printf("LoadMatrix is right");
	//}
	//printMat(newmatrix0);
	//printMat(tempmatrix0);


	////Test LoadIdentity
	//GLdouble tempmatrix1[16], newmatrix1[16];
	//// This operates the OpenGL way, retrieves the result and undoes it
	//glPushMatrix();
	//glLoadIdentity();
	//glGetDoublev(GL_MODELVIEW_MATRIX, tempmatrix1);
	//glPopMatrix();
	//// This uses your my_gl* functions, and will have effect
	//my_glLoadIdentity();
	//I_my_glGetMatrixd(newmatrix1);
	//if (!compareMat(newmatrix1, tempmatrix1)) {
	//	printf("Matrix is wrong");
	//}
	//else {
	//	printf("LoadIdentity is right");
	//}
	//printMat(newmatrix1);
	//printMat(tempmatrix1);

	////Test glTranslated
	//GLdouble tempmatrix2[16], newmatrix2[16];
	//// This operates the OpenGL way, retrieves the result and undoes it
	//glPushMatrix();
	//glTranslated(-distanceX, -distanceY, -distanceZ);
	//glGetDoublev(GL_MODELVIEW_MATRIX, tempmatrix2);
	//glPopMatrix();
	//// This uses your my_gl* functions, and will have effect
	//my_glTranslated(-distanceX, -distanceY, -distanceZ);
	//I_my_glGetMatrixd(newmatrix2);
	//if (!compareMat(newmatrix2, tempmatrix2)) {
	//	printf("Matrix is wrong");
	//}
	//else {
	//	printf("glTranslated is right");
	//}
	//printMat(newmatrix2);
	//printMat(tempmatrix2);

	////Test glRotated
	//GLdouble tempmatrix3[16], newmatrix3[16];
	//// This operates the OpenGL way, retrieves the result and undoes it
	//glPushMatrix();
	//glRotated(anglex, 1, 0, 0);
	//glGetDoublev(GL_MODELVIEW_MATRIX, tempmatrix3);
	//glPopMatrix();
	//// This uses your my_gl* functions, and will have effect
	//my_glRotated(anglex, 1, 0, 0);
	//I_my_glGetMatrixd(newmatrix3);
	//if (!compareMat(newmatrix3, tempmatrix3)) {
	//	printf("Matrix is wrong");
	//}
	//else {
	//	printf("glRotated is right");
	//}
	//printMat(newmatrix3);
	//printMat(tempmatrix3);

	////Test glFrustum
	//GLdouble tempmatrix4[16], newmatrix4[16];
	//// This operates the OpenGL way, retrieves the result and undoes it
	//glPushMatrix();
	//glFrustum(-2, 2, -4, 4, 6, 8);
	//glGetDoublev(GL_MODELVIEW_MATRIX, tempmatrix4);
	//glPopMatrix();
	//// This uses your my_gl* functions, and will have effect
	//my_glFrustum(-2,2,-4,4,6,8);
	//I_my_glGetMatrixd(newmatrix4);
	//if (!compareMat(newmatrix4, tempmatrix4)) {
	//	printf("Matrix is wrong");
	//}
	//else {
	//	printf("glFrustum is right");
	//}
	//printMat(newmatrix4);
	//printMat(tempmatrix4);

	//////Test gluPerspective
	//GLdouble tempmatrix5[16], newmatrix5[16];
	//// This operates the OpenGL way, retrieves the result and undoes it
	//glPushMatrix();
	//gluPerspective(45, 2, 1, 10);
	//glGetDoublev(GL_MODELVIEW_MATRIX, tempmatrix5);
	//glPopMatrix();
	//// This uses your my_gl* functions, and will have effect
	//my_gluPerspective(45, 2, 1, 10);
	//I_my_glGetMatrixd(newmatrix5);
	//if (!compareMat(newmatrix5, tempmatrix5)) {
	//	printf("Matrix is wrong");
	//}
	//else {
	//	printf("gluPerspective is right");
	//}
	//printMat(newmatrix5);
	//printMat(tempmatrix5);
}

void testcase2() {
	sceneTransformation();
	drawScene();
}

void testcase3() {
	GLdouble angle = 0;
	GLdouble deltaAngle = 10;
	angle += deltaAngle;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	sceneTransformation();
	glPushMatrix();
	glRotated(angle, 0.0, 1.0, 0.0);
	drawScene();
	glPopMatrix();
	glRotated(angle, 0.0, 1.0, 0.0);
	glTranslatef(2.5, 0, 0);
	glScalef(0.5, 0.5, 0.5);
	drawScene();

	//glutSwapBuffers();

}
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//test basic functions
	//testcase1();
	//test lookAt
	//testcase2();
	//test animation
	//testcase3();
	glutSwapBuffers();
}



