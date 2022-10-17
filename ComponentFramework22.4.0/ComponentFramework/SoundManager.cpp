#include "SoundManager.h"

SoundManager::SoundManager() {
	irrKlangEngine = irrklang::createIrrKlangDevice();
}

SoundManager::~SoundManager() {
	irrKlangEngine->drop(); //deletes the engine
	//delete irrKlangEngine;
	irrKlangEngine = nullptr;
}

auto SoundManager::FindSound(std::string soundName) { //helper function
	auto id = soundGraph.find(soundName);
	if (id == soundGraph.end()) { //if the sound was not in the graph
		std::cout << ("Can't find requested sound", __FILE__, __LINE__) << std::endl;
	}
	return id;
}

void SoundManager::AddSound(std::string soundName, std::string fileName, bool playLooped, SoundType soundType, float volume, Vec3 pos) {
	if (soundType == TwoD) {
		//soundGraph[soundName] = std::make_shared<irrklang::ISound>(irrKlangEngine->play2D(fileName.c_str(), playLooped, true, true));
		soundGraph[soundName] = irrKlangEngine->play2D(fileName.c_str(), playLooped, true, true);
		soundGraph[soundName]->setVolume(volume);
		return;
	}
	else { //3d
		irrklang::vec3df position;
		position.X = pos.x;
		position.Y = pos.y;
		position.Z = pos.z;

		soundGraph[soundName] = irrKlangEngine->play3D(fileName.c_str(), position, playLooped, true, true);
		soundGraph[soundName]->setVolume(volume);
		return;
	}
}

void SoundManager::DeleteSound(std::string soundName) {
	auto id = FindSound(soundName);
	id->second->stop();
	id->second->drop();
	soundGraph.erase(soundName);
}

void SoundManager::PlaySound2D(std::string soundName) {
	auto id = soundGraph.find(soundName); //this is the find sound, but we do something unique, so i wrote it again
	if (id == soundGraph.end()) { //if the sound was not in the graph
		irrKlangEngine->play2D(soundName.c_str(), false, false); //we assume soundName is a file path and we play it once
		return;
	}
	id->second->setPlayPosition(0); //set the sound to restart
	id->second->setIsPaused(false); //play the sound in the graph
	return;
}

void SoundManager::ResumeSound(std::string soundName) {
	auto id = FindSound(soundName);
	if (id->second->getIsPaused() == true) {
		id->second->setIsPaused(false);
	}
}

void SoundManager::PauseSound(std::string soundName) {
	auto id = FindSound(soundName);
	id->second->setIsPaused(true);
}

void SoundManager::StopAllSound() {
	irrKlangEngine->stopAllSounds(); //irrklang makes it easy - you could loop through all the graph variables to stop them
}

//custom sound functions

void SoundManager::FadeOutSound(std::string soundName, float newVolume, float rateOfChange) {
	auto id = FindSound(soundName);
	for (float volume = id->second->getVolume(); volume > newVolume; volume -= rateOfChange) {
		id->second->setVolume(volume);
		//need to pause some how
	}
}

void SoundManager::FadeInSound(std::string soundName, float newVolume, float rateOfChange) {
	auto id = FindSound(soundName);
	for (float volume = id->second->getVolume(); volume < newVolume; volume += rateOfChange) {
		id->second->setVolume(volume);
		//need to pause some how
	}
}

//sound settings - helper functions

bool SoundManager::IsSoundCurrentlyPlaying(std::string soundName) {
	auto id = FindSound(soundName);
	return irrKlangEngine->isCurrentlyPlaying(id->second->getSoundSource());
}

float SoundManager::GetSoundVolume(std::string soundName) {
	auto id = FindSound(soundName);
	return id->second->getVolume();
}

void SoundManager::SetSoundVolume(std::string soundName, float volume) {
	auto id = FindSound(soundName);
	id->second->setVolume(volume);
}

bool SoundManager::GetSoundLooping(std::string soundName) {
	auto id = FindSound(soundName);
	return id->second->isLooped();
}

void SoundManager::SetSoundLooping(std::string soundName, bool playLooped) {
	auto id = FindSound(soundName);
	id->second->setIsLooped(playLooped);
}

bool SoundManager::IsSoundFinished(std::string soundName) {
	auto id = FindSound(soundName);
	return 	id->second->isFinished();
}

unsigned int SoundManager::GetSoundPlayPosition(std::string soundName) {
	auto id = FindSound(soundName);
	return id->second->getPlayPosition();
}

void SoundManager::SetSoundPlayPosition(std::string soundName, unsigned int position) {
	auto id = FindSound(soundName);
	id->second->setPlayPosition(position);
}

float SoundManager::GetSoundPlaybackSpeed(std::string soundName) {
	auto id = FindSound(soundName);
	return id->second->getPlaybackSpeed();
}

void SoundManager::SetSoundPlaybackSpeed(std::string soundName, float speed) {
	auto id = FindSound(soundName);
	id->second->setPlaybackSpeed(speed);
}

unsigned int SoundManager::GetSoundPlayLength(std::string soundName) {
	auto id = FindSound(soundName);
	return id->second->getPlayLength();
}
