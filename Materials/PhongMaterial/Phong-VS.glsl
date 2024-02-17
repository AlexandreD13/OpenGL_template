#version 460

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;
uniform vec3 LightPosition;
uniform vec3 CameraPosition;

out gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
};

layout (location = 0) in vec3 Position;
layout (location = 2) in vec3 Normal;

out vectors {
    vec3 L;
    vec3 V;
    vec3 fragNormal;
};

void main()
{
    L = LightPosition - Position;
    V = CameraPosition - Position;

    fragNormal = Normal;

	gl_Position = Proj * View * Model * vec4(Position, 1.0);
 }