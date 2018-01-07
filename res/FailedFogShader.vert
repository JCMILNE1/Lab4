#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;
in vec2 texCoord;


out vec2 texCoord0;
out vec3 Position;
out vec3 Normal;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 transform;

void main()
{
	Normal = normalize(NormalMatrix * VertexNormal);
	Position = vec3(ModelViewMatrix * vec4(VertexPosition, 1.0));
	gl_Position = transform * vec4(VertexPosition, 1.0);
	texCoord0 = texCoord;
}
