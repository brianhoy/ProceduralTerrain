#pragma once
#include "Attribute.h"
#include "Uniform.h"
#include <vector>

using namespace PT;

namespace PT {

class Material {
public:
	virtual void get() = 0;

	std::vector<Uniform> uniforms = std::vector<Uniform>();
};

}