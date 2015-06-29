#ifndef _SHAPES_H
#define _SHAPES_H


#include "math.h"
#include "DataStruct.h"

using namespace std;

class Object {
	public:

	Object ();

	// method functions
	virtual Color getColor () { return Color (0.0, 0.0, 0.0, 0); }

	virtual Vect getNormalAt(Vect intersection_position) {
		return Vect (0, 0, 0);
	}

	virtual double findIntersection(Ray ray) {
		return 0;
	}

};

Object::Object () {}


class Sphere : public Object {

	public:

	Vect center;
	double radius;
	double ambient,specular,diffuse,reflection,shineness;
	Color color;


	Sphere ();

	Sphere (Vect centerValue, double radiusValue, Color colorValue,double ambientValue, double diffuseValue,double specularValue,double reflectionValue, double shinenessValue);

	// method functions
	Vect getSphereCenter () { return center; }
	double getSphereRadius () { return radius; }
	virtual Color getColor () { return color; }

	virtual Vect getNormalAt(Vect point) {
		// normal always points away from the center of a sphere
		Vect normal_Vect = point.vectAdd(center.negative()).normalize();
		return normal_Vect;
	}

    virtual double getIntersection(Vect source, Vect direction)
    {
        //cout<<source.x<<" "<<source.y<<" "<<source.z<<endl;
        Vect difference = source.vectAdd((center.negative()));
        double a = 1;
        double b = 2*difference.dotProduct(direction);
        //cout<<b<<endl;
        double c = difference.dotProduct(difference)-radius*radius;
        //cout<<c<<endl;
        double det = b*b - a*c;

        if(det<0)return -1;
        //cout<<det<<endl;
        double root1 = ((-b - sqrt(det))/2.0)-1e-6;
        //cout<<root1<<endl;
        if(root1>0)return root1;
        double root2 = (-b + sqrt(det))/2.0-1e-6;
        if(root2>0)return root2;
        //cout<<root2<<endl;
        return -1;
    }

	virtual double findIntersection(Ray ray) {

		Vect ray_origin = ray.getRayOrigin();
		double ray_origin_x = ray_origin.getVectX();
		double ray_origin_y = ray_origin.getVectY();
		double ray_origin_z = ray_origin.getVectZ();

		Vect ray_direction = ray.getRayDirection();
		double ray_direction_x = ray_direction.getVectX();
		double ray_direction_y = ray_direction.getVectY();
		double ray_direction_z = ray_direction.getVectZ();

		Vect sphere_center = center;
		double sphere_center_x = sphere_center.getVectX();
		double sphere_center_y = sphere_center.getVectY();
		double sphere_center_z = sphere_center.getVectZ();

		double a = 1; // normalized
		double b = (2*(ray_origin_x - sphere_center_x)*ray_direction_x) + (2*(ray_origin_y - sphere_center_y)*ray_direction_y) + (2*(ray_origin_z - sphere_center_z)*ray_direction_z);
		double c = pow(ray_origin_x - sphere_center_x, 2) + pow(ray_origin_y - sphere_center_y, 2) + pow(ray_origin_z - sphere_center_z, 2) - (radius*radius);

		double discriminant = b*b - 4*c;

		if (discriminant > 0) {
			/// the ray intersects the sphere

			// the first root
			double root_1 = ((-1*b - sqrt(discriminant))/2) - 0.000001;

			if (root_1 > 0) {
				// the first root is the smallest positive root
				return root_1;
			}
			else {
				// the second root is the smallest positive root
				double root_2 = ((sqrt(discriminant) - b)/2) - 0.000001;
				return root_2;
			}
		}
		else {
			// the ray missed the sphere
			return -1;
		}
	}

};

Sphere::Sphere () {
	center = Vect(0,0,0);
	radius = 1.0;
	color = Color(0.5,0.5,0.5, 0);
	ambient = 0.0;
	diffuse = 0.0;
	specular = 0.0;
	reflection = 0.0;
	shineness = 0.0;
}

Sphere::Sphere (Vect centerValue, double radiusValue, Color colorValue,double ambientValue, double diffuseValue,double specularValue,double reflectionValue, double shinenessValue) {
	center = centerValue;
	radius = radiusValue;
	color = colorValue;
	ambient = ambientValue;
	diffuse = diffuseValue;
	specular = specularValue;
	reflection = reflectionValue;
	shineness = shinenessValue;
}

class Cube : public Object {

	public:

	Vect lowestPoint;
	double sides;
	double ambient,specular,diffuse,reflection,shineness;
	Color color;
	Cube ();

	Cube(Vect lowestPoint, double sides, Color colorValue,double ambientValue, double diffuseValue,double specularValue,double reflectionValue, double shinenessValue);

	// method functions
	Vect getCubeCenter () { return lowestPoint; }
	double getCubeRadius () { return sides; }
	virtual Color getColor () { return color; }


	virtual double findIntersection(Ray ray) {
		return 0;
	}

};

Cube::Cube () {
	lowestPoint = Vect(0,0,0);
	sides = 1.0;
	color = Color(0.5,0.5,0.5, 0);
	ambient = 0.0;
	diffuse = 0.0;
	specular = 0.0;
	reflection = 0.0;
	shineness = 0.0;
}

Cube::Cube (Vect lowestPointValue, double sidesValue, Color colorValue,double ambientValue, double diffuseValue,double specularValue,double reflectionValue, double shinenessValue) {
	lowestPoint = lowestPointValue;
	sides = sidesValue;
	color = colorValue;
	ambient = ambientValue;
	diffuse = diffuseValue;
	specular = specularValue;
	reflection = reflectionValue;
	shineness = shinenessValue;

}


class Plane : public Object {
	Vect normal;
	double distance;
	Color color;

	public:

	Plane ();

	Plane (Vect, double, Color);

	// method functions
	Vect getPlaneNormal () { return normal; }
	double getPlaneDistance () { return distance; }
	virtual Color getColor () { return color; }

	virtual Vect getNormalAt(Vect point) {
		return normal;
	}

	virtual double findIntersection(Ray ray) {
		Vect ray_direction = ray.getRayDirection();

		double a = ray_direction.dotProduct(normal);

		if (a == 0) {
			// ray is parallel to the plane
			return -1;
		}
		else {
			double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distance).negative()));
			return -1*b/a;
		}
	}

};

Plane::Plane () {
	normal = Vect(1,0,0);
	distance = 0;
	color = Color(0.5,0.5,0.5, 0);
}

Plane::Plane (Vect normalValue, double distanceValue, Color colorValue) {
	normal = normalValue;
	distance = distanceValue;
	color = colorValue;
}

#endif
