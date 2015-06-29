#include<vector>

using namespace std;

struct Point3
{

    float x,y,z;
    void set(float dx, float dy, float dz){x = dx; y = dy; z = dz;}
    void set(Point3& p){x = p.x; y = p.y; z = p.z;}
    Point3(float xx, float yy, float zz){x = xx; y = yy; z = zz;}
    Point3(){x = y = z = 0;}
    void build4tuple(float v[])
    {
            v[0] = x; v[1] = y; v[2] = z; v[3] = 1.0f;
    }
};

struct Vector3
{
    float x,y,z;
    void set(float dx, float dy, float dz){ x = dx; y = dy; z = dz;}
    void set(Vector3& v){ x = v.x; y = v.y; z = v.z;}
    void flip(){x = -x; y = -y; z = -z;}

    void setDiff(Point3& a, Point3& b)
    { x = a.x - b.x; y = a.y - b.y; z = a.z - b.z;}
    void normalize()
    {
        double sizeSq = x * x + y * y + z * z;
        if(sizeSq < 0.0000001)
        {

            return;
        }
        float scaleFactor = 1.0/(float)sqrt(sizeSq);
        x *= scaleFactor; y *= scaleFactor; z *= scaleFactor;
    }
    Vector3(){x = y = z = 0;}
    Vector3(float xx, float yy, float zz){x = xx; y = yy; z = zz;}
    //Vector3(Vector3& v){x = v.x; y = v.y; z = v.z;}
    Vector3(Point3 pt){x=pt.x;y=pt.y;z=pt.z;}

    Vector3 cross(Vector3 b)
    {
       Vector3 c(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
       return c;
    }

    float dot(Vector3 b){return x * b.x + y * b.y + z * b.z;}
    void print(){cout<<x<<" "<<y<<" "<<z<<endl;}
    Vector3 neg(){Vector3 v(-x,-y,-z);return v;}
    bool eql(Vector3 vv){return (x==vv.x && y==vv.y && z==vv.z);}
    Vector3 add(Vector3 vv){return Vector3(x+vv.x,y+vv.y,z+vv.z);}
    Vector3 mult(double d){return Vector3(x*d,y*d,z*d);}
};

struct ray
{
    Vector3 origin,direction;
    ray(){origin=Vector3(0,0,0);direction=Vector3(0,0,1);}
    ray(Vector3 org,Vector3 dir){origin=org;direction=dir;}
};

struct color
{
    double r,g,b;
    color(){}
    color(double rr,double gg,double bb)
    {
        r=rr;b=bb;g=gg;
    }
    color mult(double d){return color(r*d,g*d,b*d);}
    color add(color cc){return color(r+cc.r,g+cc.g,b+cc.b);}
    color mul(Vector3 vv){return color(r*vv.x,g*vv.y,b*vv.z);}
};

struct object
{
    int type;
    virtual void print()=0;
    virtual color get_color()=0;
    virtual double get_intersection(ray r)=0;
    virtual Vector3 get_normal(Vector3 pt)=0;
};

struct sphere: public object
{
    color clr;
    double ambient,diffuse,specular,reflection,shine;
    Vector3 center;
    double rad;
    sphere(){}
    sphere(Vector3 c,double r){center=c;rad=r;}
    void print(){center.print();cout<<rad<<endl;}
    double get_intersection(ray r)
    {
        Vector3 s=r.origin,d=r.direction,c=center;
        d.normalize();
        Vector3 diff=s.add(c.neg());
        double A=1,B=2*diff.dot(d),C=diff.dot(diff)-rad*rad;
        double det=B*B-4*A*C;
        if(det<0) return -1;
        double root1=(-B-sqrt(det))/2.0-1e-6;
        if(root1>0) return root1;
        double root2=(-B+sqrt(det))/2.0-1e-6;
        if(root2>0) return root2;
        return -1;
    }
    Vector3 get_normal(Vector3 pt)
    {
        Vector3 ret=pt.add(center.neg());
        ret.normalize();
        return ret;
    }
    color get_color(){return clr;}

};


struct plane: public object
{
    Vector3 normal,point;
    double ambient,diffuse,specular,reflection,shine;
    plane(){type=3;}
    plane(Vector3 nor,Vector3 pt){normal=nor,point=pt;type=3;}
    void print(){normal.print();point.print();}
    color clr;
    double get_intersection(ray r)
    {
        Vector3 s=r.origin,d=r.direction;
        d.normalize();
        Vector3 diff=point.add(s.neg());
        if(normal.dot(d)==0) return -1;
        //cout<<normal.dot(diff)<<" "<<norm.
        return (normal.dot(diff))/(normal.dot(d));
    }
    Vector3 get_normal(Vector3 pt){return normal;}
    color get_color(){return clr;}
};

struct limit
{
    double xmin,xmax,ymin,ymax,zmin,zmax;
    limit(){}
    limit(double a,double b,double c,double d,double e,double f)
    {
        xmin=a;xmax=b;
        ymin=c;ymax=d;
        zmin=e;zmax=f;
    }
    bool check_limit(double x,double y,double z)
    {
        if(x<xmin || x>xmax) return false;
        if(y<ymin || y>ymax) return false;
        if(z<zmin || z>zmax) return false;
        return true;
    }
};

struct cube: public object
{
    color clr;
    double ambient,diffuse,specular,reflection,shine;
    Vector3 lower;
    double dim;
    cube(){}
    cube(Vector3 low,double dd){lower=low;dim=dd;}
    void print(){lower.print();cout<<dim<<endl;}

    color get_color(){return clr;}
    Vector3 get_normal(Vector3 point)
    {
        double xmin=lower.x,xmax=lower.x+dim;
        double ymin=lower.y,ymax=lower.y+dim;
        double zmin=lower.z,zmax=lower.z+dim;

        limit limits[6];
        Vector3 normals[6];
        plane planes[6];

        planes[0]=plane(Vector3(0,1,0),Vector3(xmin,ymin,zmin));
        planes[1]=plane(Vector3(0,1,0),Vector3(xmin,ymax,zmin));
        planes[2]=plane(Vector3(1,0,0),Vector3(xmin,ymin,zmin));
        planes[3]=plane(Vector3(1,0,0),Vector3(xmax,ymin,zmin));
        planes[4]=plane(Vector3(0,0,1),Vector3(xmin,ymin,zmin));
        planes[5]=plane(Vector3(0,0,1),Vector3(xmin,ymin,zmax));
        limits[0]=limit(xmin,xmax,ymin,ymin,zmin,zmax);
        limits[1]=limit(xmin,xmax,ymax,ymax,zmin,zmax);
        limits[2]=limit(xmin,xmin,ymin,ymax,zmin,zmax);
        limits[3]=limit(xmax,xmax,ymin,ymax,zmin,zmax);
        limits[4]=limit(xmin,xmax,ymin,ymax,zmin,zmin);
        limits[0]=limit(xmin,xmax,ymin,ymax,zmax,zmax);

        for(int i=0;i<6;i++)
        {
            if(limits[i].check_limit(point.x,point.y,point.z)) return planes[i].normal;
        }
    }
    double get_intersection(ray r)
    {
        double xmin=lower.x,xmax=lower.x+dim;
        double ymin=lower.y,ymax=lower.y+dim;
        double zmin=lower.z,zmax=lower.z+dim;


        plane planes[6];
        limit limits[6];

        planes[0]=plane(Vector3(0,1,0),Vector3(xmin,ymin,zmin));
        planes[1]=plane(Vector3(0,1,0),Vector3(xmin,ymax,zmin));
        planes[2]=plane(Vector3(1,0,0),Vector3(xmin,ymin,zmin));
        planes[3]=plane(Vector3(1,0,0),Vector3(xmax,ymin,zmin));
        planes[4]=plane(Vector3(0,0,1),Vector3(xmin,ymin,zmin));
        planes[5]=plane(Vector3(0,0,1),Vector3(xmin,ymin,zmax));
        limits[0]=limit(xmin,xmax,ymin,ymin,zmin,zmax);
        limits[1]=limit(xmin,xmax,ymax,ymax,zmin,zmax);
        limits[2]=limit(xmin,xmin,ymin,ymax,zmin,zmax);
        limits[3]=limit(xmax,xmax,ymin,ymax,zmin,zmax);
        limits[4]=limit(xmin,xmax,ymin,ymax,zmin,zmin);
        limits[0]=limit(xmin,xmax,ymin,ymax,zmax,zmax);

        vector<double> intersect;
        Vector3 ori=r.origin,dir=r.direction;
        dir.normalize();
        for(int i=0;i<6;i++)
        {
            double d=planes[i].get_intersection(r);
            //cout<<planes[i].normal.dot(r.direction)<<endl;
            //cout<<d<<endl;
            if(d>0) intersect.push_back(d);
            else
            {
                Vector3 point=ori.add(dir.mult(d));
                if(limits[i].check_limit(point.x,point.y,point.z)) intersect.push_back(d);
                else intersect.push_back(-2);
            }
        }
        int mn=-2;
        for(i=0;i<6;i++)
        {
            if(intersect[i]<0) continue;
            if(mn<0) mn=intersect[i];
            else if(intersect[i]<mn) mn=intersect[i];
        }
        return mn;
    }

};
