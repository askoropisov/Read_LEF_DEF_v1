#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

#include "glut.h"


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glut64.lib")



double w1 = 800.0, h1 = 800.0;
float vx = 0.0, vy = 0.0, vz = 0.0;
double Zoom = 100000.0;
double a, b;

int key_layer1 = 0;
int key_layer2 = 0;
int key_layer3 = 0;

extern std::vector <std::vector <int>> l1_print, l2_print, l3_print;



void RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, w1, h1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, w1 / h1, 0.1f, 10000000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(a, b, -Zoom);
	glRotatef(vx, 1, 0, 0);
	glRotatef(vy, 0, 1, 0);
	glRotatef(vz, 0, 0, 1);

	int xd = (def.x_2 - def.x_1) ;
	int yd = (def.y_2 - def.y_1);
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
		double x_2 = component->x_position;
		double y_2 = component->y_position;
		double z = 3;
		std::string  name_in_Macro = component->name_model_in_LEF;

		for (auto macro_element : lef.macroes) {						//open MACRO
			if (macro_element->name == name_in_Macro)
			{
				x_2 += macro_element->size_x;							//position vertex x2,y2
				y_2 += macro_element->size_y;

				glBegin(GL_LINE_LOOP);												//draw component
				glColor3f(0, 1, 0);
				glVertex3f(x_1, y_1, z);
				glVertex3f(x_2, y_1, z);
				glVertex3f(x_2, y_2, z);
				glVertex3f(x_1, y_2, z);
				glEnd();

				double mass_koord[4] = { 0,0,0,0 };

				
				for (auto pin : macro_element->pins) {					//open PIN
					for (auto polygon : pin->polygons) {				//open polygon
						int i = 0;
						for (auto position : polygon->position) {		//open position
							mass_koord[i] = position;					//read position
							++i;
						}
						glBegin(GL_LINE_LOOP);							//draw pins (rects or rolygons)
						glColor3f(1, 1, 0);
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
		
	}

	

	//draw traces

	//for (int i = 0; i < xd; i+=10) {
	//	for (int j = 0; j < yd; j+=10) {
	//		//layer 1
	//		if (key_layer1 % 2 == 0) {
	//			switch (l1_print[i][j])
	//			{
	//			case 0:
	//				/*glBegin(GL_QUADS);
	//				glColor3f(1, 1, 1);
	//				glVertex3f(i - 1, j - 1, 3);
	//				glVertex3f(i + 1, j - 1, 3);
	//				glVertex3f(i + 1, j + 1, 3);
	//				glVertex3f(i - 1, j + 1, 3);
	//				glEnd();*/
	//				break;

	//				/*case (-10):
	//					glBegin(GL_QUADS);
	//					glColor3f(1, 0, 0);
	//					glVertex3f(i - 1, j - 1, 4);
	//					glVertex3f(i + 1, j - 1, 4);
	//					glVertex3f(i + 1, j + 1, 4);
	//					glVertex3f(i - 1, j + 1, 4);
	//					break;*/
	//			default:
	//				glBegin(GL_QUADS);
	//				glColor3f(1, 0, 0);
	//				glVertex3f(i - 1,  j - 1, 6);
	//				glVertex3f( i + 1,  j - 1, 6);
	//				glVertex3f( i + 1,  j + 1, 6);
	//				glVertex3f( i - 1,  j + 1, 6);
	//				glEnd();
	//				break;
	//			}
	//		}
	//		//layer 2
	//		if (key_layer2 % 2 == 0) {
	//			switch (l2_print[i][j])
	//			{
	//			case 0:
	//				/*glBegin(GL_QUADS);
	//				glColor3f(1, 1, 1);
	//				glVertex3f(i - 1, j - 1, 3);
	//				glVertex3f(i + 1, j - 1, 3);
	//				glVertex3f(i + 1, j + 1, 3);
	//				glVertex3f(i - 1, j + 1, 3);
	//				glEnd();
	//				break;*/
	//				break;

	//			default:
	//				glBegin(GL_QUADS);
	//				glColor3f(1, 0, 0);
	//				glVertex3f( i - 1,  j - 1, 6);
	//				glVertex3f( i + 1,  j - 1, 6);
	//				glVertex3f( i + 1, j + 1, 6);
	//				glVertex3f( i - 1,  j + 1, 6);
	//				glEnd();
	//				break;
	//			}
	//		}
	//		if (key_layer3 % 2 == 0) {
	//			switch (l3_print[i][j])
	//			{
	//			case 0:
	//				/*glBegin(GL_QUADS);
	//				glColor3f(1, 1, 1);
	//				glVertex3f(i - 1, j - 1, 3);
	//				glVertex3f(i + 1, j - 1, 3);
	//				glVertex3f(i + 1, j + 1, 3);
	//				glVertex3f(i - 1, j + 1, 3);
	//				glEnd();
	//				break;*/
	//				break;

	//			default:
	//				glBegin(GL_QUADS);
	//				glColor3f(1, 0, 0);
	//				glVertex3f( i - 1,  j - 1, 6);
	//				glVertex3f( i + 1,  j - 1, 6);
	//				glVertex3f( i + 1,  j + 1, 6);
	//				glVertex3f( i - 1,  j + 1, 6);
	//				glEnd();
	//				break;
	//			}
	//		}
	//	}
	//}

    // Конец нашего кода
    glutSwapBuffers();
}
//
//void Print_newDRP(char Sboard[10000][10000])									//заполняем матрицу символами
//{
//	for (int i = 0; i < 10000; i++)
//	{
//		for (int j = 0; j < 10000; j++)
//		{
//			//if (Sboard[i][j] == '0') newDRP[i][j] = 0;
//			//if (Sboard[i][j] == 'S') newDRP[i][j] = -1;
//			//if (Sboard[i][j] == 'T') newDRP[i][j] = -2;
//			//if (Sboard[i][j] == 's') newDRP[i][j] = -1;
//			//if (Sboard[i][j] == 't') newDRP[i][j] = -2;
//			//if (Sboard[i][j] == '#') newDRP[i][j] = 999;
//			if (Sboard[i][j] == '*') newDRP[i][j] = 777;
//		}
//	}
//}





void Read_kb(unsigned char key, int, int)				// zoom and control
{
	if (key == 's')
		b -= 10.0;
	if (key == 'w')
		b += 10.0;
	if (key == 'a')
		a -= 10.0;
	if (key == 'd')
		a += 10.0;
	if (key == 'e')
		Zoom -= 200.0;
	if (key == 'q')
		Zoom += 200.0;
	if (key == '+')
		Zoom -= 10000.0;
	if (key == '-')
		Zoom += 10000.0;
	//Layer draw buttons
	if (key == '1')
		key_layer1 +=1;
	if (key == '2')
		key_layer2 +=1;
	if (key == '3')
		key_layer3 +=1;
	if (key == 27)
		exit(0);
	glutPostRedisplay();
}

void  ReadSpec(int key, int, int)						//
{
	if (key == GLUT_KEY_UP)
		b += 1000.0;
	if (key == GLUT_KEY_DOWN) 
		b -= 1000.0;
	if (key == GLUT_KEY_RIGHT)
		a += 1000.0;
	if (key == GLUT_KEY_LEFT)
		a -= 1000.0;
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