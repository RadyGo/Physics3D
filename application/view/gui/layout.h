#pragma once

#include "../engine/math/vec2.h"

class Container;

enum class Align {
	FILL,
	RELATIVE
};

class Layout {
public:
	virtual Vec2 resize(Container* container, Vec2 minDimension = Vec2()) = 0;
};

class FlowLayout : public Layout {
public:
	Vec2 resize(Container* container, Vec2 minDimension = Vec2()) override;
};