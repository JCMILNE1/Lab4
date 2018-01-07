#include "Soundmanager.h"



SoundManager::SoundManager()
{
	device = alcOpenDevice(NULL); //open sound card
	if (device == NULL)
	{
		std::cout << "cannot open sound card" << std::endl;
	}

	context = alcCreateContext(device, NULL);

	if (context == NULL)
	{
		std::cout << "cannot open context" << std::endl;
	}

	alcMakeContextCurrent(context);
}

SoundManager::~SoundManager()
{
	for (unsigned int i = 0; i < dataSets.size(); i++)
	{
		alDeleteSources(1, (unsigned int*)&dataSets[i].sourcefileID);
		if (dataSets[i].soundbufferID != -1)
		{
			alDeleteBuffers(1, (unsigned int*)&dataSets[i].soundbufferID);
			delete[] dataSets[i].buffer;
		}
	}
	alcDestroyContext(context);
	alcCloseDevice(device);
}

bool SoundManager::TooLarge()
{
	int i = 1;
	return !((char*)&i)[0];
}

int SoundManager::changeVarToInt(char* buffer, int length)
{
	int a = 0;
	if (!TooLarge())
		for (int i = 0; i < length; i++)
			((char*)&a)[i] = buffer[i];
	else
		for (int i = 0; i < length; i++)
			((char*)&a)[3 - i] = buffer[i];
	return a;
}

char* SoundManager::loadSoundFile(const char* fn, int& chan, int& samplerate, int& bps, int& size)
{
	char buffer[4];
	std::ifstream in(fn, std::ios::binary);
	in.read(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 4); //WAV
	in.read(buffer, 4);	//fmt
	in.read(buffer, 4); //16 bit
	in.read(buffer, 2); //1
	in.read(buffer, 2);
	chan = changeVarToInt(buffer, 2);
	in.read(buffer, 4);
	samplerate = changeVarToInt(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 2);
	in.read(buffer, 2);
	bps = changeVarToInt(buffer, 2);
	in.read(buffer, 4);
	in.read(buffer, 4);
	size = changeVarToInt(buffer, 4);

	char* soundData = new char[size];
	in.read(soundData, size);
	return soundData;
}

unsigned int SoundManager::ReadyClip(const char* filename)
{
	bool clipPresent = false;
	unsigned int sourcefileID, soundbufferID;
	char* soundData = NULL;
	for (unsigned int i = 0; i < dataSets.size(); i++)
	{
		if (dataSets[i].soundName == filename && dataSets[i].soundbufferID != -1)
		{
			soundbufferID = dataSets[i].soundbufferID;
			clipPresent = true;
			break;
		}
	}
	if (!clipPresent)
	{
		int channel, sampleRate, bps, size;
		soundData = loadSoundFile(filename, channel, sampleRate, bps, size);
		unsigned int format;
		alGenBuffers(1, &soundbufferID);

		if (channel == 1)
		{
			if (bps == 8)
			{
				format = AL_FORMAT_MONO8;
			}
			else
			{
				format = AL_FORMAT_MONO16;
			}
		}
		else
		{
			if (bps == 8)
			{
				format = AL_FORMAT_STEREO8;
			}
			else
			{
				format = AL_FORMAT_STEREO16;
			}

		}

		alBufferData(soundbufferID, format, soundData, size, sampleRate);
	}
	alGenSources(1, &sourcefileID);
	alSourcei(sourcefileID, AL_BUFFER, soundbufferID);
	alSourcef(sourcefileID, AL_REFERENCE_DISTANCE, 1.0f);
	dataSets.push_back(SoundData(sourcefileID, (!clipPresent ? soundbufferID : -1), soundData, filename));
	return sourcefileID;
}

void SoundManager::RemoveClip(unsigned int id)
{}

void SoundManager::ActivateClip(unsigned int id)
{
	alSourcePlay(id);
}
void SoundManager::ActivateClip(unsigned int id, glm::vec3& pos)
{
	alSource3f(id, AL_POSITION, pos.x, pos.y, pos.z);
	alSourcePlay(id);
}
void SoundManager::DeactivateClip(unsigned int id)
{
	alSourceStop(id);
}
void SoundManager::ReadyListener(glm::vec3& pos, glm::vec3& camLookAt)
{
	alListener3f(AL_POSITION, pos.x, pos.y, -pos.z);
	float orinet[6] = { camLookAt.x,camLookAt.y,camLookAt.z,0,1,0 };
	alListenerfv(AL_ORIENTATION, orinet);
}