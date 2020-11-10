#include<bits/stdc++.h>
#include<stdio.h>
#include<stdlib.h>
#include<GL/gl.h>
#include<GL/glut.h>
using namespace std;

void display(double radius, int overSample, int numStrips, int numQuads, float radiusRatio)
{
GLMaterial test;
GLuint surf;
GLuint* surface;
Images::RGBImage surfaceImage;

surfaceImage=Images::readImageFile("/home/rodrtu/Desktop/SolarSystem/EarthTopography.png",Vrui::openFile("/home/rodrtu/Desktop/SolarSystem/EarthTopography.png"));

glEnable(GL_TEXTURE_2D);

//glGenTextures(1, surface);

glBindTexture(GL_TEXTURE_2D, surf);

glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

surfaceImage.glTexImage2D(GL_TEXTURE_2D,0,GL_RGB);

double b = radius * 0.9966463577;//Geoid's semi-minor axis
double e2 = 0.0067; //Geoid's squared eccentricity, derived from flattening factor

float texY1=float(0)/float(numStrips);
double lat1=(Math::Constants<double>::pi*double(0))/double(numStrips)-0.5*Math::Constants<double>::pi;
double s1=Math::sin(lat1);
double c1=Math::cos(lat1);
double chi1=Math::sqrt(1.0-e2*s1*s1);
double xy1=radius*radiusRatio/chi1*c1;
double z1=radius*(1.0-e2)/chi1*s1;

//* Draw latitude quad strips: *
for(int i=1;i<numStrips+1;++i)
    {
    float texY0=texY1;
    double s0=s1;
    double c0=c1;
    double xy0=xy1;
    double z0=z1;
    texY1=float(i)/float(numStrips);
    lat1=(Math::Constants<double>::pi*double(i))/double(numStrips)-0.5*Math::Constants<double>::pi;
    s1=Math::sin(lat1);
    c1=Math::cos(lat1);
    chi1=Math::sqrt(1.0-e2*s1*s1);
    xy1=radius/chi1*c1;
    z1=radius*(1.0-e2)/chi1*s1;

    glBegin(GL_QUAD_STRIP);
    for(int j=0;j<=numQuads;++j)
    {
    float texX=float(j)/float(numQuads)+0.5f;
    double lng=(2.0*Math::Constants<double>::pi*double(j))/double(numQuads);
    double cl=Math::cos(lng)*radiusRatio;
    double sl=Math::sin(lng)*radiusRatio;

    glTexCoord2f(texX,texY1);
    glNormal3d(c1*cl,c1*sl,s1);
    glVertex3d(xy1*cl,xy1*sl,z1);

    glTexCoord2f(texX,texY0);
    glNormal3d(c0*cl,c0*sl,s0);
    glVertex3d(xy0*cl,xy0*sl,z0);
    }
    glEnd();
    }
    //glEndList();
}

int main(int argc,char** argv){
	read_object_file();
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(2000,1000);
	glutCreateWindow("3D Mesh");
	glutDisplayFunc(display);
	glutMainLoop();
}