#ifndef _Light_H
#define _Light_H

#include "DataStruct.h"

class Source {
	public:

	Source();

	virtual Vect getLightPosition() {return Vect(0, 0, 0);}
	virtual Color getLightColor() {return Color(1, 1, 1, 0);}

};

Source::Source() {}


class Light : public Source {
	Vect position;
	Color color;

	public:

	Light ();

	Light (Vect, Color);

	// method functions
	virtual Vect getLightPosition () { return position; }
	virtual Color getLightColor () { return color; }

};

Light::Light () {
	position = Vect(0,0,0);
	color = Color(1,1,1, 0);
}

Light::Light (Vect p, Color c) {
	position = p;
	color = c;
}

#endif
