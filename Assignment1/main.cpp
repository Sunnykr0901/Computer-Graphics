#include<bits/stdc++.h>
#include<stdio.h>
#include<stdlib.h>
#include<GL/gl.h>
#include<GL/glut.h>
using namespace std;

vector<vector<double>>vertices;
vector<vector<int>>faces;

void read_object_file(){
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

void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	glScalef(2,2,2);
	glColor3d(1.0, 1.0, 1.0);
	glBegin(GL_TRIANGLES);
	for(int i=0;i<faces.size();i++){
		for(int j=0;j<3;j++){
			vector<double>v=vertices[faces[i][j]];
			glVertex3d(v[0],v[1],v[2]);
		}
	}
	glEnd();
	glFlush();
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