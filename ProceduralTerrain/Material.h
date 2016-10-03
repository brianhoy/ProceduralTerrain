#pragma once
#include "Renderer.h"
class Material {
	friend class Renderer;

protected:
	virtual void use() = 0;
};