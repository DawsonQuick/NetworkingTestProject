#pragma once
#include <AL\alc.h>
#include <memory>
class SoundDevice
{
public:
	static SoundDevice* get();

	SoundDevice();
	~SoundDevice();
private:

	static std::unique_ptr<SoundDevice> snd_device;

	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;

};

