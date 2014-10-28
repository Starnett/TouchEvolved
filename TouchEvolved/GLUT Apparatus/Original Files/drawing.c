/*
 * drawing.c
 * ---------
 * Drawing subroutines for each of the various display modes in the canvas.
 * Also contains the quadrilateral information for a cube and the
 * triangulation information for a cone.
 *
 * Starter code for Project 1.
 *
 * Group Members: <FILL IN>
 */

#include "common.h"

#include <stdio.h>

#include "drawing.h"
#include "vrml.h"
#include <math.h>

#define PI 3.14159265358979323846



/* The current display mode */
int disp_mode;

/* The current display style */
int disp_style;




/***********************************************************
 * Begin Cube Data
 ***********************************************************/
/*
 * Vertices used in the manually rendered cube.  This is cube
 * that is 1 x 1 x 1 centered at the origin.  Each group of
 * 3 numbers is a vertex, specified in Cartesian coordinates.
 */
GLfloat cube_vertices[] = {
    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
};

/*
 * The colors of each vertex in the low level cube.  The index
 * into this array corresponds to the index into cube_vert.
 */
GLfloat cube_colors[] = {
    0.5f, 0.5f, 0.5f,
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,
};

/*
 * Indices into cube_vert, groups of 4 can be used to draw quadrilaterals
 * which represents a face of the cube.  For instance, the first
 * quad means that vertices 0, 1, 3, 2 of the cube_vertices[]
 * are used, in that order, to form the first face of the cube.
 *
 * Note that all front facing quads are specified in a counterclockwise order
 * (that is, if you are looking at the front of a quad, the vertices will
 * appear in counterclockwise order).
 */
GLuint cube_indices[] = {
    0, 2, 3, 1,
    2, 6, 7, 3,
    7, 6, 4, 5,
    4, 0, 1, 5,
    1, 3, 7, 5,
    0, 4, 6, 2,
};
/***********************************************************
 * End Cube Data
 ***********************************************************/


/***********************************************************
 * Begin Cone Data
 ***********************************************************/

/*
 * Data for the triangulation of the surface of a cone that is one
 * unit tall has a unit circle for its base.  The base is composed
 * of 8 equally sized triangles, and the lateral surface of the cone
 * is composed of a different set of 8 equally sized triangles.
 *
 * See documentation in the Cube Data section for information on
 * the meaning of the data in each array.
 */

GLfloat cone_vertices[] = {
    1.0, -0.5, 0.0,         /* begin unit circle at y = -0.5 */
    0.707, -0.5, 0.707,
    0.0, -0.5, 1.0,
    -0.707, -0.5, 0.707,
    -1.0, -0.5, 0.0,
    -0.707, -0.5, -0.707,
    0.0, -0.5, -1.0,
    0.707, -0.5, -0.707,    /* end unit circle at y = -0.5 */
    0.0, -0.5, 0.0,         /* center of the base */
    0.0, 0.5, 0.0,          /* top of the cone */
};

GLfloat cone_colors[] = {
    0.0f, 0.0f, 0.5f,
    0.0f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,
};

/*
 * Each set of 3 indices represent the vertices of a triangle.
 * Each triangle faces to the outside of the cone.  The vertices
 * of these forward facing triangles are specified in
 * counterclockwise order.
 */
GLuint cone_indices[] = {
    0, 1, 8,
    1, 2, 8,
    2, 3, 8,
    3, 4, 8,
    4, 5, 8,
    5, 6, 8,
    6, 7, 8,
    7, 0, 8,
    1, 0, 9,
    2, 1, 9,
    3, 2, 9,
    4, 3, 9,
    5, 4, 9,
    6, 5, 9,
    7, 6, 9,
    0, 7, 9,
};
/***********************************************************
 * End Cone Data
 ***********************************************************/

 
 
 /***********************************************************
 * Begin Triangular Prism Data
 ***********************************************************/
	//lines between every 3rd entry to seal the prism
	
GLfloat tri_prism1_vertices[] = 
	{
		//base of prism
		-0.5f, 0.0f, 0.0f, //front left
		0.5f, 0.0f, 0.0f, 	//front right
		0.5f, 0.0f, -0.25f,	//back right
		-0.5f, 0.0f, -0.25f, //back left
		
		
		//front face of prism
		-0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f, 
		0.0f, -1, 0.0f,
		
		//back side of prism as viewed through the front
		0.5f, 0.0f, -0.25f,
		-0.5f, 0.0f, -0.25f,
		0.0f, -1, -0.25f,
		

		//left rectangle face of prism
		-0.5f, 0.0f, -0.25f,
		-0.5f, 0.0f, 0.0f,
		0.0f, -1, 0.0f,
		0.0f, -1, -0.25f,
		
		//right rectangle face of prism
		0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, -0.25f,
		0.0f, -1, -0.25f,
		0.0f, 1, 0.0f,
		
		
		
	};
	
	
	
	GLfloat tri_prism2_vertices[] = 
	{
		//base of prism
		-1.0f, -1, 0.0f, //front left
		0.0f, -1, 0.0f, 	//front right
		0.0f, -1, -0.25f,	//back right
		-1.0f, -1, -0.25f, //back left
		
		
		//front face of prism
		-1.0f, -1, 0.0f, //front left
		0.0f, -1, 0.0f,  //front right
		-0.5f, 0.0f, 0.0f, // front point 
		
		//back side of prism as viewed through the front
		0.0f, -1, -0.25f,	//bottom back right
		-1.0f, -1, -0.25f, //bottom left right
		-0.5f, 0.0f, -0.25f, //back point
		

		//left rectangle face of prism
		-1.0f, -1, -0.25f, //back left	
		-1.0f, -1, 0.0f, //front left
		-0.5f, 0.0f, 0.0f, // front point 
		-0.5f, 0.0f, -0.25f, //back point
		
		//right rectangle face of prism
		0.0f, -1, 0.0f, 	//front right
		0.0f, -1, -0.25f,	//back right
		-0.5f, 0.0f, -0.25f, //back point
		-0.5f, 0.0f, 0.0f, // front point 	
	};
	
	GLfloat tri_prism3_vertices[] = 
	{
		//base of prism
		0.0f, -1, 0.0f, //front left
		1.0f, -1, 0.0f, 	//front right
		1.0f, -1, -0.25f,	//back right
		0.0f, -1, -0.25f, //back left
		
		
		//front face of prism
		0.0f, -1, 0.0f, //front left
		1.0f, -1, 0.0f,  //front right
		0.5f, 0.0f, 0.0f, 	// front point 
		
		//back side of prism as viewed through the front
		1.0f, -1, -0.25f,	//bottom back right
		0.0f, -1, -0.25f, //bottom left right
		0.5f, 0.0f, -0.25f, //back point
		

		//left rectangle face of prism
		0.0f, -1, -0.25f, //back left	
		0.0f, -1, 0.0f, //front left
		0.5f, 0.0f, 0.0f, 	// front point
		0.5f, 0.0f, -0.25f, //back point
		
		//right rectangle face of prism
		1.0f, -1, 0.0f, 	//front right
		1.0f, -1, -0.25f,	//back right
		0.5f, 0.0f, -0.25f, //back point
		0.5f, 0.0f, 0.0f, // front point 	
	};
	
	
 
void init_tri_prisms(void)	
{
	prism_height = sin(PI / 3);
	surround_light = 0;
	change_triforce_colors(0);
	angle = 2 * PI;
	
	tri_prism1_vertices[19] = prism_height;
	tri_prism1_vertices[28] = prism_height;
	tri_prism1_vertices[37] = prism_height;
	tri_prism1_vertices[40] = prism_height;
	tri_prism1_vertices[49] = prism_height;
	tri_prism1_vertices[52] = prism_height;
	
	// tri_prism2_vertices[1] = -prism_height;
	// tri_prism2_vertices[4] = -prism_height;
	// tri_prism2_vertices[7] = -prism_height;
	// tri_prism2_vertices[10] = -prism_height;
	// tri_prism2_vertices[13] = -prism_height;
	// tri_prism2_vertices[16] = -prism_height;	
	// tri_prism2_vertices[22] = -prism_height;
	// tri_prism2_vertices[25] = -prism_height;
	// tri_prism2_vertices[31] = -prism_height;
	// tri_prism2_vertices[34] = -prism_height;
	// tri_prism2_vertices[43] = -prism_height;
	// tri_prism2_vertices[46] = -prism_height;
 
 
	// tri_prism3_vertices[1] = -prism_height;
	// tri_prism3_vertices[4] = -prism_height;
	// tri_prism3_vertices[7] = -prism_height;
	// tri_prism3_vertices[10] = -prism_height;
	// tri_prism3_vertices[13] = -prism_height;
	// tri_prism3_vertices[16] = -prism_height;	
	// tri_prism3_vertices[22] = -prism_height;
	// tri_prism3_vertices[25] = -prism_height;
	// tri_prism3_vertices[31] = -prism_height;
	// tri_prism3_vertices[34] = -prism_height;
	// tri_prism3_vertices[43] = -prism_height;
	// tri_prism3_vertices[46] = -prism_height;
}

void change_triforce_colors(int color_number)
{
	if(color_number == GOLD)
	{
		ambient_red = 	0.24725f;	ambient_green = 0.1995f; 	ambient_blue = 0.0745f;
		diffuse_red = 0.75164f;		diffuse_green =  0.60648f;	diffuse_blue = 0.22648f;
		specular_red = 0.628281f;	specular_green = 0.555802f;	specular_blue =  0.366065f;
		shine = 0.4f;
		triforce_color = GOLD;
	}
	
	if(color_number == RUBY)
	{
		ambient_red = 	0.1745;		ambient_green =  0.01175; 	ambient_blue = 0.01175;
		diffuse_red = 0.61424;		diffuse_green =  0.04136;	diffuse_blue = 0.04136;
		specular_red = 0.727811;	specular_green = 0.626959;	specular_blue =  0.626959;
		shine = 0.6f;
		triforce_color = RUBY;
	}
	
	if(color_number == EMERALD)
	{
		ambient_red = 	0.0215;		ambient_green =  0.1745;	ambient_blue = 0.0215;
		diffuse_red = 0.07568;		diffuse_green =  0.61424;	diffuse_blue = 0.07568;
		specular_red = 0.633;		specular_green = 0.727811;	specular_blue =  0.633;
		shine = 0.6f;
		
		triforce_color = EMERALD;
	}
	
	if(color_number == SAPHIRE)
	{
		ambient_red = 	0.01175;		ambient_green =  0.01175; 	ambient_blue = 0.1745;
		diffuse_red = 0.04136;		diffuse_green =  0.04136;	diffuse_blue = 0.61424;
		specular_red = 0.626959;	specular_green = 0.626959;	specular_blue =  0.727811;
		shine = 0.6f;
		triforce_color = SAPHIRE;
	
	}

	
	mat[0] = ambient_red;	mat[1] = ambient_green;	mat[2] = ambient_blue;	mat[3] = 1.0f;
	
	mat2[0] = diffuse_red;	mat2[1] = diffuse_green; mat2[2] = diffuse_blue ; mat2[3] = 1.0f;

	mat3[0] = specular_red; mat3[1] = specular_green;  mat3[2] = specular_blue;	 mat3[3] = 1.0f;
}
 /***********************************************************
 * End Triangular Prism Data
 ***********************************************************/

/* Uses glut to draw a cube */
void draw_cube_glut(void) {
	/* Draw the cube using glut */

	glColor3f(1.0f, 0.0f, 0.0f);
	if (disp_style == DS_SOLID) {
		glutSolidCube(1.0f);
	} else if (disp_style == DS_WIRE) {
		glutWireCube(1.0f);
	}
}

/*
 * Draws a cube using the data arrays at the top of this file.
 * Iteratively draws each quad in the cube.
 */
void draw_cube_quad(void) {
	int num_indices;
	int i;
	int index1, index2, index3, index4;

	num_indices = sizeof(cube_indices) / sizeof(GLuint);
	

	/*
	 * Loop over all quads that need to be drawn.
	 * Step i by 4 because there are 4 vertices per quad.
	 */
	for (i = 0; i < num_indices; i += 4) {
		/*
		 * Find the index into the vertex array.  The value
		 * in the cube_indices array refers to the index
		 * of the ordered triples, not the index for the
		 * actual GLfloats that comprise the cube_vertices array.
		 * Thus, we need to multiple by 3 to get the real index.
		 */
		index1 = cube_indices[i] * 3;
		index2 = cube_indices[i+1] * 3;
		index3 = cube_indices[i+2] * 3;
		index4 = cube_indices[i+3] * 3;

		glBegin(GL_QUADS);

		/* All arguments here are pointers */
		glColor3fv(  &(cube_colors[index1]) );
		glVertex3fv( &(cube_vertices[index1]) );
		glColor3fv(  &(cube_colors[index2]) );
		glVertex3fv( &(cube_vertices[index2]) );
		glColor3fv(  &(cube_colors[index3]) );
		glVertex3fv( &(cube_vertices[index3]) );
		glColor3fv(  &(cube_colors[index4]) );
		glVertex3fv( &(cube_vertices[index4]) );

		glEnd();

	}
}

/*
 * Draws a cube using the data arrays at the top of this file.
 * Uses GL's vertex arrays, index arrays, color arrays, etc.
 */
void draw_cube_quad_arrays(void) {
	int num_indices;

	num_indices = sizeof(cube_indices) / sizeof(GLuint);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, cube_vertices);
	glColorPointer(3, GL_FLOAT, 0, cube_colors);
	glDrawElements(GL_QUADS, num_indices,
					GL_UNSIGNED_INT, cube_indices);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

/*
 * Uses glut to draw a cone.  Must render in either solid and wire
 * frame modes, based on the value of the variable disp_style.
 */
void draw_cone_glut(void) {
	/* ADD YOUR CODE HERE */
	
	glColor3f(0.0f, 0.0f, 1.0f);
	if (disp_style == DS_SOLID) {
		glutSolidCone(1.0f, 1.0f, 20, 20);
	} else if (disp_style == DS_WIRE) {
		glutWireCone(1.0f, 1.0f, 20, 20);
	}

}

/*
 * Draws a cone using the data arrays at the top of this file.
 * Iteratively draws each triangle in the cone.
 */
void draw_cone_tri(void) {
	/* ADD YOUR CODE HERE */
	
	int num_indices;
	int i;
	int index1, index2, index3;

	num_indices = sizeof(cone_indices) / sizeof(GLuint);

	
	/*
	 * Loop over all triangles that need to be drawn.
	 * Step i by 3 because there are 3 vertices per triangle.
	 */
	//handles the base of the cone
	for (i = 0; i < num_indices ; i += 3) {
		/*
		 * Find the index into the vertex array.  The value
		 * in the cone_indices array refers to the index
		 * of the ordered triples, not the index for the
		 * actual GLfloats that comprise the cone_vertices array.
		 * Thus, we need to multiple by 3 to get the real index.
		 */
		index1 = cone_indices[i] * 3;
		index2 = cone_indices[i+1] * 3;
		index3 = cone_indices[i+2] * 3;

		glBegin(GL_TRIANGLES);

		/* All arguments here are pointers */
		//this vertex should be a 1st base vertex
		glColor3fv(  &(cone_colors[index1]) );
		glVertex3fv( &(cone_vertices[index1]) );
		
		//this vertex should be a second base vertex
		glColor3fv(  &(cone_colors[index2]) );
		glVertex3fv( &(cone_vertices[index2]) );
		
		//this last vertex will be to either the center of the base or to the top of the cone
		glColor3fv(  &(cone_colors[index3]) );
		glVertex3fv( &(cone_vertices[index3]) );

		glEnd();
	}
	
	

}

/*
 * Draws a cone using the data arrays at the top of this file.
 * Uses GL's vertex arrays, index arrays, color arrays, etc.
 */
void draw_cone_tri_arrays(void) 
{
	int num_indices;

	num_indices = sizeof(cone_indices) / sizeof(GLuint);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, cone_vertices);
	glColorPointer(3, GL_FLOAT, 0, cone_colors);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, cone_indices);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

}



/*
 * Draws a cone using a calculated triangulation of the cone surface.
 *
 * Args
 * ----
 * The HEIGHT and RADIUS of the cone.
 *
 * BASE_TRI refers to the number of triangles used to represent
 * the base of the cone.  Each of these triangles should share a
 * common vertex, namely, the center of the base.
 *
 * The final triangulation of the cone surface should include
 * exactly 2 * BASE_TRI.
 */
void draw_cone_tri_calc(double height, double radius, int base_tri) 
{
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//variables
	GLdouble x = radius;
	GLdouble y = -0.5d;
	GLdouble z = 0.0d;
	GLdouble cone_point = height + y;
	GLdouble gl_radius = radius;
	int i;		
	double phi = (2 * PI) / base_tri;
	double theta = phi;

	
	
	
	for(i = 0; i < base_tri; i++)
	{		
		glBegin(GL_TRIANGLES);
		glColor3f(0.0f, 0.0f, 1.0f );
		glVertex3f(0.0d, -0.5d, 0.0d);
	
		z = gl_radius * sin(theta);
		x = gl_radius * cos(theta);
		
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(x, y, z);
		
		theta += phi;
		
		z = gl_radius * sin(theta);
		x = gl_radius * cos(theta);
		
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(x, y, z);
		
		glEnd();
	}
	
		
		
		
		
	theta = 0;
	

	
	for(i = 0; i < base_tri; i++)
	{		
	
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(0.0f, 0.0f, 1.0f );
		glVertex3f(0.0d, cone_point, 0.0d);
		
		z = gl_radius * sin(theta);
		x = gl_radius * cos(theta);
		
		glColor3f(0.0f, 0.0f, 1.0f );
		glVertex3f(x, y, z);
		
		theta += phi;
		
		z = gl_radius * sin(theta);
		x = gl_radius * cos(theta);
		
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(x, y, z);
		
		
		glEnd();
	}
		
			
}




/* Draw the various vrml scenes */
void draw_vrml(void) 
{

	//initilize object to default: the cube or come back to the cube
	if( vr_object == -1 || vr_object == VR_CUBE)
	{
		vr_object = VR_CUBE;
		draw_vrml_cube();		
	}
	
	//change object to DODECAHEDRON
	else if(vr_object == VR_DODECAHEDRON)
	{
		draw_vrml_dodecahedron();
	}
	
	//change object to icosahedron
	else if(vr_object == VR_ICOSAHEDRON)
	{
		draw_vrml_icosahedron();
	}
	
	//change object to pyramid
	else if(vr_object == VR_PYRAMID)
	{
		draw_vrml_pyramid();
	}
	

	if(vrml_changed_value)
		print_vrml_object();

}


/* Draws a freeform scene */
void draw_free_scene(void) {
	GLfloat light_pos[] = {0.0, 0.0, -10.0, 1.0};
	GLfloat light_pos2[] = {0.0, 0.0, 10.0, 1.0};
	if(rotate_triforce)
		rotation_angle += angle;
	else 
		rotation_angle = 0;

	glPushMatrix();
		glRotatef(rotation_angle, 0.0f, 1.0f, 0.0);
		glVertexPointer(3, GL_FLOAT, 0, tri_prism1_vertices);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		
		
		if(use_tri_color)
		{
			change_triforce_colors(RUBY);
		}
			
		


		glLightfv(GL_LIGHT0, GL_AMBIENT, mat);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, mat2);
		glLightfv(GL_LIGHT0, GL_SPECULAR, mat3);
		
		if(surround_light)
		{
			
			glEnable(GL_LIGHT1);
			glLightfv(GL_LIGHT0, GL_POSITION, light_pos2);		
			glLightfv(GL_LIGHT1, GL_AMBIENT, mat);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, mat2);
			glLightfv(GL_LIGHT1, GL_SPECULAR, mat3);
			glLightfv(GL_LIGHT1, GL_POSITION, light_pos);		
		}
		
		glShadeModel (GL_SMOOTH);
		glMaterialfv(GL_FRONT_AND_BACK , GL_AMBIENT, mat);
		glMaterialfv(GL_FRONT_AND_BACK , GL_DIFFUSE, mat2);
		glMaterialfv(GL_FRONT_AND_BACK , GL_SPECULAR, mat3);
		glMaterialf(GL_FRONT_AND_BACK , GL_SHININESS, shine * 128.0);
		glColor3f(0.5f, 0.0f, 0.0f);
		glDrawArrays(GL_QUADS, 0, 4);


		
		glColor3f(0.5f, 0.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 4, 3);
		
		
	
		glColor3f(0.5f, 0.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 7, 3);
	

		glColor3f(0.5f, 0.0f, 0.0f);
		glDrawArrays(GL_QUADS, 10, 4);
	

		glColor3f(0.5f, 0.0f, 0.0f);
		glDrawArrays(GL_QUADS, 14, 4);

		
		// glDisable(GL_LIGHT0);
		// glDisable(GL_LIGHTING);
		// glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();
	
	//bottom left triangle
	glPushMatrix();
			
		glTranslatef(-0.5f, -prism_height, 0.0f);
		glRotatef(rotation_angle, 0.0f, 1.0f, 0.0);
		glVertexPointer(3, GL_FLOAT, 0, tri_prism1_vertices);
		glEnableClientState(GL_VERTEX_ARRAY);
	
	
		if(use_tri_color)
		{
			change_triforce_colors(SAPHIRE);
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glLightfv(GL_LIGHT0, GL_AMBIENT, mat);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, mat2);
			glLightfv(GL_LIGHT0, GL_SPECULAR, mat3);
			glShadeModel (GL_SMOOTH);
			glMaterialfv(GL_FRONT_AND_BACK , GL_AMBIENT, mat);
			glMaterialfv(GL_FRONT_AND_BACK , GL_DIFFUSE, mat2);
			glMaterialfv(GL_FRONT_AND_BACK , GL_SPECULAR, mat3);
			glMaterialf(GL_FRONT_AND_BACK , GL_SHININESS, shine * 128.0);
			if(surround_light)
			{
				glLightfv(GL_LIGHT1, GL_AMBIENT, mat);
				glLightfv(GL_LIGHT1, GL_DIFFUSE, mat2);
				glLightfv(GL_LIGHT1, GL_SPECULAR, mat3);
				glLightfv(GL_LIGHT1, GL_POSITION, light_pos);		
			}
			
		}
	
		glColor3f(0.0f, 0.0f, 0.5f);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	
		glColor3f(0.0f, 0.0f, 0.5f);
		glDrawArrays(GL_TRIANGLES, 4, 3);
	
		glColor3f(0.0f, 0.0f, 0.5f);
		glDrawArrays(GL_TRIANGLES, 7, 3);
	
		glColor3f(0.0f, 0.0f, 0.5f);
		glDrawArrays(GL_TRIANGLE_FAN, 10, 4);
	
		glColor3f(0.0f, 0.0f, 0.5f);
		glDrawArrays(GL_TRIANGLE_FAN, 14, 4);

		
		// glDisable(GL_LIGHT0);
		// glDisable(GL_LIGHTING);
		// glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();
	
	
	glPushMatrix();
		glTranslatef(0.5f, -prism_height, 0.0f);
		glRotatef(rotation_angle, 0.0f, 1.0f, 0.0);
		glVertexPointer(3, GL_FLOAT, 0, tri_prism1_vertices);
		glEnableClientState(GL_VERTEX_ARRAY);
	
	
		if(use_tri_color)
		{
			change_triforce_colors(EMERALD);
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glLightfv(GL_LIGHT0, GL_AMBIENT, mat);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, mat2);
			glLightfv(GL_LIGHT0, GL_SPECULAR, mat3);
			glShadeModel (GL_SMOOTH);
			glMaterialfv(GL_FRONT_AND_BACK , GL_AMBIENT, mat);
			glMaterialfv(GL_FRONT_AND_BACK , GL_DIFFUSE, mat2);
			glMaterialfv(GL_FRONT_AND_BACK , GL_SPECULAR, mat3);
			glMaterialf(GL_FRONT_AND_BACK , GL_SHININESS, shine * 128.0);
			if(surround_light)
			{
				glLightfv(GL_LIGHT1, GL_AMBIENT, mat);
				glLightfv(GL_LIGHT1, GL_DIFFUSE, mat2);
				glLightfv(GL_LIGHT1, GL_SPECULAR, mat3);
				glLightfv(GL_LIGHT1, GL_POSITION, light_pos);		
			}
			triforce_color = TRI;
		}
	
	
	
		glColor3f(0.0f, 0.5f, 0.0f);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	
		glColor3f(0.0f, 0.5f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 4, 3);
	
		glColor3f(0.0f, 0.5f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 7, 3);
	
		glColor3f(0.0f, 0.5f, 0.0f);
		glDrawArrays(GL_TRIANGLE_FAN, 10, 4);
	
		glColor3f(0.0f, 0.5f, 0.0f);
		glDrawArrays(GL_TRIANGLE_FAN, 14, 4);

		
		
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();

}


/* Prints to stdout the current display mode */
void print_disp_mode( void ) {
    switch (disp_mode) {
        case DM_CUBE_GLUT:
            printf("Display Mode: Cube using glut\n");
            break;
        case DM_CUBE_QUAD:
            printf("Display Mode: Cube using quadrilaterals\n");
            break;
        case DM_CUBE_QUAD_ARRAYS:
            printf("Display Mode: Cube using quadrilateral arrays\n");
            break;
        case DM_CONE_GLUT:
            printf("Display Mode: Cone using glut\n");
            break;
        case DM_CONE_TRI:
            printf("Display Mode: Cone using triangles\n");
            break;
        case DM_CONE_TRI_ARRAYS:
            printf("Display Mode: Cone using triangle arrays\n");
            break;
        case DM_CONE_TRI_CALC:
            printf("Display Mode: Cone using calculated triangles\n");
			break;
        case DM_VRML:
            printf("Display Mode: VRML objects\n");
            break;
        case DM_FREE_SCENE:
            printf("Display Mode: Freeform scene\n");
            break;
        default:
			printf("Warning: unknown display mode\n");
            break;
    }
}


/* Prints to stdout the current display style */
void print_disp_style( void ) {
	if (disp_style == DS_SOLID) {
		printf("Display Style: solid (for glut modes only)\n");
	} else if (disp_style == DS_WIRE) {  
		printf("Display Style: wire (for glut modes only)\n");
	} else {
		printf("Warning: unknown display style\n");
	}
}

/* end of drawing.c */

