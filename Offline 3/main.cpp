#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <GL/glut.h>

#include "Camera.h"
#include "Shapes.h"
#include "Grid.h"
#include "Light.h"



using namespace std;

#define timeIncrease .001;
#define initialHeight 60;
#define pi 2*acos(0)
#define BLACK 0, 0, 0

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double DIV = 200.0;

class Vect {

	public:
	double x, y, z;

	Vect ();

	Vect (double, double, double);

	// method functions
	double getVectX() { return x; }
	double getVectY() { return y; }
	double getVectZ() { return z; }

	double magnitude () {
		return sqrt((x*x) + (y*y) + (z*z));
	}

	Vect normalize () {
		double magnitude = sqrt((x*x) + (y*y) + (z*z));
		return Vect (x/magnitude, y/magnitude, z/magnitude);
	}

	Vect negative () {
		return Vect (-x, -y, -z);
	}

	double dotProduct(Vect v) {
		return x*v.getVectX() + y*v.getVectY() + z*v.getVectZ();
	}

	Vect crossProduct(Vect v) {
		return Vect (y*v.getVectZ() - z*v.getVectY(), z*v.getVectX() - x*v.getVectZ(), x*v.getVectY() - y*v.getVectX());
	}

	Vect vectAdd (Vect v) {
		return Vect (x + v.getVectX(), y + v.getVectY(), z + v.getVectZ());
	}

	Vect vectMult (double scalar) {
		return Vect (x*scalar, y*scalar, z*scalar);
	}
	Vect operator+ (Vect u)
    {
        Vect res;
        res.x = x+u.x;
        res.y = y+u.y;
        res.z = z+u.z;
        return res;
    }
    Vect operator- (Vect u)
    {
        Vect res;
        res.x = x-u.x;
        res.y = y-u.y;
        res.z = z-u.z;
        return res;
    }


    Vect operator* (float r)
    {
        Vect res;
        res.x = x*r;
        res.y = y*r;
        res.z = z*r;
        return res;
    }

};

Vect::Vect () {
	x = 0;
	y = 0;
	z = 0;
}

Vect::Vect (double i, double j, double k) {
	x = i;
	y = j;
	z = k;
}


class Ray {
	Vect origin, direction;

	public:

	Ray ();

	Ray (Vect, Vect);

	// method functions
	Vect getRayOrigin () { return origin; }
	Vect getRayDirection () { return direction; }

};

Ray::Ray () {
	origin = Vect(0,0,0);
	direction = Vect(1,0,0);
}

Ray::Ray (Vect o, Vect d) {
	origin = o;
	direction = d;
}



class Color {

	public:

	double red, green, blue, special;

	Color ();

	Color (double, double, double, double);

	// method functions
	double getColorRed() { return red; }
	double getColorGreen() { return green; }
	double getColorBlue() { return blue; }
	double getColorSpecial() { return special; }

	double setColorRed(double redValue) { red = redValue; }
	double setColorGreen(double greenValue) { green = greenValue; }
	double setColorBlue(double blueValue) { blue = blueValue; }
	double setColorSpecial(double specialValue) { special = specialValue; }

	double brightness() {
		return(red + green + blue)/3;
	}

	Color colorScalar(double scalar) {
		return Color (red*scalar, green*scalar, blue*scalar, special);
	}

	Color colorAdd(Color color) {
		return Color (red + color.getColorRed(), green + color.getColorGreen(), blue + color.getColorBlue(), special);
	}

	Color colorMultiply(Color color) {
		return Color (red*color.getColorRed(), green*color.getColorGreen(), blue*color.getColorBlue(), special);
	}

	Color colorAverage(Color color) {
		return Color ((red + color.getColorRed())/2, (green + color.getColorGreen())/2, (blue + color.getColorBlue())/2, special);
	}

	Color clip() {
		double alllight = red + green + blue;
		double excesslight = alllight - 3;
		if (excesslight > 0) {
			red = red + excesslight*(red/alllight);
			green = green + excesslight*(green/alllight);
			blue = blue + excesslight*(blue/alllight);
		}
		if (red > 1) {red = 1;}
		if (green > 1) {green = 1;}
		if (blue > 1) {blue = 1;}
		if (red < 0) {red = 0;}
		if (green < 0) {green = 0;}
		if (blue < 0) {blue = 0;}

		return Color (red, green, blue, special);
	}
};

Color::Color () {
	red = 0.5;
	green = 0.5;
	blue = 0.5;
}

Color::Color (double r, double g, double b, double s) {
	red = r;
	green = g;
	blue = b;
	special = s;
}


double angle = 30;
double pos_of_z,slide_x,slide_y,slide_z;
double angle_val=5,slide_val=5;
double Angle,eye_x,eye_y,eye_z,look_x,look_y,look_z,up_x,up_y,up_z,angle_yaw,angle_roll,angle_pitch;
int upward ;
int dimension;
Camera cam;
Point3 eye,look;
Vector3 up;


int recursionDepth, noPixelsAxes, noObjects, type;
double sp_radius;
Color temp_color;
Vect sp_center;
double temp_color_r,temp_color_g,temp_color_b; 
double sp_ambient,sp_specular,sp_diffuse,sp_reflection,sp_shineness;

Sphere *sphere[3];
Cube *cube;

struct RGBType{
double r,g,b;
};

int dpi = 10;


void savebmp (const char *filename, int w, int h, int dpi, RGBType *data) {
	FILE *f;
	int k = w*h;
	int s = 4*k;
	int filesize = 54 + s;
	
	double factor = 39.375;
	int m = static_cast<int>(factor);
	
	int ppm = dpi*m;
	
	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};
	
	bmpfileheader[ 2] = (unsigned char)(filesize);
	bmpfileheader[ 3] = (unsigned char)(filesize>>8);
	bmpfileheader[ 4] = (unsigned char)(filesize>>16);
	bmpfileheader[ 5] = (unsigned char)(filesize>>24);
	
	bmpinfoheader[ 4] = (unsigned char)(w);
	bmpinfoheader[ 5] = (unsigned char)(w>>8);
	bmpinfoheader[ 6] = (unsigned char)(w>>16);
	bmpinfoheader[ 7] = (unsigned char)(w>>24);
	
	bmpinfoheader[ 8] = (unsigned char)(h);
	bmpinfoheader[ 9] = (unsigned char)(h>>8);
	bmpinfoheader[10] = (unsigned char)(h>>16);
	bmpinfoheader[11] = (unsigned char)(h>>24);
	
	bmpinfoheader[21] = (unsigned char)(s);
	bmpinfoheader[22] = (unsigned char)(s>>8);
	bmpinfoheader[23] = (unsigned char)(s>>16);
	bmpinfoheader[24] = (unsigned char)(s>>24);
	
	bmpinfoheader[25] = (unsigned char)(ppm);
	bmpinfoheader[26] = (unsigned char)(ppm>>8);
	bmpinfoheader[27] = (unsigned char)(ppm>>16);
	bmpinfoheader[28] = (unsigned char)(ppm>>24);
	
	bmpinfoheader[29] = (unsigned char)(ppm);
	bmpinfoheader[30] = (unsigned char)(ppm>>8);
	bmpinfoheader[31] = (unsigned char)(ppm>>16);
	bmpinfoheader[32] = (unsigned char)(ppm>>24);
	
	f = fopen(filename,"wb");
	
	fwrite(bmpfileheader,1,14,f);
	fwrite(bmpinfoheader,1,40,f);
	
	for (int i = 0; i < k; i++) {
		RGBType rgb = data[i];
		
		double red = (data[i].r)*255;
		double green = (data[i].g)*255;
		double blue = (data[i].b)*255;
		
		unsigned char color[3] = {(int)floor(blue),(int)floor(green),(int)floor(red)};
		
		fwrite(color,1,3,f);
	}
	
	fclose(f);
}

void input2Object()
{
	ifstream input;
	input.open("input.txt");
	input>>recursionDepth;
	input>>noPixelsAxes;
	input>>noObjects;
	int i=0;
	while(i<noObjects)
	{
		input>>type;
		if(type==1)
		{
			input>>sp_center.x>>sp_center.y>>sp_center.z;
			input>>sp_radius;
			input>>temp_color_r>>temp_color_g>>temp_color_b;
			input>>sp_ambient>>sp_diffuse>>sp_specular>>sp_reflection;
			input>>sp_shineness;
			temp_color = Color(temp_color_r,temp_color_g,temp_color_b,0.0);
			sphere[i++]= new Sphere(sp_center, sp_radius, temp_color, sp_ambient, sp_diffuse, sp_specular, sp_reflection, sp_shineness);
		}
		else if(type==2)
		{
			input>>sp_center.x>>sp_center.y>>sp_center.z;
			input>>sp_radius;
			input>>temp_color_r>>temp_color_g>>temp_color_b;
			input>>sp_ambient>>sp_diffuse>>sp_specular>>sp_reflection;
			input>>sp_shineness;
			temp_color = Color(temp_color_r,temp_color_g,temp_color_b,0.0);
			cube= new Cube(sp_center, sp_radius, temp_color, sp_ambient, sp_diffuse, sp_specular, sp_reflection, sp_shineness);
			i++;
		}
		else
			break;
	}

	input.close();
} 


void rayTrace()
{
	cout<<"Started"<<endl;
	int size = noPixelsAxes*noPixelsAxes;
	double h = 2*tan(45*pi/360);

	RGBType *pixels = new RGBType[size];
	
	for(int i=0;i<noPixelsAxes;i++){
		for(int j=0;j<noPixelsAxes;j++)
		{
			int index = i*noPixelsAxes + j ;
			
			Vector3 source = cam.eye;

			double pixelDimension = h/noPixelsAxes;
			double x = pixelDimension/2 + j*pixelDimension - h,y = pixelDimension/2 + i*pixelDimension - h;
			
			Vect tN(cam.n.x,cam.n.y,cam.n.z);
			Vect tU(cam.u.x,cam.u.y,cam.u.z);
			Vect tV(cam.v.x,cam.v.y,cam.v.z);
				

			//Vector3 direction = cam.n.mult(-1) + cam.u.mult(x) + cam.v.mult(y);
			Vect direction = tN.vectMult(-1) + tU.vectMult(x) + tV.vectMult(y);
			direction.normalize();

			double minimumDist=-1;
			int chosenSphere;

			for(int k=0;k<noObjects-1;k++)
			{
				Vect temp1(source.x,source.y,source.z);
				//Vect temp2(direction.x,direction.y,direction.z);
				double distance = sphere[k]->getIntersection(temp1,direction);
				//cout<<distance<<endl;
				//if(distance>-1)cout<<distance<<endl;
				//double distance = sphere[k]->findIntersection(ray);
				
				if(distance>0)
				{
					if(minimumDist<0){minimumDist=distance;chosenSphere=k;}
					else if(distance<minimumDist){minimumDist=distance;chosenSphere=k;}
				}
			}
			if(minimumDist<0)
			{
				
				double t = -1*(float)source.z/(float) direction.z;
				double x = source.x + t * direction.x,y = source.y + t * direction.y;
				int temp = floor(x/20)+floor(y/20);
				if(temp%2) {pixels[index].r=0;pixels[index].g=0;pixels[index].b=0;}
				else {pixels[index].r=1;pixels[index].g=1;pixels[index].b=1;}
			}
			else
			{
				
				double t = minimumDist;
				double x = source.x + t * direction.x,y = source.y + t * direction.y;
				double z = source.z + t * direction.z;
				pixels[index].r=sphere[chosenSphere]->color.red;
				pixels[index].g=sphere[chosenSphere]->color.green;
				pixels[index].b=sphere[chosenSphere]->color.blue;
			}

		}
	}
	savebmp("rakib.bmp",noPixelsAxes,noPixelsAxes,dpi,pixels);
	cout<<"Ended"<<endl;
}

void drawAxes()
{
	if(drawaxes==1)
	{
		glBegin(GL_LINES);{
		    glColor3f(1.0, 0, 0); //x = red axis
			glVertex3f( 500,0,0);
			glVertex3f(-500,0,0);

			glColor3f(0, 1.0, 0); //y = green axis
			glVertex3f(0,-500,0);
			glVertex3f(0, 500,0);

			glColor3f(0, 0, 1.0); //z = blue axis
			glVertex3f(0,0, 500);
			glVertex3f(0,0,-500);
		}glEnd();
	}
}



void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
            angle_yaw-=angle_val;
			//eye.x+=10;
			//eye.y+=10;
			//eye.z+=10;S
			break;

        case '2':
			angle_yaw+=angle_val;
			//eye.y-=10;
			//eye.z-=10;
			break;

        case '3':
			angle_pitch+=angle_val;
			break;

        case '4':
			angle_pitch-=angle_val;
			break;

        case '5':
			angle_roll+=angle_val;
			break;

        case '6':
			angle_roll-=angle_val;
			break;
        case '7':
			cam.u.print();cam.v.print();cam.n.print();
            cam.u.cross(cam.v).print();
            cam.v.cross(cam.n).print();
            cam.n.cross(cam.u).print();
			break;

        case 'o':
			eye.x*=angle_val;
			eye.y*=angle_val;
			eye.z*=angle_val;
			break;
        case 'i':
			eye.x/=angle_val;
			eye.y/=angle_val;
			eye.z/=angle_val;
			break;



		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
            slide_z+=slide_val;
			break;
		case GLUT_KEY_UP:		// up arrow key
            slide_z-=slide_val;
			break;

		case GLUT_KEY_RIGHT:
            slide_x+=slide_val;
			break;
		case GLUT_KEY_LEFT:
            slide_x-=slide_val;
			break;

		case GLUT_KEY_PAGE_UP:
		    slide_y+=slide_val;
			break;
		case GLUT_KEY_PAGE_DOWN:
		    slide_y-=slide_val;
			break;

		case GLUT_KEY_INSERT:

			break;

		case GLUT_KEY_HOME:
		    eye.x+=10;
			eye.y+=10;
			eye.z+=10;
			break;
		case GLUT_KEY_END:
		    eye.x-=10;
			eye.y-=10;
			eye.z-=10;
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
            //slide_z-=5;
            //startRayTracing();
			rayTrace();
			break;

		case GLUT_RIGHT_BUTTON:
			slide_z+=5;
			break;

		case GLUT_MIDDLE_BUTTON:
            if(state == GLUT_DOWN){
				drawaxes=1-drawaxes;
			}
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

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	eye.set(eye.x,eye.y,eye.z);
    look.set(look.x,look.y,look.z);
    up.set(up.x,up.y,up.z);
	cam.set(eye,look,up);
	cam.slide(slide_x,slide_y,slide_z);
    cam.yaw(angle_yaw);
    cam.roll(angle_roll);
    cam.pitch(angle_pitch);



	drawAxes();
	//drawGrid();
	
	

	int i;
	for(i=0;i<noObjects-1;i++){
	glPushMatrix();{
			glColor3f(sphere[i]->color.red,sphere[i]->color.green,sphere[i]->color.blue);
            glTranslated(sphere[i]->center.x,sphere[i]->center.y,sphere[i]->center.z);
            glutSolidSphere(sphere[i]->radius, 36, 36);
        }glPopMatrix();
	}
	
	
	glPushMatrix();{
	    glTranslated(cube->lowestPoint.x+10,cube->lowestPoint.y+10,cube->lowestPoint.z+cube->sides/2);
	    glColor3f(cube->color.red,cube->color.green,cube->color.blue);
        glutSolidCube(cube->sides);
	}glPopMatrix();
	
		
	//drawPlane();

	glPushMatrix();{
	glTranslatef(-200,0,0);
	drawGrid();
	}glPopMatrix();

	glPushMatrix();{
	glTranslatef(200,0,0);
	drawGrid();
	}glPopMatrix();

	drawGrid();

	glPushMatrix();{
	glTranslatef(0,200,0);
	drawGrid();
	}glPopMatrix();

	glPushMatrix();{
	glTranslatef(0,-200,0);
	drawGrid();
	}glPopMatrix();

	glPushMatrix();{
	glTranslatef(200,200,0);
	drawGrid();
	}glPopMatrix();

	glPushMatrix();{
	glTranslatef(200,-200,0);
	drawGrid();
	}glPopMatrix();

	glPushMatrix();{
	glTranslatef(-200,-200,0);
	drawGrid();
	}glPopMatrix();

	glPushMatrix();{
	glTranslatef(-200,200,0);
	drawGrid();
	}glPopMatrix();



	

/**/   

	

//////////////////////////////////////////////////////////

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Camera

	//MISSING SOMETHING? -- YES: add the following
	glutPostRedisplay();	//this will call the display AGAIN
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;
	//cameraHeight=100.0;
	eye.x=100.0;
	eye.y=100.0;
	eye.z=100.0;

	look.x=0.0;
	look.y=0.0;
	look.z=0.0;

	up.x=0.0;
	up.y=0.0;
	up.z=1.0;

	Angle=0;
	pos_of_z=0;
	slide_x=0;
	slide_y=0;
	slide_z=0;
	upward=1;
	angle_yaw=0;
	angle_roll=0;
	angle_pitch=0;


	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	//glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	//glLoadIdentity();

	//give PERSPECTIVE parameters
	//gluPerspective(80,	1,	1,	10000.0);

	cam.setShape(60,1,1,10000.0);




	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Ray Casting");

	init();

	input2Object(); 

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
