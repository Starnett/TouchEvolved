#ifndef _DRAWING_H_
#define _DRAWING_H_

/* All variables / contants declared in this file are global. */

extern double r;
extern double h;
extern int n;

//globals for personalized scene
extern int vrml_changed_value;
extern int init_others;
extern GLfloat rotation_angle;
extern int prism_index, prism_index2, prism_index3;
extern GLfloat prism_height;
extern GLfloat full_rotation;
extern int surround_light;
extern GLfloat ambient_red, ambient_green, ambient_blue, diffuse_red, diffuse_green, diffuse_blue, specular_red, specular_green, specular_blue, shine;
extern GLfloat mat[4];
extern GLfloat mat2[4];
extern GLfloat mat3[4];
extern int triforce_color;
extern int used_free_scene;
extern GLfloat angle;
extern int use_tri_color;
#define GOLD 0
#define RUBY 1
#define EMERALD 2
#define SAPHIRE 3
#define TRI 4

extern int rotate_triforce;



/*
 * Different drawing modes.  The drawing modes should begin at 0.
 * and should be consecutive integers.
 * 
 * DM_MAX should be the total number of drawing modes.
 */
#define DM_CUBE_GLUT            0
#define DM_CUBE_QUAD            1
#define DM_CUBE_QUAD_ARRAYS     2
#define DM_CONE_GLUT            3
#define DM_CONE_TRI             4
#define DM_CONE_TRI_ARRAYS      5
#define DM_CONE_TRI_CALC        6
#define DM_VRML                 7
#define DM_FREE_SCENE           8
#define DM_MAX                  8

/* The current display mode */
extern int disp_mode;


/* Styles of drawing glut objects, either solid or wire-frame */
#define DS_SOLID        0
#define DS_WIRE         1

/* The current display style */
extern int disp_style;


/* Function Declarations */
extern void draw_cube_glut(void);
extern void draw_cube_quad(void);
extern void draw_cube_quad_arrays(void);
extern void draw_cone_glut(void);
extern void draw_cone_tri(void);
extern void draw_cone_tri_arrays(void);
extern void draw_cone_tri_calc(double height, double radius, int base_tri);
extern void draw_vrml(void);
extern void draw_free_scene(void);
extern void print_disp_mode(void);
extern void print_disp_style(void);


//function to change scene colors
extern void init_tri_prisms(void);
extern void change_triforce_colors(int);


#endif	/* _DRAWING_H_ */

