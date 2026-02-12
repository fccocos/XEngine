#include <XEngine.h>

class Sandbox :public XEngine::Application {
public:
	Sandbox() {}
	~Sandbox() {}
};



XEngine::Application* XEngine::CreateApplication() {
	return new Sandbox();
}