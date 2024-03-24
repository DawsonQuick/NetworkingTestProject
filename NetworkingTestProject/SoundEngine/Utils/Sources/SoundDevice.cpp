#include "./../Headers/SoundDevice.h"
#include <AL\al.h>
#include <stdio.h>
#include <iostream>
std::unique_ptr<SoundDevice> SoundDevice::snd_device;
SoundDevice* SoundDevice::get()
{
	if (!snd_device) {
		snd_device.reset(new SoundDevice());
	}
	return snd_device.get();
}

SoundDevice::SoundDevice()
{
	p_ALCDevice = alcOpenDevice(nullptr); // nullptr = get default device
	if (!p_ALCDevice)
		throw("failed to get sound device");

	p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);  // create context
	if(!p_ALCContext)
		throw("Failed to set sound context");

	if (!alcMakeContextCurrent(p_ALCContext))   // make context current
		throw("failed to make context current");

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	if (!name || alcGetError(p_ALCDevice) != AL_NO_ERROR)
		name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);
	printf("Opened \"%s\"\n", name);
}

SoundDevice::~SoundDevice()
{
	alcMakeContextCurrent(nullptr); // No need to throw exception, just set to nullptr

	if (p_ALCContext) {
		alcDestroyContext(p_ALCContext);
		p_ALCContext = nullptr; // Reset the pointer after destruction
	}

	if (p_ALCDevice) {
		alcCloseDevice(p_ALCDevice);
		p_ALCDevice = nullptr; // Reset the pointer after closure
	}
}
