#include "core.h"

#include "testLayer.h"
#include "view/screen.h"
#include "../shader/shaders.h"
#include "../graphics/renderUtils.h"

TestLayer::TestLayer() : Layer() {}

TestLayer::TestLayer(Screen* screen, char flags) : Layer("TestLayer", screen, flags) {}

void TestLayer::onInit() {
	BufferLayout layout = BufferLayout({
		BufferElement("pos", BufferDataType::FLOAT3),
		BufferElement("col", BufferDataType::FLOAT4)
	});

	BatchConfig config = BatchConfig(layout, Renderer::LINES);
	batch = new Batch<Path3D::Vertex>(config);
}

void TestLayer::onUpdate() {

}

void TestLayer::onEvent(Event& event) {

}

void TestLayer::onRender() {
	Path3D::bind(batch);

	Path3D::lineTo(Vec3f(0));
	
	float size = 50.0;
	for (float i = 0.0; i <= size; i++) {
		float a =  2.0 * 3.14159265359 / size * i;
		Vec3f v = Vec3f(cos(a), sin(a), cos(a) * sin(a));
		Path3D::bezierTo(v, 20);
	}

	Path3D::stroke(Vec4f(1), 1);

	ApplicationShaders::lineShader.updateProjection(screen->camera.projectionMatrix, screen->camera.viewMatrix);
	Renderer::disableDepthTest();
	Path3D::batch->submit();
}

void TestLayer::onClose() {
	batch->close();
}
