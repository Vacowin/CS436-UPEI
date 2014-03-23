#version 150
in vec4 v_color;
in vec2 v_uv1;

out vec4 PixelColor;

uniform sampler2D tex;
uniform vec4 color;

void main()
{	
	PixelColor =  texture(tex, v_uv1) * v_color ;
}


