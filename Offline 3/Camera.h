#include <windows.h>
#include "Structs.h"
#define pi 2*acos(0)
#define piby180 (pi/180.0)
#define SQR(x) (x*x)
#define NULL_VECTOR F3dVector(0.0f,0.0f,0.0f)

class Camera{
  public:
    Point3 eye;
    Vector3 u,v,n;
    double viewAngle, aspect, nearDist, farDist;
    void setModelViewMatrix();

  public:
    Camera(){};

    void set(Point3 eye, Point3 look, Vector3 up);
    void roll(float angle);
    void pitch(float angle);
    void yaw(float angle);
    void slide(float delU, float delV, float delN);
    void setShape(float vAng, float asp, float nearD, float farD);
};

void Camera :: setModelViewMatrix(void)
{
    float m[16];
    Vector3 eVec(eye.x, eye.y, eye.z);
    m[0] =  u.x; m[4] =  u.y; m[8]  =  u.z;  m[12] = -eVec.dot(u);
    m[1] =  v.x; m[5] =  v.y; m[9]  =  v.z;  m[13] = -eVec.dot(v);
    m[2] =  n.x; m[6] =  n.y; m[10] =  n.z;  m[14] = -eVec.dot(n);
    m[3] =  0;   m[7] =  0;   m[11] =  0;    m[15] = 1.0;
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(m);
}

void Camera :: set(Point3 Eye, Point3 look, Vector3 up)
{
    Vector3 t;
    eye.set(Eye);
    n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z);

    u.set(up.y*n.z - up.z*n.y, up.z*n.x - up.x*n.z, up.x*n.y - up.y*n.x);


    n.normalize(); u.normalize();
    v.set(n.y*u.z - n.z*u.y, n.z*u.x - n.x*u.z, n.x*u.y - n.y*u.x);

    setModelViewMatrix();
}

void Camera :: setShape(float vAng, float asp, float nearD, float farD)
{
    viewAngle = vAng; aspect = asp; nearDist = nearD; farDist = farD;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(viewAngle, aspect, nearDist, farDist);
}

void Camera :: slide(float delU, float delV, float delN)
{
    eye.x += delU * u.x + delV * v.x + delN * n.x;
    eye.y += delU * u.y + delV * v.y + delN * n.y;
    eye.z += delU * u.z + delV * v.z + delN * n.z;
    setModelViewMatrix();
}

void Camera :: roll(float angle)
{
    float cs = cos( angle / 180 * 3.14159265 );
    float sn = sin( angle / 180 * 3.14159265 );
    Vector3 t = u;
    u.set(cs*t.x - sn*v.x, cs*t.y - sn*v.y, cs*t.z - sn*v.z );
    v.set(sn*t.x + cs*v.x, sn*t.y + cs*v.y, sn*t.z + cs*v.z );
    setModelViewMatrix();
}

void Camera :: pitch(float angle)
{
    float cs = cos( angle / 180 * 3.14159265 );
    float sn = sin( angle / 180 * 3.14159265 );
    Vector3 t = n;
    n.set(cs*t.x + sn*v.x, cs*t.y + sn*v.y, cs*t.z + sn*v.z );
    v.set(-sn*t.x + cs*v.x, -sn*t.y + cs*v.y, -sn*t.z + cs*v.z );
    setModelViewMatrix();
}

void Camera :: yaw(float angle)
{
    float cs = cos( angle / 180 * 3.14159265 );
    float sn = sin( angle / 180 * 3.14159265 );
    Vector3 t = u;
    u.set(cs*t.x + sn*n.x, cs*t.y + sn*n.y, cs*t.z + sn*n.z );
    n.set(-sn*t.x + cs*n.x, -sn*t.y + cs*n.y, -sn*t.z + cs*n.z );
    setModelViewMatrix();
}
