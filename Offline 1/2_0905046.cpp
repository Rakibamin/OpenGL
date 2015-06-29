#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include<GL/glut.h>


#define pi 2*acos(0)

double cameraHeight;
double cameraAngle;
double cameraDist;

double clockDirection;

int drawgrid;
int drawaxes;
double angle;

struct point
{
	double x,y,z;
};


void drawAxes()
{
	if(drawaxes==1)
	{
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
	}
}

void showCameraAttr()
{
	printf("cameraHeight: %f\n",cameraHeight);
	printf("cameraAngle: %f\n",cameraAngle);
	printf("cameraDist: %f\n",cameraDist);
}

//draws half sphere
void drawHalfSphere(float radius,int slices,int stacks,float direction)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));	
		r=sqrt(radius*radius-h*h);
		
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h*direction;
		}
		
		
	}
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
			if(j%2)glColor3f(0.25,0.25,0.25);
			else glColor3f(0.85,0.85,0.85);
			glBegin(GL_QUADS);{
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
				
			}glEnd();
		}
		
	}
}

//draws full sphere
void drawSphere(float radius,int slices,int stacks)
{
	drawHalfSphere(radius,slices,stacks,1.0);
	drawHalfSphere(radius,slices,stacks,-1.0);//reflection
	
}

//draws half funnel
void drawHalfBina(float radius,int slices,int stacks, float direction)
{
	struct point points[100][100];
	int i,j;
	double h,r;

    if(stacks<30)
	{
		stacks=30;
	}
	
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2)); 
		r=sqrt(radius*radius-h*h);
		
		float mult=10;
		r/=mult;

		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=2*log(r*mult+1)*direction;
		}
		mult*=mult;
	}

	for(i=4*stacks/5;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
			if(j%2)glColor3f(0.25,0.25,0.25);
			else glColor3f(0.85,0.85,0.85);

			glBegin(GL_QUADS);{
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
				
			}glEnd();

		}
		
	}
}

//draws actual shape of Snake Bina, 
//by merging a sphere with funnel
void drawSolidBina(float radius,int slices,int stacks)
{
	float sphereRadius=log(radius/3);
	float adjust=-0.5;
	
	//@params bound
	if(radius<20)radius=20;
	if(slices>90)slices=90;
	if(stacks>90)stacks=90;


	glPushMatrix();{
		glTranslatef(0,0,-2*log(radius+1)-adjust);
		drawHalfBina(radius,slices, stacks, 1.0);
	}glPopMatrix();

	drawSphere(sphereRadius,slices, stacks);

	glPushMatrix();{
	glTranslatef(0,0,2*log(radius+1)+adjust);
	drawHalfBina(radius,slices, stacks, -1.0);
	}glPopMatrix();
	
}



void keyboardListener(unsigned char key, int x,int y){
	
	int mod = glutGetModifiers();
	
	switch(key){

		case '1':	
			drawgrid=1-drawgrid;
			break;

		case 'z':
			if(mod==4)cameraDist += 5.0;//alt pressed
			else cameraDist -= 5.0;
			showCameraAttr();
			break;

		case 'Z':
			if(mod==4)cameraDist += 5.0;//alt pressed
			else cameraDist -= 5.0;
			showCameraAttr();
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			showCameraAttr();
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			showCameraAttr();
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			showCameraAttr();
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			showCameraAttr();
			break;

		case GLUT_KEY_PAGE_UP:
			clockDirection += 1;//Anti-Clock
			break;
		case GLUT_KEY_PAGE_DOWN:
			clockDirection -= 1;//Clock
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


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;	
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/ 
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	
	gluLookAt(cameraDist*cos(cameraAngle), cameraDist*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	
	
	
	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();

	glRotatef(clockDirection, 0,0,1);
	
	//drawSolidBina(20,20,20);
	//drawSolidBina(30,20,20);
	drawSolidBina(50,30,40);
	//drawSolidBina(71,71,71);
	//drawSolidBina(90,90,90);

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=7.0;
	cameraDist=15.0;
	clockDirection=0;
	cameraAngle=1.0;
	angle=0;
	
	printf("Keyboard Functions\n_ _ _ _ _ _ _ _ _\n\n");
	printf("1. Page Up : Anti-Clockwise Rotation\n");
	printf("2. Page Down : Clockwise Rotation\n");
	printf("3. UP/Down/Right/Left : Camera Navigation\n");
	printf("4. Z/Alt+Z : Zoom in/Zoom out\n");
	
	

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);
	
	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	10000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Snake Bina");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
