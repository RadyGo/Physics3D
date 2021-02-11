#include "shapeCreation.h"

#include "shapeClass.h"
#include "polyhedron.h"
#include "builtinShapeClasses.h"

#include "../../util/cpuid.h"

Shape sphereShape(double radius) {
	return Shape(&SphereClass::instance, radius * 2, radius * 2, radius * 2);
}

Shape cylinderShape(double radius, double height) {
	return Shape(&CylinderClass::instance, radius * 2, radius * 2, height);
}

Shape boxShape(double width, double height, double depth) {
	return Shape(&CubeClass::instance, width, height, depth);
}

Shape polyhedronShape(const Polyhedron& poly) {
	BoundingBox bounds = poly.getBounds();
	Vec3 center = bounds.getCenter();
	DiagonalMat3 scale{2 / bounds.getWidth(), 2 / bounds.getHeight(), 2 / bounds.getDepth()};

	PolyhedronShapeClass* shapeClass;

	if(Util::CPUIDCheck::hasTechnology(Util::CPUIDCheck::AVX | Util::CPUIDCheck::AVX2 | Util::CPUIDCheck::FMA)) {
		shapeClass = new PolyhedronShapeClassAVX(poly.translatedAndScaled(-center, scale));
	} else if(Util::CPUIDCheck::hasTechnology(Util::CPUIDCheck::SSE | Util::CPUIDCheck::SSE2)) {
		if(Util::CPUIDCheck::hasTechnology(Util::CPUIDCheck::SSE4_1)) {
			shapeClass = new PolyhedronShapeClassSSE4(poly.translatedAndScaled(-center, scale));
		} else {
			shapeClass = new PolyhedronShapeClassSSE(poly.translatedAndScaled(-center, scale));
		}
	} else {
		shapeClass = new PolyhedronShapeClassFallback(poly.translatedAndScaled(-center, scale));
	}

	return Shape(shapeClass, bounds.getWidth(), bounds.getHeight(), bounds.getDepth());
}
