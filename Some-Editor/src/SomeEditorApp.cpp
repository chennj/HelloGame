// ���ϵ�ַ //////////////////////////////////////////////
// �����ַ��github.com/TheCherno/hazel
// opengl ��ѧ��
// http://glm.g-truc.net/
// https://github.com/g-truc/glm
// opengl shader compilation
// http://www.khronos.org/opengl/wiki/Shader_Compilation
// С��Ϸ:
// https://github.com/freeman40/ChernoHazel
// ��Ϸ�ز�
// http://kenney.nl
// Entt / Entity Component System (ECS) FrameWork
// https://github.com/skypjack/entt
// ������վ
// https://www.patreon.com/thecherno
// OpenGLѧϰ��վ
// https://learnopengl.com/
// https://learnopengl-cn.github.io/
// -----------------------------------------------------
// ֪ʶ�� ///////////////////////////////////////////////
// -------------
// VertexBuffer��IndexBufferֻ�з���VertexArray�вŻ�������
// -------------
// ���ǹ۲쵽������	= Projection * View * Model * VerticesPosition(Mask)
//					= VP(PV) * Model * VerticesPosition(Mask)
//					= CAMERA * Model * VerticesPosition(Mask)
//					= MVP * VerticesPosition(Mask)
// Projection(Matrix) * View(Matrix) = Camera : ͶӰ�����󣩱任 * ��ͼ�����󣩱任 = �۲��λ��
// Model-Matrix = Object : ģ�ͱ任 = ����λ��
// -------------
// Timestep �����ã�ʹ����֮�������Ƿ�����ǿ�ƴ�ֱͬ����Ҳ����
// ˢ��Ƶ���Ƿ���ͬ�����ͼ�ε��ƶ�Ч������һ�µġ����忴
// ExampleLayer::OnUpdate �� _Window->SetVSync(false/true);
// git commit "TIMESTEP and Delta Time"
// -------------
// OpenGL Version 4.5�汾֮��������һ���µ���չ ARB_direct_state_access,
// �����չΪOpenGL������һ���µ����Ծ���Direct State Acess����ΪDSA���ڰ�
// �����ʱ�򣬺;ɵİ汾д����һ������OpenGLTexture2D�������֡�����DSA����Ϊ����
// ��ƴ����ܶ�ķ��㡣�������Ҫ���������Ƶ�ʱ���object���������ڸ��ֳ�ʼ��ʱ��
// Ҫ��������һ���̶��ϼ�����״̬���л��Ĵ�����
// -------------
// glEnable(GL_DEPTH_TEST) : ����Ĳ����ڵ������
// -------------
// texture(u_Texture, v_TexCoord * 10.0) : ��ʾ����ƽ��10��
// -------------
// Entity Component System (ECS)
// 
// ----------------------------------------------------
#include "sopch.h"

#include <Some.h>

// -- Entry Point ---------------------------
#include "stuff\core\EntryPoint.h"
// ------------------------------------------

#include "../imgui/imgui.h"

#include "platform\opengl\OpenGLShader.h"

#include "glm/gtc/matrix_transform.hpp"

#include "EditorLayer.h"

namespace SOMEENGINE
{

	class SomeEditor : public Application
	{
	public:
		//Sandbox()
		//{
		//	PushLayer(new Sandbox2D());
		//}

		SomeEditor(const std::string& filePath)
			:Application(filePath,"Some Engine Editor")
		{
			PushLayer(new EditorLayer());
		}

		~SomeEditor()
		{

		}
	};

	Application* CreateApplication()
	{
		return new SomeEditor("../GameEngine/assets/shaders/Texture.glsl");
	}
}