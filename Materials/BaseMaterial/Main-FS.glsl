#version 460

layout (location = 0) out vec4 Color;

in vec3 Normal;

void main()
{
	Color = vec4(Normal, 1.0);
}