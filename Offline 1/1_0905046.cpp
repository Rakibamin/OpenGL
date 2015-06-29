#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include<GL/glut.h>


#define pi 2*acos(0)

double cameraHeight;
double cameraAngle;
double cameraDist;

double varRadius;
double stackSize;


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


void keyboardListener(unsigned char key, int x,int y){
	
	int mod = glutGetModifiers();
	
	switch(key){

		case '1':	
			drawgrid=1-drawgrid;
			break;

		case 's':
			clockDirection++;
			break;

		case 'd':
			clockDirection--;
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
			if(varRadius<12)varRadius++;
			if(stackSize<7)stackSize++;
			printf("%f, %f\n",varRadius, stackSize);
			break;
		case GLUT_KEY_PAGE_DOWN:
			if(varRadius>0)varRadius--;
			if(stackSize>0)stackSize--;
			printf("%f, %f\n",varRadius, stackSize);
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
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

int buildRoundedSquare(point * pVertices, int nVertices, float sides, float r) 
{

    float a, da;
    int i1, i2, i3, i4, n;

    if (nVertices < 4) { return 0; }

    if (nVertices == 4) {

        pVertices [0].x = 0;	pVertices [0].y = 0;	pVertices[0].z = 0;
        pVertices [1].x = sides;pVertices [1].y = 0;	pVertices[1].z = 0;
        pVertices [2].x = sides;pVertices [2].y = sides;pVertices[2].z = 0;
        pVertices [3].x = 0;	pVertices [3].y = sides;pVertices[3].z = 0;

        return nVertices;
    }

    n = nVertices / 4;

    if (r > sides / 2) { r = sides / 2; }

    a = 0;
    da = ((double)pi/(double)2) / (float) (n - 1);

    for (i1=0, i2=2*n-1, i3=2*n, i4=4*n-1; i1<n; i1++, i2--, i3++, i4--, a += da) {

        float cosA = cos (a), sinA = sin (a);

        pVertices [i1].x = (sides - r) + r * cosA;	pVertices [i1].y = (sides - r) + r * sinA;	pVertices [i1].z = 0;
        pVertices [i2].x =     r    - r * cosA;		pVertices [i2].y = (sides - r) + r * sinA;	pVertices [i2].z = 0;
        pVertices [i3].x =     r    - r * cosA;		pVertices [i3].y =     r    - r * sinA;		pVertices [i3].z = 0;
        pVertices [i4].x = (sides - r) + r * cosA;	pVertices [i4].y =     r    - r * sinA;		pVertices [i4].z = 0;
    }

    return n * 4;
}

void drawRoundedSquare(float sides, float radius) 
{

    point vertices [100];
    glColor3f (0.3, 0.5, 0.2);

	// glVertexPointer - defines an array of vertex data
	// glVertexPointer (no_coords_per_vertex, GL_TYPE, stride(0), pointer_to_vertice_array);
    glVertexPointer (3, GL_DOUBLE, 0, vertices);

	// used to enable classes(GL_VERTEX_ARRAY etc.) with methods like: glDrawArrays etc.
    glEnableClientState (GL_VERTEX_ARRAY);
	
	// glDrawArrays - renders primitive from array
	// glDrawArrays (GL_PRIMITIVE_TYPES(LINE, QUAD etc.), starting_index, no_of_indices_to_render);
    glDrawArrays (GL_LINE_LOOP, 0, buildRoundedSquare (vertices, 100 , sides, radius));

	glDisableClientState (GL_VERTEX_ARRAY);
}

void drawRoundedCube(float dir)
{
	float j=0,i=40;
	double h;
	if(stackSize<1)stackSize=1;

	glPushMatrix();
	{
		while(j<stackSize)
		{
			h=15*sin(((double)j/(double)20)*(pi/2));
			glPushMatrix();
			{
				glTranslatef(-i/2,-i/2,h*dir);
				drawRoundedSquare(i--,varRadius);
			}glPopMatrix();
			j+=0.5;
		}
	}glPopMatrix();
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

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	gluLookAt(cameraDist*cos(cameraAngle), cameraDist*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,-1,150,	0,0,0,	0,0,1);
	
	
	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();

	glPushMatrix();
	{
	glTranslatef(0,0,-20);
	drawRoundedCube(-1);
	}glPopMatrix();

	
	glPushMatrix();
	{
	glTranslatef(0,0,20);
	drawRoundedCube(1);
	}glPopMatrix();

	
	glPushMatrix();
	{
	glRotatef(-90, 0,1,0);
	glTranslatef(0,0,20);
	drawRoundedCube(1);
	}glPopMatrix();

	glPushMatrix();
	{
	glRotatef(90, 1,0,0);
	glTranslatef(0,0,20);
	drawRoundedCube(1);
	}glPopMatrix();

	glPushMatrix();
	{
	glRotatef(-90, 1,0,0);
	glTranslatef(0,0,20);
	drawRoundedCube(1);
	}glPopMatrix();

	glPushMatrix();
	{
	glRotatef(90, 0,1,0);
	glTranslatef(0,0,20);
	drawRoundedCube(1);
	}glPopMatrix();
	
	
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
	cameraHeight= 1;
	cameraAngle= 3.16;
	cameraDist= 72;
	clockDirection=0;
	angle=0;
	varRadius=12;
	stackSize=7;

	

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

	glutCreateWindow("Sphere or Cube");

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

