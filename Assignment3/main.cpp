#include <GL/glut.h>
    GLfloat plane[4];
    GLfloat v0[3], v1[3], v2[3];
 

/*
** Create a single component texture map
*/
GLfloat *make_texture(int maxs, int maxt)
{
    int s, t;
    static GLfloat *texture;
    texture = (GLfloat *)malloc(maxs * maxt * sizeof(GLfloat));
    for(t = 0; t < maxt; t++) {
    for(s = 0; s < maxs; s++) {
        texture[s + maxs * t] = ((s >> 4) & 0x1) ^ ((t >> 4) & 0x1);
    }
    }
    return texture;
}
enum {SPHERE1 = 1,SPHERE2=2,SPHERE3=3, LIGHT, LEFTWALL, FLOOR};
enum {X, Y, Z, W};
enum {A, B, C, D};
/* create a matrix that will project the desired shadow */
void
shadowmatrix(GLfloat shadowMat[4][4],
         GLfloat groundplane[4],
         GLfloat lightpos[4])
{
  GLfloat dot;
  /* find dot product between light position vector and ground plane normal */
  dot = groundplane[X] * lightpos[X] +
        groundplane[Y] * lightpos[Y] +
        groundplane[Z] * lightpos[Z] +
        groundplane[W] * lightpos[W];
  shadowMat[0][0] = dot - lightpos[X] * groundplane[X];
  shadowMat[1][0] = 0.f - lightpos[X] * groundplane[Y];
  shadowMat[2][0] = 0.f - lightpos[X] * groundplane[Z];
  shadowMat[3][0] = 0.f - lightpos[X] * groundplane[W];
  shadowMat[X][1] = 0.f - lightpos[Y] * groundplane[X];
  shadowMat[1][1] = dot - lightpos[Y] * groundplane[Y];
  shadowMat[2][1] = 0.f - lightpos[Y] * groundplane[Z];
  shadowMat[3][1] = 0.f - lightpos[Y] * groundplane[W];
  shadowMat[X][2] = 0.f - lightpos[Z] * groundplane[X];
  shadowMat[1][2] = 0.f - lightpos[Z] * groundplane[Y];
  shadowMat[2][2] = dot - lightpos[Z] * groundplane[Z];
  shadowMat[3][2] = 0.f - lightpos[Z] * groundplane[W];
  shadowMat[X][3] = 0.f - lightpos[W] * groundplane[X];
  shadowMat[1][3] = 0.f - lightpos[W] * groundplane[Y];
  shadowMat[2][3] = 0.f - lightpos[W] * groundplane[Z];
  shadowMat[3][3] = dot - lightpos[W] * groundplane[W];
}
/* find the plane equation given 3 points */
void
findplane(GLfloat plane[4],
      GLfloat v0[3], GLfloat v1[3], GLfloat v2[3])
{
  GLfloat vec0[3], vec1[3];
  /* need 2 vectors to find cross product */
  vec0[X] = v1[X] - v0[X];
  vec0[Y] = v1[Y] - v0[Y];
  vec0[Z] = v1[Z] - v0[Z];
  vec1[X] = v2[X] - v0[X];
  vec1[Y] = v2[Y] - v0[Y];
  vec1[Z] = v2[Z] - v0[Z];
  
  /* find cross product to get A, B, and C of plane equation */
  plane[A] = vec0[Y] * vec1[Z] - vec0[Z] * vec1[Y];
  plane[B] = -(vec0[X] * vec1[Z] - vec0[Z] * vec1[X]);
  plane[C] = vec0[X] * vec1[Y] - vec0[Y] * vec1[X];
  plane[D] = -(plane[A] * v0[X] + plane[B] * v0[Y] + plane[C] * v0[Z]);
}
void sphere(void)
{
    glPushMatrix();
    glTranslatef(50.f, -50.f, -380.f);
    glCallList(SPHERE1);
    glPopMatrix();
}

void sphere2(void)
{
    glPushMatrix();
    glTranslatef(-40.f, -50.f, -380.f);
    glCallList(SPHERE2);
    glPopMatrix();
}

void sphere3(void)
{
    glPushMatrix();
    glTranslatef(0.f, -50.f, -380.f);
    glCallList(SPHERE3);
    glPopMatrix();
}

enum {NONE, SHADOW};
int rendermode = NONE;
void
menu(int mode)
{
  rendermode = mode;
  glutPostRedisplay();
}
GLfloat leftwallshadow[4][4];
GLfloat floorshadow[4][4];
GLfloat GLlightx = 50;
GLfloat lightpos[] = {50.f, 50.f, -320.f, 1.f};
void
redraw(void)
{
   /* material properties for objects in scene */
    static GLfloat wall_mat[] = {1, 1, 1, 1};
    static GLfloat sphere_mat[] = {0.75164, 0.60648, 0.22648, 1.0};
    static GLfloat cone_mat[] = {0.628281, 0.555802, 0.366065, 1.0};
    static GLfloat sphere3_mat[] = {50.0 };
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    /*
    ** Note: wall verticies are ordered so they are all front facing
    ** this lets me do back face culling to speed things up.
    */
 
    glMaterialfv(GL_FRONT, GL_AMBIENT, wall_mat);
        /* floor */
    /* make the floor textured */
    glEnable(GL_TEXTURE_2D);
    /*
    ** Since we want to turn texturing on for floor only, we have to
    ** make floor a separate glBegin()/glEnd() sequence. You can't
    ** turn texturing on and off between begin and end calls
    */
    glBegin(GL_QUADS);
    glNormal3f(0.f, 1.f, 0.f);
    glTexCoord2i(0, 0);
    glVertex3f(-100.f, -100.f, -320.f);
    glTexCoord2i(1, 0);
    glVertex3f( 100.f, -100.f, -320.f);
    glTexCoord2i(1, 1);
    glVertex3f( 100.f, -100.f, -520.f);
    glTexCoord2i(0, 1);
    glVertex3f(-100.f, -100.f, -520.f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    if(rendermode == SHADOW) {
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_LIGHTING);
      glColor3f(0.f, 0.f, 0.f); /* shadow color */
      glPushMatrix();
      glMultMatrixf((GLfloat *)floorshadow);
      sphere2();
      glPopMatrix();

      glPushMatrix();
      glMultMatrixf((GLfloat *)floorshadow);
      sphere3();
      glPopMatrix();

      glPushMatrix();
      glMultMatrixf((GLfloat *)floorshadow);
      sphere();
      glPopMatrix();
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_LIGHTING);
    }
    /* walls */
    if(rendermode == SHADOW) {
      glEnable(GL_STENCIL_TEST);
      glStencilFunc(GL_ALWAYS, 1, 0);
      glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    }
    glBegin(GL_QUADS);
    /* left wall */
    glNormal3f(1.f, 0.f, 0.f);
    glVertex3f(-100.f, -100.f, -320.f);
    glVertex3f(-100.f, -100.f, -520.f);
    glVertex3f(-100.f,  100.f, -520.f);
    glVertex3f(-100.f,  100.f, -320.f);
    glEnd();
    if(rendermode == SHADOW) {
      glStencilFunc(GL_EQUAL, 1, 1);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_LIGHTING);
      glColor3f(0.f, 0.f, 0.f); /* shadow color */
      glDisable(GL_DEPTH_TEST);
      glPushMatrix();
      glMultMatrixf((GLfloat *)leftwallshadow);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, cone_mat);
      sphere2();
      glPopMatrix();
      glEnable(GL_DEPTH_TEST);
      glDisable(GL_STENCIL_TEST);
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_LIGHTING);
    }
    
    glBegin(GL_QUADS);
    /* right wall */
    glNormal3f(-1.f, 0.f, 0.f);
    glVertex3f( 100.f, -100.f, -320.f);
    glVertex3f( 100.f,  100.f, -320.f);
    glVertex3f( 100.f,  100.f, -520.f);
    glVertex3f( 100.f, -100.f, -520.f);
    /* ceiling */
    glNormal3f(0.f, -1.f, 0.f);
    glVertex3f(-100.f,  100.f, -320.f);
    glVertex3f(-100.f,  100.f, -520.f);
    glVertex3f( 100.f,  100.f, -520.f);
    glVertex3f( 100.f,  100.f, -320.f);
    /* back wall */
    glNormal3f(0.f, 0.f, 1.f);
    glVertex3f(-100.f, -100.f, -520.f);
    glVertex3f( 100.f, -100.f, -520.f);
    glVertex3f( 100.f,  100.f, -520.f);
    glVertex3f(-100.f,  100.f, -520.f);
    glEnd();
    glPushMatrix();
    glTranslatef(lightpos[X], lightpos[Y], lightpos[Z]);
    glDisable(GL_LIGHTING);
    glColor3f(.5f, .5f, .5f);
    glCallList(LIGHT);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_DIFFUSE, sphere_mat);
    sphere();

    glMaterialfv(GL_FRONT, GL_SPECULAR, cone_mat);
    sphere2();

    glMaterialfv(GL_FRONT, GL_SHININESS, sphere3_mat);
    sphere3();

    glutSwapBuffers(); /* high end machines may need this */
}
void key(unsigned char key, int x, int y)
{
    if(key == '\033')
    exit(0);
}

const int TEXDIM = 256;
/* Parse arguments, and set up interface between OpenGL and window system */
int main(int argc, char *argv[])
{
    GLfloat *tex;
    GLUquadricObj *sphere,*sphere2,*sphere3;
    GLfloat plane[4];
    GLfloat v0[3], v1[3], v2[3];
    glutInit(&argc, argv);
    glutInitWindowSize(512, 512);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL|GLUT_DOUBLE);
    (void)glutCreateWindow("projection shadows");
    glutDisplayFunc(redraw);
    glutKeyboardFunc(key);
    glutCreateMenu(menu);
    glutAddMenuEntry("No Shadows", NONE);
    glutAddMenuEntry("Shadows", SHADOW);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    /* draw a perspective scene */
    glMatrixMode(GL_PROJECTION);
    glFrustum(-100., 100., -100., 100., 320., 640.); 
    glMatrixMode(GL_MODELVIEW);
    /* turn on features */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    /* make shadow matricies */
      /* 3 points on floor */
      v0[X] = -100.f; v0[Y] = -100.f; v0[Z] = -320.f;
      v1[X] =  100.f; v1[Y] = -100.f; v1[Z] = -320.f;
      v2[X] =  100.f; v2[Y] = -100.f; v2[Z] = -520.f;
      
      findplane(plane, v0, v1, v2);
      shadowmatrix(floorshadow, plane, lightpos);
      /* 3 points on left wall */
      v0[X] = -100.f; v0[Y] = -100.f; v0[Z] = -320.f;
      v1[X] = -100.f; v1[Y] = -100.f; v1[Z] = -520.f;
      v2[X] = -100.f; v2[Y] =  100.f; v2[Z] = -520.f;
      findplane(plane, v0, v1, v2);
      shadowmatrix(leftwallshadow, plane, lightpos);
    /* place light 0 in the right place */
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    /* remove back faces to speed things up */
    glCullFace(GL_BACK);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    /* make display lists for sphere and cone; for efficiency */
    glNewList(SPHERE1, GL_COMPILE);
    sphere = gluNewQuadric();
    gluSphere(sphere, 20.f, 20, 20);
    gluDeleteQuadric(sphere);
    glEndList();
    glNewList(LIGHT, GL_COMPILE);
    sphere = gluNewQuadric();
    gluSphere(sphere, 5.f, 20, 20);
    gluDeleteQuadric(sphere);
    glEndList();

    glNewList(SPHERE2, GL_COMPILE);
    sphere2 = gluNewQuadric();
    gluSphere(sphere2, 20.f, 20, 20);
    gluDeleteQuadric(sphere2);
    glEndList();
    glNewList(LIGHT, GL_COMPILE);
    sphere2 = gluNewQuadric();
    gluSphere(sphere2, 5.f, 20, 20);
    gluDeleteQuadric(sphere2);
    glEndList();

    glNewList(SPHERE3, GL_COMPILE);
    sphere3 = gluNewQuadric();
    gluSphere(sphere3, 20.f, 20, 20);
    gluDeleteQuadric(sphere3);
    glEndList();
    glNewList(LIGHT, GL_COMPILE);
    sphere3 = gluNewQuadric();
    gluSphere(sphere3, 5.f, 20, 20);
    gluDeleteQuadric(sphere3);
    glEndList();
    
    glNewList(FLOOR, GL_COMPILE);
    glEndList();
    glNewList(LEFTWALL, GL_COMPILE);
    glEndList();
    /* load pattern for current 2d texture */
    tex = make_texture(TEXDIM, TEXDIM);
    glTexImage2D(GL_TEXTURE_2D, 0, 1, TEXDIM, TEXDIM, 0, GL_RED, GL_FLOAT, tex);
    free(tex);
    glutMainLoop();
}