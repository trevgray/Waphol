#pragma once
#include <irrKlang.h>
#include <unordered_map>
#include "Vector.h"

using namespace MATH;

enum SoundType {
	TwoD, ThreeD
};

class SoundManager {
public:
	SoundManager();
	~SoundManager();

	//graph functions
	void AddSound(std::string soundName, std::string fileName, bool playLooped, SoundType soundType, float volume = 1.0f, Vec3 pos = Vec3());
	void DeleteSound(std::string soundName);

	//play sounds
	void PlaySound2D(std::string soundName);
	//void PlaySound3D(std::string soundName);

	//Resume sounds
	void ResumeSound(std::string soundName);

	//stop sounds
	void PauseSound(std::string soundName);
	void StopAllSound();

	//custom functions
	void FadeOutSound(std::string soundName, float newVolume, float rateOfChange); //WIP

	void FadeInSound(std::string soundName, float newVolume, float rateOfChange); //WIP

	//sound settings - helper functions
	bool IsSoundCurrentlyPlaying(std::string soundName);

	float GetSoundVolume(std::string soundName);
	void SetSoundVolume(std::string soundName, float volume);

	bool GetSoundLooping(std::string soundName);
	void SetSoundLooping(std::string soundName, bool playLooped); //looping == true means the sound will loop

	bool IsSoundFinished(std::string soundName);

	unsigned int GetSoundPlayPosition(std::string soundName);
	void SetSoundPlayPosition(std::string soundName, unsigned int position);

	float GetSoundPlaybackSpeed(std::string soundName);
	void SetSoundPlaybackSpeed(std::string soundName, float speed); //default == 1.0f

	unsigned int GetSoundPlayLength(std::string soundName);
private:
	irrklang::ISoundEngine* irrKlangEngine;

	std::unordered_map<std::string, irrklang::ISound*> soundGraph;
	//std::unordered_map<std::string, Ref<irrklang::ISound>> soundGraph;

	auto FindSound(std::string soundName);
};