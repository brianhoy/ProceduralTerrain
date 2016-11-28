#pragma once

#include "Constants.h"
#include "Any.h"

namespace PT {

class Uniform {

	Uniform() {

	}

	PT::UniformType type;
	PT::any data;
	int size;
	int itemSize;

};

}