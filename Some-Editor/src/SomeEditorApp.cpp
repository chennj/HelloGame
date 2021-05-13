// 资料地址 //////////////////////////////////////////////
// 引擎地址：github.com/TheCherno/hazel
// opengl 数学库
// http://glm.g-truc.net/
// https://github.com/g-truc/glm
// opengl shader compilation
// http://www.khronos.org/opengl/wiki/Shader_Compilation
// 小游戏:
// https://github.com/freeman40/ChernoHazel
// 游戏素材
// http://kenney.nl
// Entt / Entity Component System (ECS) FrameWork
// https://github.com/skypjack/entt
// 作者网站
// https://www.patreon.com/thecherno
// OpenGL学习网站
// https://learnopengl.com/
// https://learnopengl-cn.github.io/
// -----------------------------------------------------
// 知识点 ///////////////////////////////////////////////
// -------------
// VertexBuffer、IndexBuffer只有放入VertexArray中才会起作用
// -------------
// 我们观察到的物体	= Projection * View * Model * VerticesPosition(Mask)
//					= VP(PV) * Model * VerticesPosition(Mask)
//					= CAMERA * Model * VerticesPosition(Mask)
//					= MVP * VerticesPosition(Mask)
// Projection(Matrix) * View(Matrix) = Camera : 投影（矩阵）变换 * 视图（矩阵）变换 = 观察点位置
// Model-Matrix = Object : 模型变换 = 物体位置
// -------------
// Timestep 的作用：使用它之后，无论是否设置强制垂直同步，也不管
// 刷新频率是否相同，最后图形的移动效果都是一致的。具体看
// ExampleLayer::OnUpdate 和 _Window->SetVSync(false/true);
// git commit "TIMESTEP and Delta Time"
// -------------
// OpenGL Version 4.5版本之后，引入了一个新的扩展 ARB_direct_state_access,
// 这个扩展为OpenGL引入了一个新的特性就是Direct State Acess，称为DSA。在绑定
// 纹理的时候，和旧的版本写法不一样，在OpenGLTexture2D中有体现。有了DSA可以为程序
// 设计带来很多的方便。你仅仅需要在真正绘制的时候绑定object，而不是在各种初始化时就
// 要绑定它，从一定程度上减少了状态机切换的次数。
// -------------
// glEnable(GL_DEPTH_TEST) : 上面的不会遮挡下面的
// -------------
// texture(u_Texture, v_TexCoord * 10.0) : 表示纹理平铺10次
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