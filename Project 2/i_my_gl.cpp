

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <cmath>

#include <cstdio>

// Defines maximum stack capacity.
#define STACK_CAP 16
// Defines pi for converting angles.
#define PI 3.14159265359

// Structure for the matrix stack, top specifies current top position on the stack, initially zero (which means one matrix in the stack).
struct matrix_stack
{
	GLdouble m[STACK_CAP][16];
	int top;
};

// Define a macro for retrieving current matrix from top of current stack.
#define current_matrix (current_stack->m[current_stack->top])

// Identity matrix constant.
const GLdouble identity[16] =
{ 1, 0, 0, 0,
0, 1, 0, 0,
0, 0, 1, 0,
0, 0, 0, 1 };

// The model view matrix stack.
struct matrix_stack model_view = { { { 0 } }, 0 };
// The projection matrix stack.
struct matrix_stack projection = { { { 0 } }, 0 };
// The current stack pointer that specifies the matrix mode.
struct matrix_stack *current_stack = &model_view;

void I_my_glLoadMatrixd(const GLdouble *m);

// Multiplies current matrix with matrix b, put the result in current matrix.
// current = current * b

void matrix_multiply(const GLdouble* b) {
	bool flag = true;
	for (GLint i = 0; i < 16; i++) {
		if (current_matrix[i] != 0)
			flag = false;
		break;
	}
	if (!flag) {
		for (GLint i = 0; i < 4; i++) {
			GLdouble c0 = current_matrix[i + 4 * 0], c1 = current_matrix[i + 4 * 1], c2 = current_matrix[i + 4 * 2], c3 = current_matrix[i + 4 * 3];
			current_matrix[i + 4 * 0] = c0 * b[0 + 4 * 0] + c1 * b[1 + 4 * 0] + c2 * b[2 + 4 * 0] + c3 * b[3 + 4 * 0];
			current_matrix[i + 4 * 1] = c0 * b[0 + 4 * 1] + c1 * b[1 + 4 * 1] + c2 * b[2 + 4 * 1] + c3 * b[3 + 4 * 1];
			current_matrix[i + 4 * 2] = c0 * b[0 + 4 * 2] + c1 * b[1 + 4 * 2] + c2 * b[2 + 4 * 2] + c3 * b[3 + 4 * 2];
			current_matrix[i + 4 * 3] = c0 * b[0 + 4 * 3] + c1 * b[1 + 4 * 3] + c2 * b[2 + 4 * 3] + c3 * b[3 + 4 * 3];
		}
	}
	else
		I_my_glLoadMatrixd(b);
}


// Calculates cross product of b and c, put the result in a
// a = b x c.
void cross_product(GLdouble *ax, GLdouble *ay, GLdouble *az,
	GLdouble bx, GLdouble by, GLdouble bz,
	GLdouble cx, GLdouble cy, GLdouble cz)
{
	*ax = by * cz - bz * cy;
	*ay = bz * cx - bx * cz;
	*az = bx * cy - by * cx;
}

// Normalizes vector (x, y, z).
void normalize(GLdouble *x, GLdouble *y, GLdouble *z)
{
	GLdouble magnitude = (GLdouble)sqrt(*x * *x + *y * *y + *z * *z);
	*x = *x / magnitude, *y = *y / magnitude, *z = *z / magnitude;
}

// Switches matrix mode by changing the current stack pointer.
void I_my_glMatrixMode(GLenum mode)
{
	// Throw errors for other unacceptable modes?
	if (mode == GL_MODELVIEW) {
		current_stack = &model_view;
	}
	else if (mode == GL_PROJECTION) {
		current_stack = &projection;
	}
}


// Overwrites current matrix with identity matrix.
void I_my_glLoadIdentity(void)
{
	for (int i = 0; i < 16; i++) {
		current_matrix[i] = identity[i];
	}
}

// Pushes current matrix onto current stack, report error if the stack is already full.
void I_my_glPushMatrix(void)
{
	if (current_stack->top < STACK_CAP) {
		for (GLint i = 0; i < 16; i++) {
			current_stack->m[++current_stack->top][i] = current_matrix[i];
		}
	}
	else {
		throw GL_STACK_OVERFLOW;
	}
}

// Pops current matrix from current stack, report error if the stack has only one matrix left.
void I_my_glPopMatrix(void)
{
	if (current_stack->top > 0) {
		current_stack->top--;
	}
	else {
		throw GL_STACK_UNDERFLOW;
	}
}
// Overwrites currentmatrix with m.
void I_my_glLoadMatrixf(const GLfloat *m)
{
	for (GLint i = 0; i < 16; i++) {
		current_matrix[i] = (GLdouble)m[i];
	}
}

void I_my_glLoadMatrixd(const GLdouble *m)
{
	for (GLint i = 0; i < 16; i++) {
		current_matrix[i] = m[i];
	}
}

void I_my_glTranslated(GLdouble x, GLdouble y, GLdouble z)
{
	GLdouble translation[16] =
	{1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	x, y, z, 1 };
	matrix_multiply(translation);
}


void I_my_glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
	I_my_glTranslated((GLdouble)x, (GLdouble)y, (GLdouble)z);
}



// Remembers to normalize vector (x, y, z), and to convert angle from degree to radius before calling sin and cos.
void I_my_glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
	normalize(&x, &y, &z);
	GLdouble radians = angle *PI / 180.0;
	GLdouble s = sin(radians), c = cos(radians);

	GLdouble rotationMatrix[16];
	rotationMatrix[0] = x*x*(1 - c) + c;
	rotationMatrix[1] = y*x*(1 - c) + z*s;
	rotationMatrix[2] = x*z*(1 - c) - y*s;
	rotationMatrix[3] = 0;
	rotationMatrix[4] = x*y*(1 - c) - z*s;
	rotationMatrix[5] = y*y*(1 - c) + c;
	rotationMatrix[6] = y*z*(1 - c) + x*s;
	rotationMatrix[7] = 0;
	rotationMatrix[8] = x*z*(1 - c) + y*s;
	rotationMatrix[9] = y*z*(1 - c) - x*s;
	rotationMatrix[10] = z*z*(1 - c) + c;
	rotationMatrix[11] = 0;
	rotationMatrix[12] = 0;
	rotationMatrix[13] = 0;
	rotationMatrix[14] = 0;
	rotationMatrix[15] = 1;

	matrix_multiply(rotationMatrix);
}

void I_my_glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	I_my_glRotated((GLdouble)angle, (GLdouble)x, (GLdouble)y, (GLdouble)z);
}

void I_my_glScaled(GLdouble x, GLdouble y, GLdouble z)
{
	GLdouble scale[16] =
	{ x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1 };
	matrix_multiply(scale);
}

void I_my_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
	I_my_glScaled((GLdouble)x, (GLdouble)y, (GLdouble)z);
}


// Copies current matrix to m.
void I_my_glGetMatrixf(GLfloat *m)
{
	for (GLint i = 0; i < 16; i++) {
		m[i] = (GLfloat)current_matrix[i];
	}

}

void I_my_glGetMatrixd(GLdouble *m)
{
	for (GLint i = 0; i < 16; i++) {
		m[i] = (GLdouble)current_matrix[i];
	}
}

// Remember to normalize vectors.
void I_my_gluLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
	GLdouble centerX, GLdouble centerY, GLdouble centerZ,
	GLdouble upX, GLdouble upY, GLdouble upZ)
{
	GLdouble lookAtMatrix[16];
	GLdouble fx = centerX - eyeX, fy = centerY - eyeY, fz = centerZ - eyeZ;
	normalize(&fx, &fy, &fz);
	GLdouble sx, sy, sz;
	cross_product(&sx, &sy, &sz, fx, fy, fz, upX, upY, upZ);
	normalize(&sx, &sy, &sz);
	GLdouble ux, uy, uz;
	cross_product(&ux, &uy, &uz, sx, sy, sz, fx, fy, fz);

	lookAtMatrix[0] = sx;
	lookAtMatrix[1] = ux;
	lookAtMatrix[2] = -fx;
	lookAtMatrix[3] = 0;
	lookAtMatrix[4] = sy;
	lookAtMatrix[5] = uy;
	lookAtMatrix[6] = -fy;
	lookAtMatrix[7] = 0;
	lookAtMatrix[8] = sz;
	lookAtMatrix[9] = uz;
	lookAtMatrix[10] = -fz;
	lookAtMatrix[11] = 0;
	lookAtMatrix[12] = 0;
	lookAtMatrix[13] = 0;
	lookAtMatrix[14] = 0;
	lookAtMatrix[15] = 1;

	matrix_multiply(lookAtMatrix);
}

void I_my_glFrustum(GLdouble left, GLdouble right, GLdouble bottom,
	GLdouble top, GLdouble zNear, GLdouble zFar)
{
	GLdouble A = (right + left) / (right - left);
	GLdouble B = (top + bottom) / (top - bottom);
	GLdouble C = -(zFar + zNear) / (zFar - zNear);
	GLdouble D = -(2 * zFar *zNear) / (zFar - zNear);

	GLdouble frustumMatrix[16] = {
		(2 * zNear) / (right - left), 0, 0, 0,
		0, (2 * zNear) / (top - bottom), 0, 0,
		A, B, C, -1,
		0,0,D,0
	};
	matrix_multiply(frustumMatrix);

}
// Based on the inputs, calculate left, right, bottom, top, and call I_my_glFrustum accordingly
// remember to convert fovy from degree to radius before calling tan.
void I_my_gluPerspective(GLdouble fovy, GLdouble aspect,
	GLdouble zNear, GLdouble zFar)
{
	GLdouble f = fovy * PI / 180.0;
	GLdouble t = zNear*tan(f / 2);
	GLdouble b = -t;
	GLdouble r = t * aspect;
	GLdouble l = -r;
	I_my_glFrustum(l, r, b, t, zNear, zFar);

}

