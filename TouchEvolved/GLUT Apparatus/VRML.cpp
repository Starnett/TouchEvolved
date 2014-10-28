/*
 * vrml.c
 * ------
 * Drawing subroutines for each vrml object.
 * Also contains the polygon data for each vrml object.
 *
 * Starter code for Project 1.
 *
 * Group Members: <FILL IN>
 */

#include "common.h"

#include <stdio.h>

#include "vrml.h"

/*
 * Note that in VRML, the face indices have a -1 to represent
 * the termination of a index sequence.
 */

/***********************************************************
 * Begin VRML Cube Data
 ***********************************************************/
GLfloat v_cube_vertices[] = {
	1.632990, 0.000000, 1.154700,
	0.000000, 1.632990, 1.154700,
	-1.632990, 0.000000, 1.154700,
	0.000000, -1.632990, 1.154700,
	0.000000, -1.632990, -1.154700,
	1.632990, 0.000000, -1.154700,
	0.000000, 1.632990, -1.154700,
	-1.632990, 0.000000, -1.154700,
};

GLint v_cube_indices[] = {
	0, 1, 2, 3, -1,
	4, 5, 0, 3, -1,
	5, 6, 1, 0, -1,
	6, 7, 2, 1, -1,
	3, 2, 7, 4, -1,
	7, 6, 5, 4, -1,
};


/***********************************************************
 * Begin VRML Dodecahedron Data
 ***********************************************************/
GLfloat v_dodeca_vertices[] = {
	1.214120, 0.000000, 1.589310,
	0.375185, 1.154700, 1.589310,
	-0.982247, 0.713644, 1.589310,
	-0.982247, -0.713644, 1.589310,
	0.375185, -1.154700, 1.589310,
	1.964490, 0.000000, 0.375185,
	1.589310, 1.154700, -0.375185,
	0.607062, 1.868350, 0.375185,
	-0.607062, 1.868350, -0.375185,
	-1.589310, 1.154700, 0.375185,
	-1.964490, 0.000000, -0.375185,
	-1.589310, -1.154700, 0.375185,
	-0.607062, -1.868350, -0.375185,
	0.607062, -1.868350, 0.375185,
	1.589310, -1.154700, -0.375185,
	0.982247, 0.713644, -1.589310,
	0.982247, -0.713644, -1.589310,
	-0.375185, 1.154700, -1.589310,
	-1.214120, 0.000000, -1.589310,
	-0.375185, -1.154700, -1.589310,
};

GLint v_dodeca_indices[] = {
	0, 1, 2, 3, 4, -1,
	0, 5, 6, 7, 1, -1,
	1, 7, 8, 9, 2, -1,
	2, 9, 10, 11, 3, -1,
	3, 11, 12, 13, 4, -1,
	4, 13, 14, 5, 0, -1,
	15, 6, 5, 14, 16, -1,
	17, 8, 7, 6, 15, -1,
	18, 10, 9, 8, 17, -1,
	19, 12, 11, 10, 18, -1,
	16, 14, 13, 12, 19, -1,
	16, 19, 18, 17, 15, -1,
};


/***********************************************************
 * Begin VRML Icosahedron Data
 ***********************************************************/
GLfloat v_icosa_vertices[] = {
	0.552786, 1.701300, 0.894427,
	0.000000, 0.000000, 2.000000,
	1.788850, 0.000000, 0.894427,
	-1.447210, 1.051460, 0.894427,
	-1.447210, -1.051460, 0.894427,
	0.552786, -1.701300, 0.894427,
	1.447210, 1.051460, -0.894427,
	-0.552786, 1.701300, -0.894427,
	-1.788850, 0.000000, -0.894427,
	-0.552786, -1.701300, -0.894427,
	1.447210, -1.051460, -0.894427,
	0.000000, 0.000000, -2.000000,
};

GLint v_icosa_indices[] = {
	0, 1, 2, -1,
	3, 1, 0, -1,
	4, 1, 3, -1,
	5, 1, 4, -1,
	2, 1, 5, -1,
	0, 2, 6, -1,
	7, 0, 6, -1,
	3, 0, 7, -1,
	8, 3, 7, -1,
	4, 3, 8, -1,
	9, 4, 8, -1,
	5, 4, 9, -1,
	10, 5, 9, -1,
	6, 2, 10, -1,
	2, 5, 10, -1,
	6, 11, 7, -1,
	7, 11, 8, -1,
	8, 11, 9, -1,
	9, 11, 10, -1,
	10, 11, 6, -1,
};


/***********************************************************
 * Begin VRML Pyramid Data
 ***********************************************************/
GLfloat v_pyramid_vertices[] = {
	0.997029, 0.000000, -0.997029,
	0.012175, 1.000000, -0.012175,
	-0.997029, 0.000000, -0.997029,
	-0.012175, 1.000000, -0.012175,
	-0.997029, 0.000000, 0.997029,
	-0.012175, 1.000000, 0.012175,
	0.997029, 0.000000, 0.997029,
	0.012175, 1.000000, 0.012175,
};

GLint v_pyramid_indices[] = {
	6, 0, 7, -1,
	7, 0, 1, -1,
	0, 2, 1, -1,
	1, 2, 3, -1,
	2, 4, 3, -1,
	3, 4, 5, -1,
	4, 6, 5, -1,
	5, 6, 7, -1,
	4, 0, 6, -1,
	4, 2, 0, -1,
	7, 1, 5, -1,
	1, 3, 5, -1,
};


/*
 * ADD YOUR FUNCTION DEFINITIONS HERE.
 * See vrml.h for a list of suggested functions.
 */
 
 /* Function Declarations */
void draw_vrml_cube(void)
{

	int num_faces = 6;
	int i;
	int index = 0;
	int set1, set2, set3, set4;
	
	for(i = 0; i < num_faces; i++, index+=5)
	{	
		set1 = v_cube_indices[index] * 3;
		set2 = v_cube_indices[index+1] * 3;
		set3 = v_cube_indices[index+2] * 3;
		set4 = v_cube_indices[index+3] * 3;
		

		glBegin(GL_LINE_LOOP);
		
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3fv( &(v_cube_vertices[set1]) );
		
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3fv( &(v_cube_vertices[set2]) );
		
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3fv( &(v_cube_vertices[set3]) );
		
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3fv( &(v_cube_vertices[set4]) );
		
		glEnd();
	
	}


}




void draw_vrml_dodecahedron(void)
{
	GLint indices[5];
	int num_faces = 12;
	int i;
	int index = 0;
	
	for(i = 0; i < num_faces; i++, index+=6)
	{	
		indices[0] = v_dodeca_indices[index] * 3;
		indices[1] = v_dodeca_indices[index+1] * 3;
		indices[2] = v_dodeca_indices[index+2] * 3;
		indices[3] = v_dodeca_indices[index+3] * 3;
		indices[4] = v_dodeca_indices[index+4] * 3;
		
		
		glBegin(GL_LINE_LOOP);
		
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3fv( &(v_dodeca_vertices[indices[0]]) );
		
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3fv( &(v_dodeca_vertices[indices[1]]) );
		
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3fv( &(v_dodeca_vertices[indices[2]]) );
		
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3fv( &(v_dodeca_vertices[indices[3]]) );
		
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3fv( &(v_dodeca_vertices[indices[4]]) );
		
		glEnd();
	
	}



}




void draw_vrml_icosahedron(void)
{
	GLint indices[3];
	int num_faces = 20;
	int i;
	int index = 0;
	
	for(i = 0; i < num_faces; i++, index+=4)
	{	
		indices[0] = v_icosa_indices[index] * 3;
		indices[1] = v_icosa_indices[index+1] * 3;
		indices[2] = v_icosa_indices[index+2] * 3;

		
		
		glBegin(GL_LINE_LOOP);
		
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3fv( &(v_icosa_vertices[indices[0]]) );
		
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3fv( &(v_icosa_vertices[indices[1]]) );
		
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3fv( &(v_icosa_vertices[indices[2]]) );


		
		glEnd();
	
	}

}




void draw_vrml_pyramid(void)
{
	GLint indices[3];
	int num_faces = 12;
	int i;
	int index = 0;
	
	for(i = 0; i < num_faces; i++, index+=4)
	{	
		indices[0] = v_pyramid_indices[index] * 3;
		indices[1] = v_pyramid_indices[index+1] * 3;
		indices[2] = v_pyramid_indices[index+2] * 3;

		
		
		glBegin(GL_LINE_LOOP);
		
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3fv( &(v_pyramid_vertices[indices[0]]) );
		
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3fv( &(v_pyramid_vertices[indices[1]]) );
		
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3fv( &(v_pyramid_vertices[indices[2]]) );

		
		glEnd();
	
	}

}




/* Print the current vrml object.  Used when the user selects a new object. */
void print_vrml_object(void)
{

	if(vr_object == VR_CUBE)
	{
		printf("Current VRML object: CUBE\n");
	}
	

	else if(vr_object == VR_DODECAHEDRON)
	{
		printf("Current VRML object: DODECAHEDRON\n");
	}
	

	else if(vr_object == VR_ICOSAHEDRON)
	{
		printf("Current VRML object: ICOSAHEDRON\n");
	}
	
	if( vr_object == VR_PYRAMID)
	{
		printf("Current VRML object: PYRAMID\n");	
	}


}


/* end of vrml.c */

