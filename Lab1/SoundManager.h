#pragma once

#include <glm\glm.hpp>
#include <AL\al.h>
#include <AL\alc.h>
#include <vector>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <SDL\SDL.h>
#include <glm/gtc/type_ptr.hpp>

class SoundManager
{
	struct SoundData {
		int sourcefileID, soundbufferID;
		char* buffer;
		std::string soundName;
		SoundData(unsigned int sI, unsigned int bI, char* b, const char* n)
		{
			this->sourcefileID = sI;
			this->soundbufferID = bI;
			this->buffer = b;
			this->soundName = n;
		}

	};

	struct Vector3
	{
		float x, y, z;
	};
	std::vector<SoundData> dataSets;
	ALCcontext* context;
	ALCdevice *device;
	bool TooLarge();
	int changeVarToInt(char* buffer, int length);
	char* loadSoundFile(const char* fn, int& chan, int& samplerate, int& bps, int& size);

public:
	SoundManager();
	~SoundManager();
	unsigned int ReadyClip(const char* filename);
	void RemoveClip(unsigned int id);
	void ActivateClip(unsigned int id);
	void ActivateClip(unsigned int id, glm::vec3& pos);
	void DeactivateClip(unsigned int id);
	void ReadyListener(glm::vec3& pos, glm::vec3& camLookAt);

private:

};
