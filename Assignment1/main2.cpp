/*
Name-Sunny Kumar
Roll No-170101068
*/
#include<bits/stdc++.h>
#include<GL/glut.h>
#include<stdio.h>

using namespace std;

bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;
float cameraAngleX = 0;
float cameraAngleY = 0;

vector<vector<double>>vertices;
vector<vector<int>>faces;

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

void draw(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glScalef(4,4,4);
	glRotatef(cameraAngleX, 1, 0, 0);   
	glRotatef(cameraAngleY, 0, 1, 0);  

	/*Drawing figures */
	glBegin(GL_TRIANGLES);
	for(int i=0;i<faces.size();i++){
		vector<double>v=vertices[faces[i][0]];
		glColor3f(1.0, 0.0, 0.0);glVertex3f(v[0],v[1],v[2]);
		v=vertices[faces[i][1]];
		glColor3f(0.0, 1.0, 0.0);glVertex3f(v[0],v[1],v[2]);
		v=vertices[faces[i][2]];
		glColor3f(0.0, 0.0, 1.0);glVertex3f(v[0],v[1],v[2]);
	}
	glEnd();
	glFlush();
}

void resize(int w, int h){

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void mouseCB(int button, int state, int x, int y){

	mouseX = x;
	mouseY = y;
	if (button == GLUT_LEFT_BUTTON){
		if(state==GLUT_DOWN)mouseLeftDown=true;
		else if(state==GLUT_UP)mouseLeftDown=false;
	}
	else if(button==GLUT_RIGHT_BUTTON){
		if(state==GLUT_DOWN)mouseRightDown=true;
		else if(state==GLUT_UP)mouseRightDown=false;
	}
}

void mouseMotionCB(int x, int y){

	if (mouseLeftDown || mouseRightDown){
		cameraAngleY += (x - mouseX);
		cameraAngleX += (y - mouseY);
		mouseX = x;
		mouseY = y;
	}
	glutPostRedisplay();
}

void initial(){
	mouseLeftDown = mouseRightDown = false;
}

int main(int argc,char** argv){
	objloader();
	initial();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE|GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("3D MESH");
	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(draw);
	glutReshapeFunc(resize);
	glutMouseFunc(mouseCB);
	glutMotionFunc(mouseMotionCB);
	glutMainLoop();
	return 0;
}