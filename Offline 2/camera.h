#include <windows.h>

#define pi 2*acos(0)		
#define piby180 (pi/180.0)
#define SQR(x) (x*x)
#define NULL_VECTOR F3dVector(0.0f,0.0f,0.0f)

struct vector3D  
{
	float x,y,z;
};
struct vector2D
{
	float x,y;
};

class CCamera
{
private:
	
	vector3D ViewDir;
	vector3D RightVector;	
	vector3D UpVector;
	vector3D Position;

	float RotatedX, RotatedY, RotatedZ;	
	
public:
	CCamera();				
	void Render ( void );
	void Focus ( void );
	void ShowCameraAttr ( void ); 

	void Move ( vector3D Direction );
	void RotateX ( float Angle );
	void RotateY ( float Angle );
	void RotateZ ( float Angle );

	void MoveForward ( float Distance );
	void MoveUpward ( float Distance );
	void StrafeRight ( float Distance );


};


vector3D F3dVector ( float x, float y, float z )
{
	vector3D tmp;
	tmp.x = x;
	tmp.y = y;
	tmp.z = z;
	return tmp;
}

float GetF3dVectorLength( vector3D * v)
{
	return (float)(sqrt(SQR(v->x)+SQR(v->y)+SQR(v->z)));
}

vector3D Normalize3dVector( vector3D v)
{
	vector3D res;
	float l = GetF3dVectorLength(&v);
	if (l == 0.0f) return NULL_VECTOR;
	res.x = v.x / l;
	res.y = v.y / l;
	res.z = v.z / l;
	return res;
}

vector3D operator+ (vector3D v, vector3D u)
{
	vector3D res;
	res.x = v.x+u.x;
	res.y = v.y+u.y;
	res.z = v.z+u.z;
	return res;
}
vector3D operator- (vector3D v, vector3D u)
{
	vector3D res;
	res.x = v.x-u.x;
	res.y = v.y-u.y;
	res.z = v.z-u.z;
	return res;
}


vector3D operator* (vector3D v, float r)
{
	vector3D res;
	res.x = v.x*r;
	res.y = v.y*r;
	res.z = v.z*r;
	return res;
}

vector3D CrossProduct (vector3D * u, vector3D * v)
{
	vector3D resVector;
	resVector.x = u->y*v->z - u->z*v->y;
	resVector.y = u->z*v->x - u->x*v->z;
	resVector.z = u->x*v->y - u->y*v->x;

	return resVector;
}
float operator* (vector3D v, vector3D u)	
{
	return v.x*u.x+v.y*u.y+v.z*u.z;
}



CCamera::CCamera()
{
	Position = F3dVector (0.0, 0.0,	0.0);
	ViewDir = F3dVector( 0.0, 0.0, -1.0);
	RightVector = F3dVector (1.0, 0.0, 0.0);
	UpVector = F3dVector (0.0, 1.0, 0.0);
	RotatedX = RotatedY = RotatedZ = 0.0;
}

void CCamera::ShowCameraAttr ( void )
{
	printf("ViewDir: %f,%f,%f\n",ViewDir.x,ViewDir.y,ViewDir.z);
	printf("RightVector: %f,%f,%f\n",RightVector.x,RightVector.y,RightVector.z);
	printf("UpVector: %f,%f,%f\n",UpVector.x,UpVector.y,UpVector.z);
	printf("\n");
}

void CCamera::Move (vector3D Direction)
{
	Position = Position + Direction;
}

void CCamera::Focus ( void )
{
	ViewDir = F3dVector (0.0, -1.0, -0.173);
	RightVector = F3dVector (1.0, 0.1, -0.5);
	UpVector = F3dVector (-0.5, 0.16, -1.0);
}

void CCamera::RotateX (float Angle)
{
	RotatedX += Angle;
	
	//Rotate viewdir around the right vector:
	ViewDir = Normalize3dVector(ViewDir*cos(Angle*piby180)
								+ UpVector*sin(Angle*piby180));

	//now compute the new UpVector (by cross product)
	UpVector = CrossProduct(&ViewDir, &RightVector)*-1;

	
}

void CCamera::RotateY (float Angle)
{
	RotatedY += Angle;
	
	//Rotate viewdir around the up vector:
	ViewDir = Normalize3dVector(ViewDir*cos(Angle*piby180)
								- RightVector*sin(Angle*piby180));

	//now compute the new RightVector (by cross product)
	RightVector = CrossProduct(&ViewDir, &UpVector);
}

void CCamera::RotateZ (float Angle)
{
	RotatedZ += Angle;
	
	//Rotate viewdir around the right vector:
	RightVector = Normalize3dVector(RightVector*cos(Angle*piby180)
								+ UpVector*sin(Angle*piby180));

	//now compute the new UpVector (by cross product)
	UpVector = CrossProduct(&ViewDir, &RightVector)*-1;
}

void CCamera::Render( void )
{

	//The point at which the camera looks:
	vector3D ViewPoint = Position+ViewDir;
	

	//as we know the up vector, we can easily use gluLookAt:
	gluLookAt(	Position.x,Position.y,Position.z,
				ViewPoint.x,ViewPoint.y,ViewPoint.z,
				UpVector.x,UpVector.y,UpVector.z);

}

void CCamera::MoveForward( float Distance )
{
	Position = Position + (ViewDir*-Distance);
}

void CCamera::StrafeRight ( float Distance )
{
	Position = Position + (RightVector*Distance);
}

void CCamera::MoveUpward( float Distance )
{
	Position = Position + (UpVector*Distance);
}

