// Basic Texture Shader

#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
			
uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
			
out vec2 v_TexCoord;
out vec2 v_ScreenPos;
			
void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position,1.0);
	v_ScreenPos = gl_Position.xy;
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;
			
in vec2 v_TexCoord;	
in vec2 v_ScreenPos;	

uniform vec4 u_Color;
uniform sampler2D u_Texture;
			
void main()
{
	// 计算屏幕上的点到屏幕中心的距离
	float dist = 1.0f - distance(v_ScreenPos * 0.8f, vec2(0.0f));
	// 确保dist的值确实在0~1之间
	dist = clamp(dist, 0.0f, 1.0f);	
	// 距离开平方,减缓color变化的速度
	// 用它乘以颜色，效果是距离中心点越远，颜色越暗
	dist = sqrt(dist);
	color = texture(u_Texture, v_TexCoord) * u_Color * dist;
}