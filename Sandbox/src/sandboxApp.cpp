#include <xepch.h>
#include <XEngine.h>

class ExampleLayer : public XEngine::Layer {
public:
	ExampleLayer() :Layer("Example") {}
	void OnUpdate() override {
		if (XEngine::Input::IsKeyPressed(XE_KEY_TAB))
			XE_INFO("Tab key is pressed!");
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