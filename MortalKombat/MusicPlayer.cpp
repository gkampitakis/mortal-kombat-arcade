#include "MusicPlayer.h"
#include <assert.h>


MusicPlayer* MusicPlayer::musicplayer = new MusicPlayer();

void MusicPlayer::CleanUp(void) {
	for (Playlist::iterator i = playlist.begin(); i != playlist.end(); i++) Mix_FreeMusic(i->second);
	for (SoundEffect::iterator i = soundeffects.begin(); i != soundeffects.end(); i++) Mix_FreeChunk(i->second);
	playlist.clear();
	soundeffects.clear();
};

MusicPlayer* MusicPlayer::Get(void) {
	return musicplayer;
};

void MusicPlayer::LoadMusic(const string &path, string id) {
	Mix_Music* tmp = Mix_LoadMUS(path.c_str());
	if (tmp == NULL) {
		cout << "Error Sound: " << path.c_str() << " could not be loaded";
	}
	else 	playlist[id] = tmp;
};

void MusicPlayer::LoadEffect(const string &path, string id) {
	Mix_Chunk* tmp = Mix_LoadWAV(path.c_str());
	if (tmp == NULL) {
		cout << "Error Effect: " << path.c_str() << " could not be loaded";
	}
	else soundeffects[id] = tmp;
};

Mix_Chunk* MusicPlayer::RetrieveEffect(string id) const {
	SoundEffect::const_iterator i = soundeffects.find(id);
	return i != soundeffects.end() ? i->second : (Mix_Chunk*)0;
};

Mix_Music* MusicPlayer::RetrieveMusic(string id) const {
	Playlist::const_iterator i = playlist.find(id);
	return i != playlist.end() ? i->second : (Mix_Music*)0;
};

void MusicPlayer::PlayMusic(Mix_Music *music) {
	if (Mix_PlayingMusic() == 0) Mix_PlayMusic(music, -1);
	else {
		if (Mix_PausedMusic() == 1) Mix_ResumeMusic();
		else Mix_PauseMusic();
	}//This might not be the required functionallity maybe when we call play music with another
	//song we should just start playing the new one 
};

void MusicPlayer::PlayEffect(Mix_Chunk* effect, int loops) {
	Mix_PlayChannel(-1, effect, loops);
};

void MusicPlayer::PauseMusic(void) {
	if (Mix_PausedMusic() == 1) Mix_ResumeMusic();
	else Mix_PauseMusic();
	//check if no music exists loaded what happens
};

void MusicPlayer::StopMusic(void) {
	Mix_HaltMusic();
};

void MusicPlayer::StopEffect(void) {
	Mix_HaltChannel(-1);//Check this if stops all sounds forever and needs re "opening"
}