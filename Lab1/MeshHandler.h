#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <string>
#include "Modelloader.h"

struct VertexClass
{
public:
	VertexClass(const glm::vec3& VertexPosition, const glm::vec2& VertexTextureCoordinates)
	{
		this->position = VertexPosition;
		this->textureCoordinates = VertexTextureCoordinates;
		this->normal = normal;
	}
	glm::vec3* GetPosition() { return &position; }
	glm::vec2* GetTextureCoord() { return &textureCoordinates; }
	glm::vec3* GetNormal() { return &normal; }

private:
	glm::vec3 position;
	glm::vec2 textureCoordinates;
	glm::vec3 normal;
};

struct CollisionSphere
{
public:
	CollisionSphere() 
	{
	
	}

	CollisionSphere(glm::vec3& SpherePosition, float SphereRadius)
	{
		this->position = SpherePosition;
	}

	glm::vec3 GetPosition() { return position; }
	float GetRadius() { return radius; }

	void SetPosition(glm::vec3 position)
	{
		this->position = position;
	}

	void SetRadius(float radius)
	{
		this->radius = radius;
	}

private:
	glm::vec3 position;
	float radius;
};

class MeshHandler
{
public:
	MeshHandler();
	~MeshHandler();
	void drawMesh();
	void InitaliseMesh(VertexClass* ModelVertices, unsigned int VertexCount, unsigned int* modelIndices, unsigned int IndicesCount);
	void LoadMeshFromFile(const std::string& fileTitle);
	void InitaliseModel(const StoredModel& model);
	void updateSphereData(glm::vec3 position, float radius);
	glm::vec3 getSpherePosition() { return modelSphere.GetPosition(); }
	float getSphereRadius() { return modelSphere.GetRadius(); }
private:
	enum
	{
		POSITION_VB,
		TEXTURECOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		BUFFER_COUNT
	};
	CollisionSphere modelSphere;
	GLuint vertexArray_OBJ;
	GLuint vertexArrayBuffers[BUFFER_COUNT]; // create our array of buffers
	unsigned int drawCount; //how much of the vertexArrayObject do we want to draw
};