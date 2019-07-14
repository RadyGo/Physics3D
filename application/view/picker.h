#pragma once

#include "screen.h"

#include "../engine/math/vec2.h"

namespace Picker {

	enum class EditMode {
		TRANSLATE,
		ROTATE,
		SCALE
	};

	extern EditMode editMode;

	void init();
	void update(Screen& screen, Vec2 mousePosition);
	void render(Screen& screen);

	void press(Screen& screen);
	void release(Screen& screen);
	void drag(Screen& screen);

	void moveGrabbedPhysicalTransversal(Screen& screen, double dz);
	void moveGrabbedPhysicalLateral(Screen& screen);
}