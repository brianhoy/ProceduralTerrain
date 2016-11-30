#pragma once
#include "Constants.h"
#include "Any.h"

namespace PT {

class Attribute {
	
	Attribute() {

	}

	PT::AttributeType type;
	PT::any data; //vec of GLint/GLfloat...
	int size;
	int itemSize;
	std::string 
};

}