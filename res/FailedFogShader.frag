#version 400
in vec3 Position;
in vec3 Normal;
in vec2 texCoord0;
struct LightData
{
	vec4 position;
	vec3 intensity;
};

uniform LightData Light;

struct FogData
{
	float maxDistance;
	float minDistance;
	vec3 color;
};

uniform FogData Fog;
uniform vec3 DiffuseReflectivity;
uniform vec3 AmbientReflectivity;
uniform vec3 SpecularReflectivity;
uniform float Shininess;
layout (location = 1) out vec4 FragmentColor;

vec3 LightingValues()
{
	vec3 a = normalize (Light.position.xyz - Position.xyz);
	vec3 b = normalize (vec3(-Position));
	vec3 c = normalize (b+a);
	vec3 ambientLight = AmbientReflectivity * Light.intensity;
	vec3 diffuseLight = DiffuseReflectivity * Light.intensity * max(0.0, dot(a,Normal));
	vec3 specularLight = SpecularReflectivity * Light.intensity * pow(max(0.0, dot(c,Normal)),Shininess);
	return ambientLight + diffuseLight + specularLight;
}

void main()
{
	float distance = abs(Position.z);
	float fogStrength = (Fog.maxDistance- distance)/(Fog.maxDistance-Fog.minDistance);
	fogStrength = clamp(fogStrength, 0.0, 1.0);
	vec3 fogShade = LightingValues();
	vec3 color = mix(Fog.color, fogShade, fogStrength);
	FragmentColor = vec4(color, texCoord0);
}