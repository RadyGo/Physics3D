#pragma once

#include "../quad.h"
#include "../font.h"
#include "../shaderProgram.h"
#include "../../../engine/math/vec3.h"
#include "../../../engine/math/mat4.h"


namespace GUI {
	// Shader
	extern Quad* defaultQuad;
	extern GUIShader* defaultShader;

	// Label
	extern double defaultLabelOffset;
	extern Vec4 defaultLabelBackgroundColor;

	// Panel
	extern double defaultPanelOffset;
	extern double defaultPanelTitleBarHeight;
	extern Vec4 defaultPanelTitleBarColor;
	extern Vec4 defaultPanelBackgroundColor;

	// Font
	extern Font* defaultFont;
	extern Vec4 defaultFontColor;
	extern double defaultFontSize;

	void init(GUIShader* shader, Font* font);
	void update(Mat4 orthoMatrix);
};