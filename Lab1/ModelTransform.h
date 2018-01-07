#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "camera.h"

struct ModelTransform
{
public:
	ModelTransform(const glm::vec3& position = glm::vec3(), const glm::vec3& rotation = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f))
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}

	inline glm::mat4 GetCurrentModel() const
	{
		glm::mat4 positionMatrix = glm::translate(position);
		glm::mat4 scaleMatrix = glm::scale(scale);
		glm::mat4 rotationXaxis = glm::rotate(rotation.x, glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 ratationYaxis = glm::rotate(rotation.y, glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 rotationZaxis = glm::rotate(rotation.z, glm::vec3(0.0, 0.0, 1.0));
		glm::mat4 RotationMatrix = rotationXaxis * ratationYaxis * rotationZaxis;

		return positionMatrix * RotationMatrix * scaleMatrix;
	}

	inline glm::vec3* GetPosition() { return &position; } //getters
	inline glm::vec3* GetRotation() { return &rotation; }
	inline glm::vec3* GetScale() { return &scale; }

	inline void SetPos(glm::vec3& position) { this->position = position; } // setters
	inline void SetRot(glm::vec3& rotation) { this->rotation = rotation; }
	inline void SetScale(glm::vec3& scale) { this->scale = scale; }
protected:
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};


