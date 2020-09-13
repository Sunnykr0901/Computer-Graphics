/*
Name-Sunny Kumar
Roll No-170101068
*/
#include<bits/stdc++.h>
#include<GL/glut.h>
#include<stdio.h>
using namespace std;

float angle=0.0;						//calculate angle to rotate
vector<vector<double>>vertices;			//store vertices
vector<vector<int>>faces;				//store faces as indices of three vertices of a triangle

//load all vertices and faces
void objloader(){
	string line;
	ifstream readfile("lowpolybunny.obj");
	while(getline(readfile,line)){
		istringstream ss(line);
		vector<int>t1;
		vector<double>t2;
		string flag;
		ss>>flag;
		while(ss){
			double word;
			ss>>word;
			if(flag=="v")t2.push_back(word);
			else if(flag=="f")t1.push_back(((int)word)-1);
		}
		if(flag=="v")vertices.push_back(t2);
		else if(flag=="f")faces.push_back(t1);
	}
}

// main display function
void draw(){

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glPushMatrix();
	glScalef(4,4,4);
	glRotatef(angle,0,1,0);
	glColor3f(0.8,0.8,0.8);
	glBegin(GL_TRIANGLES);
	for(int i=0;i<faces.size();i++){
		vector<double>v=vertices[faces[i][0]];
		glVertex3f(v[0],v[1],v[2]);
		v=vertices[faces[i][1]];
		glVertex3f(v[0],v[1],v[2]);
		v=vertices[faces[i][2]];
		glVertex3f(v[0],v[1],v[2]);
	}
	glEnd();

	glLineWidth(1.0);
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINES);
	for(int i=0;i<faces.size();i++){
		vector<double>v=vertices[faces[i][0]];
		glVertex3f(v[0],v[1],v[2]);
		v=vertices[faces[i][1]];
		glVertex3f(v[0],v[1],v[2]);
		v=vertices[faces[i][1]];
		glVertex3f(v[0],v[1],v[2]);
		v=vertices[faces[i][2]];
		glVertex3f(v[0],v[1],v[2]);
		v=vertices[faces[i][2]];
		glVertex3f(v[0],v[1],v[2]);
		v=vertices[faces[i][0]];
		glVertex3f(v[0],v[1],v[2]);
	}
	glEnd();
	glPopMatrix();
	glFlush();
}

void resize(int w, int h){

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

//updating the rotating angle
void update(int value){
	angle+=1.0f;
	if(angle>360.0f)angle-=360;
	glutPostRedisplay();
	glutTimerFunc(25,update,0);
}

int main(int argc,char** argv){
	objloader();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE|GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("3D Rotating Mesh");
	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(draw);
	glutReshapeFunc(resize);
	glutTimerFunc(25,update,0);
	glutMainLoop();
	return 0;
}