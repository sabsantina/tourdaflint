#version 330 core

in vec3 outNormal;
in vec3 fragPosition;
in vec2 outUV;

uniform vec3 object_color;
uniform float alpha;

out vec4 color;

void main()
{
	color = vec4(object_color, alpha);
}