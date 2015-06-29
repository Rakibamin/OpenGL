#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include<GL/glut.h>


#define pi 2*acos(0)

double cameraHeight;
double cameraAngle;
double cameraDist;
double dis_z;
double dir_z;

double gravity;
double dz;
int flag;


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
			if(i%2&&j%2)glColor3f(0.25,0.25,0.25);
			else if(i%2&&j%2==0)glColor3f(0.85,0.85,0.85);
			else if(i%2==0&&j%2)glColor3f(0.85,0.85,0.85);
			else glColor3f(0.25,0.25,0.25);

			glBegin(GL_QUADS);{
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
				
			}glEnd();
		}
		
	}
}

void drawSphere(float radius,int slices,int stacks)
{
	drawHalfSphere(radius,slices,stacks,1.0);
	drawHalfSphere(radius,slices,stacks,-1.0);
	
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
			clockDirection += 1;//left-right
			break;
		case GLUT_KEY_PAGE_DOWN:
			clockDirection -= 1;//right-left
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

	dis_z = 0.5*gravity*dz;
	glTranslatef(0,0,dis_z);
	drawSphere(20,20,20);
	
	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	if(dis_z<100 && flag==1){ gravity=1; dz+=0.4; if(dis_z>90)flag=0;}//up
	else if(dis_z>0 && flag==0) { gravity+=0.003; dz-=0.3; if(dis_z<5)flag=1;}//down
	
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150;
	cameraDist=150;
	clockDirection=0;
	cameraAngle=1.0;
	angle=0;
	dis_z=20;
	gravity=1;
	flag=1;

	

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
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Rain Drops Keep Falling");

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
