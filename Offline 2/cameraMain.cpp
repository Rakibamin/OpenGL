#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#include "camera.h"

//global variable for the camera
CCamera Camera;

//draws a plane "net"
void DrawNet(float size, GLint LinesX, GLint LinesZ)
{
	glBegin(GL_LINES);
	for (int xc = 0; xc < LinesX; xc++)
	{
		glVertex3f(	-size / 2.0 + xc / (float)(LinesX-1)*size,
					0.0,
					size / 2.0);
		glVertex3f(	-size / 2.0 + xc / (float)(LinesX-1)*size,
					0.0,
					size / -2.0);
	}
	for (int zc = 0; zc < LinesX; zc++)
	{
		glVertex3f(	size / 2.0,
					0.0,
					-size / 2.0 + zc / (float)(LinesZ-1)*size);
		glVertex3f(	size / -2.0,
					0.0,
					-size / 2.0 + zc / (float)(LinesZ-1)*size);
	}
	glEnd();
}

//handles the window's resizing event
void reshape(int x, int y)
{
	if (y == 0 || x == 0) return;  //Nothing is visible then, so return
	
	//Set a new projection matrix
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();
	//Angle of view:40 degrees
	//Near clipping plane distance: 0.5
	//Far clipping plane distance: 20.0
	gluPerspective(40.0,(GLdouble)x/(GLdouble)y,0.5,20.0);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0,0,x,y);  //Use the whole window for rendering
}

//displays the scene.
void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
		
	Camera.Render();

	//Draw the "world" (which consists of six "nets" forming a cuboid


	glBegin(GL_LINES);{
		glColor3f(1.0, 0, 0);//x axis
		glVertex3f( 100,0,0);
		glVertex3f(-100,0,0);

		glColor3f(0.35,0.96,0);//y axis
		glVertex3f(0,-100,0);
		glVertex3f(0, 100,0);
		
		glColor3f(0, 0, 1.0);//z axis
		glVertex3f(0,0, 100);
		glVertex3f(0,0,-100);
	}glEnd();


	float size = 2.0;
	GLint LinesX = 30;
	GLint LinesZ = 30;

	float halfsize = size / 2.0;
	glColor3f(1.0,1.0,1.0);
	glPushMatrix();
		glTranslatef(0.0,-halfsize ,0.0);
		DrawNet(size,LinesX,LinesZ);
		glTranslatef(0.0,size,0.0);
		DrawNet(size,LinesX,LinesZ);
	glPopMatrix();
	glColor3f(0.0,0.0,1.0);
	glPushMatrix();
		glTranslatef(-halfsize,0.0,0.0);	
		glRotatef(90.0,0.0,0.0,halfsize);
		DrawNet(size,LinesX,LinesZ);
		glTranslatef(0.0,-size,0.0);
		DrawNet(size,LinesX,LinesZ);
	glPopMatrix();
	glColor3f(1.0,0.0,0.0);
	glPushMatrix();
		glTranslatef(0.0,0.0,-halfsize);	
		glRotatef(90.0,halfsize,0.0,0.0);
		DrawNet(size,LinesX,LinesZ);
		glTranslatef(0.0,size,0.0);
		DrawNet(size,LinesX,LinesZ);
	glPopMatrix();


	//finish rendering:
	glFlush();  
	glutSwapBuffers();

}

//handles the key down events
void KeyDown(unsigned char key, int x, int y)
{
	if(key -'0')Camera.ShowCameraAttr();

	switch (key) 
	{
	case '1':		
		Camera.RotateY(5.0);
		Display();
		break;
	case '2':		
		Camera.RotateY(-5.0);
		Display();
		break;
	case '3':		
		Camera.RotateX(5.0);
		Display();
		break;
	case '4':		
		Camera.RotateX(-5.0);
		Display();
		break;
	case '5':
		Camera.RotateZ(-5.0);
		Display();
		break;
	case '6':
		Camera.RotateZ(5.0);
		Display();
		break;

	}
}



void specialKeyListener(int key, int x,int y){
	
	if(key)Camera.ShowCameraAttr();

	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			Camera.MoveForward( 0.1 ) ;
			Display();
			break;
		case GLUT_KEY_UP:		// up arrow key
			Camera.MoveForward( -0.1 ) ;
			Display();
			break;

		case GLUT_KEY_RIGHT:
			Camera.StrafeRight(0.1);
			Display();
			break;
		case GLUT_KEY_LEFT:
			Camera.StrafeRight(-0.1);
			Display();
			break;

		case GLUT_KEY_PAGE_UP:
			Camera.MoveUpward(0.3);
			Display();
			break;
		case GLUT_KEY_PAGE_DOWN:
			Camera.MoveUpward(-0.3);
			Display();
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			exit(1);
			break;

		default:
			break;
	}
}



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800,600);
	glutCreateWindow("Camera");
	Camera.Focus();
	Camera.MoveForward( 10 ) ;
	glutDisplayFunc(Display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(KeyDown);
	glutSpecialFunc(specialKeyListener);
	glutMainLoop();
	return 0;             
}
