#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "LEF_class.h"
#include "DEF_class.h"
#include "VER_class.h"
#include "LEF_reading.h"
#include "DEF_reading.h"
#include "VER_reading.h"
#include "glut.h"


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")



double w1 = 800.0, h1 = 800.0;
float vx = 0.0, vy = 0.0, vz = 0.0;
double Zoom = 100.0;
double a, b;


void RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, w1, h1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, w1 / h1, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(a, b, -Zoom);
	glRotatef(vx, 1, 0, 0);
	glRotatef(vy, 0, 1, 0);
	glRotatef(vz, 0, 0, 1);
    // Начало нашего кода

	//draw cristall
	glBegin(GL_LINE_LOOP);
	glColor3f(1, 0, 0);
	glVertex3f(def.x_1, def.y_1, 3);
	glVertex3f(def.x_2, def.y_1, 3);
	glVertex3f(def.x_2, def.y_2, 3);
	glVertex3f(def.x_1, def.y_2, 3);
	glEnd();

	//draw components

	for (auto component : def.components) {								//read component in DEF
		double x_1 = component->x_position;								//position vertex x1,y1
		double y_1 = component->y_position;
		double x_2 = component->y_position;
		double y_2 = component->y_position;
		double z = 3;
		std::string  name_in_Macro = component->name_model_in_LEF;

		for (auto macro_element : lef.macroes) {						//open MACRO
			if (macro_element->name == name_in_Macro)
			{
				x_2 += macro_element->size_x;							//position vertex x2,y2
				y_2 += macro_element->size_y;

				double mass_koord[4];
				
				for (auto pin : macro_element->pins) {					//open PIN
					for (auto polygon : pin->polygons) {				//open polygon
						int i = 0;
						for (auto position : polygon->position) {		//open position
							mass_koord[i] = position;					//read position
							++i;
						}
						glBegin(GL_POLYGON);							//draw pins (rects or rolygons)
						glColor3f(0.5, 0.4, 0);
						glVertex3f(mass_koord[0] + x_1, mass_koord[1] + y_1, z);		
						glVertex3f(mass_koord[2] + x_1, mass_koord[1] + y_1, z);
						glVertex3f(mass_koord[2] + x_1, mass_koord[3] + y_1, z);
						glVertex3f(mass_koord[0] + x_1, mass_koord[3] + y_1, z);
						glEnd();
					}
				}
				
				break;
			}
		}
		glBegin(GL_QUADS);												//draw component
		glColor3f(0, 1, 0);
		glVertex3f(x_1, y_1, z);
		glVertex3f(x_2, y_1, z);
		glVertex3f(x_2, y_2, z);
		glVertex3f(x_1, y_2, z);
		glEnd();
	}
    // Конец нашего кода
    glutSwapBuffers();
}





void Read_kb(unsigned char key, int, int)				// zoom and control
{
	if (key == 's')
		vx -= 2.0;
	if (key == 'w')
		vx += 2.0;
	if (key == 'a')
		vy -= 2.0;
	if (key == 'd')
		vy += 2.0;
	if (key == '1')
		vz -= 2.0;
	if (key == '3')
		vz += 2.0;
	if (key == '+')
		Zoom -= 2.0;
	if (key == '-')
		Zoom += 10.0;
	if (key == 27)
		exit(0);
	glutPostRedisplay();
}

void  ReadSpec(int key, int, int)						//
{
	if (key == GLUT_KEY_UP)
		b += 100.0;
	if (key == GLUT_KEY_DOWN)
		b -= 100.0;
	if (key == GLUT_KEY_RIGHT)
		a += 100.0;
	if (key == GLUT_KEY_LEFT)
		a -= 100.0;
	glutPostRedisplay();
}

void reshape(int w, int h)								//window
{
	w1 = w;
	h1 = h;
	glutPostRedisplay();
}

void ReshapeWindow(GLsizei width, GLsizei height) {
    if (height == 0)
        height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
}