#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor; // 平铺因子
layout(location = 5) in float a_MixFactor;    // 颜色与纹理的混合因子

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexIndex;
out float v_TilingFactor;
out float v_MixFactor;

uniform mat4 u_ViewProjection;

void main(){
	gl_Position =  u_ViewProjection * vec4(a_Position, 1.0f);
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
	v_TexIndex =  a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	v_MixFactor = a_MixFactor;
}	


#type fragment
#version 330 core
out vec4 FragColor;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;
in float v_TilingFactor;
in float v_MixFactor;

uniform sampler2D u_Textures[32];
void main(){
	FragColor = mix(texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_TilingFactor ) , v_Color, v_MixFactor);	
}
