#pragma once

struct Triangle;
struct NormalizedShape;
struct CenteredShape;
struct Shape;

#include "../math/vec2.h"
#include "../math/vec3.h"
#include "../math/mat3.h"
#include "../math/cframe.h"
#include "boundingBox.h"
#include <memory>


struct Sphere {
	Vec3 origin;
	double radius;
};

struct Triangle {
	union {
		struct { 
			unsigned int firstIndex, secondIndex, thirdIndex; 
		};
		unsigned int indexes[3];
	};

	bool sharesEdgeWith(Triangle other) const;
	Triangle rightShift() const;
	Triangle leftShift() const;
	Triangle operator~() const;
	bool operator==(const Triangle& other) const;
	inline unsigned int operator[](int i) const {
		return indexes[i]; 
	};
};

struct Shape {
	struct TriangleIter {
		const Triangle* first;
		int size;
		inline const Triangle* begin() const { 
			return first;
		};
		inline const Triangle* end() const { 
			return first + size; 
		};
	};

	struct VertexIter {
		Vec3f* first;
		int size;
		inline Vec3f* begin() const {
			return first; 
		};
		inline Vec3f* end() const { 
			return first + size;
		};
	};
private:
	Vec3f* vertices;
public:
	std::shared_ptr<Vec3> normals;
	std::shared_ptr<Vec2> uvs;
	const Triangle* triangles;
	int vertexCount;
	int triangleCount;

	Shape();
	Shape(Vec3f* vertices, const Triangle* triangles, int vertexCount, int triangleCount);
	Shape(Vec3f* vertices, Vec3* normals, const Triangle* triangles, int vertexCount, int triangleCount);
	Shape(Vec3f* vertices, Vec3* normals, Vec2* uvs, const Triangle* triangles, int vertexCount, int triangleCount);
	Shape(Vec3f* vertices, Vec2* uvs, const Triangle* triangles, int vertexCount, int triangleCount);

	Shape translated(Vec3f offset, Vec3f* newVecBuf) const;
	Shape rotated(RotMat3f rotation, Vec3f* newVecBuf) const;
	Shape localToGlobal(CFramef frame, Vec3f* newVecBuf) const;
	Shape globalToLocal(CFramef frame, Vec3f* newVecBuf) const;

	bool isValid() const;
	bool containsPoint(Vec3f point) const;
	float getIntersectionDistance(Vec3f origin, Vec3f direction);
	double getVolume() const;
	NormalizedShape normalized(Vec3f* vecBuf, Vec3* normalBuf, CFramef& backTransformation) const;
	CenteredShape centered(Vec3f* vecBuf, Vec3& backOffset) const;

	CFramef getInertialEigenVectors() const;
	BoundingBox getBounds() const;
	void computeNormals(Vec3* buffer) const;
	Vec3 getCenterOfMass() const;
	SymmetricMat3 getInertia() const;
	SymmetricMat3 getInertia(Vec3 reference) const;
	SymmetricMat3 getInertia(Mat3 reference) const;
	SymmetricMat3 getInertia(CFrame reference) const;
	Sphere getCircumscribedSphere() const;
	double getMaxRadius() const;
	Vec3f getNormalVecOfTriangle(Triangle triangle) const;

	bool intersects(const Shape& other, Vec3f& intersection, Vec3f& exitVector, const Vec3& centerConnection) const;
	bool intersectsTransformed(const Shape& other, const CFramef& relativeCFrame, Vec3f& intersection, Vec3f& exitVector) const;
	int furthestIndexInDirection(const Vec3f& direction) const;
	Vec3f furthestInDirection(const Vec3f& direction) const;

	inline const Vec3f& operator[](int index) const { return vertices[index]; }
	inline Vec3f& operator[](int index) { return vertices[index]; }

	inline TriangleIter iterTriangles() const { 
		return TriangleIter { triangles, triangleCount };
	};
	inline VertexIter iterVertices() const { 
		return VertexIter { vertices, vertexCount };
	};
};

struct CenteredShape : public Shape {
	friend struct Shape;
	CenteredShape() : Shape() {}
	CenteredShape(Vec3f * vertices, const Triangle * triangles, int vertexCount, int triangleCount);
	CenteredShape(Vec3f * vertices, Vec3 * normals, Vec2 * uvs, const Triangle * triangles, int vertexCount, int triangleCount);
private:
	CenteredShape(Vec3f * vertices, const Triangle * triangles, int vertexCount, int triangleCount, Vec3& offset);
};

struct NormalizedShape : public CenteredShape {
	friend struct Shape;
	NormalizedShape() : CenteredShape() {}
	NormalizedShape(Vec3f * vertices, const Triangle * triangles, int vertexCount, int triangleCount);
	NormalizedShape(Vec3f * vertices, Vec3 * normals, Vec2 * uvs, const Triangle * triangles, int vertexCount, int triangleCount);
private:
	NormalizedShape(Vec3f* vertices, const Triangle* triangles, int vertexCount, int triangleCount, CFramef& transformation);
};
