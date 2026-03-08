#pragma once
// For Application
#include "XEngine/Core/Application.h"
#include "XEngine/Core/Layer.h"
#include "XEngine/Core/log.h"
#include "XEngine/Core/core.h"
#include "XEngine/Core/TimeStep.h"

#include "XEngine/Core/Input.h"
#include "XEngine/Core/KeyCodes.h"
#include "XEngine/Core/MouseButtonCodes.h"
#include "XEngine/ImGui/ImGuiLayer.h"

//-------------- Renderer ------------
#include "XEngine/Renderer/Shader.h"
#include "XEngine/Renderer/Texture.h"
#include "XEngine/Renderer/Buffer.h"
#include "XEngine/Renderer/Renderer.h"
#include "XEngine/Renderer/Renderer2D.h"
#include "XEngine/Renderer/RenderCommand.h"
#include "XEngine/Renderer/VertexArray.h"
#include "XEngine/Renderer/Framebuffer.h"
#include "XEngine/OrthographicCameraController.h"
#include "XEngine/Renderer/Camera.h"
//-------------------------------------

//----------------scene and ECS----------------
#include "XEngine/Scene/Scene.h"
#include "XEngine/Scene/Components.h"
#include "XEngine/Scene/Entity.h"

//---------------- script -------------------
#include "XEngine/Scene/ScriptableEnttity.h"

//------------- Platform -----------------
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLTexture.h"
//--------------------------------------

//-------------Particle System---------------------
#include "XEngine/Renderer/ParticleSystem/Particle.h"
#include "XEngine/Renderer/ParticleSystem/ParticleRulesConfig.h"
//--------------------------------------------------

#include "XEngine/Debug/Instrumentor.h"


