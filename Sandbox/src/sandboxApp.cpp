#include <xepch.h>
#include <XEngine.h>

class ExampleLayer : public XEngine::Layer {
public:
	ExampleLayer() :Layer("Example"){}
	void OnUpdate() override {
		XE_INFO("ExampleLayer::Update");
	}
	void OnEvent(XEngine::Event& e) override {
		XE_TRAC("{0}", e.ToString());
	}
};

class Sandbox :public XEngine::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}
};



XEngine::Application* XEngine::CreateApplication() {
	return new Sandbox();
}