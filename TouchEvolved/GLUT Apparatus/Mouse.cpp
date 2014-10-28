/*
 * mouse.c
 * -------
 * Mouse callbacks for the canvas.  Currently a little clunky, because
 * all rotations are in terms of the original axes of the model, rather
 * than the transformed axes of the current viewpoint.
 *
 * You shouldn't have to modify anything in this file.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

#include "mouse.h"

/* The frustum and zoom factor variables from canvas.c */
extern GLfloat fleft;
extern GLfloat fright;
extern GLfloat ftop;
extern GLfloat fbottom;
extern GLfloat zNear;
extern GLfloat zFar;
extern GLfloat zoomFactor;
extern int win_width;
extern int win_height;

/* The current mode the mouse is in, based on what button(s) is pressed */
int mouse_mode;

/* The last position of the mouse since the last callback */
int m_last_x, m_last_y;


//this breaks the window into 4 quadrants which go from -1 to 1 in x and y directions
void convert_coordinate(GLfloat *results, GLfloat x, GLfloat y)
{
	GLfloat realX = x / (GLfloat)win_width * (fright - fleft) + fleft;
	GLfloat realY = (1 - y / (GLfloat)win_height) * (ftop - fbottom) + fbottom;

	printf("after convert: ( %f , %f ) \n", realX, realY);

	results[0] = realX;
	results[1] = realY;

}


void myMouseButton(int button, int state, int x, int y) 
{
	GLfloat proper_coordinates[2];
	if (state == GLUT_DOWN) 
	{
		m_last_x = x;
		m_last_y = y;

		if (button == GLUT_LEFT_BUTTON) 
		{
			printf("before convert: ( %d , %d ) \n", x, y);
			convert_coordinate(proper_coordinates, x, y);
			mouse_mode = MOUSE_ROTATE_YX;
		}
		else if (button == GLUT_MIDDLE_BUTTON) 
		{
			mouse_mode = MOUSE_ROTATE_YZ;
		} 
		else if (button == GLUT_RIGHT_BUTTON) 
		{
			mouse_mode = MOUSE_ZOOM;
		}
	}
}

//will need to modify this so that only and individual object is moved at a time.
void myMouseMotion(int x, int y) {
	double d_x, d_y;	/* The change in x and y since the last callback */

	d_x = x - m_last_x;
	d_y = y - m_last_y;

	m_last_x = x;
	m_last_y = y;

	if (mouse_mode == MOUSE_ROTATE_YX) {
		/* scaling factors */
		d_x /= 2.0;
		d_y /= 2.0;

		glRotatef(d_x, 0.0, 1.0, 0.0);	/* y-axis rotation */
		glRotatef(-d_y, 1.0, 0.0, 0.0);	/* x-axis rotation */

	} else if (mouse_mode == MOUSE_ROTATE_YZ) {
		/* scaling factors */
		d_x /= 2.0;
		d_y /= 2.0;

		glRotatef(d_x, 0.0, 1.0, 0.0);	/* y-axis rotation */
		glRotatef(-d_y, 0.0, 0.0, 1.0);	/* z-axis rotation */

	} else if (mouse_mode == MOUSE_ZOOM) {
		d_y /= 100.0;

		zoomFactor += d_y;

		if (zoomFactor <= 0.0) {
			/* The zoom factor should be positive */
			zoomFactor = 0.001;
		}

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		/*
		 * glFrustum must receive positive values for the near and far
		 * clip planes ( arguments 5 and 6 ).
		 */
		glFrustum(fleft*zoomFactor, fright*zoomFactor,
			fbottom*zoomFactor, ftop*zoomFactor,
			-zNear, -zFar);
	}

	/* Redraw the screen */
	glutPostRedisplay();
}

/* end of mouse.c */

