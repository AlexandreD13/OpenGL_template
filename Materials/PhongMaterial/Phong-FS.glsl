#version 460

layout (location = 0) out vec4 Color;

uniform float AmbientReflectionCoefficient;
uniform float DiffuseReflectionCoefficient;
uniform float SpecularReflectionCoefficient;
uniform float ConeSize;

uniform vec3 AmbientReflectionColor;
uniform vec3 DiffuseReflectionColor;
uniform vec3 LightColor;

float AmbientIntensity;
float DiffuseIntensity;
float SpecularIntensity;

vec3 R;
vec3 Lighting;
vec3 normL;
vec3 normV;
vec3 normN;

in vectors {
    vec3 L;
    vec3 V;
    vec3 fragNormal;
};

void main()
{
    normL = normalize(L);
    normV = normalize(V);
    normN = normalize(fragNormal);
    R = normalize(reflect(-normL, normN));

    AmbientIntensity = AmbientReflectionCoefficient;
    DiffuseIntensity = DiffuseReflectionCoefficient * max(dot(normN, normL), 0.0);
    SpecularIntensity = SpecularReflectionCoefficient * pow(max(dot(R, normV), 0.0), ConeSize);

    Lighting = AmbientIntensity * AmbientReflectionColor + (DiffuseIntensity * DiffuseReflectionColor + vec3(SpecularIntensity)) * LightColor;

    Color = vec4(Lighting, 1.0);
}