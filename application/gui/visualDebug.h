#pragma once

#include "../debug.h"
#include "../buffers.h"
#include "vectorMesh.h"
#include "profilerUI.h"
#include "font.h"

extern const char* const graphicsDebugLabels[];

enum GraphicsProcess {
	UPDATE,
	SKYBOX,
	VECTORS,
	PHYSICALS,
	LIGHTING,
	ORIGIN,
	PROFILER,
	FINALIZE,
	OTHER,
	COUNT
};

extern BreakdownAverageProfiler<60, GraphicsProcess> graphicsMeasure;

// Debug
using namespace Debug;
extern std::map<VecType, bool> debug_enabled;
extern std::map<VecType, double> vecColors;

extern bool renderPies;
extern int fieldIndex;

void toggleDebugVecType(Debug::VecType t);

template<typename T>
void renderDebugField(Vec2 dimension, Font* font, const char* varName, T value, const char* unit) {
	std::stringstream ss;
	ss.precision(4);
	ss << varName << ": " << value << unit;
	font->render(ss.str().c_str(), Vec2(-dimension.x / dimension.y * 0.99, (1 - fieldIndex * 0.05) * 0.95), Vec3f(1, 1, 1), 0.001);
	fieldIndex++;
}

std::string toString(std::chrono::nanoseconds t);
std::string toString(size_t v);

template<size_t N, typename EnumType>
PieChart toPieChart(BreakdownAverageProfiler<N, EnumType>& profiler, const char* title, Vec2f piePosition, float pieSize) {
	auto results = profiler.history.avg();
	std::chrono::nanoseconds avgTotalTime = results.sum();

	PieChart chart(title, toString(avgTotalTime), piePosition, pieSize);
	for (size_t i = 0; i < profiler.size(); i++) {
		float weight = static_cast<float>(results[i].count());
		DataPoint p = DataPoint(weight, toString(results[i]), pieColors[i], profiler.labels[i]);
		chart.add(p);
	}

	return chart;
}

template<size_t N, typename Unit, typename EnumType>
PieChart toPieChart(HistoricTally<N, Unit, EnumType>& tally, const char* title, Vec2f piePosition, float pieSize) {
	auto sum = 0;
	for (auto entry : tally.history) {
		sum += entry.sum();
	}
	auto results = tally.history.avg();
	Unit avgTotal = (tally.history.size() != 0) ? (sum / tally.history.size()) : 0;
	//Unit avgTotal = results.sum();

	PieChart chart(title, toString(avgTotal), piePosition, pieSize);
	for (size_t i = 0; i < tally.size(); i++) {
		float result = results[i] * 1.0f;
		DataPoint p = DataPoint(result, toString(results[i]), pieColors[i], tally.labels[i]);
		chart.add(p);
	}

	return chart;
}

size_t getTheoreticalNumberOfIntersections(size_t objCount);

extern AddableBuffer<double> visibleVecs;

void updateVecMesh(VectorMesh* vectorMesh, AppDebug::ColoredVec* data, size_t size);