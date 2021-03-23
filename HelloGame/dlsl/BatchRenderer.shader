#shader vertex
#version 330 core

layout(location = 0) in vec3	a_Position;
layout(location = 1) in vec4	a_Color;
layout(location = 2) in vec2	a_TexCoord;
layout(location = 3) in float	a_TexIndex;

uniform mat4 u_MVP;

out vec4	v_Color;
out vec2	v_TexCoord;
out float	v_TexIndex;

void main()
{
	v_Color		= a_Color;
	v_TexCoord	= a_TexCoord;
	v_TexIndex	= a_TexIndex;
	gl_Position = u_MVP * vec4(a_Position, 1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec4		v_Color;
in vec2		v_TexCoord;
in float	v_TexIndex;

uniform sampler2D u_Textures[8];

void main()
{
	//获取texture的索引: 0 表示texture-01.png; 1 表示texture-02.png
	int index = int(v_TexIndex);
	//--如果没有u_Textures，这个时候u_Textures实际上有一个空的u_Textures
	//	s_Data.WhiteTexture (1,1,1,1)相当于一个单位矩阵和v_Color相乘
	//	乘积等于v_Color,相当于只有v_Color起作用
	//--如果没有v_Color,这个是由Color实际上有一个空的Color
	//	const glm::vec4 color (1,1,1,1)相当于一个单位矩阵和u_Textures
	//	乘积等于u_Textures,相当于只有u_Textures起作用
	o_Color = texture(u_Textures[index], v_TexCoord) * v_Color;
};