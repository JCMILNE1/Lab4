#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct Camera
{
public:
	Camera()
	{
	}

	void CreateGameCamera(const glm::vec3& position, float fov, float aspect, float nearClip, float farClip)    //sets up the camera, its position and orentation and the area it renders
	{
		this->CameraPosition = position;
		this->CameraForward = glm::vec3(0.0f, 0.0f, 1.0f);
		this->CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		this->CameraProjectionArea = glm::perspective(fov, aspect, nearClip, farClip);
	}

	inline glm::mat4 GetViewProjection() const
	{
		return CameraProjectionArea * glm::lookAt(CameraPosition, CameraPosition + CameraForward, CameraUp);
	}

	//these methods allow the player to controll the camera
	void ForwardBack(float magnitude)
	{
		CameraPosition += CameraForward * magnitude;
	}

	void RightLeft(float magnitude)
	{
		CameraPosition += glm::cross(CameraUp, CameraForward) * magnitude;
	}

	void Pitch(float angle)
	{
		glm::vec3 right = glm::normalize(glm::cross(CameraUp, CameraForward));

		CameraForward = glm::vec3(glm::normalize(glm::rotate(angle, right) * glm::vec4(CameraForward, 0.0)));
		CameraUp = glm::normalize(glm::cross(CameraForward, right));
	}

	void Turn(float angle)
	{
		static const glm::vec3 UP(0.0f, 1.0f, 0.0f);

		glm::mat4 rotation = glm::rotate(angle, UP);

		CameraForward = glm::vec3(glm::normalize(rotation * glm::vec4(CameraForward, 0.0)));
		CameraUp = glm::vec3(glm::normalize(rotation * glm::vec4(CameraUp, 0.0)));
	}

protected:
private:
	glm::mat4 CameraProjectionArea;
	glm::vec3 CameraPosition;
	glm::vec3 CameraForward;
	glm::vec3 CameraUp;
};


