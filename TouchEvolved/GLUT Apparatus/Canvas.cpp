/*
 * canvas.c
 * --------
 * Implements a canvas with multiple display modes.
 * Starter code for Project 1.
 *
 * Group Members: <FILL IN>

 TouchEvolved\GLUT Apparatus\Canvas.cpp
 */

#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "drawing.h"
#include "vrml.h"
#include "mouse.h"
#include "LeapControl.h"
#include "Leap.h"

using namespace Leap;
#define PI 3.14159265358979323846

/* Function Declarations */
void myInit (int argc, char **argv);

void myDisplay (void);
void myReshape (int, int);
void myKeyHandler (unsigned char, int, int);

void resetCamera(void);
int endCanvas(int status);
void performanceTest(void);
void initLighting(void);

double r;
double h;
int n;
int vrml_changed_value;
int init_others;
GLfloat rotation_angle;
int rotate_triforce;

/* The current vrml object */
int vr_object = -1;

//Cylinder Object
GLfloat cylbase = 0.5f;
GLfloat cyltop = 0.5f;
GLfloat cylheight = 1.5f;
GLint cylslices = 30;
GLint cylStacks = 30;
bool rotate_cylinder = true;

#define SOLID_CLOSED_CYLINDER(QUAD, BASE, TOP, HEIGHT, SLICES, STACKS) \
	gluCylinder(QUAD, BASE, TOP, HEIGHT, SLICES, STACKS); \
	glRotatef(180, 1, 0, 0); \
	gluDisk(QUAD, 0.0f, BASE, SLICES, 1); \
	glRotatef(180, 1, 0, 0); \
	glTranslatef(0.0f, 0.0f, HEIGHT); \
	gluDisk(QUAD, 0.0f, TOP, SLICES, 1); \
	glTranslatef(0.0f, 0.0f, -HEIGHT); 


//Initiate the Leap Listener and controller objects
LeapListener myLeapListener;
Controller myLeapController;



/*************************************************************
 * Global Variables / Constants
 * ----------------------------
 * The following variables are visible globally, and most
 * control the global state of the canvas or contain data
 * that would be cumbersome to pass around.
 *************************************************************/


/* The canvas's width and height, in pixels */
int win_width = 500;
int win_height = 500;



/* The dimensions of the viewing frustum */
GLfloat fleft   = -1.0;
GLfloat fright  =  1.0;
GLfloat fbottom = -1.0;
GLfloat ftop    =  1.0;
GLfloat zNear   = -2.0;
GLfloat zFar    = -7.0;


/* Global zoom factor.  Modified by user input. Initially 1.0 */
GLfloat zoomFactor = 1.0; 


/*Global that keeps track of changes between written programs*/
int calc_tri_changed = 0;


/* Constants for specifying the 3 coordinate axes */
#define X_AXIS			0
#define Y_AXIS			1
#define Z_AXIS			2

/*************************************************************
 * End Global Variables / Constants
 *************************************************************/



/* Begin function definitions */

/*
 * Performs specific initializations for this program (as opposed to
 * glut initialization.
 */
void myInit (int argc, char **argv) {
	/* Set the default display mode and style */
	disp_mode = DM_CUBE_GLUT;
	disp_style = DS_SOLID;

	/* Set up a black background */
	glClearColor(0.0, 0.0, 0.0, 0.0);

	resetCamera();
	//print_disp_mode();
}


/*
 * The main drawing routine.  Based on the current display mode, other
 * helper functions may be called.
 */
void myDisplay (void) {

	glEnable(GL_DEPTH_TEST);	/* Use the Z - buffer for visibility */
	glMatrixMode(GL_MODELVIEW);	/* All matrix operations are for the model */

	/* Clear the pixels (aka colors) and the z-buffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (disp_mode) {
		case DM_CUBE_GLUT:
			if(used_free_scene)
			{				
				used_free_scene = 0;
				rotate_cylinder = true;
			}
				
			draw_cube_glut();
			break;
		case DM_CUBE_QUAD:
			draw_cube_quad();
			break;
		case DM_CUBE_QUAD_ARRAYS:
			draw_cube_quad_arrays();
			break;
		case DM_CONE_GLUT:
			draw_cone_glut();
			break;
		case DM_CONE_TRI:

			draw_cone_tri();
			break;
		case DM_CONE_TRI_ARRAYS:
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			draw_cone_tri_arrays();
			break;
		case DM_CONE_TRI_CALC:
			/*
			 * NOTE
			 * ----
			 * This call will need to be changed to use the user-specified
			 * parameters.  Right now, hard coded parameters are used.
			 */
			 vr_object = -1;
			 vrml_changed_value = -1;
			if(!calc_tri_changed)
			{
				r = 1;
				h= 1;
				n = 8;
				draw_cone_tri_calc(r, h, n);
				calc_tri_changed = 1;
			}
			else
				draw_cone_tri_calc(r, h, n);
			
			break;
		case DM_VRML:
			if(used_free_scene)
			{
				glClearColor(0.0, 0.0, 0.0, 0.0);
				used_free_scene = 0;
				rotate_cylinder = true;
			}
			rotation_angle = 0;
			calc_tri_changed = 0;
			draw_vrml();
			vrml_changed_value = 0;
			
			break;
		case DM_FREE_SCENE:
			if (rotate_cylinder)
			{
				glRotatef(90.0, 1.0, 0.0, 0.0);
				//glTranslatef(0.0, -1.0, 0.0);
			}

			glPushMatrix();
				GLUquadric *Cylinder;
				Cylinder = gluNewQuadric();
				gluQuadricDrawStyle(Cylinder, GLU_LINE);
				gluQuadricOrientation(Cylinder, GLU_OUTSIDE);
				SOLID_CLOSED_CYLINDER(Cylinder, cylbase, cyltop, cylheight, cylslices, cylStacks)
			glPopMatrix();
			
			used_free_scene = 1;
			rotate_cylinder = false;
			break;
		default:
			rotation_angle = 0;
			printf("myDisplay Warning: unrecognized Display Mode\n");
			break;
	}

	glFlush();	/* Flush all executed OpenGL ops finish */

	/*
	 * Since we are using double buffers, we need to call the swap
	 * function every time we are done drawing.
	 */
	glutSwapBuffers();

}


/*
 * Changes the size of the canvas's window, and will implicitly
 * the function bound by glutReshapeFunc(), which should be
 * Reshape().
 */
void myResize (int x, int y) {
	glViewport(0,0,x,y);
	glutReshapeWindow(x, y);
}


/* Stretch the image to fit the reshaped window */
void myReshape (int x, int y) {
 	glViewport(0,0,x,y);
}


/*
 * The rotation is specified in degrees about a certain axis of
 * the original model.
 *
 * AXIS should be either X_AXIS, Y_AXIS, or Z_AXIS.
 *
 * Positive degrees rotate in the counterclockwise direction.
 */
void rotateCamera(double deg, int axis) {
	double x, y, z;

	x = 0;
	y = 0;
	z = 0;

	if (axis == X_AXIS) {
		x = 1.0f;
	} else if (axis == Y_AXIS) {
		y = 1.0f;
	} else if (axis == Z_AXIS) {
		z = 1.0f;
	}
 
	glRotatef(deg, x, y, z);
}


/*
 * Changes the level of zooming by adjusting the dimensions of the viewing
 * frustum.
 *
 * Args: delta - the change in the zoom factor.  Negative deltas cause the
 * camera to zoom in, while positive values cause the camera to zoom out.
 */
void zoomCamera(double delta) {
	zoomFactor += delta;

	if (zoomFactor <= 0.0) {
		/* The zoom factor should be positive */
		zoomFactor = 0.001;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*
	 * glFrustum must receive positive values for the near and far clip planes
	 * ( arguments 5 and 6 ).
	 */
	glFrustum(fleft*zoomFactor, fright*zoomFactor,
		fbottom*zoomFactor, ftop*zoomFactor,
		-zNear, -zFar);

}

/*
 * Resets the viewing frustum and moves the drawing point to the center of
 * the frustum.
 */
void resetCamera( void ) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*
	 * glFrustum must receive positive values for the near and far clip planes
	 * ( arguments 5 and 6 ).
	 */
	glFrustum(fleft, fright, fbottom, ftop, -zNear, -zFar);

	/* Set the drawing point at the center of the frustum */
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	/* Go to the center of the scene */
	glTranslatef(
		(fleft + fright) / 2,
		(fbottom + ftop) / 2,
		(zNear + zFar) / 2);

	zoomFactor = 1.0;
}


void performanceTest(void) {
	int start, end;
	int i;
	int curr_width, curr_height;

	/* Give a warning if the window has been resized */
	curr_width = glutGet(GLUT_WINDOW_WIDTH);
	curr_height = glutGet(GLUT_WINDOW_HEIGHT);

	if ((curr_width != win_width) || (curr_height != win_height)) {
		printf("*** Warning ***\n");
		printf("The window has been resized and results may be inaccurate.\n");
		printf("First press 'z' to restore the default window size.\n");
		printf("*** Warning ***\n");
	}

	resetCamera();

	printf("Initiating Performance Test\n");
	start = glutGet(GLUT_ELAPSED_TIME);

	/* For every rotation, the display loop will be recalled */
	for (i = 0; i < 360; i++) {
		rotateCamera(1.0, X_AXIS);
		myDisplay();		/* refresh the screen */
	}
	for (i = 0; i < 360; i++) {
		rotateCamera(1.0, Y_AXIS);
		myDisplay();		/* refresh the screen */
	}
	for (i = 0; i < 360; i++) {
		rotateCamera(1.0, Z_AXIS);
		myDisplay();		/* refresh the screen */
	}

	end = glutGet(GLUT_ELAPSED_TIME);

	/* Return the number of milliseconds elapsed */
	printf("Performance Test completed in %.2f sec\n",
		(end - start) / 1000.0f);
}


/* Handle user input */
void myKeyHandler(unsigned char ch, int x, int y) {
	
	
	switch(ch) {
		case 'c':
			resetCamera();
			printf("Camera reset.\n");
			break;

		case 'C':
			resetCamera();
			break;

		case 's':
		case 'S':
			if (disp_style == DS_SOLID) {
				disp_style = DS_WIRE;
			} else {
				disp_style = DS_SOLID;
			}
			print_disp_style();
			break;

		case 'd':
			/* Cycle through the various display modes */
			//printf("\n \n char: %c, int x: %d  int y: %d \n \n", ch,x,y);
			disp_mode = (disp_mode + 1) % DM_MAX;
			print_disp_mode();
			break;

		case 'D':
			/* Cycle through the various display modes backwards */
			/* By adding DM_MAX, the args to "%" wil never be negative */
			disp_mode = (disp_mode + DM_MAX - 1) % DM_MAX;
			//printf("\n \n char: %c, int x: %d  int y: %d \n \n", ch, x, y);
			print_disp_mode();
			break;

		case ',':
			rotateCamera(5, X_AXIS);
			break;

		case '<':
			rotateCamera(-5, X_AXIS);
			break;

		case '.':
			rotateCamera(5, Y_AXIS);
			break;

		case '>':
			rotateCamera(-5, Y_AXIS);
			break;

		case '/':
			rotateCamera(5, Z_AXIS);
			break;

		case '?':
			rotateCamera(-5, Z_AXIS);
			break;

		case '+':
			/* Zoom in */
			zoomCamera(-0.1);
			break;

		case '=':
			/* Zoom out */
			zoomCamera(0.1);
			break;

		case 'z':
			myResize(win_width, win_height);
			printf("Window set to default size.\n");
			break;

		case 't':
			performanceTest();
			break;

		case 'q':
			/* Quit with exit code 0 */
			// Remove the sample listener when done
			//myLeapController.removeListener(myLeapListener);
			endCanvas(0);
			break;


			
		/*********************************************/
		/* ADD ADDITIONAL KEYS HERE                  */
		/*********************************************/
		case 'I':
			r += 0.1f;			
			printf("Radius increased by 0.1\n");
			break;
			
		case 'i':
			if((r - 0.1f) < 0.1f)
			{
				printf("Radius cannot be less than 0.1\n");
				break;
			}
			r -= 0.1f;	
			printf("Radius decreased by 0.1\n");			
			break;
			
		case 'O':
			printf("Height increased by 0.1\n");	
			break;
			
		case 'o':
			if((h - 0.1f) < 0.1f)
			{
				printf("Height cannot be less than 0.1\n");
				break;
			}
			h -= 0.1f;
			printf("Height decreased by 0.1\n");	
			break;
			
			
		case 'P':
			n += 1;
			printf("Number of base triangles increased by 1\n");	
			break;
			
			
		case 'p':
			if((n - 1) < 3)
			{
				printf("Number of base triangles cannot be less than 3\n");
				break;
			}
			n -= 1;	
			printf("Number of base triangles decreased by 1\n");	
			break;

			
		case 'v':
			if( vr_object == -1 || vr_object == VR_PYRAMID)
			{
				vr_object = VR_CUBE;	
			}
	
			//change object to DODECAHEDRON
			else if(vr_object == VR_CUBE)
			{
				vr_object = VR_DODECAHEDRON;
			}
	
			//change object to icosahedron
			else if(vr_object == VR_DODECAHEDRON)
			{
				vr_object = VR_ICOSAHEDRON;
			}
	
			//change object to pyramid
			else if(vr_object == VR_ICOSAHEDRON)
			{
				vr_object = VR_PYRAMID;
			}

			vrml_changed_value = 1;
			break;
		
		case 'F':
			disp_mode = (disp_mode + 8) % DM_MAX;
			print_disp_mode();
			break;
		
		case 'l':
				if(!surround_light)
				{
					surround_light = 1;
					printf("Making scene brighter\n");
				}

				else
				{
					surround_light = 0;
					printf("Making scene darker\n");
				}
			break;
			
		case 'L':
			if(triforce_color == GOLD)
			{
				change_triforce_colors(RUBY);
				printf("Changing Triforce color to Ruby\n");
			}
				
			else if(triforce_color == RUBY)
			{
				change_triforce_colors(EMERALD);
				printf("Changing Triforce color to Emerald\n");
			}
				
			else if(triforce_color == EMERALD)
			{
				change_triforce_colors(SAPHIRE);
				printf("Changing Triforce color to Saphire\n");
			}
			
			else if(triforce_color == SAPHIRE)
			{
				use_tri_color = 1;
				triforce_color = TRI;
				printf("Changing to all Triforce colors\n");
			}
			else if(triforce_color == TRI)
			{
				change_triforce_colors(GOLD);
				use_tri_color = 0;
				printf("Changing Triforce color to Gold\n");
				
			}
				
			break;
			
		case 'f':
			rotate_triforce = 1;
			int l;
			printf("Rotating Triforce, animation must complete to continue...\n");
			for(l = 0; l < (58<<7); l++)
			{
				myDisplay();
			}			
			break;
			

		default:
			/* Unrecognized key press, just return */
			return;

			break;
	}
	

	/*
	 * If control reaches here, the key press was recognized.  Refresh
	 * the screen, since most key presses change the display in some way.
	 */
	myDisplay();

	return;
}


int endCanvas(int status) {
  printf("\nQuitting canvas.\n\n");
  fflush(stdout);

  exit(status);
}


int main(int argc, char **argv)
{	
	
	
	
	glutInit(&argc, argv);

	/* Set initial window size and screen offset */
	glutInitWindowSize(win_width, win_height);
	glutInitWindowPosition(50, 50);

	/* Using: RGB (no alpha), double buffering, z-buffer */
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("Canvas");

	/* Set the function callbacks */
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glutKeyboardFunc(myKeyHandler);
	glutMouseFunc(myMouseButton);
	glutMotionFunc(myMouseMotion);

	// glutIdleFunc(myDisplay);
	
	
	/* User specific initialization */
	myInit(argc, argv);
	init_others = 1;
	
	
	// Have the listener receive events from the controller
	myLeapController.addListener(myLeapListener);
	if (argc > 1 && strcmp(argv[1], "--bg") == 0)
		myLeapController.setPolicyFlags(Leap::Controller::POLICY_BACKGROUND_FRAMES);

	printf("\n");
	print_disp_mode();
	glutMainLoop();	/* Go into the main glut control loop, will not return */

	/* Control flow will never reach here */
	return 0;
}

/* end of canvas.c */
