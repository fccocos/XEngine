#include "xepch.h"
#include "XEngine/Core/Input.h"
#include "XEngine/Core/Application.h"
#include <GLFW/glfw3.h>

namespace XEngine {

	bool Input::IsKeyPressed(int keycode) {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	
	bool Input::IsMouseButtonPressed(int button) {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	std::pair<float, float> Input::GetMousePosition() {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos{}, ypos{};
		glfwGetCursorPos(window, &xpos, &ypos);
		return { static_cast<float>(xpos), static_cast<float>(ypos) };
	}

	float Input::GetMouseX() {
		auto [x, y] = GetMousePosition();
		return x;
	}
	float Input::GetMouseY() {
		auto [x, y] = GetMousePosition();
		return y;
	}
}