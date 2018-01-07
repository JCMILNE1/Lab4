#include "MeshHandler.h"
#include <vector>


void MeshHandler::InitaliseMesh(VertexClass* ModelVertices, unsigned int VerticesCount, unsigned int* modelIndices, unsigned int IndicesCount)
{
	StoredModel currentModel;

	for (unsigned int i = 0; i < VerticesCount; i++)
	{
		currentModel.modelPositions.push_back(*ModelVertices[i].GetPosition());
		currentModel.modelTextureCoordinates.push_back(*ModelVertices[i].GetTextureCoord());
		currentModel.modelNormals.push_back(*ModelVertices[i].GetNormal());
	}

	for (unsigned int i = 0; i < IndicesCount; i++)
		currentModel.modelIndices.push_back(modelIndices[i]);

	InitaliseModel(currentModel);
}

void MeshHandler::InitaliseModel(const StoredModel& currentModel)
{

	drawCount = currentModel.modelIndices.size();

	glGenVertexArrays(1, &vertexArray_OBJ); //generate a vertex array and store it in the VAO
	glBindVertexArray(vertexArray_OBJ); //bind the VAO (any operation that works on a VAO will work on our bound VAO - binding)

	glGenBuffers(BUFFER_COUNT, vertexArrayBuffers); //generate our buffers based of our array of data/buffers - GLuint vertexArrayBuffers[BUFFER_COUNT];

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ARRAY_BUFFER, currentModel.modelPositions.size() * sizeof(currentModel.modelPositions[0]), &currentModel.modelPositions[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU (determined by type)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[TEXTURECOORD_VB]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ARRAY_BUFFER, currentModel.modelPositions.size() * sizeof(currentModel.modelTextureCoordinates[0]), &currentModel.modelTextureCoordinates[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(currentModel.modelNormals[0]) * currentModel.modelNormals.size(), &currentModel.modelNormals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDEX_VB]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentModel.modelIndices.size() * sizeof(currentModel.modelIndices[0]), &currentModel.modelIndices[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU
	
	glBindVertexArray(0); // unbind our VAO
}

MeshHandler::MeshHandler()
{
	drawCount = NULL;
}

void MeshHandler::LoadMeshFromFile(const std::string& filename)
{
	StoredModel currentModel = ModelObject(filename).ToStoredModel();
	InitaliseModel(currentModel);
	CollisionSphere modelSphere();
}



MeshHandler::~MeshHandler()
{
	glDeleteVertexArrays(1, &vertexArray_OBJ); // delete arrays
}

void MeshHandler::drawMesh()
{
	glBindVertexArray(vertexArray_OBJ);

	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
	

	glBindVertexArray(0);
}

void MeshHandler::updateSphereData(glm::vec3 position, float radius)
{
	modelSphere.SetPosition(position);
	modelSphere.SetRadius(radius);
}


