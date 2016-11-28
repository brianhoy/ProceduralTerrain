#pragma once

/* stolen from https://github.com/jdduke/three_cpp/blob/master/three/constants.hpp */

namespace PT {

enum UniformType {
	c = 0,
	i,
	iv,
	iv1,
	f,
	fv,
	fv1,
	v2,
	v3,
	v4,
	v2v,
	v3v,
	v4v,
	t,
	tv,
	m4,
	m4v,
	INVALID_UNIFORM,
	INVALID_ATTRIBUTE = INVALID_UNIFORM
};

typedef UniformType AttributeType;

}

