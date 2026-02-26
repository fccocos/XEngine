#include "xepch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
namespace XEngine {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}