#pragma once
#include "Constants.h"
#include "Any.h"

namespace PT {

class Attribute {
	
	Attribute() {

	}

	PT::AttributeType type;
	PT::any data;
	int size;
	int itemSize;
	std::string 
};

}