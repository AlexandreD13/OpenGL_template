#version 460

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;
uniform float Time;

 out gl_PerVertex {
        vec4 gl_Position;
        float gl_PointSize;
        float gl_ClipDistance[];
    };

layout (location = 0) in vec3 Position;
layout (location = 2) in vec3 Normal;

out vec3 Color;

void main()
{
    vec3 deformation = Position + (Normal * abs(sin(Time / 1000))) * 0.01;

	gl_Position = Proj * View * Model * vec4(deformation,1.0);
 	
 	Color = abs(Normal);
}